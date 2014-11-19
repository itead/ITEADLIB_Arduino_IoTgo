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
#include "THSensorInterface.h"

/**
 * THSensor, subclass of IoTgo, provides simple methods to read temperature and 
 * humidity from devices. 
 */
class THSensor: public IoTgo
{
public: /* public methods */
    THSensor(THSensorInterface *sensor);
    int32_t begin(void);
    int32_t end(void);
    int32_t sync(void);
    int32_t getTemperatureC(float *temp_c);
    int32_t getTemperatureF(float *temp_f);
    int32_t getHumidity(float *hum);
    int32_t getAll(float *temp_c, float *temp_f, float *hum);
    
private: /* private data */
    THSensorInterface *sensor;
};

#endif /* #ifndef __THSENSOR_H__ */
