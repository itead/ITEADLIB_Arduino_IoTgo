/**
 * @file Eth_ESP8266.cpp
 *
 * ESP8266 library 
 *
 * @author  Wu Pengfei (email:<pengfei.wu@itead.cc>)
 * @date    2014/11/17
 * @copyright 
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */
#include "Eth_ESP8266.h"

/*
 * Start communication and reset ESP8266. 
 */
void ESP8266::begin(void)
{
	boolean result = false;
	uint32_t delay_cnt = 0;
	
	ESP8266Serial.begin(ESP8266_BAUD_RATE);
	ESP8266Serial.setTimeout(ESP8266SERIAL_TIMEOUT_DEFAULT);
    Reset();
}


/*************************************************************************
//Initialize port

	mode:	setting operation mode
		STA: 	Station
		AP:	 	Access Point
		AT_STA:	Access Point & Station

	chl:	channel number
	ecn:	encryption
		ESP8266_ENC_OPEN          0
		ESP8266_ENC_WEP           1
		ESP8266_ENC_WAP_PSK       2
		ESP8266_ENC_WAP2_PSK      3
		ESP8266_ENC_WAP_WAP2_PSK  4		

	return:
		true	-	successfully
		false	-	unsuccessfully

***************************************************************************/
bool ESP8266::Initialize(byte mode, String ssid, String pwd, byte chl, byte ecn)
{
	if (mode == STA)
	{	
        DebugSerial.println("Set to station mode and reset... ");
		bool b = confMode(mode);
		if (!b)
		{
			return false;
		}
		Reset();
		
        DebugSerial.print("Connecting to ");
        DebugSerial.println(ssid);
		return confJAP(ssid, pwd);
	}
	else if (mode == AP)
	{
		bool b = confMode(mode);
		if (!b)
		{
			return false;
		}
		Reset();
		confSAP(ssid, pwd, chl, ecn);
	}
	else if (mode == AP_STA)
	{
		bool b = confMode(mode);
		if (!b)
		{
			return false;
		}
		Reset();
		confJAP(ssid, pwd);
		confSAP(ssid, pwd, chl, ecn);
	}
	
	return true;
}

/*************************************************************************
//Set up tcp or udp connection

	type:	tcp or udp
	
	addr:	ip address
	
	port:	port number
	
	a:	set multiple connection
		0 for sigle connection
		1 for multiple connection
		
	id:	id number(0-4)

	return:
		true	-	successfully
		false	-	unsuccessfully

***************************************************************************/
boolean ESP8266::ipConfig(byte type, String addr, int32_t port, boolean a, byte id)
{
	boolean result = false;
	if (a == 0 )
	{
		confMux(a);
		
		long timeStart = millis();
		while (1)
		{
			long time0 = millis();
			if (time0 - timeStart > 5000)
			{
				break;
			}
		}
		result = newMux(type, addr, port);
	}
	else if (a == 1)
	{
		confMux(a);
		long timeStart = millis();
		while (1)
		{
			long time0 = millis();
			if (time0 - timeStart > 5000)
			{
				break;
			}
		}
		result = newMux(id, type, addr, port);
	}
	return result;
}

