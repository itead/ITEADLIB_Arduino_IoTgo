#include <String.h>
#include <Arduino.h>
#include <Debug.h>
#include <Dhcp.h>
#include <ArduinoRobot.h>

#include "IoTgo.h"
#include "Eth_W5X00.h"

#ifdef NET_USE_W5X00

static EthernetClient client_w5x00;
EthernetClient *c = &client_w5x00;

bool EthW5X00::Initial(uint8_t *mac)
{
	int ret = 0;
	int r = 0;
	
	ret = Ethernet.begin(mac);

	if(ret != 1)
	{
		DebugSerial.println("Net initial config failed!");
		while(1);
	}
	else
	{
		DebugSerial.print("My module's IP address: ");
		for(byte i = 0; i<4; i++)
		{
		    DebugSerial.print(Ethernet.localIP()[i], DEC);
		    DebugSerial.print(".");
		}
		DebugSerial.println();
	}

	return true;
	
}


/**
 * Initialize the W5X00 module
 *
 * @retval true - success. 
 * @retval false - failed. 
 */
bool EthW5X00::configW5X00(uint8_t *mac)
{	
    return Initial(mac);
}



int32_t EthW5X00::createTCPConnection(String host, uint32_t port)
{
    int cnt;
    const char *domain_name = host.c_str();
    
    
    for (cnt = 0; cnt < 10; cnt++)
	{
		if (1 == c->connect(domain_name, (uint16_t)port))
		{		    
            //DebugSerial.println("Ready OK!");
            return 0;
		}
		delay(100);
	}

	DebugSerial.println("TCP connection failed!");
	return ERR_TCP_CONN_FAILED;

}


int32_t EthW5X00::send(String data)
{
	size_t ret = 0;
	size_t len = 0;
	
	const char *ndata = data.c_str();
	len = strlen(ndata);
	ret = c->write((const uint8_t*)ndata, len);
	if(ret == len)
	{
		//DebugSerial.println("Send data successed!");
		return 0;
	}
	else
	{
		DebugSerial.println("Send data failed!");
		return ERR_TCP_SEND_FAILED;
	}
	
}


int32_t EthW5X00::recv(char * buffer, uint32_t length)
{
	int ret = 0;
	int cnt;
	
	for (cnt = 0; cnt < 100; cnt++)
	{
	    ret = c->read((uint8_t *)buffer, (size_t)length);
	    if (ret != -1)
	    {
	        return ret;
	    }
	    delay(100);
	}
	
	//DebugSerial.println("Recv data failed!");
	return ERR_TCP_RECV_FAILED;
}


int32_t EthW5X00::releaseTCPConnection()
{
	c->stop();
}

#endif /* #ifdef NET_USE_W5X00 */


