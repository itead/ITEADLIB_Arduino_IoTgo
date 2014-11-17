/**
 * @file Switch.h
 *
 * Switch API of IoTgo (iotgo.iteadstudio.com)
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
 
#ifndef __SWITCH_H__
#define __SWITCH_H__

#include "IoTgo.h"

enum SwitchState
{
    SWITCH_STATE_OFF   = 0,
    SWITCH_STATE_ON    = 1,
};

enum SwitchError
{
    SWITCH_ERR_NO_RESPONSE = -1,
    SWITCH_ERR_NO_EXPECT   = -2,
    SWITCH_ERR_INVALID_PARAMETER = -3,
};

/**
 * Switch, subclass of IoTgo, provides simple methods to control switch device. 
 * 
 */
class Switch: public IoTgo
{
public: /* public methods */  
    Switch(uint16_t sw_pin);
    
    int32_t sync(void);
    int32_t getState(void);
    int32_t setState(int32_t state);
    int32_t on(void);
    int32_t off(void); 

private: /* private static data members */
    static const char *STATE[];
    static const char *VALUE_ON[];
    static const char *VALUE_OFF[];
    
private:
    uint16_t sw_pin;
};

#endif /* #ifndef __SWITCH_H__ */
