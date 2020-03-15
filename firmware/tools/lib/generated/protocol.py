# -*- coding: utf-8 -*-
#
# TARGET arch is: ['-I/usr/lib/gcc/arm-none-eabi/4.9.3/include']
# WORD_SIZE is: 8
# POINTER_SIZE is: 8
# LONGDOUBLE_SIZE is: 16
#
import ctypes





# values for enumeration 'c__EA_protocol_MID_E'
c__EA_protocol_MID_E__enumvalues = {
    0: 'protocol_RESERVED',
    1: 'protocol_MID_POLARIS_deviceName',
    2: 'protocol_MID_POLARIS_motorSetSpeed',
    3: 'protocol_MID_POLARIS_powerEnable',
    4: 'protocol_MID_POLARIS_PBMessageRequest',
    5: 'protocol_MID_POLARIS_MCMessageRequest',
    21: 'protocol_MID_MC_deviceName',
    22: 'protocol_MID_MC_motorRPMLow',
    23: 'protocol_MID_MC_motorRPMHigh',
    41: 'protocol_MID_PB_deviceName',
    42: 'protocol_MID_PB_envData',
    43: 'protocol_MID_PB_battVoltages',
    44: 'protocol_MID_PB_battCurrents',
}
protocol_RESERVED = 0
protocol_MID_POLARIS_deviceName = 1
protocol_MID_POLARIS_motorSetSpeed = 2
protocol_MID_POLARIS_powerEnable = 3
protocol_MID_POLARIS_PBMessageRequest = 4
protocol_MID_POLARIS_MCMessageRequest = 5
protocol_MID_MC_deviceName = 21
protocol_MID_MC_motorRPMLow = 22
protocol_MID_MC_motorRPMHigh = 23
protocol_MID_PB_deviceName = 41
protocol_MID_PB_envData = 42
protocol_MID_PB_battVoltages = 43
protocol_MID_PB_battCurrents = 44
c__EA_protocol_MID_E = ctypes.c_int # enum
protocol_MID_E = c__EA_protocol_MID_E
protocol_MID_E__enumvalues = c__EA_protocol_MID_E__enumvalues

# values for enumeration 'c__EA_protocol_node_E'
c__EA_protocol_node_E__enumvalues = {
    0: 'PROTOCOL_NODE_POLARIS',
    1: 'PROTOCOL_NODE_POWER_BOARD',
    2: 'PROTOCOL_NODE_MOTOR_CONTROLLER',
    3: 'PROTOCOL_NODE_COUNT',
}
PROTOCOL_NODE_POLARIS = 0
PROTOCOL_NODE_POWER_BOARD = 1
PROTOCOL_NODE_MOTOR_CONTROLLER = 2
PROTOCOL_NODE_COUNT = 3
c__EA_protocol_node_E = ctypes.c_int # enum
protocol_node_E = c__EA_protocol_node_E
protocol_node_E__enumvalues = c__EA_protocol_node_E__enumvalues
class struct_c__SA_protocol_deviceName_S(ctypes.Structure):
    _pack_ = True # source:True
    _fields_ = [
    ('name', ctypes.c_ubyte * 8),
     ]

protocol_deviceName_S = struct_c__SA_protocol_deviceName_S
class struct_c__SA_protocol_motorSetSpeed_S(ctypes.Structure):
    _pack_ = True # source:True
    _fields_ = [
    ('motorSpeed', ctypes.c_byte * 8),
     ]

protocol_motorSetSpeed_S = struct_c__SA_protocol_motorSetSpeed_S
class struct_c__SA_protocol_motorRPM_S(ctypes.Structure):
    _pack_ = True # source:False
    _fields_ = [
    ('motorSpeed', ctypes.c_int16 * 4),
     ]

protocol_motorRPM_S = struct_c__SA_protocol_motorRPM_S
class struct_c__SA_protocol_powerEnable_S(ctypes.Structure):
    _pack_ = True # source:True
    _fields_ = [
    ('motorPowerEnable', ctypes.c_bool),
    ('_5VPowerEnable', ctypes.c_bool),
    ('_12V9VPowerEnable', ctypes.c_bool),
     ]

