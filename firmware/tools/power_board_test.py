import sys
import serial
import time
import argparse
import lib.auvic_serial_lib as serial_lib
import lib.auvic_test_lib as test_lib
import lib.generated.protocol as protocol

#######################
#    SCRIPT START
#######################

parser = argparse.ArgumentParser()
parser.add_argument("port")
args = parser.parse_args()

serial_handle = None
if args.port:
    serial_handle = serial_lib.SerialLib(port=args.port, baudrate=115200, timeout=0.1)

if serial_handle is None:
    raise "ERROR"
    quit()

pb_tester = test_lib.SerialTester(serial_handle, b')PWR_BRD\x00\x00', protocol.protocol_MID_POLARIS_PBMessageRequest)
pb_tester.run_all_tests(10)

# Comment out if using iPython
# serial_handle.close()
