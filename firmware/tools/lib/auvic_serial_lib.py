import sys
import serial
import time
import isotp

class SerialLib(serial.Serial):
    def __init__(self, port, baudrate, timeout, isotp_txid, isotp_rxid, message_request_id):
        super().__init__(port=port, baudrate=baudrate, timeout=timeout)
        addr = isotp.Address(addressing_mode=isotp.AddressingMode.Normal_11bits, txid=isotp_txid, rxid=isotp_rxid)
        self.isotp_handle = isotp.TransportLayer(rxfn=self.isotp_rx_callback, txfn=self.isotp_tx_callback, address=addr)
        self.message_request_id = message_request_id

    def construct_frame(self, message_ID, message, message_length=None):

        payload_bytes = message_ID.to_bytes(length=1, byteorder='little')
        if type(message) == bytes:
            if message_length is None:
                payload_bytes += message
            elif type(message_length) == int:
                payload_bytes += message[0:message_length]

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
    
    def receive_message(self):
        header = self.read(1)
        if len(header) > 0
            crc = self.read(2)
            payload = self.read(int.from_bytes(header, byteorder='little'))

        return header, crc, payload

    def send_and_receive(self, message_ID, message, message_length):

        self.send_message(message_ID, message, message_length)

        return self.receive_message()

    def request_message(self, message_to_request:int):
        return self.send_and_receive(self.message_request_id, message_to_request, 1)

    def send_isotp_message(self, message:bytes):
        self.isotp_handle.reset()
        self.isotp_handle.send(message)

        while self.isotp_handle.transmitting():
            self.isotp_handle.process()
        
    def send_and_receive_isotp_message(self, message:bytes):

        send_isotp_message(message)
        timeStarted = time.time()

        while self.isotp_handle.available() is False: # or timeout
            self.isotp_handle.process()

            if (time.time() - timeStarted) > 0.1:
                break
        
        if self.isotp_handle.available():
            receivedData = self.isotp_handle.recv()

        return receivedData

    def isotp_rx_callback(self):
        header, crc, payload = self.receive_message()
        arbitration_id = int.from_bytes(payload[0], byteorder='little')
        dlc = int.from_bytes(header, byteorder='little')
        data = payload[1:]

        return isotp.CanMessage(arbitration_id=arbitration_id, data=data, dlc=dlc)

    def isotp_tx_callback(self, tx_message):
        self.send_message(message_ID=tx_message.arbitration_id, message=tx_message.data, message_length=tx_message.dlc)
