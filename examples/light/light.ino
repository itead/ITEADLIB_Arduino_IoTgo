/**
 * @example light.ino
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

#include <IoTgo.h>

#define LIGHT_ID          "5580000003"
#define LIGHT_CHECK_CODE  "86f32f24-70bc-44e0-82f2-e95da6f8cde5"
#define WIFI_SSID       "ITEAD"
#define WIFI_PASS       "12345678"
#define IOT_SERVER      "172.16.7.6"

IoTgo light;

void printBody(const char *buffer)
{
    if (buffer != NULL)
    {
        buffer = strstr(buffer, "{");
        if (buffer == NULL)
        {
            Serial.println("no \"{\" found!");
            return;
        }
        Serial.print("[");
        Serial.print(buffer);
        Serial.println("]");
    }
}

void setup()
{
    const char *apikey;
    light.setServer(IOT_SERVER);
    if (!light.connectWiFi(WIFI_SSID, WIFI_PASS))
    {
        Serial.println("connectWiFI error and halt...");
        while(1);
    }

    Serial.println("Connecting device to server...");
    apikey = light.init(LIGHT_ID, LIGHT_CHECK_CODE);
    Serial.print("apikey = ");
    Serial.println(apikey);    
    
    Serial.println("setup done.");
}


void loop()
{
    static int counter = 0;
    Serial.print("\ncounter = ");
    Serial.println(++counter);
    
    const char *light_state[] = 
    {
        "state",
        NULL
    };
    const char *light_on[] = 
    {
        "on",
        NULL
    };
    const char *light_off[] = 
    {
        "off",
        NULL
    };
    
    const char *ret;

    Serial.println("update to on");
    ret = light.update(light_state, light_on);
    Serial.print("light.update =  ");
    printBody(ret);

    ret = light.query(light_state);
    Serial.print("light.query =   ");
    printBody(ret);

    Serial.println("update to off");
    ret = light.update(light_state, light_off);
    Serial.print("light.update =  ");
    printBody(ret);

    ret = light.query(NULL);
    Serial.print("light.query =   ");
    printBody(ret);

    delay(1000);
}


