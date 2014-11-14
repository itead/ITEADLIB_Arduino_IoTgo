/**
 * @example IoT_LED.ino
 *
 * Show how to use API of ITEADIoT Library. 
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/13
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include <ITEADIoT.h>

#define LED_ID          "5580000003"
#define LED_CHECK_CODE  "86f32f24-70bc-44e0-82f2-e95da6f8cde5"
#define WIFI_SSID       "ITEAD"
#define WIFI_PASS       "12345678"
#define IOT_SERVER      "172.16.7.6"

ITEADIoT led;

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
    const char *ret;
    led.setServer(IOT_SERVER);
    if (!led.connectWiFi(WIFI_SSID, WIFI_PASS))
    {
        Serial.println("connectWiFI error and halt...");
        while(1);
    }

    Serial.println("Connecting device to server...");
    ret = led.init(LED_ID, LED_CHECK_CODE);
    Serial.print("led.init = ");
    printBody(ret);
    
    Serial.println("setup done.");
}


void loop()
{
    static int counter = 0;
    Serial.print("\ncounter = ");
    Serial.println(++counter);
    
    const char *led_state[] = 
    {
        "state",
        NULL
    };
    const char *led_on[] = 
    {
        "on",
        NULL
    };
    const char *led_off[] = 
    {
        "off",
        NULL
    };
    
    const char *ret;

    Serial.println("update to on");
    ret = led.update(led_state, led_on);
    Serial.print("led.update =  ");
    printBody(ret);

    ret = led.query(led_state);
    Serial.print("led.query =   ");
    printBody(ret);

    Serial.println("update to off");
    ret = led.update(led_state, led_off);
    Serial.print("led.update =  ");
    printBody(ret);

    ret = led.query(NULL);
    Serial.print("led.query =   ");
    printBody(ret);

    delay(1000);
}


