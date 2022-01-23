#ifndef ZERO_NETWORK_H
#define ZERO_NETWORK_H

#define ZERO_MESSAGE_LENGTH 8

/***************\
Define Buses
\***************/

#define  CONTROLLER "controller"
#define  BMS "bms"
#define  BMS1 "bms1"
#define  DASH "dash"
#define  CALEX "calex sds"

/***************\
Define Message Count
\***************/

#define TOTAL_MESSAGES 31
#define TOTAL_MESSAGES_DASH 5
#define TOTAL_MESSAGES_CONTROLLER 6
#define TOTAL_MESSAGES_CALEX 5
#define TOTAL_MESSAGES_BMS 8
#define TOTAL_MESSAGES_BMS1 8

/***************\
Create Types
\***************/

//Message
typedef struct {
  const char* name;
  const short id;
} Message;



/**************************\
Create Controller messages
\**************************/
//tpdos
const Message CONTROLLER_SPEED_TORQUE={"CONTROLLER_SPEED_TORQUE",0x181};
const Message CONTROLLER_RPM_THROTTLE_MOT_TEMP={"CONTROLLER_RPM_THROTTLE_MOT_TEMP",0x281};
const Message CONTROLLER_TEMP_DIGITAL_IN_18={"CONTROLLER_TEMP_DIGITAL_IN_18",0x381};
const Message CONTROLLER_VOLT_MOD_INDUCTANCE_TEMP_EST={"CONTROLLER_VOLT_MOD_INDUCTANCE_TEMP_EST",0x481};
const Message CONTROLLER_TARGET_VEL_MAX_IQ_CONT_VOLT={"CONTROLLER_TARGET_VEL_MAX_IQ_CONT_VOLT",0x501};

//rpdos
const Message CONTROLLER_DRIVE_CONTROL={"CONTROLLER_DRIVE_CONTROL",0x205};

/**************************\
Create dash messages
\**************************/
//tpdos
const Message DASH_STATUS={"DASH_STATUS",0x1C0};
const Message DASH_ODO_FROM_DASH={"ODO_FROM_DASH",0x2C0};

//rpdos
const Message DASH_STATUS2={"DASH_STATUS2",0x440};
//long trip 1 km * 100
//
//short range km * 100
//short temp C * 100

const Message DASH_STATUS3={"DASH_STATUS3",0x540};
// ? empty
//short  wh/km * 100
//short  trip av wh/km * 100
//short  life av wh/km * 100

const Message DASH_ODO_TO_DASH={"DASH_ODO_TO_DASH",0x3C0};



/**************************\
Create bms messages
\**************************/
//tpdos for bms 0
const Message BMS_PACK_STATUS={"BMS_PACK_STATUS",0x188};
const Message BMS_PACK_CONFIG={"BMS_PACK_CONFIG",0x288};
const Message BMS_PACK_STATS={"BMS_PACK_STATS",0x308};
const Message BMS_CELL_VOLTAGE={"BMS_CELL_VOLTAGE",0x388};
const Message BMS_PACK_ACTIVE_DATA={"BMS_PACK_ACTIVE_DATA",0x408};
const Message BMS_PACK_TEMP_DATA={"BMS_PACK_TEMP_DATA",0x488};
const Message BMS_PACK_TIME={"BMS_PACK_TIME",0x508};

//rpdos for bms 0
const Message BMS_CONTROL={"BMS_CONTROL",0x506};


/**************************\
Create bms1 messages
\**************************/
//tpdos for bms 1
const Message BMS1_PACK_STATUS={"BMS1_PACK_STATUS",0x189};
const Message BMS1_PACK_CONFIG={"BMS1_PACK_CONFIG",0x289};
const Message BMS1_PACK_STATS={"BMS1_PACK_STATS",0x309};
const Message BMS1_CELL_VOLTAGE={"BMS1_CELL_VOLTAGE",0x389};
const Message BMS1_PACK_ACTIVE_DATA={"BMS1_PACK_ACTIVE_DATA",0x409};
const Message BMS1_PACK_TEMP_DATA={"BMS1_PACK_TEMP_DATA",0x489};
const Message BMS1_PACK_TIME={"BMS1_PACK_TIME",0x509};

//rpdos for bms 1 same as BMS above




/**************************\
Create calex messages
\**************************/
//tpdos
const Message CALEX_CHARGE_STATUS={"CALEX_CHARGE_STATUS",0x192};
const Message T_CALEX_MAX_CHARGE_VOLTAGE_CURRENT={"T_CALEX_MAX_CHARGE_VOLTAGE_CURRENT",0x292};

