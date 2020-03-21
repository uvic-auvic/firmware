import sys
import serial
import time
import isotp
import enum
import numpy
import threading
import copy

class RX_STATE(enum.Enum):
    INITIALIZE = 0
    WAIT_FOR_HEADER = 1
    WAIT_FOR_PAYLOAD = 2

class RX_buffer():
    header:int
    crc:int
    message_ID:int
    dlc:int
    message:bytes

class SerialLib(serial.Serial):

    def __init__(self, port, baudrate, isotp_txid, isotp_rxid, message_request_id):
        super().__init__(port=port, baudrate=baudrate, timeout=0.1)
        addr = isotp.Address(addressing_mode=isotp.AddressingMode.Normal_11bits, txid=isotp_txid, rxid=isotp_rxid)
        self.isotp_handle = isotp.TransportLayer(rxfn=self.isotp_rx_callback, txfn=self.isotp_tx_callback, address=addr)
        self.message_request_id = message_request_id
        self.isotp_rxid = isotp_rxid
        
        self.exit_requested = False
        self.RX_state = RX_STATE.INITIALIZE
        self.RX_buffer = RX_buffer()
        self.RX_queue = []

        self.RX_thread = threading.Thread(target = self.RX_thread_func)
        self.RX_thread.start()

    def __del__(self):
        self.shutdown()

    def shutdown(self):
        self.exit_requested = True
        print("Shutting Down RX thread")
        if self.RX_thread.is_alive:
            self.RX_thread.join()

    def RX_thread_func(self):

        while self.exit_requested == False:
            
            print("RX_THREAD HERE")

            if self.RX_state == RX_STATE.WAIT_FOR_HEADER:
                if self.in_waiting > 0:
                    self.RX_buffer.header = int.from_bytes(self.read(1), byteorder='little')
                    if self.RX_buffer.header > 0 and self.RX_buffer.header <= 10: 
                        self.RX_state = RX_STATE.WAIT_FOR_PAYLOAD
                        self.timeStartedReception = time.time()
                    else:
                        self.RX_state = RX_STATE.INITIALIZE
                        # print("RX THREAD: Received invalid header. Header:{}".format(self.RX_buffer.header))

            elif self.RX_state == RX_STATE.WAIT_FOR_PAYLOAD:
                if self.in_waiting > (self.RX_buffer.header + 2):
                    self.RX_buffer.crc = int.from_bytes(self.read(2), byteorder='little')
                    self.RX_buffer.dlc = numpy.clip((self.RX_buffer.header - 1), a_min=0, a_max=8)
                    self.RX_buffer.message_ID = int.from_bytes(self.read(1), byteorder='little')
                    self.RX_buffer.message = self.read((self.RX_buffer.header - 1))
                    self.RX_buffer.message = self.RX_buffer.message[0:8]

                    RX_queue_local.append(copy.deepcopy(self.RX_buffer))

                    self.RX_state = RX_STATE.INITIALIZE

                    print("RX THREAD: LEN:{}, CRC:{}, MID:{}, DLC:{}, MESSAGE:{}, MESSAGE_STR:{}".format(self.RX_buffer.header, self.RX_buffer.crc, self.RX_buffer.message_ID, self.RX_buffer.dlc, self.RX_buffer.message.hex(), self.RX_buffer.message))
                    print("RX THREAD: RX Queue length:{}".format(len(self.RX_queue)))

                elif (time.time() - self.timeStartedReception) > (1.1):
                    self.RX_state = RX_STATE.INITIALIZE
                    print("RX THREAD: RX timed out")
                else:
                    # Do nothing
                    pass

            else: # RX_STATE.INITIALIZE and default
                self.RX_buffer.header = 0
                self.RX_buffer.crc = 0
                self.RX_buffer.message_ID = 0
                self.RX_buffer.dlc = 0
                self.RX_buffer.message = bytes()

                self.RX_state = RX_STATE.WAIT_FOR_HEADER

                # self.reset_input_buffer()

        # self.isotp_handle.process() # Also process ISOTP

    def construct_frame(self, message_ID, message, message_length=None):

        payload_bytes = message_ID.to_bytes(length=1, byteorder='little')
        if type(message) == bytes or type(message) == bytearray:
            if type(message_length) == int:
                payload_bytes += message[0:message_length]
            else:
                payload_bytes += message
        elif type(message) == int:
            payload_bytes += message.to_bytes(length=message_length, byteorder='little')

        crc_bytes = 0x00.to_bytes(length=2, byteorder='little')
        header_bytes = len(payload_bytes).to_bytes(length=1, byteorder='little')

        frame = header_bytes + crc_bytes + payload_bytes

        return frame

    def send_message(self, message_ID, message, message_length):
        frame = self.construct_frame(message_ID, message, message_length)
        self.write(frame)

    def receive_message(self, timeout:int):
        time_started = time.time()
        header = None
        crc = None
        message_ID = None
        message = None
        while True:

            if len(self.RX_queue) > 0:
                frame = self.RX_queue.pop(0)
                header = frame.header
                crc = frame.crc
                message_ID = frame.message_ID
                message = frame.message

                break

            if (time.time() - time_started) > timeout or timeout == 0: # Basically a do-while loop
                break

            print("RECEIVE_FUNC HERE")
            
            # time.sleep(0.01) # sleep for 10ms

        return header, crc, message_ID, message

    def test_test(self):
        item1 = self.RX_queue.pop(0)
        return item1.header, item1.crc, item1.message_ID, item1.message

    def send_and_receive(self, message_ID, message, message_length):
        self.send_message(message_ID, message, message_length)
        return self.receive_message(1.0)

    def request_message(self, message_to_request:int):
        return self.send_and_receive(self.message_request_id, message_to_request, 1)

    def send_isotp_message(self, ISOTP_MID:int, message:bytes):
        self.isotp_handle.reset()
        self.isotp_handle.send(ISOTP_MID.to_bytes(byteorder='little', length=1) + message)

    def receive_isotp_message(self, timeout:int):
        time_started = time.time()

        receivedData = None
        while (time.time() - time_started) > timeout:
            if self.isotp_handle.available():
                receivedData = self.isotp_handle.recv()
                break

            time.sleep(0.01) # sleep 10ms
        
        return receivedData

    def send_and_receive_isotp_message(self, ISOTP_MID:int, message:bytes):
        self.send_isotp_message(ISOTP_MID, message)
        return self.receive_isotp_message(1.0)

    def isotp_rx_callback(self):
        ret = None

        if len(self.RX_queue) > 0:
            if self.RX_queue[0].message_ID == self.isotp_rxid:
                header, crc, message_ID, message = self.receive_message(0)
        
                if header is not None:
                    print("ISOTP RX CALLBACK: ")
                    if type(header) is int:
                        print("  Header: {}".format(header))
                    if type(crc) is int:
                        print("  CRC: {}".format(crc))
                    if type(message_ID) is int:
                        print("  MID: {}".format(message_ID))
                    if type(message) is bytes:
                        print("  MESSAGE: {}".format(message.hex()))

                    if len(message) > 0:
                        dlc = len(message)

                        ret = isotp.CanMessage(arbitration_id=message_ID, data=message, dlc=dlc)

        return ret

    def isotp_tx_callback(self, tx_message):
        print("ISOTP TX Callback: MID:{} MESSAGE:{}, LEN:{}".format(tx_message.arbitration_id, tx_message.data.hex(), tx_message.dlc))
        self.send_message(message_ID=tx_message.arbitration_id, message=tx_message.data, message_length=tx_message.dlc)
