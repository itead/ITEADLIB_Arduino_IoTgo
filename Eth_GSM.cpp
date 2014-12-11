#include <Arduino.h>
#include <Dhcp.h>
#include <String.h>

#include "IoTgo.h"
#include "Eth_GSM.h"

#ifdef NET_USE_GSM

/* Create a object of InetGSM class */
static InetGSM inet;
static InetGSM *gsm_inet = &inet;


/*Initialize the GSM module
*/
bool Gsm::intialGSM()
{
	if(gsm.begin(9600))	
	{
		DebugSerial.println("Gsm ready OK!");
		
		//GPRS attach, put in order APN, username and password.
		if(gsm_inet->attachGPRS("internet.wind", "", ""))
		{
			DebugSerial.println("attachGPRS status=ATTACHED");
		}
		else
		{
			DebugSerial.println("attachGPRS status=ERROR");
			return false;
		}
		delay(1000);

		//Read IP address.
		gsm.SimpleWriteln("AT+CIFSR");
		delay(5000);
		gsm.WhileSimpleRead();

		return true;
	}
	else
	{
		DebugSerial.println("gsm.begin err!");
		return false;
	}
}


/* TCP connection, if return value equal 0,It was successed.
   else failed.
*/
int32_t Gsm::createTCPConnection(String host, uint32_t port)
{
	char msg[64] = {0};
	const char *server_name = host.c_str();
	bool connected = false;
	int i;

	for (i = 0; i < 10; i++)
	{
		DebugSerial.print("Try ");
		DebugSerial.println(i);
		if(gsm_inet->connectTCP(server_name, (int)port))
		{
			DebugSerial.println("TCP connect ok!");
			return 0;
		}
		delay(1000);
	}
	
	DebugSerial.println("TCP connect err!");
	return ERR_TCP_CONN_FAILED;

}


/*Send the AT command data
  return value is 0.
*/
int32_t Gsm::send(String data)
{
	int32_t ret = 0;
	char end_c[2];
	end_c[0]=0x1a;
	end_c[1]='\0';

	gsm.SimpleWrite(data.c_str());
	gsm.SimpleWrite(end_c);

	switch(gsm.WaitResp(10000, 10, "SEND OK")) 
	{
		case RX_TMOUT_ERR:
			ret = ERR_TCP_SEND_FAILED;
			DebugSerial.println("send timeout!");
			break;
		case RX_FINISHED_STR_NOT_RECV:
			ret = ERR_TCP_SEND_FAILED;
			DebugSerial.println("send no response!");
			break;
		default:
			DebugSerial.println("send ok!");
	}
	
	return ret;
}


/*Recvive the resp
  if successed, return 0
  else return ERR_TCP_CONN_FAILED.
*/
int32_t Gsm::recv(char * buffer, uint32_t length)
{
	return gsm.read(buffer, length);
}


/*Break the TCP connection
  if successed, return 0
  else return ERR_TCP_CONN_FAILED.
*/
int32_t Gsm::releaseTCPConnection()
{
	return gsm_inet->disconnectTCP() == 1 ? 0 : ERR_TCP_DISCONN_FAILED;
}


#endif		/*NET_USE_GSM*/
