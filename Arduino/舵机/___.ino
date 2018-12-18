#include <ESP8266.h>
#ifdef ESP32
#error "This code is not recommended to run on the ESP32 platform! Please check your Tools->Board setting."
#endif
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define EspSerial Serial1
#define UARTSPEED  115200
#endif
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); 
#define servopin 4
#define EspSerial mySerial
#define UARTSPEED  9600
#endif
#define SSID        F("iPhone")//WIFI名
#define PASSWORD    F("20151214")//WIFI密码
#define HOST_NAME   F("api.heclouds.com")//域名
#define HOST_PORT   (80)
static const byte  GETDATA[]  PROGMEM = {
  "GET http://api.heclouds.com/devices/503083986/datapoints?datastream_id=a,am,status,lx&limit=1 HTTP/1.1\r\nHost:api.heclouds.com\r\napi-key:Lz6kPLRd=Qe9Qt=JkoUPnkyLLys=\r\nConnection: close\r\n\r\n"
};//关键头文件

ESP8266 wifi(&EspSerial);
//wifi
short a=0;
bool Rstatus=0,ws=0,tcp=0,ala=0,touch=1;
String rstatus,wstatus,astatus;
void setup() {
  Serial.begin(9600);
 
 
  WifiInit(EspSerial, UARTSPEED);
  if (wifi.setOprToStationSoftAP()) {} else {}
if (wifi.joinAP(SSID, PASSWORD)) {ws=1;} else {ws=0;}
  if (wifi.disableMUX()) {} else {}
}
//WiFi

void servopulse(int angle)
{
  int pulsewidth=(angle*11)+500;
  digitalWrite(servopin,HIGH);
  delayMicroseconds(pulsewidth); 
  digitalWrite(servopin,LOW);
  delayMicroseconds(20000-pulsewidth);
}

//_______________________________________________________________________________________________________________________   /online
void online()
{

 //if(!tcp)
wifi.createTCP(HOST_NAME, HOST_PORT);

 
 wifi.sendFromFlash(GETDATA, sizeof(GETDATA));
//从Flash读取发送内容，节约内存
  uint8_t buffer[512] = {0};
  uint32_t len = wifi.recv(buffer, sizeof(buffer), 20000);
  if (len > 0)
  {
     short k=0,temp=0,tax[1];
 tax[0]=a;

    for (uint32_t i = 0; i < len-12; i++) {
      if((char)buffer[i]=='v'&&(char)buffer[i+1]=='a'&&(char)buffer[i+2]=='l'&&(char)buffer[i+3]=='u'&&(char)buffer[i+4]=='e')
      {
          for (uint32_t j = 0; j<4;j++)
              if((char)buffer[i+j+7]>='0'&&(char)buffer[i+j+7]<='9')
              {temp=((short)buffer[i+j+7]-48)+temp*10;
              Serial.print(temp);
              }
               Serial.print('\n');
              Serial.println(temp);

       
           if(k==0)
           {Serial.println(temp);
           a=temp;}
          
           temp=0;  
           
    } }
  if(a!=tax[0]){
  Rstatus=1;
 
  }
//---------------------------------------------------------------------------------------------------------------------------抓包过程
  
  }
  else
 {ws=0;}
 //wifi.releaseTCP();
}
//---------------------------------------------------------舵机
void loop()
{online();

  if(a==1){
 for(int i=0;i<50;i++)  //发送50个脉冲   //为什么是50个？
  {
   int angle=45;
      servopulse(angle);
     }
  }
    if(a==2){
      for(int i=0;i<50;i++)  //发送50个脉冲   //为什么是50个？
  {
    int angle=135;
       servopulse(angle);  }
    }
         //引用脉冲函数
         //------------
         
}















