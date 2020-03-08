
import sys
import time
import serial
import threading

def construct_frame(message_ID, message, message_length):

    payload_bytes = message_ID.to_bytes(length=1, byteorder='little')
    payload_bytes += message.to_bytes(length=message_length, byteorder='little')
    crc_bytes = 0x00.to_bytes(length=2, byteorder='little')
    header_bytes = len(payload_bytes).to_bytes(length=1, byteorder='little')

    frame = header_bytes + crc_bytes + payload_bytes

    return frame 

def rx_thread(port):

    while 1:
        print("In Thread")
        if(port.in_waiting):
            header = port.read()
            crc = port.read(2)
            payload = port.read(header)

            print("Received: {}{}{}".format(header, crc, payload))

            break


def send_message(port, message_ID, message, message_length):
    frame = construct_frame(5, 1, 1)
    port.reset_input_buffer()
    port.reset_output_buffer()

def main():

    port = serial.Serial("COM3", baudrate=9600, timeout=1)

    # rx_thread_handle = threading.Thread(target=rx_thread, args=(port,))
    # rx_thread_handle.start()
    frame = construct_frame(5, 0, 1)

    success_count = 0
    failure_count = 0

    while(1):

        port.reset_input_buffer()
        port.reset_output_buffer()
        
        print("Sending: {}".format(frame))
        port.write(frame)

        header = port.read()
        crc = port.read(2)
        payload = port.read(int.from_bytes(header, byteorder='little'))

        print("Received: {}{}{}".format(header, crc, payload))

        if(payload == b'\x15MTR_CONT\x00'):
            success_count += 1
        else:
            failure_count += 1

        print("Success: {} / Failure: {}".format(success_count, failure_count))

    # rx_thread_handle.join()
    

    port.close()

if __name__  == "__main__":
    main()

