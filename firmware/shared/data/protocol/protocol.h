
#ifndef PROTOCOL_H_
#define PROTOCOL_H_

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

    protocol_MID_TRIDENT_deviceName,
    protocol_MID_TRIDENT_motorSetSpeed,
    protocol_MID_TRIDENT_powerEnable,
    protocol_MID_TRIDENT_MCMessageRequest, // Motor Controller Message Request
    protocol_MID_TRIDENT_PBMessageRequest,  // Power Board Message Request
    protocol_MID_TRIDENT_DVLMessageRequest, // DVL Message Request
    protocol_MID_TRIDENT_HYDMessageRequest,  // Hydrophone Message Request
    protocol_MID_TRIDENT_LCDMessageRequest, // LCD board Message Request
    protocol_MID_TRIDENT_DROMessageRequest,  // dropper Message Request
    protocol_MID_TRIDENT_GRAMessageRequest, // grabber Message Request
    protocol_MID_TRIDENT_IMUMessageRequest, // IMU Message Request
    protocol_MID_TRIDENT_TORMessageRequest, // TORPEDO Message Request
    protocol_MID_TRIDENT_MCISOTP, // ISOTP message sent from TRIDENT to Motor Controller
    protocol_MID_TRIDENT_PBISOTP, // ISOTP message sent from TRIDENT to Power Board
    protocol_MID_TRIDENT_DVLISOTP, // ISOTP message sent from TRIDENT to DVL
    protocol_MID_TRIDENT_HYDISOTP, // ISOTP message sent from TRIDENT to Hydrophone
    protocol_MID_TRIDENT_LCDISOTP, // ISOTP message sent from TRIDENT to LCD board
    protocol_MID_TRIDENT_DROISOTP, // ISOTP message sent from TRIDENT to dropper
    protocol_MID_TRIDENT_GRAISOTP, // ISOTP message sent from TRIDENT to grabber
    protocol_MID_TRIDENT_IMUISOTP, // ISOTP message sent from TRIDENT to IMU
    protocol_MID_TRIDENT_TORISOTP, // ISOTP message sent from TRIDENT to TORPEDO

    protocol_MID_MC_deviceName = 41U,
    protocol_MID_MC_motorRPMLow, // NOTE: why did we break into high and low RPMs? why is it preffered over a single message with 64 bits describing thruster RPMS.
    protocol_MID_MC_motorRPMHigh,
    protocol_MID_MC_envData,
    protocol_MID_MC_boardData, //motor temp, motor current, system current, etc.    
    protocol_MID_MC_ISOTP, // ISOTP message sent from the Motor Controller

    protocol_MID_PB_deviceName = 61U,
    protocol_MID_PB_envData, // Environmental Data
    protocol_MID_PB_battVoltages,
    protocol_MID_PB_battCurrents,
    protocol_MID_PB_PeriphCurrents, // current measurements for peripherals
    protocol_MID_PB_ISOTP, 

    protocol_MID_DROPPER_deviceName = 81U,
    protocol_MID_DROPPER_state, //value of the stepper motor (encoder val)
    protocol_MID_DROPPER_ISOTP, 

    protocol_MID_DVL_deviceName = 101U, // These messages are for PD6 profile, PD0 is more extensive, but this is a start.
    protocol_MID_DVL_systemAltitude,
    protocol_MID_DVL_timingAndScaling,
    protocol_MID_DVL_waterMassInstrumentRefVelocity,
    protocol_MID_DVL_bottomTrackInstrumentRefVelocity,
    protocol_MID_DVL_waterMassShipRefVelocity,
    protocol_MID_DVL_bottomTrackShipRefVelocity,
    protocol_MID_DVL_waterMassEarthRefVelocity,
    protocol_MID_DVL_bottomTrackEarthRefVelocity,
    protocol_MID_DVL_waterMassEarthRefDistance,
    protocol_MID_DVL_bottomTrackEarthRefDistance,
    protocol_MID_DVL_ISOTP,
 
    protocol_MID_GRABBER_deviceName = 121U,
    protocol_MID_GRABBER_state, //open? closed? extended? etc.
    protocol_MID_GRABBER_ISOTP,


    protocol_MID_IMU_deviceName = 141U,
    protocol_MID_IMU_temperature,
    protocol_MID_IMU_pitch,
    protocol_MID_IMU_yaw,
    protocol_MID_IMU_roll,
    protocol_MID_IMU_ISOTP,


    protocol_MID_LCD_deviceName = 161U,
    protocol_MID_LCD_state, // for testing lights, i guess...
    protocol_MID_LCD_ISOTP,


    protocol_MID_TORPEDO_deviceName = 181U,
    protocol_MID_TORPEDO_attackAck, 
    protocol_MID_TORPEDO_ISOTP,

    protocol_MID_HYDROPHONE_deviceName = 201U,
    protocol_MID_HYDROPHONE_ISOTP,


} protocol_MID_E; // Cannot be higher than 11 bits

