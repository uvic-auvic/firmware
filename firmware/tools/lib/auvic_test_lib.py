import sys
import serial
import time
from .auvic_serial_lib import SerialLib as SerialLib
import generated.protocol

class SerialTester:
    def __init__(self, serial_handle:SerialLib, RID_response:bytes):
        self.serial_handle = serial_handle
        self.RID_response = RID_response

    def test_RID(self, verbose:bool=True):
        success_count = 0
        failure_count = 0
        for i in range(10):
            # header, crc, payload = self.serial_handle.send_and_receive(self.request_MID, 0, 1)
            header, crc, payload = self.serial_handle.request_message(0) # Message Request 0 is RID

            if payload == self.RID_response:
                success_count += 1
            else:
                failure_count += 1

            if verbose:
                print("Success: {},   Failure: {}".format(success_count, failure_count))
                print("Received: {} {} {}".format(header, crc, payload))
            
            time.sleep(0.1) # wait 100ms
        
        if verbose:
            out_str = "Test End:\n"
            out_str += "  Success Count: {}\n".format(success_count)
            out_str += "  Failure Count: {}\n".format(failure_count)
            out_str += "  Success Rate: {}%".format(success_count*100/(success_count + failure_count))
            print(out_str)
        
        result = False
        if (success_count/(success_count + failure_count)) == 1:
            result = True
        
        return result

    def test_aborted_UART_packet(self, verbose:bool=True):
        # Test normal tranmission. Just a sanity check to make sure normal operation works before continuing
        if self.test_RID(False) == False:
            if verbose:
                print("Test Aborted UART Packet: Failed: Sanity check failed")
            return False
        
        self.serial_handle.reset_input_buffer()

        # Test: Simulate a packet transmission being aborted
        for i in range(10):
            self.serial_handle.reset_output_buffer()

            frame = self.serial_handle.construct_frame(5, 0, 1) # Contruct the RID request frame
            frame = frame[0:-1] # remove the last byte

            self.serial_handle.write(frame)

            time.sleep(0.1) # wait 100ms

            if self.serial_handle.in_waiting != 0:
                if verbose:
                    print("Test Aborted UART Packet: Failed: Received data when not expected")
                return False

        # Test that the board still responds after receiveing a bad packet
        if self.test_RID(False) == False:
            if verbose: print("Test Aborted UART Packet: Failed: Post test comms check")
            return False

        if verbose:
            print("Success: Test Aborted UART Packet")
        
        return True

    def test_undersized_UART_packet(self, verbose:bool=True):
        
        # Test normal tranmission. Just a sanity check to make sure normal operation works before continuing
        if self.test_RID(False) == False:
            if verbose:
                print("Test Undersized UART Packet: Failed: Sanity check failed")
            return False

        self.serial_handle.reset_input_buffer()

        # Test: Length too small. Send a packet with a payload length of 0. So the packet only has a CRC
        for i in range(10):
            self.serial_handle.reset_output_buffer()

            frame = int(0).to_bytes(length=1, byteorder='little') # Send packet with payload of size 0
            frame += 0x0.to_bytes(length=2, byteorder='little') # crc
            # No payload in this one

            self.serial_handle.write(frame)

            time.sleep(0.01) # wait 10ms

            if self.serial_handle.in_waiting != 0:
                if verbose:
                    print("Test Undersized UART Packet: Failed: Received data when not expected")
                return False

        # Test that the board still responds after receiveing a bad packet
        if self.test_RID(False) == False:
            if verbose: print("Test Undersized UART Packet: Failed: Post test comms check")
            return False

        if verbose:
            print("Success: Test Undersized UART Packet")
        
        return True

    def test_oversized_UART_packet(self, verbose:bool=True):

        # Test normal tranmission. Just a sanity check to make sure normal operation works before continuing
        if self.test_RID(False) == False:
            if verbose:
                print("Test Oversized UART Packet: Failed: Sanity check failed")
            return False

        self.serial_handle.reset_input_buffer()

        # Test: Length too long. Send a packet with a length that is greater than what the device can support.
        for i in range(10):
            self.serial_handle.reset_output_buffer()

            frame = int(20).to_bytes(length=1, byteorder='little') # Send packet with payload of size 20
            frame += 0x0.to_bytes(length=2, byteorder='little') # crc
            frame += 0x0.to_bytes(length=20, byteorder='little') # Just zeros for payload

            self.serial_handle.write(frame)

            time.sleep(0.01) # wait 10ms

            if self.serial_handle.in_waiting != 0:
                if verbose:
                    print("Test Oversized UART Packet: Failed: Received data when not expected")
                return False


        # Test that the board still responds after receiveing a bad packet
        if self.test_RID(False) == False:
            if verbose: print("Test Oversized UART Packet: Failed: Post test comms check")
            return False

        if verbose:
            print("Success: Test Oversized UART Packet")
        
        return True

    def run_all_tests(self, iteration:int=10):
        
        for i in range(iteration):
            print("Interation {}".format(i))
            if self.test_RID(verbose=False):
                print("SUCCESS: Test RID")
                pass
            else:
                print("FAILED: Test RID")
                return False
            
            if self.test_aborted_UART_packet(verbose=False):
                print("SUCCESS: Test Aborted UART Packet")
                pass
            else:
                print("FAILED: Test Aborted UART Packet")
                return False
            
            if self.test_oversized_UART_packet(verbose=False):
                print("SUCCESS: Test Oversized UART packet")
                pass
            else:
                print("FAILED: Test Oversized UART packet")
                return False
            
            if self.test_undersized_UART_packet(verbose=False):
                print("SUCCESS: Test Undersized UART packet")
                pass
            else:
                print("FAILED: Test Undersized UART packet")
                return False
        
        return True

    def test_ISOTP_loopback(self, verbose:bool=True):

        test_message = b'This is a test message'
        received_message = self.serial_handle.send_and_receive_isotp_message(test_message)
        
        if received_message == test_message:
            print("Success: Test ISOTP Loopback")
            ret = True
        else:
            print("Test ISOTP Loopback: Failed: Received data did not match sent data")
            ret = False

        return ret
        
