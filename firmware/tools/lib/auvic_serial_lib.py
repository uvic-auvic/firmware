import sys
import serial
import time

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
