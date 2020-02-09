
import sys
import time
import serial

def main():

    port = serial.Serial("/dev/ttyS2", baudrate=115200, timeout=1)

    input("Press enter to continue");

    length = 5

    packet = length.to_bytes(1, byteorder='little')

    print("Sending: {}".format(packet))
    port.write(packet)

    port.close()


if __name__  == "__main__":
    main()

