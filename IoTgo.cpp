/**
 * @file IoTgo.cpp
 *
 * API of IoTgo (iotgo.iteadstudio.com)
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/11
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include "IoTgo.h"

IoTgo::IoTgo(void)
{
    memset(buffer, '\0', sizeof(buffer));
    memset(apikey, '\0', sizeof(apikey));
    memset(device_id, '\0', sizeof(device_id));
    strcpy(server, "172.16.7.6");
}

IoTgo::~IoTgo(void)
{
    /* Do nothing for now! */
}

/**
 * Set IoT Server. 
 * 
 * @param server - IP address of server
 * @return void
 */
void IoTgo::setServer(const char *server)
{
    strcpy(this->server, server);
}

/**
 * Connect to WiFi. 
 * 
 * @param ssid - WiFi's SSID (Name of WiFi access point)
 * @param password - Passward of SSID
 * @retval true - if connected
 * @retval false - if failed
 */
bool IoTgo::connectWiFi(const char *ssid, const char *password)
{
    wifi.begin();
    if(!wifi.Initialize(STA, String(ssid), String(password)))
    {
        DebugSerial.print("connect to ");
        DebugSerial.print(ssid);
        DebugSerial.println(" failed!");
        return false;
    }
    DebugSerial.print("Getting IP from ");
    DebugSerial.print(ssid);
    DebugSerial.println(". Please wait ...");
    delay(5000);
    DebugSerial.println(wifi.showIP());

    return true;
}

/**
 * Send http request to iotgo.iteadstudio.com and get the response. 
 * 
 * @param http_body - the body of http request
 * @param buffer - the buffer to store the response of this request
 * @param len - the length of buffer
 *
 * @return the pointer of response buffer terminated with '\0', if success. NULL, if falied!
 *
 * @warning 
 *  You must deal with the response in buffer BEFORE next calling of this function as
 *  the buffer is shared. 
 */
const char * IoTgo::request(const char *http_body, char *const buffer, int32_t len)
{
    static int32_t counter = 0;
    static bool connectTCP = false;
    
    int32_t time_delay = 0; 
    int32_t reconnectTCP = 0;
    
    if (buffer == NULL)
    {
        DebugSerial.println("buffer cannot be NULL!");
        return NULL;
    }
    
    buffer[0] = '\0';
    
request_reconnect:    
    if (connectTCP == false && wifi.ipConfig(TCP, server, 80))
    {
        connectTCP = true;
    } 
    if (connectTCP == false)
    {
        DebugSerial.println("Cannot connect to server");
        return NULL;
    }
    
    if (connectTCP == true) 
    {
        String http_req;
        /* Request line */
        http_req = "POST /api/http HTTP/1.1\r\n";
        /* Http header */
        http_req += "Host: iotgo.iteadstudio.com\r\n";
        http_req += "Content-Type: application/json\r\n";
        http_req += "Content-Length: " + String(strlen(http_body));
        http_req += "\r\n\r\n";
        /* Http body */
        http_req += http_body;
#if 0        
        DebugSerial.print("http_req=[");
        DebugSerial.print(http_req);
        DebugSerial.println("]");
#endif        
        bool sent = wifi.Send(http_req);
        if (sent == true)
        {
            //DebugSerial.println("Request Sent Successfully!");
        }
        else
        {
            if ((reconnectTCP++) < 5)
            {
                connectTCP = false;
                goto request_reconnect;
            }
            else
            {
                DebugSerial.println("Request Sending Failed!");
                return NULL;
            }
        }
        
        /* Waiting for response for 10 seconds */ 
        time_delay = 1000;
        while (time_delay--)
        {   
            if (wifi.ReceiveMessage(buffer))
            {
                //DebugSerial.print("Received:\n");
                //DebugSerial.print(buffer);
                break;
            }
            delay(10);
        }
        if (time_delay <= 0)
        {
            DebugSerial.println("Request timeout!");
            return NULL;
        }
    }
    
    buffer[len - 1] = '\0';
    
    //DebugSerial.print("request counter=");
    //DebugSerial.println(++counter);
    //DebugSerial.print("time_delay=");
    //DebugSerial.println(time_delay);
    //DebugSerial.print("strlen(buffer)=");
    //DebugSerial.println(strlen(buffer));
    
    return buffer;
}

/**
 * Get the apikey of device specified by device_id and check_code. 
 * 
 * You must call this before update and query. 
 *
 * @param device_id - device identifier
 * @param apikey - just like a password for accessing the respective device
 * @param device_type - indicate the type of your device from DEVICE_DIY or DEVICE_PRODUCT. 
 *  For developers, device_flag=DEVICE_DIY(default). For users, device_flag=DEVICE_PRODUCT.
 *
 * @return the apikey terminated with '\0', if success. NULL, if falied!
 */
