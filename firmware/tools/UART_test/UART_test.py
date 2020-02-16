
import sys
import time
import serial

def delay(delay):

    for i in range(0,delay):
        pass

def main():

    port = serial.Serial("COM3", baudrate=115200, timeout=1)
    
    while 1:
        crc = 0x00.to_bytes(length=2, byteorder='little')
        messageID = 0x2A.to_bytes(length=1, byteorder='little')
        data = 0xAABBCCDD.to_bytes(length=4, byteorder='little')
        
        frame_2 = crc + messageID + data
        length = len(messageID) + len(data)
        frame_1 = length.to_bytes(length=1, byteorder='little')
        
        frame = frame_1 + frame_2
        
        # print("Sending {}".format(frame))
        port.write(frame)
        
        delay(50000)
        #time.sleep(0.001)
    
    port.close()

if __name__  == "__main__":
    main()

