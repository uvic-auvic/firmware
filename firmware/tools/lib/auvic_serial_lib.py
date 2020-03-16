import sys
import serial
import time

class SerialLib(serial.Serial):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def construct_frame(self, message_ID, message, message_length):

        payload_bytes = message_ID.to_bytes(length=1, byteorder='little')
        if type(message) == bytes:
            payload_bytes += message
        elif type(message) == int:
            payload_bytes += message.to_bytes(length=message_length, byteorder='little')
            
        crc_bytes = 0x00.to_bytes(length=2, byteorder='little')
        header_bytes = len(payload_bytes).to_bytes(length=1, byteorder='little')

        frame = header_bytes + crc_bytes + payload_bytes

        return frame 

    def send_message(self, message_ID, message, message_length):
        frame = self.construct_frame(message_ID, message, message_length)
        self.reset_input_buffer()
        self.reset_output_buffer()
        self.write(frame)

    def send_and_receive(self, message_ID, message, message_length):

        self.send_message(message_ID, message, message_length)

        header = self.read(1)
        crc = self.read(2)
        payload = self.read(int.from_bytes(header, byteorder='little'))

        return [header, crc, payload]
