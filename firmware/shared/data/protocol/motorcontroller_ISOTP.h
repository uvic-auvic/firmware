#ifndef MOTORCONTROLLER_ISOTP_H_
#define MOTORCONTROLLER_ISOTP_H_

#include <stdlib.h>
#include <stdint.h>

typedef enum __attribute__((packed))
{
    MOTORCONTROLLER_ISOTP_MID_RESERVED_0 = 0,
    
    MOTORCONTROLLER_ISOTP_MID_LOOPBACK,
    MOTORCONTROLLER_ISOTP_MID_FIRMWARE_INFO,
} motorcontroller_ISOTP_MID_E;

typedef struct __attribute__((packed))
{
    uint32_t crc;
    uint32_t size;
} motorcontroller_ISOTP_firmwareInfo_S;

#endif // MOTORCONTROLLER_ISOTP_H_
