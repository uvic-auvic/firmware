import sys
import serial
import time
import argparse

def construct_frame(message_ID, message, message_length):

    payload_bytes = message_ID.to_bytes(length=1, byteorder='little')
    if type(message) == bytes:
        payload_bytes += message
    elif type(message) == int:
        payload_bytes += message.to_bytes(length=message_length, byteorder='little')
        
    crc_bytes = 0x00.to_bytes(length=2, byteorder='little')
    header_bytes = len(payload_bytes).to_bytes(length=1, byteorder='little')

    frame = header_bytes + crc_bytes + payload_bytes

    return frame 

def send_message(port:serial.Serial, message_ID, message, message_length):
    frame = construct_frame(message_ID, message, message_length)
    port.reset_input_buffer()
    port.reset_output_buffer()
    port.write(frame)

def send_and_receive(port:serial.Serial, message_ID, message, message_length):

    send_message(port, message_ID, message, message_length)

    header = port.read(1)
    crc = port.read(2)
    payload = port.read(int.from_bytes(header, byteorder='little'))

    return [header, crc, payload]

def get_rpms(port:serial.Serial):

    header, crc, payload_low_rpms = send_and_receive(port, 5, 1, 1)
    header, crc, payload_high_rpms = send_and_receive(port, 5, 2, 1)

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
    
    send_message(port, 2, motor_speed_bytes, 8)

def stop_all_motors(port:serial.Serial):
    zero_speeds = [0] * 8
    set_motor_speed(port, zero_speeds)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("port")
    args = parser.parse_args()

    port = None
    if args.port:
        port = serial.Serial(port=args.port, baudrate=9600, timeout=0.5)
    
    if port is None:
        raise "ERROR"
        quit()

    while(1):
        motor, speed = input("Enter motor 0 speed: ").split()
        all_speeds = [0] * 8
        all_speeds[int(motor)] = int(speed)
        set_motor_speed(port, all_speeds)

        for i in range(10):
            print_rpms(port)
            time.sleep(0.1)
    
        stop_all_motors(port)

    port.close()


if __name__ == "__main__":
    main()