//rpdos
const Message CALEX_CHARGE_CONTROL={"CALEX_CHARGE_CONTROL", 0x206};
const Message R_CALEX_MAX_CHARGE_VOLTAGE_CURRENT={"R_CALEX_MAX_CHARGE_VOLTAGE_CURRENT",0x306};
const Message CALEX_TAPER_CUTOFF_CURRENT={"CALEX_TAPER_CUTOFF_CURRENT",0x406};



/**************************\
Create Network
\**************************/
const Message Zero_Messages[TOTAL_MESSAGES] = {
  //6 controller
  CONTROLLER_SPEED_TORQUE,
  CONTROLLER_RPM_THROTTLE_MOT_TEMP,
  CONTROLLER_TEMP_DIGITAL_IN_18,
  CONTROLLER_VOLT_MOD_INDUCTANCE_TEMP_EST,
  CONTROLLER_TARGET_VEL_MAX_IQ_CONT_VOLT,
  CONTROLLER_DRIVE_CONTROL,

  //5 Dash
  DASH_STATUS,
  DASH_ODO_FROM_DASH,
  DASH_STATUS2,
  DASH_STATUS3,
  DASH_ODO_TO_DASH,

  //8 bms
  BMS_PACK_STATUS,
  BMS_PACK_CONFIG,
  BMS_PACK_STATS,
  BMS_CELL_VOLTAGE,
  BMS_PACK_ACTIVE_DATA,
  BMS_PACK_TEMP_DATA,
  BMS_PACK_TIME,
  BMS_CONTROL,

  //7 bms1
  BMS1_PACK_STATUS,
  BMS1_PACK_CONFIG,
  BMS1_PACK_STATS,
  BMS1_CELL_VOLTAGE,
  BMS1_PACK_ACTIVE_DATA,
  BMS1_PACK_TEMP_DATA,
  BMS1_PACK_TIME,

  //5 calex
  CALEX_CHARGE_STATUS,
  T_CALEX_MAX_CHARGE_VOLTAGE_CURRENT,
  CALEX_CHARGE_CONTROL,
  R_CALEX_MAX_CHARGE_VOLTAGE_CURRENT,
  CALEX_TAPER_CUTOFF_CURRENT
};

const Message Zero_Dash_Message[TOTAL_MESSAGES_DASH]{
  //5 Dash
  DASH_STATUS,
  DASH_ODO_FROM_DASH,
  DASH_STATUS2,
  DASH_STATUS3,
  DASH_ODO_TO_DASH
};

const Message Zero_Controller_Message[TOTAL_MESSAGES_CONTROLLER]{
  //6 controller
  CONTROLLER_SPEED_TORQUE,
  CONTROLLER_RPM_THROTTLE_MOT_TEMP,
  CONTROLLER_TEMP_DIGITAL_IN_18,
  CONTROLLER_VOLT_MOD_INDUCTANCE_TEMP_EST,
  CONTROLLER_TARGET_VEL_MAX_IQ_CONT_VOLT,
  CONTROLLER_DRIVE_CONTROL
};

const Message Zero_Calex_Message[TOTAL_MESSAGES_CALEX]{
  //5 calex
  CALEX_CHARGE_STATUS,
  T_CALEX_MAX_CHARGE_VOLTAGE_CURRENT,
  CALEX_CHARGE_CONTROL,
  R_CALEX_MAX_CHARGE_VOLTAGE_CURRENT,
  CALEX_TAPER_CUTOFF_CURRENT
};

const Message Zero_BMS_Message[TOTAL_MESSAGES_BMS]{
  //8 bms
  BMS_PACK_STATUS,
  BMS_PACK_CONFIG,
  BMS_PACK_STATS,
  BMS_CELL_VOLTAGE,
  BMS_PACK_ACTIVE_DATA,
  BMS_PACK_TEMP_DATA,
  BMS_PACK_TIME,
  BMS_CONTROL
};

const Message Zero_BMS1_Message[TOTAL_MESSAGES_BMS1]{
  //8 bms1
  BMS1_PACK_STATUS,
  BMS1_PACK_CONFIG,
  BMS1_PACK_STATS,
  BMS1_CELL_VOLTAGE,
  BMS1_PACK_ACTIVE_DATA,
  BMS1_PACK_TEMP_DATA,
  BMS1_PACK_TIME,
  BMS_CONTROL
};

#endif
