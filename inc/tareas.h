/*
 * tareas.h
 *
 *  Created on: 3 sep. 2019
 *      Author: leandro
 */

#ifndef MISPROGRAMAS_SMA_RTOS1_TORRENT_INC_TAREAS_H_
#define MISPROGRAMAS_SMA_RTOS1_TORRENT_INC_TAREAS_H_
#include "sma.h"

void am2301Task( void *pvParameters );
void mq2Task( void *pvParameters );
void vReceiverTask( void *pvParameters );

#endif /* MISPROGRAMAS_SMA_RTOS1_TORRENT_INC_TAREAS_H_ */
