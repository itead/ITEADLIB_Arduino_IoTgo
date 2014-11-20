/**
 * @example temp_hum_sensor.ino
 *
 * Show how to use API of This Library. 
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/14
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
#include <IoTgo_device.h>

#define THSENSOR_ID            "0300000001"
#define THSENSOR_CHECK_CODE    ""
#define THSENSOR_APIKEY        "9bed6c2c-fbc6-4400-9ef8-087f9f0d996e"
#define WIFI_SSID           "ITEAD"
#define WIFI_PASS           "12345678"
#define IOT_SERVER          "172.16.7.6"

#define THSENSOR_DATA_PIN    (6)
#define THSENSOR_CLOCK_PIN   (7)

SHT1x sht1x(THSENSOR_DATA_PIN, THSENSOR_CLOCK_PIN);
THSensor th(&sht1x);

void setup()
{
    const char *apikey;
    Serial.begin(DEBUG_BAUD_RATE);
    th.begin();
    th.setServer(IOT_SERVER);
    if (!th.connectWiFi(WIFI_SSID, WIFI_PASS))
    {
        Serial.println("connectWiFI error and halt...");
        while(1);
    }

    Serial.println("Connecting device to server...");
    apikey = th.init(THSENSOR_ID, THSENSOR_APIKEY);
    //apikey = th.init(THSENSOR_ID, THSENSOR_APIKEY, DEVICE_PRODUCT);
    Serial.print("apikey = ");
    Serial.println(apikey);    
    
    Serial.println("setup done.");
}

void loop()
{
    static int32_t counter = 0;
    Serial.print("\ncounter = ");
    Serial.println(++counter);
    int32_t ret;
    float temp_c;
    float temp_f;
    float hum;
    
    ret = th.sync();
    if (ret)
    {
        Serial.print("ERROR: sync() = ");
        Serial.println(ret);
    }
    else
    {
        Serial.println("sync successfully!");
    }

    ret = th.getAll(&temp_c, &temp_f, &hum);
    if (ret)
    {
        Serial.print("ERROR: getAll() = ");
        Serial.println(ret);
    }
    {
        Serial.print("TempC = ");
        Serial.print(temp_c, 2);
        Serial.print(", TempF = ");
        Serial.print(temp_f, 2);
        Serial.print(", Hum = ");
        Serial.println(hum, 2);
    }
    
    ret = th.getTemperatureC(&temp_c);
    if (ret)
    {
        Serial.print("ERROR: getTemperatureC() = ");
        Serial.println(ret);
    }
    {
        Serial.print("TempC = ");
        Serial.println(temp_c, 2);
    }
    
    ret = th.getTemperatureF(&temp_f);
    if (ret)
    {
        Serial.print("ERROR: getTemperatureF() = ");
        Serial.println(ret);
    }
    {
        Serial.print("TempF = ");
        Serial.println(temp_f, 2);
    }

    ret = th.getHumidity(&hum);
    if (ret)
    {
        Serial.print("ERROR: getHumidity() = ");
        Serial.println(ret);
    }
    {
        Serial.print("Hum = ");
        Serial.println(hum, 2);
    }

    delay(1000);
}
