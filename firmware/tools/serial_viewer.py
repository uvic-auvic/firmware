import serial
import threading
import time

def rxThread(comPort):

    while 1:
        dataByte = comPort.read(1)
        if len(dataByte) != 0:
            print(dataByte)

com1 = serial.Serial("COM3", 115200, timeout=1)

rxT = threading.Thread(target=rxThread, args=(com1,))
rxT.start()

dataToSend = "HELLO"
crc = 0xFFFFFFFF

for i in range(10):
    packet = len(dataToSend).to_bytes(2, byteorder = 'little')
    packet += dataToSend.encode('ascii')
    packet += crc.to_bytes(4, byteorder = 'little')

    com1.write(packet)


    input("Press enter\n")

rxT.join()

com1.close()