protocol_powerEnable_S = struct_c__SA_protocol_powerEnable_S

# values for enumeration 'c__EA_protocol_PBMessageRequest_message_E'
c__EA_protocol_PBMessageRequest_message_E__enumvalues = {
    0: 'PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_RID',
    1: 'PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_ENV_DATA',
    2: 'PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_BATT_VOLTAGES',
    3: 'PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_BATT_CURRENTS',
    4: 'PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_COUNT',
}
PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_RID = 0
PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_ENV_DATA = 1
PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_BATT_VOLTAGES = 2
PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_BATT_CURRENTS = 3
PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_COUNT = 4
c__EA_protocol_PBMessageRequest_message_E = ctypes.c_int # enum
protocol_PBMessageRequest_message_E = c__EA_protocol_PBMessageRequest_message_E
protocol_PBMessageRequest_message_E__enumvalues = c__EA_protocol_PBMessageRequest_message_E__enumvalues
class struct_c__SA_protocol_PBMessageRequest_S(ctypes.Structure):
    _pack_ = True # source:True
    _fields_ = [
    ('requestedMessage', protocol_PBMessageRequest_message_E),
     ]

protocol_PBMessageRequest_S = struct_c__SA_protocol_PBMessageRequest_S

# values for enumeration 'c__EA_protocol_MCMessageRequest_message_E'
c__EA_protocol_MCMessageRequest_message_E__enumvalues = {
    0: 'PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_RID',
    1: 'PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_RPM_LOW',
    2: 'PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_RPM_HIGH',
    3: 'PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_COUNT',
}
PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_RID = 0
PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_RPM_LOW = 1
PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_RPM_HIGH = 2
PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_COUNT = 3
c__EA_protocol_MCMessageRequest_message_E = ctypes.c_int # enum
protocol_MCMessageRequest_message_E = c__EA_protocol_MCMessageRequest_message_E
protocol_MCMessageRequest_message_E__enumvalues = c__EA_protocol_MCMessageRequest_message_E__enumvalues
class struct_c__SA_protocol_MCMessageRequest_S(ctypes.Structure):
    _pack_ = True # source:True
    _fields_ = [
    ('requestedMessage', protocol_MCMessageRequest_message_E),
     ]

protocol_MCMessageRequest_S = struct_c__SA_protocol_MCMessageRequest_S
class struct_c__SA_protocol_PBEnvData_S(ctypes.Structure):
    _pack_ = True # source:True
    _fields_ = [
    ('extPressure', ctypes.c_uint16),
    ('intPressure', ctypes.c_uint16),
    ('intTemperature', ctypes.c_uint16),
    ('intHumidity', ctypes.c_uint16),
     ]

protocol_PBEnvData_S = struct_c__SA_protocol_PBEnvData_S
class struct_c__SA_protocol_PBBattVoltages_S(ctypes.Structure):
    _pack_ = True # source:True
    _fields_ = [
    ('leftBattVoltage', ctypes.c_uint16),
    ('rightBattVoltage', ctypes.c_uint16),
     ]

protocol_PBBattVoltages_S = struct_c__SA_protocol_PBBattVoltages_S
class struct_c__SA_protocol_PBBattCurrents_S(ctypes.Structure):
    _pack_ = True # source:True
    _fields_ = [
    ('leftBattCurrent', ctypes.c_uint32),
    ('rightBattCurrent', ctypes.c_uint32),
     ]

protocol_PBBattCurrents_S = struct_c__SA_protocol_PBBattCurrents_S
class union_c__UA_protocol_allMessages_U(ctypes.Union):
    _pack_ = True # source:False
    _fields_ = [
    ('POLARIS_deviceName', protocol_deviceName_S),
    ('POLARIS_motorSetSpeed', protocol_motorSetSpeed_S),
    ('POLARIS_powerEnable', protocol_powerEnable_S),
    ('POLARIS_PBMessageRequest', protocol_PBMessageRequest_S),
    ('POLARIS_MCMessageRequest', protocol_MCMessageRequest_S),
    ('MC_deviceName', protocol_deviceName_S),
    ('MC_motorRPMLow', protocol_motorRPM_S),
    ('MC_motorRPMHigh', protocol_motorRPM_S),
    ('PB_deviceName', protocol_deviceName_S),
    ('PB_envData', protocol_PBEnvData_S),
    ('PB_battVoltages', protocol_PBBattVoltages_S),
    ('PB_battCurrents', protocol_PBBattCurrents_S),
     ]

