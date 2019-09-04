/*=============================================================================
 * Copyright (c) 2019, SMA Project
 * Leandro Torrent <leandrotorrent92@gmail.com>
 * Miguel del Valle <m.e.delvallecamino@ieee.org>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Date: 2019/07/27
 * Version: 1.0
 *===========================================================================*/
/*=====[Inclusion of own header]=============================================*/
//#include "userTasks.h"
#include "mq2.h"
#include <math.h>
#include "sapi.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/
int _pin;
int RL_VALUE = 5;
//define the load resistance on the board, in kilo ohms
int RO_CLEAN_AIR_FACTOR = 9.83;
int CALIBARAION_SAMPLE_TIMES = 5;
int CALIBRATION_SAMPLE_INTERVAL = 50;
int READ_SAMPLE_INTERVAL = 50;
int READ_SAMPLE_TIMES = 5;

real32_t LPGCurve[3] = { 2.3, 0.21, -0.47 };
real32_t COCurve[3] = { 2.3, 0.72, -0.34 };
real32_t SmokeCurve[3] = { 2.3, 0.53, -0.44 };
real32_t Ro = 10;
uint8_t GAS_LPG = 0;
uint8_t GAS_CO = 1;
uint8_t GAS_SMOKE = 2;

real32_t lpg = 0;
real32_t co = 0;
real32_t smoke = 0;
/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/
void mq2Begin() {
	Ro = MQ2MQCalibration();
}

real32_t* mq2Read(bool_t print) {

	lpg = MQ2MQGetGasPercentage(MQ2MQRead() / Ro, GAS_LPG);
	co = MQ2MQGetGasPercentage(MQ2MQRead() / Ro, GAS_CO);
	smoke = MQ2MQGetGasPercentage(MQ2MQRead() / Ro, GAS_SMOKE);

	// Send the task to the locked state for 10 ms (delay)
	vTaskDelay(10 / portTICK_RATE_MS);

	real32_t values[3] = { lpg, co, smoke };
	return values;
}

real32_t mq2ReadLPG() {
	/*if (millis()<(lastReadTime + 10000) && lpg != 0){
	 return lpg;
	 }else{*/
	return lpg = MQ2MQGetGasPercentage(MQ2MQRead() / 10, GAS_LPG);
	//}
}

real32_t mq2ReadCO() {
	/*if (millis()<(lastReadTime + 10000) && co != 0){
	 return co;
	 }else{*/
	return co = MQ2MQGetGasPercentage(MQ2MQRead() / 10, GAS_CO);
	//}
}

real32_t mq2ReadSmoke() {
	return smoke = MQ2MQGetGasPercentage(MQ2MQRead() / 10, GAS_SMOKE);
}

real32_t MQ2MQResistanceCalculation(int raw_adc) {
	return (((real32_t) RL_VALUE * (1023 - raw_adc) / raw_adc));
}

real32_t MQ2MQCalibration() {
	real32_t val = 0;

	for (int i = 0; i < CALIBARAION_SAMPLE_TIMES; i++) {
		//take multiple samples
		val += MQ2MQResistanceCalculation(adcRead(CH1));
		//delay(CALIBRATION_SAMPLE_INTERVAL);
		vTaskDelay( CALIBRATION_SAMPLE_INTERVAL / portTICK_RATE_MS);
	}
	val = val / CALIBARAION_SAMPLE_TIMES;
	//calculate the average value

	val = val / RO_CLEAN_AIR_FACTOR;
	//divided by RO_CLEAN_AIR_FACTOR yields the Ro
	//according to the chart in the datasheet
	return val;
}

real32_t MQ2MQRead() {
	int i;
	real32_t rs = 0;
	uint16_t val = adcRead(CH1); //reemplazar por uint16

	for (i = 0; i < READ_SAMPLE_TIMES; i++) {
		rs += MQ2MQResistanceCalculation(val);
		//delay(READ_SAMPLE_INTERVAL);
		// Send the task to the locked state for 1 s (delay)
		//vTaskDelay(READ_SAMPLE_INTERVAL / portTICK_RATE_MS);
	}

	rs = rs / READ_SAMPLE_TIMES;
	return rs;
}
real32_t MQ2MQGetGasPercentage(real32_t rs_ro_ratio, uint8_t gas_id) {
	if (gas_id == GAS_LPG) {
		return MQ2MQGetPercentage(rs_ro_ratio, LPGCurve);
	} else if (gas_id == GAS_CO) {
		return MQ2MQGetPercentage(rs_ro_ratio, COCurve);
	} else if (gas_id == GAS_SMOKE) {
		return MQ2MQGetPercentage(rs_ro_ratio, SmokeCurve);
	}
	return 0;
}
int MQ2MQGetPercentage(real32_t rs_ro_ratio, real32_t *pcurve) {
	return (pow(10, (((log(rs_ro_ratio) - pcurve[1]) / pcurve[2]) + pcurve[0])));
}