/*************************************************************************
//receive message from ESP8266

	buf:	buffer for receiving data
	
	return:	size of the buffer
	

***************************************************************************/
int32_t ESP8266::ReceiveMessage(char *buf)
{
	//+IPD,<len>:<data>
	//+IPD,<id>,<len>:<data>
	String data = "";
	if (ESP8266Serial.available()>0)
	{
		
		unsigned long start;
		start = millis();
		char c0 = ESP8266Serial.read();
		if (c0 == '+')
		{
			
			while (millis()-start<5000) 
			{
				if (ESP8266Serial.available()>0)
				{
					char c = ESP8266Serial.read();
					data += c;
				}
				if (data.indexOf("\nOK")!=-1)
				{
					break;
				}
			}
			//Serial.println(data);
			int32_t sLen = strlen(data.c_str());
			int32_t i,j;
			for (i = 0; i <= sLen; i++)
			{
				if (data[i] == ':')
				{
					break;
				}
				
			}
			boolean found = false;
			for (j = 4; j <= i; j++)
			{
				if (data[j] == ',')
				{
					found = true;
					break;
				}
				
			}
			int32_t iSize;
			//DBG(data);
			//DBG("\r\n");
			if(found ==true)
			{
    			String _id = data.substring(4, j);
    			String _size = data.substring(j+1, i);
    			iSize = _size.toInt();
    			//DBG(_size);
    			String str = data.substring(i+1, i+1+iSize);
    			strcpy(buf, str.c_str());	
    			//DBG(str);
						
			}
			else
			{			
    			String _size = data.substring(4, i);
    			iSize = _size.toInt();
    			//DBG(iSize);
    			//DBG("\r\n");
    			String str = data.substring(i+1, i+1+iSize);
    			strcpy(buf, str.c_str());
    			//DBG(str);
			}
			return iSize;
		}
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////


/*************************************************************************
//reboot the ESP8266 module



***************************************************************************/
void ESP8266::Reset(void)
{
	uint32_t delay_cnt = 0;
	
	ESP8266Serial.setTimeout(10);
	ESP8266Serial.flush();
	ESP8266Serial.println("AT+RST");
	if (ESP8266Serial.find("OK"))
	{
	    DebugSerial.println("Reset module...");
	}
	else
	{
	    DebugSerial.println("Reset module failed and halt now...");
	    while(1);
	}
	
	delay(3000); /* Waiting for boot */

    while(delay_cnt < 500)
    {
        ESP8266Serial.flush();
        ESP8266Serial.println("AT");
        if (ESP8266Serial.find("OK"))
        {
            break;
        }
        delay_cnt ++;
    }
    ESP8266Serial.setTimeout(ESP8266SERIAL_TIMEOUT_DEFAULT);
    
    if (delay_cnt < 500)
    {
        DebugSerial.println("Module is ready");
    }
    else
	{
		DebugSerial.println("Module have no response. Halt now...");
		while(1);
	}
	
}

/*********************************************
 *********************************************
 *********************************************
             ESP8266 Function Commands
 *********************************************
 *********************************************
 *********************************************
 */

/*************************************************************************
//inquire the current mode of ESP8266 module

	return:	string of current mode
		Station
		AP
		AP+Station

***************************************************************************/
String ESP8266::showMode()
{
    String data;
    ESP8266Serial.println("AT+CWMODE?");  
	unsigned long start;
	start = millis();
    while (millis()-start<2000) {
     if(ESP8266Serial.available()>0)
     {
     char a =ESP8266Serial.read();
     data=data+a;
     }
     if (data.indexOf("OK")!=-1)
     {
         break;
     }
  }
    if(data.indexOf("1")!=-1)
    {
        return "Station";
    }else if(data.indexOf("2")!=-1)
    {
            return "AP";
    }else if(data.indexOf("3")!=-1)
    {
         return "AP+Station";
    }
}



/*************************************************************************
//configure the operation mode

	a:	
		1	-	Station
		2	-	AP
		3	-	AP+Station
		
	return:
		true	-	successfully
		false	-	unsuccessfully

***************************************************************************/

bool ESP8266::confMode(byte a)
{
    String data;
    ESP8266Serial.print("AT+CWMODE=");  
    ESP8266Serial.println(String(a));
    unsigned long start;
    start = millis();
    while (millis()-start<2000) 
    {
        if(ESP8266Serial.available()>0)
        {
            char a =ESP8266Serial.read();
            data=data+a;
        }
        if (data.indexOf("OK")!=-1 || data.indexOf("no change")!=-1)
        {
            return true;
        }
        if (data.indexOf("ERROR")!=-1 || data.indexOf("busy")!=-1)
        {
            return false;
        }

    }
}


/*************************************************************************
//show the list of ESP8266 hotspot
		
	return:	string of ESP8266 information
		encryption,SSID,RSSI
		

***************************************************************************/

String ESP8266::showAP(void)
{
    String data;
	ESP8266Serial.flush();
    ESP8266Serial.print("AT+CWLAP\r\n");  
	delay(1000);
	while(1);
    unsigned long start;
	start = millis();
    while (millis()-start<8000) {
   if(ESP8266Serial.available()>0)
   {
     char a =ESP8266Serial.read();
     data=data+a;
   }
     if (data.indexOf("OK")!=-1 || data.indexOf("ERROR")!=-1 )
     {
         break;
     }
  }
    if(data.indexOf("ERROR")!=-1)
    {
        return "ERROR";
    }
    else{
       char head[4] = {0x0D,0x0A};   
       char tail[7] = {0x0D,0x0A,0x0D,0x0A};        
       data.replace("AT+CWLAP","");
       data.replace("OK","");
       data.replace("+CWLAP","WIFI");
       data.replace(tail,"");
	   data.replace(head,"");

        return data;
        }
 }


/*************************************************************************
//show the name of current ESP8266 access port
		
	return:	string of access port name
		AP:<SSID>
		

***************************************************************************/
String ESP8266::showJAP(void)
{
	ESP8266Serial.flush();
    ESP8266Serial.println("AT+CWJAP?");  
      String data;
	  unsigned long start;
	start = millis();
    while (millis()-start<3000) {
       if(ESP8266Serial.available()>0)
       {
       char a =ESP8266Serial.read();
       data=data+a;
       }
       if (data.indexOf("OK")!=-1 || data.indexOf("ERROR")!=-1 )
       {
           break;
       }
    }
      char head[4] = {0x0D,0x0A};   
      char tail[7] = {0x0D,0x0A,0x0D,0x0A};        
      data.replace("AT+CWJAP?","");
      data.replace("+CWJAP","AP");
      data.replace("OK","");
	  data.replace(tail,"");
      data.replace(head,"");
      
          return data;
}


/*************************************************************************
//configure the SSID and password of the access port
		
		return:
		true	-	successfully
		false	-	unsuccessfully
		

***************************************************************************/
boolean ESP8266::confJAP(String ssid , String pwd)
{
    boolean join = false;

    ESP8266Serial.flush();
    ESP8266Serial.print("AT+CWJAP=");
    ESP8266Serial.print("\"");     //"ssid"
    ESP8266Serial.print(ssid);
    ESP8266Serial.print("\"");

    ESP8266Serial.print(",");

    ESP8266Serial.print("\"");      //"pwd"
    ESP8266Serial.print(pwd);
    ESP8266Serial.println("\"");


    unsigned long start;
	start = millis();
    while (millis()-start<10000) 
    {                            
        if(ESP8266Serial.find("OK")==true)
        { 
            join = true;
            break;
        }
    }

    if (join)
    {
        uint32_t n = 0;
        while(n++ < 100)
        {
            ESP8266Serial.flush();
            ESP8266Serial.println("AT+CIFSR");
            String data = "";
            char c;
            start = millis();
            while (millis()-start<3000) 
            {
                while(ESP8266Serial.available()>0)
                {
                    c = ESP8266Serial.read(); 
                    //DebugSerial.print(c);
                    data += c;
                }
                if (data.indexOf("0.0.0.0") == -1 &&
                    data.indexOf("OK") !=-1 )
                {
                    return true;
                }
            }
            delay(100);
        }
    }
    return false;    
}
/*************************************************************************
//quite the access port
		
		return:
			true	-	successfully
			false	-	unsuccessfully
		

***************************************************************************/

boolean ESP8266::quitAP(void)
{
    ESP8266Serial.println("AT+CWQAP");
    unsigned long start;
	start = millis();
    while (millis()-start<3000) {                            
        if(ESP8266Serial.find("OK")==true)
        {
		   return true;
           
        }
    }
	return false;

}

/*************************************************************************
//show the parameter of ssid, password, channel, encryption in AP mode
		
		return:
			mySAP:<SSID>,<password>,<channel>,<encryption>

***************************************************************************/
String ESP8266::showSAP()
{
    ESP8266Serial.println("AT+CWSAP?");  
      String data;
      unsigned long start;
	start = millis();
    while (millis()-start<3000) {
       if(ESP8266Serial.available()>0)
       {
       char a =ESP8266Serial.read();
       data=data+a;
       }
       if (data.indexOf("OK")!=-1 || data.indexOf("ERROR")!=-1 )
       {
           break;
       }
    }
      char head[4] = {0x0D,0x0A};   
      char tail[7] = {0x0D,0x0A,0x0D,0x0A};        
      data.replace("AT+CWSAP?","");
      data.replace("+CWSAP","mySAP");
      data.replace("OK","");
	  data.replace(tail,"");
      data.replace(head,"");
      
          return data;
}

/*************************************************************************
//configure the parameter of ssid, password, channel, encryption in AP mode
		
		return:
			true	-	successfully
			false	-	unsuccessfully

***************************************************************************/

boolean ESP8266::confSAP(String ssid , String pwd , byte chl , byte ecn)
{
    ESP8266Serial.print("AT+CWSAP=");  
    ESP8266Serial.print("\"");     //"ssid"
    ESP8266Serial.print(ssid);
    ESP8266Serial.print("\"");

    ESP8266Serial.print(",");

    ESP8266Serial.print("\"");      //"pwd"
    ESP8266Serial.print(pwd);
    ESP8266Serial.print("\"");

    ESP8266Serial.print(",");
    ESP8266Serial.print(String(chl));

    ESP8266Serial.print(",");
    ESP8266Serial.println(String(ecn));
	unsigned long start;
	start = millis();
    while (millis()-start<3000) {                            
        if(ESP8266Serial.find("OK")==true )
        {
           return true;
        }
     }
	 
	 return false;

}


/*********************************************
 *********************************************
 *********************************************
             TPC/IP Function Command
 *********************************************
 *********************************************
 *********************************************
 */

/*************************************************************************
//inquire the connection status
		
		return:		string of connection status
			<ID>  0-4
			<type>  tcp or udp
			<addr>  ip
			<port>  port number

***************************************************************************/

String ESP8266::showStatus(void)
{
    ESP8266Serial.println("AT+CIPSTATUS");  
      String data;
    unsigned long start;
	start = millis();
    while (millis()-start<3000) {
       if(ESP8266Serial.available()>0)
       {
       char a =ESP8266Serial.read();
       data=data+a;
       }
       if (data.indexOf("OK")!=-1)
       {
           break;
       }
    }

          char head[4] = {0x0D,0x0A};   
          char tail[7] = {0x0D,0x0A,0x0D,0x0A};        
          data.replace("AT+CIPSTATUS","");
          data.replace("OK","");
		  data.replace(tail,"");
          data.replace(head,"");
          
          return data;
}

/*************************************************************************
//show the current connection mode(sigle or multiple)
		
		return:		string of connection mode
			0	-	sigle
			1	-	multiple

***************************************************************************/
String ESP8266::showMux(void)
{
    String data;
    ESP8266Serial.println("AT+CIPMUX?");  

      unsigned long start;
	start = millis();
    while (millis()-start<3000) {
       if(ESP8266Serial.available()>0)
       {
       char a =ESP8266Serial.read();
       data=data+a;
       }
       if (data.indexOf("OK")!=-1)
       {
           break;
       }
    }
          char head[4] = {0x0D,0x0A};   
          char tail[7] = {0x0D,0x0A,0x0D,0x0A};        
          data.replace("AT+CIPMUX?","");
          data.replace("+CIPMUX","showMux");
          data.replace("OK","");
		  data.replace(tail,"");
          data.replace(head,"");
          
          return data;
}

/*************************************************************************
//configure the current connection mode(sigle or multiple)
		
		a:		connection mode
			0	-	sigle
			1	-	multiple
			
	return:
		true	-	successfully
		false	-	unsuccessfully
***************************************************************************/
boolean ESP8266::confMux(boolean a)
{
	ESP8266Serial.print("AT+CIPMUX=");
	ESP8266Serial.println(a);           
	unsigned long start;
	start = millis();
	while (millis()-start<3000) {                            
        if(ESP8266Serial.find("OK")==true )
        {
           return true;
        }
     }
	 
	 return false;
}


/*************************************************************************
//Set up tcp or udp connection	(signle connection mode)

	type:	tcp or udp
	
	addr:	ip address
	
	port:	port number
		

	return:
		true	-	successfully
		false	-	unsuccessfully

***************************************************************************/
boolean ESP8266::newMux(byte type, String addr, int32_t port)
{
    String data;
    ESP8266Serial.print("AT+CIPSTART=");
    if(type>0)
    {
        ESP8266Serial.print("\"TCP\"");
    }
    else
    {
        ESP8266Serial.print("\"UDP\"");
    }
    ESP8266Serial.print(",");
    ESP8266Serial.print("\"");
    ESP8266Serial.print(addr);
    ESP8266Serial.print("\"");
    ESP8266Serial.print(",");
    ESP8266Serial.println(String(port));
    unsigned long start;
	start = millis();
	char c;
	while (millis()-start<3000) 
	{ 
        while(ESP8266Serial.available()>0)
        {
            c = ESP8266Serial.read();
            data = data + c;
        }
        if (data.indexOf("OK") != -1 || 
            data.indexOf("ALREADY CONNECT") != -1)
        {
            return true;
        }
  }
  return false;
}

/*************************************************************************
//Set up tcp or udp connection	(multiple connection mode)

	type:	tcp or udp
	
	addr:	ip address
	
	port:	port number
		
	id:	id number(0-4)

	return:
		true	-	successfully
		false	-	unsuccessfully

***************************************************************************/
boolean ESP8266::newMux( byte id, byte type, String addr, int32_t port)

{

    ESP8266Serial.print("AT+CIPSTART=");
    ESP8266Serial.print("\"");
    ESP8266Serial.print(String(id));
    ESP8266Serial.print("\"");
    if(type>0)
    {
        ESP8266Serial.print("\"TCP\"");
    }
	else
    {
        ESP8266Serial.print("\"UDP\"");
    }
    ESP8266Serial.print(",");
    ESP8266Serial.print("\"");
    ESP8266Serial.print(addr);
    ESP8266Serial.print("\"");
    ESP8266Serial.print(",");
//    ESP8266Serial.print("\"");
    ESP8266Serial.println(String(port));
//    ESP8266Serial.println("\"");
    String data;
    unsigned long start;
	start = millis();
	while (millis()-start<3000) { 
     if(ESP8266Serial.available()>0)
     {
     char a =ESP8266Serial.read();
     data=data+a;
     }
     if (data.indexOf("OK")!=-1 || data.indexOf("ALREAY CONNECT")!=-1 )
     {
         return true;
     }
  }
  return false;
  

}
/*************************************************************************
//send data in sigle connection mode

	str:	string of message

	return:
		true	-	successfully
		false	-	unsuccessfully

***************************************************************************/
boolean ESP8266::Send(String str)
{
    ESP8266Serial.print("AT+CIPSEND=");
//    ESP8266Serial.print("\"");
    ESP8266Serial.println(str.length());
//    ESP8266Serial.println("\"");
    unsigned long start;
	start = millis();
	bool found;
	while (millis()-start<5000) {                            
        if(ESP8266Serial.find(">")==true )
        {
			found = true;
           break;
        }
     }
	 if(found)
		ESP8266Serial.print(str);
	else
	{
		closeMux();
		return false;
	}


    String data;
    start = millis();
	while (millis()-start<5000) {
     if(ESP8266Serial.available()>0)
     {
     char a =ESP8266Serial.read();
     data=data+a;
     }
     if (data.indexOf("SEND OK")!=-1)
     {
         return true;
     }
  }
  return false;
}

/*************************************************************************
//send data in multiple connection mode

	id:		<id>(0-4)
	
	str:	string of message

	return:
		true	-	successfully
		false	-	unsuccessfully

***************************************************************************/
boolean ESP8266::Send(byte id, String str)
{
    ESP8266Serial.print("AT+CIPSEND=");

    ESP8266Serial.print(String(id));
    ESP8266Serial.print(",");
    ESP8266Serial.println(str.length());
    unsigned long start;
	start = millis();
	bool found;
	while (millis()-start<5000) {                          
        if(ESP8266Serial.find(">")==true )
        {
			found = true;
           break;
        }
     }
	 if(found)
		ESP8266Serial.print(str);
	else
	{
		closeMux(id);
		return false;
	}


    String data;
    start = millis();
	while (millis()-start<5000) {
     if(ESP8266Serial.available()>0)
     {
     char a =ESP8266Serial.read();
     data=data+a;
     }
     if (data.indexOf("SEND OK")!=-1)
     {
         return true;
     }
  }
  return false;
}

/*************************************************************************
//Close up tcp or udp connection	(sigle connection mode)


***************************************************************************/
void ESP8266::closeMux(void)
{
    ESP8266Serial.println("AT+CIPCLOSE");

    String data;
    unsigned long start;
	start = millis();
	while (millis()-start<3000) {
     if(ESP8266Serial.available()>0)
     {
     char a =ESP8266Serial.read();
     data=data+a;
     }
     if (data.indexOf("Linked")!=-1 || data.indexOf("ERROR")!=-1 || data.indexOf("we must restart")!=-1)
     {
         break;
     }
  }
}


/*************************************************************************
//Set up tcp or udp connection	(multiple connection mode)
		
	id:	id number(0-4)

***************************************************************************/
void ESP8266::closeMux(byte id)
{
    ESP8266Serial.print("AT+CIPCLOSE=");
    ESP8266Serial.println(String(id));
    String data;
    unsigned long start;
	start = millis();
	while (millis()-start<3000) {
     if(ESP8266Serial.available()>0)
     {
     char a =ESP8266Serial.read();
     data=data+a;
     }
     if (data.indexOf("OK")!=-1 || data.indexOf("Link is not")!=-1 || data.indexOf("Cant close")!=-1)
     {
         break;
     }
  }

}

/*************************************************************************
//show the current ip address
		
	return:	string of ip address

***************************************************************************/
String ESP8266::showIP(void)
{
    String data;
    unsigned long start;
    for(int32_t a=0; a<3;a++)
    {
        ESP8266Serial.println("AT+CIFSR");  
        start = millis();
        while (millis()-start<3000) {
             while(ESP8266Serial.available()>0)
             {
                 char a =ESP8266Serial.read();
                 data=data+a;
             }
             if (data.indexOf("AT+CIFSR")!=-1)
             {
                 break;
             }
        }
        if(data.indexOf(".") != -1)
        {
        	break;
        }
    	data = "";
    }
    
    char head[4] = {0x0D,0x0A};   
    char tail[7] = {0x0D,0x0D,0x0A};        
    data.replace("AT+CIFSR","");
    data.replace(tail,"");
    data.replace(head,"");
    data.replace("OK","");
    return data;
}

/*************************************************************************
////set the parameter of server

	mode:
		0	-	close server mode
		1	-	open server mode
		
	port:	<port>
		
	return:
		true	-	successfully
		false	-	unsuccessfully

***************************************************************************/

boolean ESP8266::confServer(byte mode, int32_t port)
{
    ESP8266Serial.print("AT+CIPSERVER=");  
    ESP8266Serial.print(String(mode));
    ESP8266Serial.print(",");
    ESP8266Serial.println(String(port));

    String data;
    unsigned long start;
	start = millis();
	boolean found = false;
	while (millis()-start<3000) {
     if(ESP8266Serial.available()>0)
     {
     char a =ESP8266Serial.read();
     data=data+a;
     }
     if (data.indexOf("OK")!=-1 || data.indexOf("no char_tge")!=-1)
     {
		found = true;
         break;
     }
  }
  return found;
}

/**
 * Connect to WiFi. 
 * 
 * @param ssid - SSID of WiFi Network. 
 * @param password - guess what!
 * 
 * @retval true - success. 
 * @retval false - failed. 
 */
bool ESP8266::connectWiFi(String ssid, String password)
{
    begin();
    if(!Initialize(STA, ssid, password))
    {
        DebugSerial.print("Connect to ");
        DebugSerial.print(ssid);
        DebugSerial.println(" failed and halt now...");
        while(1);
    }
    DebugSerial.print("Getting IP from ");
    DebugSerial.print(ssid);
    DebugSerial.println(". Please wait ...");
    DebugSerial.println(showIP());
    
    return true;
}

/**
 * Disconnect from WiFi connection before. 
 * 
 * @retval true - success. 
 * @retval false - failed. 
 */
bool ESP8266::disconnectWiFi(void)
{
    return quitAP();
}

int32_t ESP8266::createTCPConnection(String host, uint32_t port)
{
    return ipConfig(TCP, host, port) ? 0 : ERR_TCP_CONN_FAILED;
}

int32_t ESP8266::send(String data)
{
    return Send(data) ? 0 : ERR_TCP_SEND_FAILED;
}

int32_t ESP8266::recv(char *buffer, uint32_t length)
{
    int32_t len = ReceiveMessage(buffer);
    buffer[length-1] = '\0';
    return (len >= length) ? length-1 : len;
}

int32_t ESP8266::releaseTCPConnection(void)
{
    closeMux();
    return 0;
}

