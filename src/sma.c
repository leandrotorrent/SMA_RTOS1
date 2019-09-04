/*=============================================================================
 * Copyright (c) 2019, SMA Project
 * Leandro Torrent <leandrotorrent92@gmail.com>
 * Miguel del Valle <m.e.delvallecamino@ieee.org>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/07/27
 * Version: 1.0
 *
 * Trabajo Final para Protocolos de Comunicacion en Sistemas Embebidos y RTOS 1
 *
 * Para usar el almacenamiento de datos en SD,descomentar la siguiente tarea del main:
 *
 * xTaskCreate( sdTask,(const char *) "Sd",
          configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 3, NULL );
 *
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"
#include "ff.h"
#include "fssdc.h"

/* Demo includes. */
#include "supporting_functions.h"
#include "sapi.h"
#include "sma.h"
#include "tareas.h"






/*-----------------------------------------------------------*/
int main( void )
{
	/* The queue is created to hold a maximum of 5 long values. */
	xQueueDatos = xQueueCreate( 2, sizeof( Data_t ) );

	boardConfig(); // Inicializar y configurar la plataforma
	uartConfig( UART_USB, 115200 ); // Inicializar periferico UART_USB
	i2cInit(OLED_I2C_PORT, OLED_I2C_RATE);

	rtc.year = 2019;
	rtc.month = 9;
	rtc.mday = 4;
	rtc.wday = 3;
	rtc.hour = 16;
	rtc.min = 41;
	rtc.sec= 0;
	rtcInit();
	rtcWrite( &rtc );


	//oledInit();

	if( xQueueDatos != NULL ) {


		xTaskCreate( am2301Task, (const char *) "am2301",
				configMINIMAL_STACK_SIZE * 2, ( void * ) 2000,
				tskIDLE_PRIORITY + 2, NULL );

		xTaskCreate( mq2Task, (const char *) "mq2",
				configMINIMAL_STACK_SIZE * 2, ( void * ) 2500,
				tskIDLE_PRIORITY + 2, NULL );


		/* Create the task that will read from the queue.  The task is created with
      priority 2, so above the priority of the sender tasks. */
		xTaskCreate( vReceiverTask,(const char *) "Receiver",
				configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL );

		/* Start the scheduler so the created tasks start executing. */
		vTaskStartScheduler();

	} else {
		/* The queue could not be created. */
	}

	/* The following line should never be reached because vTaskStartScheduler()
   will only return if there was not enough FreeRTOS heap memory available to
   create the Idle and (if configured) Timer tasks.  Heap management, and
   techniques for trapping heap exhaustion, are described in the book text. */
	for( ;; );
	return 0;
}
