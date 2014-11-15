/**
 * @file Light.cpp
 *
 * Light API of IoTgo (iotgo.iteadstudio.com)
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

#include "Light.h"

/*
 * Initialize static constants of ITEADIoTLED
 */
const char *Light::STATE[]        = {"state", NULL};
const char *Light::VALUE_ON[]     = {"on",    NULL};
const char *Light::VALUE_OFF[]    = {"off",   NULL};

Light::Light(uint16_t light_pin)
{
    this->light_pin = light_pin;
    pinMode(this->light_pin, OUTPUT);
}

/**
 * Read the state from server and turn on/off the light. 
 * 
 * We always assume that the state from server is newest. 
 * 
 * @retval 0 - success
 * @retval ERR_NO_RESPONSE - no response package from server. 
 * @retval ERR_NO_EXPECT - no state information expected in response package from server. 
 */
int32_t Light::sync(void)
{
    int32_t state;
    int32_t ret = 0;
    state = getState();
    if (state == STATE_ON)
    {
        digitalWrite(light_pin, HIGH);
    }
    else if (state == STATE_OFF)
    {
        digitalWrite(light_pin, LOW);
    }
    else
    {
        DebugSerial.print("\ngetState() error: state = ");
        DebugSerial.println(state);
        ret = state;
    }
    return ret;
}

/**
 * Read state from server. 
 *
 * @retval STATE_ON - indicate the state is on. 
 * @retval STATE_OFF - indicate the state is off. 
 * @retval ERR_NO_RESPONSE - no response package from server. 
 * @retval ERR_NO_EXPECT - no state information expected in response package from server. 
 */
int32_t Light::getState(void)
{
    const char *response;
    char *str_error;
    char *str_params;
    char *str_state_on;
    char *str_state_off;
    
    response = query(STATE); /* {"error":0,"params":{"state":"on"}} {"error":0,"params":{"state":"off"}}*/
    if (response != NULL)
    {
        str_error       = strstr(response, "\"error\":0");
        str_params      = strstr(response, "\"params\":");
        str_state_on    = strstr(response, "\"state\":\"on\"");
        str_state_off   = strstr(response, "\"state\":\"off\"");
        
        if (str_error && str_params)
        {
            if (str_state_on && !str_state_off)
            {
                return STATE_ON;
            }
            else if (!str_state_on && str_state_off)
            {
                return STATE_OFF;
            }
            else
            {
                return ERR_NO_EXPECT;
            }
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
 * Update the specific state to server. 
 * 
 * @param state - the state you want to update, only STATE_ON or STATE_OFF. 
 * 
 * @retval 0 - success
 * @retval ERR_INVALID_PARAMETER - state is invalid value. 
 * @retval ERR_NO_RESPONSE - no response package from server. 
 * @retval ERR_NO_EXPECT - no state information expected in response package from server. 
 */
int32_t Light::setState(int32_t state)
{
    const char **value;
    const char *response;
    char *str_error;
    
    if (state == STATE_ON)
    {
        value = VALUE_ON;
    }
    else if (state == STATE_OFF)
    {
        value = VALUE_OFF;
    }
    else
    {
        return ERR_INVALID_PARAMETER;
    }
    
    response = update(STATE, value);
    if (response != NULL)
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
 * Turn on the light and update the state to server. 
 *
 * @retval 0 - success
 * @retval ERR_NO_RESPONSE - no response package from server. 
 * @retval ERR_NO_EXPECT - no state information expected in response package from server. 
 */
int32_t Light::on(void)
{
    int32_t ret;
    ret = setState(STATE_ON) ;
    if (ret == 0)
    {
        digitalWrite(light_pin, HIGH);        
    }
    return ret;
}

/**
 * Turn off the light and update the state to server. 
 *
 * @retval 0 - success
 * @retval ERR_NO_RESPONSE - no response package from server. 
 * @retval ERR_NO_EXPECT - no state information expected in response package from server. 
 */
int32_t Light::off(void)
{
    int32_t ret;
    ret = setState(STATE_OFF) ;
    if (ret == 0)
    {
        digitalWrite(light_pin, LOW);        
    }
    return ret;
}

