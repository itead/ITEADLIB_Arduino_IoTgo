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

/**
 * Constructor. 
 *
 * @param net - the pointer to object of subclass of NetInterface. 
 *
 * @see NetInterface
 */
IoTgo::IoTgo(NetInterface *net)
    :net(net)
{
    memset(buffer, '\0', sizeof(buffer));
    memset(apikey, '\0', sizeof(apikey));
    memset(device_id, '\0', sizeof(device_id));
    memset(ip, '\0', sizeof(ip));
    memset(domain_name, '\0', sizeof(domain_name));
}

/**
 * Set IoT Server. 
 * 
 * @param ip - IP address of server
 * @param domain_name - Domain name of server
 *
 * @return void
 */
void IoTgo::setHost(const char *ip, const char *domain_name)
{
    strncpy(this->ip, ip, sizeof(this->ip) - 1);
    strncpy(this->domain_name, domain_name, sizeof(this->domain_name) - 1);
}

/**
 * Send http request to host and get the response. 
 * 
 * @param http_body - the body of http request
 * @param buffer - the buffer to store the response of this request
 * @param len - the length of buffer
 *
 * @return the pointer of response buffer terminated with '\0', if success. NULL, if falied!
 */
const char * IoTgo::request(const char *http_body, char *const buffer, 
    int32_t len)
{
    static int32_t counter = 0;
    static bool connect_TCP = false;
    int32_t time_delay = 1000; 
    int32_t reconnectTCP = 0;
    
    if (buffer == NULL)
    {
        DebugSerial.println("buffer cannot be NULL!");
        return NULL;
    }
    
    buffer[0] = '\0';
    
request_reconnect:    
    if (connect_TCP == false && !net->createTCPConnection(ip, 80))
    {
        connect_TCP = true;
    } 
    if (connect_TCP == false)
    {
        DebugSerial.println("Cannot connect to host");
        return NULL;
    }
    
   
    String http_req;
    /* Request line */
    http_req = "POST /api/http HTTP/1.1\r\n";
    /* Http header */
#if 0        
    http_req += "Host: iotgo.iteadstudio.com\r\n";
#else
    http_req += "Host: ";
    http_req += domain_name;
    http_req += "\r\n";
#endif
    http_req += "Content-Type: application/json\r\n";
    http_req += "Content-Length: " + String(strlen(http_body));
    http_req += "\r\n\r\n";
    /* Http body */
    http_req += http_body;
#ifdef DEBUG
    DebugSerial.print("http_req=[");
    DebugSerial.print(http_req);
    DebugSerial.println("]");
#endif        
    if (!net->send(http_req))
    {
        //DebugSerial.println("Request Sent Successfully!");
    }
    else
    {
        if ((reconnectTCP++) < 5)
        {
            connect_TCP = false;
            net->releaseTCPConnection();
            goto request_reconnect;
        }
        else
        {
            DebugSerial.println("Request Sending Failed!");
            return NULL;
        }
    }
    
    /* Waiting for response for 10 seconds */ 
    while (time_delay--)
    {   
        if (net->recv(buffer, len) > 0)
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
    
    buffer[len - 1] = '\0';
    
    //DebugSerial.print("request counter=");
    //DebugSerial.println(++counter);
    //DebugSerial.print("time_delay=");
    //DebugSerial.println(time_delay);
    //DebugSerial.print("strlen(buffer)=");
    //DebugSerial.println(strlen(buffer));
    
#ifdef DEBUG
    DebugSerial.println(buffer);
#endif    

    return buffer;
}

/**
 * Get the apikey of user who own the device specified by device_id. 
 * 
 * If the device_type = DEVICE_DIY, apikey_like should be the apikey of 
 * the user who owns the device.
 * If the device_type = DEVICE_PRODUCT, apikey_like should be found around
 * your finished device purchased from ITEAD. 
 * 
 * @warning You must call this before update and query. 
 *
 * @param device_id - device identifier
 * @param apikey_like - just like a password for accessing the respective device
 * @param device_type - indicate the type of your device from DEVICE_DIY or DEVICE_PRODUCT. 
 *  For developers, device_type=DEVICE_DIY(default). For users, device_type=DEVICE_PRODUCT.
 *
 * @retval apikey - a pointer of char array terminated with '\0'.
 * @retval NULL - if falied!
 */
const char *IoTgo::init(const char *device_id, const char *apikey_like, 
    IoTgoDeviceType device_type)
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
        strcat(http_body, apikey_like);
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
        strncpy(this->apikey, apikey_like, APIKEY_LEN);
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
