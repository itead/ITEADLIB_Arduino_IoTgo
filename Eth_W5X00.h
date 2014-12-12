#ifndef __ETH_W5X00_H__
#define __ETH_W5X00_H__


#include <Arduino.h>
#include <SoftwareSerial.h>
#include "utility/HW_Eth_Ethernet.h"
#include "utility/HW_Eth_EthernetClient.h"

#include "IoTgo_config.h"
#include "Net_NetInterface.h"

#ifdef NET_USE_W5X00

class EthW5X00:public NetInterface, public EthernetClient, public EthernetClass
{
public:		/* Implementation of NetInterface */
	virtual int32_t createTCPConnection(String host, uint32_t port);
    virtual int32_t send(String data);
    virtual int32_t recv(char *buffer, uint32_t length);
    virtual int32_t releaseTCPConnection(void);

public:		/* Initial the EthW5X00 class*/
	bool configW5X00(uint8_t *mac);

private:	/* The private funtion of EthW5X00 class*/
	//void reset();
	//IPAddress StringToIp(String str_ip);
	bool Initial(uint8_t *mac);
};

#endif

#endif

