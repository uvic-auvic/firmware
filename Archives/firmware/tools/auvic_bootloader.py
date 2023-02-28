# AUVIC Bootloader Console App

import serial
import sys
import math
from lib.stm32_library import stm32_crc32

com_port = "COM3"
data_list = []
max_data_size = 1024
packet_ID = 0

def send_packet():
	pass

def send_start():
	
	global packet_ID
	global com_port
	
	packet_ID += 1
	packet = packet_ID.to_bytes(2, byteorder = 'little')
	packet += ord('A').to_bytes(2, byteorder = 'little')
	packet += "START".encode('ascii')
	packet_size = len(packet)
	packet_size = packet_size.to_bytes(2, byteorder = 'little')
	
	com.write(packet_size)
	response = com.readline().decode('ascii').rstrip('\r\n')
	print(response)
	if(response != "ACK"):
		print("ERROR")
	
	com.write(packet)
	response = com.readline().decode('ascii').rstrip('\r\n')
	print(response)
	if(response != "ACK"):
		print("ERROR")
	
# start script
print("")
print("**AUVIC Bootloader**")
print("")

#open com port
com = serial.Serial(com_port, 9600, timeout = 2)

#read file
program = open("/home/poorna/Poorna/AUVIC/firmware/firmware/firmware/motorcontroller2/MOTORCONTROLLER/app/STM32F411/motorcontroller_app.bin", "rb").read()
# program = open("Motor_Controller_2.0.bin", "rb").read()
program_size = len(program)
num_of_packets = math.ceil(program_size/max_data_size)
print("Number of packets: " + str(num_of_packets))

input("Send start?")
send_start()

response = com.readline().decode('ascii').rstrip('\r\n')
print(response)
response = com.readline().decode('ascii').rstrip('\r\n')
print(response)

input("Continue bootloading?")

com.flushInput()

for i in range(num_of_packets):
	
	packet_ID += 1
	
	packet = packet_ID.to_bytes(2, byteorder = 'little')
	packet += ord('M').to_bytes(2, byteorder = 'little')
	packet += program[i*max_data_size:(i+1)*max_data_size]
	crc = (stm32_crc32(packet, 0xFFFFFFFF))
	packet += crc.to_bytes(4, byteorder = 'little')
	
	packet_size = len(packet)
	print("Packet ID: " + str(packet_ID))
	print("Packet size: " + str(packet_size))
	packet_size = packet_size.to_bytes(2, byteorder = 'little')

	com.write(packet_size)
	response = com.readline().decode('ascii').rstrip('\r\n')
	print(response)
	if(response != "ACK"):
		print("ERROR")
	
	com.write(packet)
	response = com.readline().decode('ascii').rstrip('\r\n')
	print(response)
	if(response != "ACK"):
		print("ERROR")

print("Done bootloading")
input("Exit?")
com.close()