/*******************************************
 * MESSAGE DEFINITIONS, ENUMS
 * *****************************************/

typedef enum
{
    PROTOCOL_NODE_TRIDENT,
    PROTOCOL_NODE_POWER_BOARD,
    PROTOCOL_NODE_MOTOR_CONTROLLER,
    PROTOCOL_NODE_LCD_BOARD,
    PROTOCOL_NODE_DOPPLER_VELOCITY_LOGGER,
    PROTOCOL_NODE_HYDROPHONE,
    PROTOCOL_NODE_TORPEDO,
    PROTOCOL_NODE_DROPPER,
    PROTOCOL_NODE_GRABBER,
    PROTOCOL_NODE_IMU,
    PROTOCOL_NODE_COUNT,
} protocol_node_E;

// protocol_MID_MC_deviceName, protocol_MID_PB_deviceName, protocol_MID_TRIDENT_deviceName, etc.
typedef struct __attribute__((packed))
{
    uint8_t name[8U];
} protocol_deviceName_S;

// protocol_MID_TRIDENT_motorSetSpeed
typedef struct __attribute__((packed))
{
    int8_t motorSpeed[8U]; // In percent with a base of 127
} protocol_motorSetSpeed_S;

// protocol_MID_TRIDENT_powerEnable
typedef struct __attribute__((packed))
{
    bool motorPowerEnable;
    bool _5VPowerEnable;
    bool _12V9VPowerEnable;
    bool grabber_PowerEnable;
    bool dropper_PowerEnable;
    bool imu_PowerEnable;
    bool torpedo_PowerEnable;
    bool dvl_PowerEnable;
    bool lcd_PowerEnable;
} protocol_powerEnable_S;

