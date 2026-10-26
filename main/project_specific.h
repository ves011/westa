/*
 * project_specific.h
 *
 *  Created on: Mar 20, 2023
 *      Author: viorel_serbu
 */

/**
 * @file project_specific.h
 * @brief defines the controller type: ACTIVE_CONTROLLER and ID: CTRL_DEV_ID
 */
 
#ifndef MAIN_PROJECT_SPECIFIC_H_
#define MAIN_PROJECT_SPECIFIC_H_
#include "common_defines.h"

/** Communication protocol used by the device (e.g. MQTT) */
#define COMM_PROTO            		MQTT_PROTO
#define CONFIG_BROKER_URL 			"mqtts://proxy.gnet:1886"
#define MQTT_RECONNECT_TIMEOUT_MS	10000
#define ENABLE_MQTT_FAIL_REBOOT		1
#define MQTT_FAILATTEMPTS			10		//this means a rebbot is triggered if MQTT_FAILATTEMPTS in a row = 100 sec

/*
#define TEST_BUILD (1)
#if(TEST_BUILD == 1)
	#define WITH_CONSOLE
	#define TEST1
	#define CTRL_DEV_ID					(99)
#else
	#define CTRL_DEV_ID					(1)
#endif
*/
#define WITH_CONSOLE
#define OTA_SUPPORT

#define FILESYSTEM	LITTLEFS



#define ACTIVE_CONTROLLER			(WESTA_CONTROLLER)

#define WIFI_STA_ON 					(1)
#define MQTT_PUBLISH					(1)

#define DHT_DATA_PIN 			4
#define USE_I2C
#define I2C_MASTER_SCL_IO_0     5      					/*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO_0     1      					/*!< GPIO number used for I2C master data  */
#define I2C_MASTER_SCL_IO_1     99     					/*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO_1     99    					/*!< GPIO number used for I2C master data  */
#define RG_GPIO					7

//#define DEV_NAME					"Statia meteo"
#define PROMPT_STR 					"WESTA"
//#define PD_USER						"westa"

extern int controller_op_registered;

#endif /* MAIN_PROJECT_SPECIFIC_H_ */
