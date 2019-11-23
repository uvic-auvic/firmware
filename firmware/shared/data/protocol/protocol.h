#ifndef SHARED_DATA_PROTOCOL_H_
#define SHARED_DATA_PROTOCOL_H_

#include <stdint.h>

typedef enum
{
    protocol_RESERVED = 0U,
    protocol_MID_POLARIS_deviceName,
    protocol_MID_POLARIS_motorSetSpeed,


    protocol_MID_MC_deviceName = 21U,
    

    protocol_MID_PB_deviceName = 41U,

} protocol_MID_E; // Cannot be higher than 11 bits

typedef struct
{
    uint8_t name[8U];
} protocol_deviceName_S;

typedef struct
{
    uint8_t motorSpeed[8U];
} protocol_motorSetSpeed_S;

typedef union
{
    protocol_deviceName_S    POLARIS_deviceName;
    protocol_motorSetSpeed_S POLARIS_motorSetSpeed;

    protocol_deviceName_S    MC_deviceName;


    protocol_deviceName_S    PB_deviceName;

} protocol_allMessages_U;

typedef struct
{
    protocol_MID_E messageID;
    protocol_allMessages_U  message;
} protocol_message_S;

uint8_t assert[(sizeof(protocol_allMessages_U) > 8U) ? -1 : 1];

#endif // SHARED_DATA_PROTOCOL_H_