{
// protocol_MID_TRIDENT_PBMessageRequest
typedef enum __attribute__((packed))
{
    PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_RID,
    PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_ENV_DATA,
    PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_BATT_VOLTAGES,
    PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_BATT_CURRENTS,
    PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_PERIPH_CURRENTS,

    PROTOCOL_PB_MESSAGE_REQUEST_MESSAGE_COUNT,
} protocol_PBMessageRequest_message_E;

typedef struct __attribute__((packed))
{
    protocol_PBMessageRequest_message_E requestedMessage;
} protocol_PBMessageRequest_S;

// protocol_MID_TRIDENT_MCMessageRequest
typedef enum __attribute__((packed))
{
    PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_RID,
    PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_RPM_LOW,
    PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_RPM_HIGH,
    PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_ENV_DATA,
    PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_BOARD_DATA,

    PROTOCOL_MC_MESSAGE_REQUEST_MESSAGE_COUNT,
} protocol_MCMessageRequest_message_E;

typedef struct __attribute__((packed))
{
    protocol_MCMessageRequest_message_E requestedMessage;
} protocol_MCMessageRequest_S;

// protocol_MID_TRIDENT_DROMessageRequest
typedef enum __attribute__((packed))
{
    PROTOCOL_DRO_MESSAGE_REQUEST_MESSAGE_RID,
    PROTOCOL_DRO_MESSAGE_REQUEST_MESSAGE_STATE,

    PROTOCOL_DRO_MESSAGE_REQUEST_MESSAGE_COUNT,
} protocol_DROMessageRequest_message_E;

typedef struct __attribute__((packed))
{
    protocol_DROMessageRequest_message_E requestedMessage;
} protocol_DROMessageRequest_S;

// protocol_MID_TRIDENT_DVLMessageRequest
typedef enum __attribute__((packed))
{
    PROTOCOL_DVL_MESSAGE_REQUEST_MESSAGE_RID,
    PROTOCOL_DVL_MESSAGE_REQUEST_MESSAGE_TIMING_AND_SCALING,
    PROTOCOL_DVL_MESSAGE_REQUEST_MESSAGE_WATER_MASS_INSTRUMENT_REF_VELOCITY,
    PROTOCOL_DVL_MESSAGE_REQUEST_MESSAGE_BOTTOM_TRACK_INSTRUMENT_REF_VELOCITY,
    PROTOCOL_DVL_MESSAGE_REQUEST_MESSAGE_WATER_MASS_SHIP_REF_VELOCITY,
    PROTOCOL_DVL_MESSAGE_REQUEST_MESSAGE_BOTTOM_TRACK_SHIP_REF_VELOCITY,
    PROTOCOL_DVL_MESSAGE_REQUEST_MESSAGE_WATER_MASS_EARTH_REF_VELOCITY,
    PROTOCOL_DVL_MESSAGE_REQUEST_MESSAGE_BOTTOM_TRACK_EARTH_REF_VELOCITY,
    PROTOCOL_DVL_MESSAGE_REQUEST_MESSAGE_WATER_MASS_EARTH_REF_DISTANCE,
    PROTOCOL_DVL_MESSAGE_REQUEST_MESSAGE_BOTTOM_TRACK_EARTH_REF_DISTANCE,

    PROTOCOL_DVL_MESSAGE_REQUEST_MESSAGE_COUNT,
} protocol_DVLMessageRequest_message_E;

typedef struct __attribute__((packed))
{
    protocol_DVLMessageRequest_message_E requestedMessage;
} protocol_DVLMessageRequest_S;

// protocol_MID_TRIDENT_GRAMessageRequest
typedef enum __attribute__((packed))
{
    PROTOCOL_GRA_MESSAGE_REQUEST_MESSAGE_RID,
    PROTOCOL_GRA_MESSAGE_REQUEST_MESSAGE_STATE,

    PROTOCOL_GRA_MESSAGE_REQUEST_MESSAGE_COUNT,
} protocol_GRAMessageRequest_message_E;

typedef struct __attribute__((packed))
{
    protocol_GRAMessageRequest_message_E requestedMessage;
} protocol_GRAMessageRequest_S;

// protocol_MID_TRIDENT_IMUMessageRequest
typedef enum __attribute__((packed))
{
    PROTOCOL_IMU_MESSAGE_REQUEST_MESSAGE_RID,
    PROTOCOL_IMU_MESSAGE_REQUEST_MESSAGE_TEMEPERATURE,
    PROTOCOL_IMU_MESSAGE_REQUEST_MESSAGE_PITCH,
    PROTOCOL_IMU_MESSAGE_REQUEST_MESSAGE_YAW,
    PROTOCOL_IMU_MESSAGE_REQUEST_MESSAGE_ROLL,

    PROTOCOL_IMU_MESSAGE_REQUEST_MESSAGE_COUNT,
} protocol_IMUMessageRequest_message_E;

typedef struct __attribute__((packed))
{
    protocol_IMUMessageRequest_message_E requestedMessage;
} protocol_IMUMessageRequest_S;

// protocol_MID_TRIDENT_LCDMessageRequest
typedef enum __attribute__((packed))
{
    PROTOCOL_LCD_MESSAGE_REQUEST_MESSAGE_RID,
    PROTOCOL_LCD_MESSAGE_REQUEST_MESSAGE_STATE,

    PROTOCOL_LCD_MESSAGE_REQUEST_MESSAGE_COUNT,
} protocol_LCDMessageRequest_message_E;

typedef struct __attribute__((packed))
{
    protocol_LCDMessageRequest_message_E requestedMessage;
} protocol_LCDMessageRequest_S;

// protocol_MID_TRIDENT_TORMessageRequest
typedef enum __attribute__((packed))
{
    PROTOCOL_TOR_MESSAGE_REQUEST_MESSAGE_RID,
    PROTOCOL_TOR_MESSAGE_REQUEST_MESSAGE_ATTACK_ACK,

    PROTOCOL_TOR_MESSAGE_REQUEST_MESSAGE_COUNT,
} protocol_TORMessageRequest_message_E;

typedef struct __attribute__((packed))
{
    protocol_TORMessageRequest_message_E requestedMessage;
} protocol_TORMessageRequest_S;

// protocol_MID_TRIDENT_HYDMessageRequest
typedef enum __attribute__((packed))
{
    PROTOCOL_HYD_MESSAGE_REQUEST_MESSAGE_RID,

    PROTOCOL_HYD_MESSAGE_REQUEST_MESSAGE_COUNT,
} protocol_HYDMessageRequest_message_E;

typedef struct __attribute__((packed))
{
    protocol_HYDMessageRequest_message_E requestedMessage;
} protocol_HYDMessageRequest_S;
}

