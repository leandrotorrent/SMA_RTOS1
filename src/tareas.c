
#include "sma.h"
#include "sapi.h"
#include "mq2.h"
#include "oled.h"




void am2301Task( void *pvParameters ){

	BaseType_t xStatus;
	real32_t humidity = 0, temperature = 0;
	dht11Init( GPIO1 ); // Inicializo el sensor DHT11

	for( ;; ) {
		if(dht11Read(&humidity, &temperature)){

			gpioWrite( LEDG, ON );
			gpioWrite( LEDR, OFF );

			xStructsToSend[0].valor1 = temperature;
			xStructsToSend[0].valor2 = humidity;
			xStructsToSend[0].fuente = AM2301;

			xStatus = xQueueSendToBack( xQueueDatos, &xStructsToSend[0], 0 );

			if( xStatus != pdPASS ) {

				vPrintString( "Could not send to the queue.\r\n" );
			}
		}
		else {
			gpioWrite( LEDG, OFF );
			gpioWrite( LEDR, ON );
		}
		vTaskDelay( 1000 / portTICK_RATE_MS );
	}
}

void mq2Task( void *pvParameters ){

	BaseType_t xStatus;


	for( ;; ) {

		xStructsToSend[1].valor1 =  MQ2MQGetGasPercentage(MQ2MQRead()/10, 2)/10000;
		xStructsToSend[1].valor2 = 0;
		xStructsToSend[1].fuente = MQ2;

		xStatus = xQueueSendToBack( xQueueDatos, &xStructsToSend[1], 0 );

		if( xStatus != pdPASS ) {

			vPrintString( "Could not send to the queue.\r\n" );
		}
		vTaskDelay( 2500 / portTICK_RATE_MS );
	}

}

void vReceiverTask( void *pvParameters ){

	Data_t receivedStruct;
	BaseType_t xStatus;
	const TickType_t xTicksToWait = pdMS_TO_TICKS( 100UL );
	static char uartBuff[10];
	oledInit();


	for( ;; ) {

		if( uxQueueMessagesWaiting( xQueueDatos ) != 0 ) {
			vPrintString( "Queue should have been empty!\r\n" );
		}


		xStatus = xQueueReceive( xQueueDatos, &receivedStruct, xTicksToWait );

		if( xStatus == pdPASS ) {
			if(receivedStruct.fuente==AM2301){
				uartWriteString( UART_USB, "Temperatura: " );
				floatToString( receivedStruct.valor1, uartBuff, 2 );
				uartWriteString( UART_USB, uartBuff);
				uartWriteString( UART_USB, " grados C\r\n" );

				SSD1306_DrawText(0,10, "Temp: " , 1);
				SSD1306_DrawText(30,10, uartBuff , 1);
				SSD1306_DrawText(60,10, " C " , 1);
				SSD1306_Display();

				uartWriteString( UART_USB, "Humedad: " );
				floatToString( receivedStruct.valor2, uartBuff, 4 );
				uartWriteString( UART_USB, uartBuff);
				uartWriteString( UART_USB, " %\r\n" );

				SSD1306_DrawText(0,20, "Hum: " , 1);
				SSD1306_DrawText(30,20, uartBuff , 1);
				SSD1306_DrawText(60,20, " % " , 1);
				SSD1306_Display();
			}
			else {
				uartWriteString( UART_USB, "Humo: " );
				floatToString( receivedStruct.valor1, uartBuff, 4 );
				uartWriteString( UART_USB, uartBuff);
				uartWriteString( UART_USB, " % \r\n" );

				SSD1306_DrawText(0,30, "Humo: " , 1);
				SSD1306_DrawText(30,30, uartBuff , 1);
				SSD1306_DrawText(60,30, " % " , 1);
				SSD1306_Display();

			}

		} else {

			vPrintString( "Could not receive from the queue.\r\n" );
		}


	}

}
