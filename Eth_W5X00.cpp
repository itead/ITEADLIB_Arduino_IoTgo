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
		DebugSerial.println("Net initial config success....");
		delay(1000);

		DebugSerial.print(" My module's IP address: ");
		for(byte i = 0; i<4; i++)
		{
		    DebugSerial.print(Ethernet.localIP()[i], DEC);
		    DebugSerial.print(".");
		}
		DebugSerial.println("\n");

		DebugSerial.print(" My module's netMask: ");
		for(byte j=0; j<4; j++)
		{
		    DebugSerial.print(Ethernet.subnetMask()[j], DEC);
		    DebugSerial.print(".");
		}
		DebugSerial.println("\n");
		
		DebugSerial.print(" My module's gateway: ");
		for(byte k=0; k<4; k++)
		{
		    DebugSerial.print(Ethernet.gatewayIP(), DEC);
		    DebugSerial.print(".");
		}
		DebugSerial.println("\n");
	
	}

	return true;
	
}

bool EthW5X00::configW5X00(uint8_t *mac)
{	
    if(Initial(mac))
    {
		return true;
    }
	else
	{
		return false;
	}
}


/*Convert String to IPAddress
  Return value is the type of IPAddress 
*/
#if 0
IPAddress EthW5X00::StringToIp(String str_ip)
{	
	size_t len = 0;
	size_t i;
	int n = 0;
	int j = 0;
	char ip[16] = {0};
	const char *ip_org = str_ip.c_str();
	strcpy(ip, ip_org);
	
	long a[8] = {0};
	
	len = strlen(ip);

	for(i=0; i<len+1; i++)
	{
		if((ip[i]=='.')||(ip[i]=='\0'))
		{
			const char tem[8] = {0};
			memset((void *)tem, 0, 8);
			memcpy((void *)tem, ip_org+n, i-n);
			a[j] = atoi(tem);
			j++;
			n = i+1;
			continue;
		}
		else
		{
			continue;
		}
	}

#if 0
	for(i=0;i<4;i++)
	{
		Serial.print(a[i]);
		Serial.print(".");
	}
	Serial.println("\n");
#endif

	IPAddress ip_a = IPAddress(a[0], a[1], a[2], a[3]);

	return ip_a;
	
}
#endif



/* TCP connection, if return value equal 0,It was successed.
   else failed.
*/
int32_t EthW5X00::createTCPConnection(String host, uint32_t port)
{
	int ret = 0;
	int i = 0;

    const char *domain_name = host.c_str();
    
    
    while(1 != ret)
	{
		i++;
		ret = c->connect(domain_name, (uint16_t)port);
		delay(3000);
		if(10 == i)
		{
			break;
		}
	}
    
 #if 0
    else    //Connect by the IP address
    {
    	IPAddress ip_addr = StringToIp(host);
    	DebugSerial.print("The Server IP_address is: ");
    	DebugSerial.print(ip_addr);
    	DebugSerial.println();
    	
    	//IPAddress ipad(172, 16, 7, 6);
    	//ret = c->connect(ipad, 80);
    	
    	while(1 != ret)
    	{
    		i++;
    		ret = c->connect(ip_addr, (uint16_t)port);
    		delay(3000);
    		if(10 == i)
    		{
    			break;
    		}
    	}
    	
	}
#endif
	
	if(10 > i)
	{
		DebugSerial.println("Ready OK!");
		return 0;
	}
	else
	{
		DebugSerial.println("TCP connection failed!");
		return ERR_TCP_CONN_FAILED;
	}

}


/* Send TCP data, if return value more than 0,It was successed.
   else be failed.
*/
int32_t EthW5X00::send(String data)
{
	DebugSerial.println("Call send function!");
	size_t ret = 0;
	size_t len = 0;
	
	const char *ndata = data.c_str();
	len = strlen(ndata);
	DebugSerial.println(len);

	char d[512] = {0};
	strcpy(d, ndata);
	DebugSerial.println(d);
	
	ret = c->write((const uint8_t*)d, len);
	if(ret == len)
	{
		DebugSerial.println("Send data successed!");
		return 0;
	}
	else
	{
		DebugSerial.println("Send data failed!");
		return ERR_TCP_SEND_FAILED;
	}
	
}


/* Recv TCP data, if return value more than 0,It was successed.
   else be failed.
*/
int32_t EthW5X00::recv(char * buffer, uint32_t length)
{
	int ret = 0;
	ret = c->read((uint8_t *)buffer, (size_t)length);
	if(ret > 0)
	{
		DebugSerial.println("Recv data successed!");
		return ret;
	}
	else
	{
		DebugSerial.println("Recv data failed!");
		return ERR_TCP_RECV_FAILED;
	}
	
}


int32_t EthW5X00::releaseTCPConnection()
{
	c->stop();

}

#endif /* #ifdef NET_USE_W5X00 */