protocol_allMessages_U = union_c__UA_protocol_allMessages_U
class struct_c__SA_protocol_message_S(ctypes.Structure):
    _pack_ = True # source:True
    _fields_ = [
    ('messageID', protocol_MID_E),
    ('message', protocol_allMessages_U),
     ]

protocol_message_S = struct_c__SA_protocol_message_S
assert_protocol_maxMessageSize = ctypes.c_ubyte * 1 # Variable ctypes.c_ubyte * 1
assert_protocol_messageIDSize = ctypes.c_ubyte * 1 # Variable ctypes.c_ubyte * 1
__all__ = \
    ['PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_COUNT',
    'PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_RID',
    'PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_RPM_HIGH',
    'PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_RPM_LOW',
    'PROTOCOL_NODE_COUNT', 'PROTOCOL_NODE_MOTOR_CONTROLLER',
    'PROTOCOL_NODE_POLARIS', 'PROTOCOL_NODE_POWER_BOARD',
    'PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_BATT_CURRENTS',
    'PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_BATT_VOLTAGES',
    'PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_COUNT',
    'PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_ENV_DATA',
    'PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_RID',
    'assert_protocol_maxMessageSize', 'assert_protocol_messageIDSize',
    'c__EA_protocol_MCMessageRequest_message_E',
    'c__EA_protocol_MID_E',
    'c__EA_protocol_PBMessageRequest_message_E',
    'c__EA_protocol_node_E', 'protocol_MCMessageRequest_S',
    'protocol_MCMessageRequest_message_E',
    'protocol_MCMessageRequest_message_E__enumvalues',
    'protocol_MID_E', 'protocol_MID_E__enumvalues',
    'protocol_MID_MC_deviceName', 'protocol_MID_MC_motorRPMHigh',
    'protocol_MID_MC_motorRPMLow', 'protocol_MID_PB_battCurrents',
    'protocol_MID_PB_battVoltages', 'protocol_MID_PB_deviceName',
    'protocol_MID_PB_envData',
    'protocol_MID_POLARIS_MCMessageRequest',
    'protocol_MID_POLARIS_PBMessageRequest',
    'protocol_MID_POLARIS_deviceName',
    'protocol_MID_POLARIS_motorSetSpeed',
    'protocol_MID_POLARIS_powerEnable', 'protocol_PBBattCurrents_S',
    'protocol_PBBattVoltages_S', 'protocol_PBEnvData_S',
    'protocol_PBMessageRequest_S',
    'protocol_PBMessageRequest_message_E',
    'protocol_PBMessageRequest_message_E__enumvalues',
    'protocol_RESERVED', 'protocol_allMessages_U',
    'protocol_deviceName_S', 'protocol_message_S',
    'protocol_motorRPM_S', 'protocol_motorSetSpeed_S',
    'protocol_node_E', 'protocol_node_E__enumvalues',
    'protocol_powerEnable_S',
    'struct_c__SA_protocol_MCMessageRequest_S',
    'struct_c__SA_protocol_PBBattCurrents_S',
    'struct_c__SA_protocol_PBBattVoltages_S',
    'struct_c__SA_protocol_PBEnvData_S',
    'struct_c__SA_protocol_PBMessageRequest_S',
    'struct_c__SA_protocol_deviceName_S',
    'struct_c__SA_protocol_message_S',
    'struct_c__SA_protocol_motorRPM_S',
    'struct_c__SA_protocol_motorSetSpeed_S',
    'struct_c__SA_protocol_powerEnable_S',
    'union_c__UA_protocol_allMessages_U']
