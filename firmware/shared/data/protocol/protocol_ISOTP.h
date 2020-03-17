#ifndef PROTOCOL_ISOTP_H_
#define PROTOCOL_ISOTP_H_

#include <stdlib.h>
#include <stdint.h>
#include "motorcontroller_ISOTP.h"

#define PROTOCOL_ISOTP_MESSAGE_ID_SIZE  (1U)
#define PROTOCOL_ISOTP_MAX_DATA_SIZE    (1024U)
#define PROTOCOL_ISOTP_MAX_PAYLOAD_SIZE (PROTOCOL_ISOTP_MESSAGE_ID_SIZE + PROTOCOL_ISOTP_MAX_DATA_SIZE)

// ISOTP Message ID
typedef union
{
    motorcontroller_ISOTP_MID_E MC_ISOTP_MID;
    uint8_t asUint8;
} protocol_ISOTP_MID_U;

// Individual ISOTP message definitions
typedef struct
{
    uint8_t data[PROTOCOL_ISOTP_MAX_DATA_SIZE];
} protocol_ISOTP_genericMessage_S;


// All ISOTP message definitions
typedef union
{
    motorcontroller_ISOTP_firmwareInfo_S    MC_firmwareInfo;
    protocol_ISOTP_genericMessage_S         genericMessage;
} protocol_ISOTP_allMessages_U;

// High Level Struct
typedef struct
{
    protocol_ISOTP_MID_U         messageID;
    protocol_ISOTP_allMessages_U message;
} protocol_ISOTP_message_S;

uint8_t assert_protocol_ISOTP_messageIDSize[sizeof(protocol_ISOTP_MID_U) > 1U ? -1 : 0];
uint8_t assert_protocol_ISOTP_messageSize[sizeof(protocol_ISOTP_allMessages_U) > PROTOCOL_ISOTP_MAX_DATA_SIZE ? -1 : 0];

#endif // PROTOCOL_ISOTP_H_
