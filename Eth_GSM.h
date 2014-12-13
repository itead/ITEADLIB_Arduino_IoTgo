#ifndef __ETH_GSM_H__
#define __ETH_GSM_H__

#include <Arduino.h>
#include <SoftwareSerial.h>

#include "utility/HW_GSM_SIM900.h"
#include "utility/HW_GSM_inetGSM.h"
#include "utility/HW_GSM_GSM.h"

#include "IoTgo_config.h"
#include "Net_NetInterface.h"

#ifdef NET_USE_GSM

/**
 * Provides TCP service for Application Layer of Network Protocol Stack. 
 * 
 * The implemetation is based on GSM/GPRS (SIM900/908/800/808). 
 *
 * @ingroup NetInterface
 */
class Gsm: public NetInterface, public InetGSM, public SIMCOM900
{
public:		/* Implementation of NetInterface */
	virtual int32_t createTCPConnection(String host, uint32_t port);
    virtual int32_t send(String data);
    virtual int32_t recv(char *buffer, uint32_t length);
    virtual int32_t releaseTCPConnection(void);

public:		/* Initial the EthW5X00 class*/
	bool intialGSM();
};

#endif

#endif




