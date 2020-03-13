import sys
import serial
import time
import argparse
import lib.auvic_serial_lib as serial_lib

def get_rpms(port:serial.Serial):

    header, crc, payload_low_rpms = serial_lib.send_and_receive(port, 5, 1, 1)
    header, crc, payload_high_rpms = serial_lib.send_and_receive(port, 5, 2, 1)

    rpms = []

    for i in range(1, 9, 2):
        rpms += [ payload_low_rpms[i] | (payload_low_rpms[i+1] << 8) ]

    for i in range(1, 9, 2):
        rpms += [ payload_high_rpms[i] | (payload_high_rpms[i+1] << 8) ]

    return rpms

def print_rpms(port:serial.Serial):

    rpms = get_rpms(port)

    for i in range(8):
        print("RPM_{}: {}".format(i, rpms[i]))

# speed range [-100 100]
def set_motor_speed(port:serial.Serial, motor_speed:list):

    motor_speed_bytes = bytes()
    for speed in motor_speed:
        if speed > 100:
            speed = 100
        
        if speed < -100:
            speed = -100
        
        speed = speed * 127 / 100
        if speed < 0:
            speed = 256 + speed
        
        speed = int(speed)
        motor_speed_bytes += speed.to_bytes(length=1, byteorder='little')
    
    serial_lib.send_message(port, 2, motor_speed_bytes, 8)

def stop_all_motors(port:serial.Serial):
    zero_speeds = [0] * 8
    set_motor_speed(port, zero_speeds)

def motors_control_console(port:serial.Serial):

    while(1):
        motor, speed = input("Enter motor 0 speed: ").split()
        all_speeds = [0] * 8
        all_speeds[int(motor)] = int(speed)
        set_motor_speed(port, all_speeds)

        for i in range(10):
            print_rpms(port)
            time.sleep(0.1)

        stop_all_motors(port)

def motors_test_RID(port:serial.Serial, verbose:bool=True):

    success_count = 0
    failure_count = 0
    for i in range(10):
        header, crc, payload = serial_lib.send_and_receive(port, 5, 0, 1)

        if payload == b'\x15MTR_CONT\x00':
            success_count += 1
        else:
            failure_count += 1

        if verbose:
            print("Success: {},   Failure: {}".format(success_count, failure_count))
    
    if verbose:
        out_str = "Test End:\n"
        out_str += "  Success Count: {}\n".format(success_count)
        out_str += "  Failure Count: {}\n".format(failure_count)
        out_str += "  Success Rate: {}%".format(success_count*100/(success_count + failure_count))
        print(out_str)
    
    result = False
    if (success_count/(success_count + failure_count)) == 1:
        result = True
    
    return result

def motors_test_faulty_UART_packets(port:serial.Serial, verbose:bool=True):

    # Test normal tranmission
    if motors_test_RID(port, False) == False:
        if verbose:
            print("Failed: Test Normal Transmission")
        return False

    # Test: Length too long
    frame = int(20).to_bytes(length=1, byteorder='little') # Send packet with payload of size 20
    frame += 0x0.to_bytes(length=2, byteorder='little') # crc
    frame += 0x0.to_bytes(length=20, byteorder='little') # Just zeros for payload

    port.write(frame)

    time.sleep(0.01) # wait 10ms

    if motors_test_RID(port, False) == False: # Test that the board still responds after receiveing a bad packet
        if verbose: print("Failed: Test Length Too Long")
        return False

    print("Success: Test Fault UART Packets")
    return True

########
# SCRIPT START
#######
parser = argparse.ArgumentParser()
parser.add_argument("port")
args = parser.parse_args()

port = None
if args.port:
    port = serial.Serial(port=args.port, baudrate=9600, timeout=0.5)

if port is None:
    raise "ERROR"
    quit()

# motors_control_console(port)
# motors_test_RID(port, verbose=True)

# port.close()


