/*=============================================================================
 * Copyright (c) 2019, SMA Project
 * Leandro Torrent <leandrotorrent92@gmail.com>
 * Miguel del Valle <m.e.delvallecamino@ieee.org>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/07/27
 * Version: 1.0
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __SMA_H__
#define __SMA_H__



/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/


#include "sapi.h"
#include "supporting_functions.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"


#define OLED_I2C_PORT 	I2C0
#define OLED_I2C_RATE 	400000
/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/
/* Declare a variable of type QueueHandle_t.  This is used to store the queue
that is accessed by all three tasks. */
QueueHandle_t xQueueDatos, xQueueSd;

typedef enum {
	AM2301,
	MQ2
} DataSource_t;

typedef struct {
	real32_t valor1;
	real32_t valor2;
	DataSource_t fuente;

} Data_t;

Data_t xStructsToSend[2];

/*=====[Prototypes (declarations) of public functions]=======================*/



/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __SMA_H__ */
