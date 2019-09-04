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

#ifndef __MQ2_H__
#define __MQ2_H__

/*=====[Inclusions of public function dependencies]==========================*/
#include "sapi.h"
/*=====[C++ - begin]=========================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*=====[Definition macros of public constants]===============================*/

/*=====[Public function-like macros]=========================================*/

/*=====[Definitions of public data types]====================================*/

/*=====[Prototypes (declarations) of public functions]=======================*/

float* mq2Read(bool_t print);
float mq2ReadLPG();
float mq2ReadCO();
float mq2ReadSmoke();
void mq2Begin();

float MQ2MQRead();
real32_t MQ2MQGetGasPercentage(real32_t rs_ro_ratio, uint8_t gas_id);
int MQ2MQGetPercentage(float rs_ro_ratio, float *pcurve);
float MQ2MQCalibration();
float MQ2MQResistanceCalculation(int raw_adc);

/*=====[Prototypes (declarations) of public interrupt functions]=============*/

/*=====[C++ - end]===========================================================*/

#ifdef __cplusplus
}
#endif

/*=====[Avoid multiple inclusion - end]======================================*/

#endif /* __MQ2_H__ */
