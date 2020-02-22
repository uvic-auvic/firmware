#ifndef SHARED_DATA_PROTOCOL_H_
#define SHARED_DATA_PROTOCOL_H_

#include <stdint.h>
#include <stdbool.h>

#define PROTOCOL_MAX_MESSAGE_SIZE  (8U)

/********************************
 * MESSAGE ID DEFINITION
 * ******************************/
// `protocol_MID_<sending_node>_<message_name>`
typedef enum __attribute__((packed))
{
    protocol_RESERVED = 0U,

    protocol_MID_POLARIS_deviceName,
    protocol_MID_POLARIS_motorSetSpeed,
    protocol_MID_POLARIS_powerEnable,
    protocol_MID_POLARIS_PBMessageRequest,  // Power Board Message Request
    protocol_MID_POLARIS_MCMessageRequest, // Motor Controller Message Request


    protocol_MID_MC_deviceName = 21U,


    protocol_MID_PB_deviceName = 41U,
    protocol_MID_PB_envData, // Environmental Data
    protocol_MID_PB_battVoltages,
    protocol_MID_PB_battCurrents,

} protocol_MID_E; // Cannot be higher than 11 bits

/*******************************************
 * MESSAGE DEFINITIONS, ENUMS
 * *****************************************/
// protocol_MID_MC_deviceName, protocol_MID_PB_deviceName, protocol_MID_POLARIS_deviceName
typedef struct __attribute__((packed))
{
    uint8_t name[8U];
} protocol_deviceName_S;

// protocol_MID_POLARIS_motorSetSpeed
typedef struct __attribute__((packed))
{
    uint8_t motorSpeed[8U];
} protocol_motorSetSpeed_S;

// protocol_MID_POLARIS_powerEnable
typedef struct __attribute__((packed))
{
    bool motorPowerEnable;
    bool _5VPowerEnable;
    bool _12V9VPowerEnable;
} protocol_powerEnable_S;

// protocol_MID_POLARIS_PBMessageRequest
typedef enum __attribute__((packed))
{
    PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_RID,
    PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_ENV_DATA,
    PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_BATT_VOLTAGES,
    PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_BATT_CURRENTS,

    PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_COUNT,
} protocol_PBMessageRequest_message_E;

typedef struct __attribute__((packed))
{
    protocol_PBMessageRequest_message_E requestedMessage;
} protocol_PBMessageRequest_S;

// protocol_MID_POLARIS_MCMessageRequest
typedef enum __attribute__((packed))
{
    PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_RID,

    PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_COUNT,
} protocol_MCMessageRequest_message_E;

typedef struct __attribute__((packed))
{
    protocol_MCMessageRequest_message_E requestedMessage;
} protocol_MCMessageRequest_S;

// protocol_MID_PB_envData
typedef struct __attribute__((packed))
{
    uint16_t extPressure;
    uint16_t intPressure;
    uint16_t intTemperature;
    uint16_t intHumidity;
} protocol_PBEnvData_S;

// protocol_MID_PB_battVoltages
typedef struct __attribute__((packed))
{
    uint16_t leftBattVoltage; // mV
    uint16_t rightBattVoltage; // mV
} protocol_PBBattVoltages_S;

// protocol_MID_PB_battCurrents
typedef struct __attribute__((packed))
{
    uint32_t leftBattCurrent; // mA
    uint32_t rightBattCurrent; // mA
} protocol_PBBattCurrents_S;

/********************************
 * LINK LAYER STUFF
 * ******************************/
typedef union
{
    protocol_deviceName_S    POLARIS_deviceName;  // Sent by: Polaris, Received by: No One
    protocol_motorSetSpeed_S POLARIS_motorSetSpeed; // Sent by: Polaris, Received by: Motor Controller
    protocol_powerEnable_S   POLARIS_powerEnable; // Sent by: Polaris, Received by: Power Board
    protocol_PBMessageRequest_S POLARIS_PBMessageRequest; // Sent by: Polaris, Received by: Power Board 
    protocol_MCMessageRequest_S POLARIS_MCMessageRequest; // Sent by Polaris, Receiver by: Motor Controller

    protocol_deviceName_S    MC_deviceName; // Sent by Motor Controller, Received by Polaris

    protocol_deviceName_S    PB_deviceName; // Sent by Power Board, Received by Polaris
    protocol_PBEnvData_S     PB_envData; // Sent by Power Board, Received by Polaris
    protocol_PBBattVoltages_S PB_battVoltages; // Sent by Power Board, Received by Polaris
    protocol_PBBattCurrents_S PB_battCurrents; // Sent by Power Board, Received by Polaris
} protocol_allMessages_U;

typedef struct __attribute__((packed))
{
    protocol_MID_E messageID;
    protocol_allMessages_U  message;
} protocol_message_S;

// Asserts
uint8_t assert_protocol_maxMessageSize[(sizeof(protocol_allMessages_U) > PROTOCOL_MAX_MESSAGE_SIZE) ? -1 : 1];
uint8_t assert_protocol_messageIDSize[(sizeof(protocol_MID_E) == 1U) ? 1 : -1];

#endif // SHARED_DATA_PROTOCOL_H_
