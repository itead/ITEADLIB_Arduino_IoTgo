/**
 * @file THSensor.h
 *
 * THSensor API of IoTgo (iotgo.iteadstudio.com)
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/17
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
 
#ifndef __THSENSOR_H__
#define __THSENSOR_H__

#include "IoTgo.h"

#ifndef THSENSOR_USE_SHT1X_LIBRARY
#define THSENSOR_USE_SHT1X_LIBRARY
#endif

#ifdef THSENSOR_USE_SHT1X_LIBRARY
#include "SHT1x.h"
#endif

/**
 * THSensor, subclass of IoTgo, provides simple methods to read temperature and 
 * humidity from devices. 
 */
class THSensor: public IoTgo
{
public: /* public methods */
#ifdef THSENSOR_USE_SHT1X_LIBRARY
    THSensor(uint16_t data_pin, uint16_t clock_pin);
#endif
    int32_t sync(void);
    int32_t getTemperatureC(float *temp_c);
    int32_t getTemperatureF(float *temp_f);
    int32_t getHumidity(float *hum);
    int32_t getAll(float *temp_c, float *temp_f, float *hum);
    
private: /* private data */
#ifdef THSENSOR_USE_SHT1X_LIBRARY
    SHT1x sht1x_dev;
#endif
};

#endif /* #ifndef __THSENSOR_H__ */
