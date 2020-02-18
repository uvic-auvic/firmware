#ifndef SHARED_DATA_PROTOCOL_H_
#define SHARED_DATA_PROTOCOL_H_

#include <stdint.h>
#include <stdbool.h>

#define PROTOCOL_MAX_MESSAGE_SIZE  (8U)

// Message ID Definition `protocol_MID_<sending_node>_<message_name>`
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

} protocol_MID_E; // Cannot be higher than 11 bits

// Individual Messages Definitions and any relevant enums

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
    bool systemPowerEnable;
    bool _5VPowerEnable;
    bool _9VPowerEnable;
    bool _12VPowerEnable;
} protocol_powerEnable_S;

// protocol_MID_POLARIS_PBMessageRequest
typedef enum
{
    PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_RID,
    PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_ENV_DATA,

    PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_COUNT,
} protocol_PBMessageRequest_message_E;

typedef struct __attribute__((packed))
{
    protocol_PBMessageRequest_message_E requestedMessage;
} protocol_PBMessageRequest_S;

// protocol_MID_PB_envData
typedef struct __attribute__((packed))
{
    uint16_t extPressure;
    uint16_t intPressure;
    uint16_t intTemperature;
    uint16_t intHumidity;
} protocol_PBEnvData_S;

// Link Layer Stuff
typedef union
{
    protocol_deviceName_S    POLARIS_deviceName;  // Sent by: Polaris, Received by: No One
    protocol_motorSetSpeed_S POLARIS_motorSetSpeed; // Sent by: Polaris, Received by: Motor Controller
    protocol_powerEnable_S   POLARIS_powerEnable; // Sent by: Polaris, Received by: Power Board
    protocol_PBMessageRequest_S POLARIS_messageRequest; // Sent by: Polaris, Received by: Power Board 

    protocol_deviceName_S    MC_deviceName; // Sent by Motor Controller, Received by Polaris


    protocol_deviceName_S    PB_deviceName; // Sent by Power Board, Received by Polaris
    protocol_PBEnvData_S     PB_envData; // Sent by Power Board, Received by Polaris

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
