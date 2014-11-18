/**
 * @file THSensor.cpp
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

#include "THSensor.h"

#ifdef THSENSOR_USE_SHT1X_LIBRARY
THSensor::THSensor(uint16_t data_pin, uint16_t clock_pin)
    :sht1x_dev(data_pin, clock_pin)
{
}
#endif

/**
 * Read temperature and humidity from device then push them to server. 
 *
 * @retval 0 - success
 * @retval ERR_NO_RESPONSE - if no response from server. 
 * @retval ERR_NO_EXPECT - if no content expected in response. 
 * @retval ERR_NO_DEVICES_AVAILABLE - if no devices available. 
 */
int32_t THSensor::sync(void)
{
    char str_temp_c[8] = {'\0'};
    char str_hum[8] = {'\0'};
    char str_tmp[8] = {'\0'};
    const char *params[] = {"temperature", "humidity", NULL};
    const char *values[] = {str_temp_c, str_hum, NULL};
    const char *response;
    float temp_c;
    float hum;
    int32_t ret;
    char *str_error;
    int32_t len;
    
    ret = getAll(&temp_c, NULL, &hum);
    if (ret)
    {
        return ERR_NO_DEVICES_AVAILABLE;
    }
    
#ifdef DEBUG    
    DebugSerial.print("temp_c=");
    DebugSerial.println(temp_c, 2);
    DebugSerial.print("hum=");
    DebugSerial.println(hum, 2);
#endif

    temp_c += 0.005;
    hum += 0.005;
    
    snprintf(str_tmp, sizeof(str_tmp), "%d", (int)(temp_c*100));
    len = strlen(str_tmp);
    strncpy(str_temp_c, str_tmp, len-2);
    strcat(str_temp_c, ".");
    strcat(str_temp_c, &str_tmp[len-2]);
    
    snprintf(str_tmp, sizeof(str_tmp), "%d", (int)(hum*100));
    len = strlen(str_tmp);
    strncpy(str_hum, str_tmp, len-2);
    strcat(str_hum, ".");
    strcat(str_hum, &str_tmp[len-2]);
    
#ifdef DEBUG
    DebugSerial.print("str_temp_c=");
    DebugSerial.println(str_temp_c);
    DebugSerial.print("str_hum=");
    DebugSerial.println(str_hum);
#endif
    
    response = update(params, values);
    
    if(response)
    {  
        str_error = strstr(response, "\"error\":0");
        if (str_error)
        {
            return 0;
        }
        else
        {
            return ERR_NO_EXPECT;
        }
    }
    else
    {
        return ERR_NO_RESPONSE;
    }
}

/** 
 * Read temperature from device by Celsius. 
 * 
 * @param temp_c - the pointer to store temperature by Celsius. 
 *
 * @retval 0 - success
 * @retval ERR_INVALID_PARAMETER - if temp_c is NULL. 
 * @retval ERR_NO_DEVICES_AVAILABLE - if no devices available. 
 */
int32_t THSensor::getTemperatureC(float *temp_c)
{
    if (!temp_c)
    {
        return ERR_INVALID_PARAMETER;
    }

#ifdef THSENSOR_USE_SHT1X_LIBRARY
    if (temp_c)
    {
        *temp_c = sht1x_dev.readTemperatureC();
    }
    return 0;
#else
    return ERR_NO_DEVICES_AVAILABLE;
#endif

}

/** 
 * Read temperature from device by Fahrenheit. 
 * 
 * @param temp_f - the pointer to store temperature by Fahrenheit. 
 *
 * @retval 0 - success
 * @retval ERR_INVALID_PARAMETER - if temp_f is NULL. 
 * @retval ERR_NO_DEVICES_AVAILABLE - if no devices available. 
 */
int32_t THSensor::getTemperatureF(float *temp_f)
{
    if (!temp_f)
    {
        return ERR_INVALID_PARAMETER;
    }

#ifdef THSENSOR_USE_SHT1X_LIBRARY
    if (temp_f)
    {
        *temp_f = sht1x_dev.readTemperatureF();
    }
    return 0;
#else
    return ERR_NO_DEVICES_AVAILABLE;
#endif

}

/** 
 * Read humidity from device. 
 * 
 * @param hum - the pointer to store humidity. 
 *
 * @retval 0 - success
 * @retval ERR_INVALID_PARAMETER - if hum is NULL. 
 * @retval ERR_NO_DEVICES_AVAILABLE - if no devices available. 
 */
int32_t THSensor::getHumidity(float *hum)
{
    if (!hum)
    {
        return ERR_INVALID_PARAMETER;
    }
#ifdef THSENSOR_USE_SHT1X_LIBRARY
    if (hum)
    {
        *hum = sht1x_dev.readHumidity();
    }
    return 0;
#else
    return ERR_NO_DEVICES_AVAILABLE;
#endif

}

/** 
 * Read temperature and humidity from device. 
 * 
 * @param temp_c - the pointer to store temperature by Celsius. 
 * @param temp_f - the pointer to store temperature by Fahrenheit. 
 * @param hum - the pointer to store humidity. 
 *
 * @retval 0 - success
 * @retval ERR_INVALID_PARAMETER - if all parameters is NULL. 
 * @retval ERR_NO_DEVICES_AVAILABLE - if no devices available. 
 */
int32_t THSensor::getAll(float *temp_c, float *temp_f, float *hum)
{
    if (temp_c == NULL && temp_f == NULL && hum == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }
#ifdef THSENSOR_USE_SHT1X_LIBRARY
    if (temp_c)
    {
        *temp_c = sht1x_dev.readTemperatureC();
    }
    if (temp_f)
    {
        *temp_f = sht1x_dev.readTemperatureF();
    }
    if (hum)
    {
        *hum = sht1x_dev.readHumidity();
    }
    return 0;
#else
    return ERR_NO_DEVICES_AVAILABLE;
#endif
} 
