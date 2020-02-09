#ifndef SHARED_DATA_PROTOCOL_H_
#define SHARED_DATA_PROTOCOL_H_

#include <stdint.h>
#include <stdbool.h>

#define PROTOCOL_MAX_MESSAGE_SIZE  (8U)

typedef enum
{
    protocol_RESERVED = 0U,
    protocol_MID_POLARIS_deviceName,
    protocol_MID_POLARIS_motorSetSpeed,


    protocol_MID_MC_deviceName = 21U,
    

    protocol_MID_PB_deviceName = 41U,
    protocol_MID_PB_powerEnable,

} protocol_MID_E; // Cannot be higher than 11 bits

typedef struct __attribute__((packed))
{
    uint8_t name[8U];
} protocol_deviceName_S;

typedef struct __attribute__((packed))
{
    uint8_t motorSpeed[8U];
} protocol_motorSetSpeed_S;

typedef struct __attribute__((packed))
{
    bool motorPowerEnable;
    bool systemPowerEnable;
    bool _5VPowerEnable;
    bool _9VPowerEnable;
    bool _12VPowerEnable;
} protocol_powerEnable_S;

typedef union
{
    protocol_deviceName_S    POLARIS_deviceName;
    protocol_motorSetSpeed_S POLARIS_motorSetSpeed;

    protocol_deviceName_S    MC_deviceName;


    protocol_deviceName_S    PB_deviceName;
    protocol_powerEnable_S   PB_powerEnable;

} protocol_allMessages_U;

typedef struct __packed
{
    protocol_MID_E messageID;
    protocol_allMessages_U  message;
} protocol_message_S;

uint8_t assert[(sizeof(protocol_allMessages_U) > PROTOCOL_MAX_MESSAGE_SIZE) ? -1 : 1];

#endif // SHARED_DATA_PROTOCOL_H_