const char *IoTgo::init(const char *device_id, const char *apikey, IoTgoDeviceType device_type)
{
    char http_body[100];
    char *temp;
    const char *response;
    
    strcpy(this->device_id, device_id);
    
    if (device_type == DEVICE_PRODUCT)
    {
        /* Construct init http_body */
        strcpy(http_body, "{");
        strcat(http_body, "\"apikey\":");
        strcat(http_body, "\"");
        strcat(http_body, apikey);
        strcat(http_body, "\"");
        strcat(http_body, ",");
        strcat(http_body, "\"deviceid\":");
        strcat(http_body, "\"");
        strcat(http_body, this->device_id);
        strcat(http_body, "\"");
        strcat(http_body, ",");
        strcat(http_body, "\"action\":\"register\"");
        strcat(http_body, "}");
        
        /* {"error":0,"apikey":"d0555f12-a67c-4c54-9ee0-8f5b7f4268fa"} */
        
        response = request(http_body, buffer, sizeof(buffer));
        if (response == NULL)
        {
            DebugSerial.println("request failed!");
            return NULL;
        }
        
        temp = strstr(response, "apikey");
        if (temp == NULL)
        {
            DebugSerial.println("Cannot find \"apikey\" from response!");
            return NULL;
        }
        else
        {
            temp += 7;
            char *apikey_index = strstr(temp, "\"");
            if (apikey_index != NULL)
            {
                apikey_index += 1;
                strncpy(this->apikey, apikey_index, APIKEY_LEN);
                //DebugSerial.print("apikey=");
                //DebugSerial.println(this->apikey);
            }
        }
        return this->apikey;
    }
    else if (device_type == DEVICE_DIY)
    {
        strncpy(this->apikey, apikey, APIKEY_LEN);
        return this->apikey;
    }
    else
    {
        DebugSerial.println("Unknown device flag!");
        return NULL;
    }
}

/**
 * Get the values specified by params from server. 
 * 
 * You must call this after init. 
 *
 * @param params - the list(terminated with NULL) of attributes need to query. 
 *
 * @return the pointer of response buffer terminated with '\0', if success. NULL, if falied!
 */
const char *IoTgo::query(const char *params[])
{
    char http_body[100+IOT_BUFFER_SIZE];
    int32_t i;
    
    /* Construct query http_body */
    strcpy(http_body, "{");
    strcat(http_body, "\"apikey\":");
    strcat(http_body, "\"");
    strcat(http_body, this->apikey);
    strcat(http_body, "\"");
    strcat(http_body, ",");
    strcat(http_body, "\"deviceid\":");
    strcat(http_body, "\"");
    strcat(http_body, this->device_id);
    strcat(http_body, "\"");
    strcat(http_body, ",");
    strcat(http_body, "\"action\":\"query\"");
    strcat(http_body, ",");
    strcat(http_body, "\"params\":[");
    for (i=0; params[i] != NULL; i++)
    {
        strcat(http_body, "\"");
        strcat(http_body, params[i]);
        strcat(http_body, "\"");
        if (params[i+1] != NULL)
        {
            strcat(http_body, ",");
        }
    }
    strcat(http_body, "]");
    strcat(http_body, "}");
    
    return request(http_body, buffer, sizeof(buffer));;
}

/**
 * Update the values specified by params to server. 
 * 
 * You must call this after init. 
 *
 * @param params - the list(terminated with NULL) of attributes need to update. 
 * @param values - the list(terminated with NULL) of values corresponding to 
 *                 items of attributes. 
 *
 * @return the pointer of response buffer terminated with '\0', if success. NULL, if falied!
 * 
 * @warning the number of items in params must be equal to that in values.
 */
const char *IoTgo::update(const char *params[], const char *values[])
{
    char http_body[100+IOT_BUFFER_SIZE];
    int32_t i;
    
    /* Construct update http_body */
    strcpy(http_body, "{");
    strcat(http_body, "\"apikey\":");
    strcat(http_body, "\"");
    strcat(http_body, this->apikey);
    strcat(http_body, "\"");
    strcat(http_body, ",");
    strcat(http_body, "\"deviceid\":");
    strcat(http_body, "\"");
    strcat(http_body, this->device_id);
    strcat(http_body, "\"");
    strcat(http_body, ",");
    strcat(http_body, "\"action\":\"update\"");
    strcat(http_body, ",");
    strcat(http_body, "\"params\":{");
    for (i=0; (params[i] != NULL && values[i] != NULL); i++)
    {
        strcat(http_body, "\"");
        strcat(http_body, params[i]);
        strcat(http_body, "\"");
        strcat(http_body, ":");
        strcat(http_body, "\"");
        strcat(http_body, values[i]);
        strcat(http_body, "\"");
        if (params[i+1] != NULL && values[i+1] != NULL)
        {
            strcat(http_body, ",");
        }
    }
    strcat(http_body, "}");
    strcat(http_body, "}");

    return request(http_body, buffer, sizeof(buffer));
}
