/**
 * @file Net_NetInterface.h
 *
 * The interface of internet. 
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/27
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
 
#ifndef __NET_NETINTERFACE_H__
#define __NET_NETINTERFACE_H__

#include <Arduino.h>

#include "IoTgo_error.h"

/**
 * Abstract class, declares the unified interface to access the internet. 
 *
 * @ingroup NetInterface
 */
class NetInterface
{
public: /* public methods */

    /**
     * Create TCP connection. 
     *
     * @param host - IP address of remote host. 
     * @param port - the port number for connection. 
     * 
     * @retval 0 - success. 
     * @retval ERR_TCP_CONN_FAILED - failed. 
     */
    virtual int32_t createTCPConnection(String host, uint32_t port) = 0;

    /**
     * Send data based on TCP connection created before. 
     *
     * @param data - the data to be sent. 
     * 
     * @retval 0 - success. 
     * @retval ERR_TCP_SEND_FAILED - failed. 
     */
    virtual int32_t send(String data) = 0;

    /**
     * Receive data based on TCP connection created before. 
     *
     * @param buffer - the buffer storing received data. 
     * @param length - the length of buffer. 
     * 
     * @retval positive - the length of data received. 
     * @retval ERR_TCP_RECV_FAILED - failed. 
     */
    virtual int32_t recv(char *buffer, uint32_t length) = 0;

    /**
     * Release TCP connection created by createTCPConnection before. 
     *
     * @retval 0 - success. 
     * @retval ERR_TCP_DISCONN_FAILED - failed. 
     */
    virtual int32_t releaseTCPConnection(void) = 0;
};


#endif /* #ifndef __NET_NETINTERFACE_H__ */