// protocol_MID_MC_motorRPMLow, protocol_MID_MC_motorRPMHigh
typedef struct
{
    int16_t motorSpeed[4U]; // Motor 0 to 3 got motorRPMLow, Motor 4 to 7 for motorRPMHigh
} protocol_motorRPM_S;

// protocol_MID_MC_envData,
typedef struct __attribute__((packed))
{
    uint16_t intPressure;
    uint16_t intTemperature;
    uint16_t intHumidity;
} protocol_MCEnvData_S;

// protocol_MID_MC_boardData,
typedef struct __attribute__((packed))
{
    uint16_t motorTemperature;
    uint16_t motorCurrent;
    uint16_t systemCurrent;
} protocol_MCBoardData_S;

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

// protocol_MID_PB_PBPeriphCurrents
typedef struct __attribute__((packed))
{
    uint32_t jetsonCurrent; // mA
    uint32_t imuCurrent; // mA
    uint32_t dvlCurrent; // mA
    uint32_t torpedoCurrent; // mA
    uint32_t hydrophoneCurrent; // mA
    uint32_t dropperCurrent; // mA
    uint32_t grabberCurrent; // mA
    uint32_t lcdCurrent; // mA
    uint32_t mcCurrent; // mA
} protocol_PBPeriphCurrents_S; 

//protocol_MID_DROPPER_state,
typedef struct
{
    int64_t state; //how much has the stepper motor spinned to release the balls
    
} protocol_dropperState_S;

// protocol_DVL_PD6_S
typedef struct __attribute__((packed))
{
    uint64_t systemAltitude;
    uint64_t timingAndScaling;
    uint64_t waterMassInstrumentRefVelocity;
    uint64_t bottomTrackInstrumentRefVelocity;
    uint64_t waterMassShipRefVelocity;
    uint64_t bottomTrackShipRefVelocity;
    uint64_t waterMassEarthRefVelocity;
    uint64_t bottomTrackEarthRefVelocity;
    uint64_t waterMassEarthRefDistance;
    uint64_t bottomTrackEarthRefDistance;
} protocol_DVL_PD6_S; // NOT TESTED DO NOT USE

//protocol_MID_GRABBER_state,
typedef struct __attribute__((packed))
{
    int4_t claw; // 0 is closed, 127 is open
    int4_t arm; // 0 is not extended, 127 fully extended
} protocol_grabbberState_S;

 
    protocol_MID_IMU_deviceName = 141U,
    protocol_MID_IMU_temperature,
    protocol_MID_IMU_pitch,
    protocol_MID_IMU_yaw,
    protocol_MID_IMU_roll,
    protocol_MID_IMU_ISOTP,


    protocol_MID_LCD_deviceName = 161U,
    protocol_MID_LCD_state, // for testing lights, i guess...
    protocol_MID_LCD_ISOTP,


    protocol_MID_TORPEDO_deviceName = 181U,
    protocol_MID_TORPEDO_attackAck, 
    protocol_MID_TORPEDO_ISOTP,

    protocol_MID_HYDROPHONE_deviceName = 201U,
    protocol_MID_HYDROPHONE_ISOTP,
