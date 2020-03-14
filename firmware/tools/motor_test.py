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
            print("Received: {} {} {}".format(header, crc, payload))
        
        time.sleep(0.1) # wait 100ms
    
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

def motors_test_oversized_UART_packet(port:serial.Serial, verbose:bool=True):

    # Test normal tranmission. Just a sanity check to make sure normal operation works before continuing
    if motors_test_RID(port, False) == False:
        if verbose:
            print("Test Oversized UART Packet: Failed: Sanity check failed")
        return False

    port.reset_input_buffer()

    # Test: Length too long. Send a packet with a length that is greater than what the device can support.
    for i in range(10):
        port.reset_output_buffer()

        frame = int(20).to_bytes(length=1, byteorder='little') # Send packet with payload of size 20
        frame += 0x0.to_bytes(length=2, byteorder='little') # crc
        frame += 0x0.to_bytes(length=20, byteorder='little') # Just zeros for payload

        port.write(frame)

        time.sleep(0.01) # wait 10ms

        if port.in_waiting != 0:
            if verbose:
                print("Test Oversized UART Packet: Failed: Received data when not expected")
            return False


    # Test that the board still responds after receiveing a bad packet
    if motors_test_RID(port, False) == False:
        if verbose: print("Test Oversized UART Packet: Failed: Post test comms check")
        return False

    print("Success: Test Oversized UART Packet")
    return True

def motors_test_undersized_UART_packet(port:serial.Serial, verbose:bool=True):

    # Test normal tranmission. Just a sanity check to make sure normal operation works before continuing
    if motors_test_RID(port, False) == False:
        if verbose:
            print("Test Undersized UART Packet: Failed: Sanity check failed")
        return False

    port.reset_input_buffer()

    # Test: Length too small. Send a packet with a payload length of 0. So the packet only has a CRC
    for i in range(10):
        port.reset_output_buffer()

        frame = int(0).to_bytes(length=1, byteorder='little') # Send packet with payload of size 0
        frame += 0x0.to_bytes(length=2, byteorder='little') # crc
        # No payload in this one

        port.write(frame)

        time.sleep(0.01) # wait 10ms

        if port.in_waiting != 0:
            if verbose:
                print("Test Undersized UART Packet: Failed: Received data when not expected")
            return False

    # Test that the board still responds after receiveing a bad packet
    if motors_test_RID(port, False) == False:
        if verbose: print("Test Undersized UART Packet: Failed: Post test comms check")
        return False

    print("Success: Test Undersized UART Packet")
    return True

def motors_test_aborted_UART_packet(port:serial.Serial, verbose:bool=True):

    # Test normal tranmission. Just a sanity check to make sure normal operation works before continuing
    if motors_test_RID(port, False) == False:
        if verbose:
            print("Test Aborted UART Packet: Failed: Sanity check failed")
        return False
    
    port.reset_input_buffer()

    # Test: Simulate a packet transmission being aborted
    for i in range(10):
        port.reset_output_buffer()

        frame = serial_lib.construct_frame(5, 0, 1) # Contruct the RID request frame
        frame = frame[0:-1] # remove the last byte

        port.write(frame)

        time.sleep(0.1) # wait 100ms

        if port.in_waiting != 0:
            if verbose:
                print("Test Aborted UART Packet: Failed: Received data when not expected")
            return False

    # Test that the board still responds after receiveing a bad packet
    if motors_test_RID(port, False) == False:
        if verbose: print("Test Aborted UART Packet: Failed: Post test comms check")
        return False

    print("Success: Test Aborted UART Packet")
    return True

def test_helper(port:serial.Serial, interation:int=10):
    success = 0
    failure = 0
    for i in range(interation):
        if motors_test_aborted_UART_packet(port):
            success += 1
        else:
            failure += 1
        
        print("Success: {}   Failure: {}   Success Rate: {}".format(success, failure, (success * 100)/(success+failure)))

#######################
#    SCRIPT START
#######################
parser = argparse.ArgumentParser()
parser.add_argument("port")
args = parser.parse_args()

port = None
if args.port:
    port = serial.Serial(port=args.port, baudrate=9600, timeout=0.1)

if port is None:
    raise "ERROR"
    quit()

motors_test_RID(port)
motors_test_oversized_UART_packet(port)
motors_test_undersized_UART_packet(port)
motors_test_aborted_UART_packet(port)

port.close()
