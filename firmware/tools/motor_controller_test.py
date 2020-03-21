import sys
import serial
import time
import argparse
import lib.auvic_serial_lib as serial_lib
import lib.auvic_test_lib as test_lib
import lib.generated.protocol as protocol

class MotorControl():
    def __init__(self, serial_handle:serial_lib.SerialLib):
        self.serial_handle = serial_handle

    def get_rpms(self):

        header, crc, payload_low_rpms = self.serial_handle.send_and_receive(5, 1, 1)
        header, crc, payload_high_rpms = self.serial_handle.send_and_receive(5, 2, 1)

        rpms = []

        for i in range(1, 9, 2):
            rpms += [ payload_low_rpms[i] | (payload_low_rpms[i+1] << 8) ]

        for i in range(1, 9, 2):
            rpms += [ payload_high_rpms[i] | (payload_high_rpms[i+1] << 8) ]

        return rpms

    def print_rpms(self):

        rpms = self.get_rpms()

        for i in range(8):
            print("RPM_{}: {}".format(i, rpms[i]))

    # speed range [-100 100]
    def set_motor_speed(self, motor_speed:list):

        motor_speed_bytes = bytes()
        for speed in motor_speed:
            if speed > 100:
                speed = 100
            
            if speed < -100:
                speed = -100
            
            speed = speed * 127 / 100
            if speed < 0:
                speed = 256 + speed
            
            speed = int(speed)
            motor_speed_bytes += speed.to_bytes(length=1, byteorder='little')
        
        self.serial_handle.send_message(2, motor_speed_bytes, 8)

    def stop_all_motors(self):
        zero_speeds = [0] * 8
        self.set_motor_speed(zero_speeds)

    def motors_control_console(self):

        while(1):
            motor, speed = input("Enter motor 0 speed: ").split()
            all_speeds = [0] * 8
            all_speeds[int(motor)] = int(speed)
            self.set_motor_speed(all_speeds)

            for i in range(10):
                self.print_rpms()
                time.sleep(0.1)

            self.stop_all_motors()


#######################
#    SCRIPT START
#######################

parser = argparse.ArgumentParser()
parser.add_argument("port")
args = parser.parse_args()

serial_handle = None
if args.port:
    serial_handle = serial_lib.SerialLib(port=args.port, baudrate=9600, isotp_txid=protocol.protocol_MID_POLARIS_MCISOTP, isotp_rxid=protocol.protocol_MID_MC_ISOTP, message_request_id=protocol.protocol_MID_POLARIS_MCMessageRequest)

if serial_handle is None:
    raise "ERROR"
    quit()

motors_tester = test_lib.SerialTester(serial_handle, b'\x15MTR_CONT')
# motors_tester.run_all_tests(10)
# motors_tester.test_RID()
# serial_handle.receive_message(1)
    
# Comment out if using iPython
# serial_handle.close()