// protocol_MID_TRIDENT_MCISOTP, protocol_MID_MC_ISOTP
typedef struct
{
    uint8_t data[8U];
} protocol_ISOTP_S;

/********************************
 * LINK LAYER STUFF
 * ******************************/
typedef union
{
    protocol_deviceName_S       TRIDENT_deviceName;  // Sent by: TRIDENT, Received by: No One
    protocol_motorSetSpeed_S    TRIDENT_motorSetSpeed; // Sent by: TRIDENT, Received by: Motor Controller
    protocol_powerEnable_S      TRIDENT_powerEnable; // Sent by: TRIDENT, Received by: Power Board
    protocol_PBMessageRequest_S TRIDENT_PBMessageRequest; // Sent by: TRIDENT, Received by: Power Board 
    protocol_MCMessageRequest_S TRIDENT_MCMessageRequest; // Sent by TRIDENT, Received by: Motor Controller
    protocol_LCDMessageRequest_S TRIDENT_LCDMessageRequest;
    protocol_DVLMessageRequest_S TRIDENT_DVLMessageRequest;
    protocol_IMUMessageRequest_S TRIDENT_IMUMessageRequest;
    protocol_GRAMessageRequest_S TRIDENT_GRAMessageRequest;
    protocol_DROMessageRequest_S TRIDENT_DROMessageRequest;
    protocol_TORMessageRequest_S TRIDENT_TORMessageRequest;
    protocol_HYDMessageRequest_S TRIDENT_HYDMessageRequest;
    protocol_ISOTP_S            TRIDENT_MCISOTP; // Sent by TRIDENT, Received by: Motor Controller

    protocol_deviceName_S       MC_deviceName; // Sent by Motor Controller, Received by TRIDENT
    protocol_motorRPM_S         MC_motorRPMLow; // Sent by Motor Controller, Received by TRIDENT
    protocol_motorRPM_S         MC_motorRPMHigh; // Sent by Motor Controller, Received by TRIDENT
    protocol_MCEnvData_S        MC_envData; // Sent by Motor Controller, Received by TRIDENT
    protocol_MCBoardData_S      MC_boardData; // Sent by Motor Controller, Received by TRIDENT
    protocol_ISOTP_S            MC_ISOTP; // Sent by Motor Controller, Received by TRIDENT

    protocol_deviceName_S       PB_deviceName; // Sent by Power Board, Received by TRIDENT
    protocol_PBEnvData_S        PB_envData; // Sent by Power Board, Received by TRIDENT
    protocol_PBBattVoltages_S   PB_battVoltages; // Sent by Power Board, Received by TRIDENT
    protocol_PBBattCurrents_S   PB_battCurrents; // Sent by Power Board, Received by TRIDENT
    protocol_PBPeriphCurrents_S PB_mcCurrents; // Sent by Power Board, Received by TRIDENT
    protocol_PBPeriphCurrents_S PB_jetsonCurrents; // Sent by Power Board, Received by TRIDENT
    protocol_PBPeriphCurrents_S PB_lcdCurrents; // Sent by Power Board, Received by TRIDENT
    protocol_PBPeriphCurrents_S PB_hydrophoneCurrent; // Sent by Power Board, Received by TRIDENT
    protocol_PBPeriphCurrents_S PB_dropperCurrent; // Sent by Power Board, Received by TRIDENT
    protocol_PBPeriphCurrents_S PB_grabberCurrent; // Sent by Power Board, Received by TRIDENT
    protocol_PBPeriphCurrents_S PB_torpdeoCurrent; // Sent by Power Board, Received by TRIDENT
    protocol_PBPeriphCurrents_S PB_dvlCurrent; // Sent by Power Board, Received by TRIDENT
    protocol_PBPeriphCurrents_S PB_imuCurrent; // Sent by Power Board, Received by TRIDENT
    protocol_ISOTP_S            PB_ISOTP; // Sent by Power Board, Received by TRIDENT

    protocol_deviceName_S       LCD_deviceName; // Sent by LCD Board, Received by TRIDENT
    protocol_PBEnvData_S        LCD_state; // Sent by LCD Board, Received by TRIDENT
    protocol_ISOTP_S            LCD_ISOTP; // Sent by LCD Board, Received by TRIDENT

    protocol_deviceName_S       DVL_deviceName; // Sent by DVL Board, Received by TRIDENT
    protocol_DVL_PD6_S	        DVL_systemAltitude; // Sent by DVL Board, Received by TRIDENT
    protocol_DVL_PD6_S	        DVL_timingAndScaling; // Sent by DVL Board, Received by TRIDENT
    protocol_DVL_PD6_S 	        DVL_waterMassInstrumentRefVelocity; // Sent by DVL Board, Received by TRIDENT
    protocol_DVL_PD6_S          DVL_bottomTrackInstrumentRefVelocity; // Sent by DVL Board, Received by TRIDENT
    protocol_DVL_PD6_S          DVL_waterMassShipRefVelocity; // Sent by DVL Board, Received by TRIDENT
    protocol_DVL_PD6_S          DVL_bottomTrackShipRefVelocity; // Sent by DVL Board, Received by TRIDENT
    protocol_DVL_PD6_S          DVL_waterMassEarthRefVelocity;// Sent by DVL Board, Received by TRIDENT
    protocol_DVL_PD6_S          DVL_bottomTrackEarthRefVelocity; // Sent by DVL Board, Received by TRIDENT
    protocol_DVL_PD6_S          DVL_waterMassEarthRefDistance; // Sent by DVL Board, Received by TRIDENT
    protocol_DVL_PD6_S          DVL_bottomTrackEarthRefDistance; // Sent by DVL Board, Received by TRIDENT
    protocol_ISOTP_S            DVL_ISOTP; // Sent by DVL Board, Received by TRIDENT

    protocol_deviceName_S       HYD_deviceName; // Sent by Hydrophone Board, Received by TRIDENT
    protocol_ISOTP_S            HYD_ISOTP; // Sent by Hydrophone Board, Received by TRIDENT

    protocol_deviceName_S       TOR_deviceName; // Sent by Torpedo Board, Received by TRIDENT
    protocol_TORstate_S         TOR_state; // Sent by Torpedo Board, Received by TRIDENT
    protocol_ISOTP_S            TOR_ISOTP; // Sent by Torpedo Board, Received by TRIDENT

    protocol_deviceName_S       DRO_deviceName; // Sent by Dropper Board, Received by TRIDENT
    protocol_dropperState_S     DRO_state; // Sent by Dropper Board, Received by TRIDENT
    protocol_ISOTP_S            DRO_ISOTP; // Sent by Dropper Board, Received by TRIDENT

    protocol_deviceName_S       GRA_deviceName; // Sent by Grabber Board, Received by TRIDENT
    protocol_grabbberState_S    GRA_state; // Sent by Grabber Board, Received by TRIDENT
    protocol_ISOTP_S            GRA_ISOTP; // Sent by Dropper Board, Received by TRIDENT

    protocol_deviceName_S       IMU_deviceName; // Sent by IMU Board, Received by TRIDENT
    protocol_IMUData_S          IMU_orientation; // Sent by IMU Board, Received by TRIDENT
    protocol_IMUEnvData_S       IMU_envData; // Sent by IMU Board, Received by TRIDENT
    protocol_ISOTP_S            IMU_ISOTP; // Sent by Dropper Board, Received by TRIDENT
  
} protocol_allMessages_U;

typedef struct __attribute__((packed))
{
    protocol_MID_E messageID;
    protocol_allMessages_U  message;
} protocol_message_S;

// Asserts
uint8_t assert_protocol_maxMessageSize[(sizeof(protocol_allMessages_U) > PROTOCOL_MAX_MESSAGE_SIZE) ? -1 : 1];
uint8_t assert_protocol_messageIDSize[(sizeof(protocol_MID_E) == 1U) ? 1 : -1];

#endif // PROTOCOL_H_
