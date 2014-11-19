/**
 * @file IoTgo_error.h
 *
 * Define macros indicate error type. 
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/19
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
 
#ifndef __IOTGOERROR_H__
#define __IOTGOERROR_H__

enum IoTgoError
{
    /* General */
    ERR_INVALID_PARAMETER       = -1,

    /* Net */
    ERR_NO_RESPONSE             = -100,
    ERR_NO_EXPECT               = -101,
    
    /* Device */
    ERR_NO_DEVICES_AVAILABLE    = -200,
    ERR_INIT_DEVICE_FAILED      = -201,
    ERR_READ_DEVICE_FAILED      = -202,
    ERR_WRITE_DEVICE_FAILED     = -203,
    ERR_CLOSE_DEVICE_FAILED     = -204,
};


#endif /* #ifndef __IOTGOERROR_H__ */
