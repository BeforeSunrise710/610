
//传感器部分================================   
#include <Wire.h>     //调用库  
#include "./ESP8266.h"
#include "I2Cdev.h"                                //调用库  
//温湿度   
#include "./SHT2x.h"
//光照
#define  sensorPin_1  A0
#define  d_pin 4
#define SSID           "flypig"                   // cannot be longer than 32 characters!
#define PASSWORD       "123456789"
#define IDLE_TIMEOUT_MS  3000      
#define HOST_NAME   "api.heclouds.com"
// 修改成自己的ID
#define DEVICEID   "503083986"
#define PROJECTID "183704"
#define HOST_PORT   (80)                                                                                                               
#define INTERVAL_sensor 2000
#define INTERVAL_OLED 1000
String apiKey="Lz6kPLRd=Qe9Qt=JkoUPnkyLLys=";       // 修改成自己在OneNet上面的apikey // 大概长这样: VeFI0HZ44Qn5dZO14AuLbWSlSlI=
char buf[10];
unsigned long sensorlastTime = millis();
float tempOLED, humiOLED, lightnessOLED;
String mCottenData;
String jsonToSend;
//3,传感器值的设置 
float sensortem, sensorhum, sensorlux;                    //传感器温度、湿度、光照   
char  sensortem_c[7], sensorhum_c[7], sensorlux_c[7] ;    //换成char数组传输
float sensor_d;
char sensor_d_c[7];
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D3, TX:D2 */
ESP8266 wifi(mySerial);
//ESP8266 wifi(Serial1);                                      //定义一个ESP8266（wifi）的对象
unsigned long net_time1 = millis();                          //数据上传服务器时间
unsigned long sensor_time = millis();                        //传感器采样时间计时器
//int SensorData;                                   //用于存储传感器数据
String postString;                                //用于存储发送数据的字符串
//String jsonToSend;                                //用于存储发送的json格式参数    // variable to store the servo position
void setup(void)     //初始化函数  
{    
  //初始化串口波特率  
    Wire.begin();
    Serial.begin(115200);   
    while(!Serial);
    pinMode(sensorPin_1, INPUT);
    pinMode(d_pin,INPUT);
   //ESP8266初始化
    Serial.println(wifi.getVersion().c_str());
  if (wifi.setOprToStationSoftAP())
  {
   }
  else
  {
   }
  if (wifi.joinAP(SSID, PASSWORD))
  {      //加入无线网
    Serial.print("success\r\n");  
    Serial.println(wifi.getLocalIP().c_str());
  } 
  else
  {
    Serial.print("failure\r\n");
  }

  if (wifi.disableMUX())
  {
    Serial.print("ok\r\n");
  } 
  else 
  {
    Serial.print("err\r\n");
  }
  Serial.print("setup end\r\n");  
}
void loop(void)
{  
      if (sensor_time > millis())
      { 
      sensor_time = millis(); 
      } 
      if(millis() - sensor_time > 1000)                //传感器采样时间间隔  
     {  
        sensortem = SHT2x.GetTemperature() ;   
        sensorhum = SHT2x.GetHumidity();   
        sensorlux = analogRead(A0);  
        delay(1000);
        dtostrf(sensortem, 2, 1, sensortem_c);
        dtostrf(sensorhum, 2, 1, sensorhum_c);
        dtostrf(sensorlux, 3, 1, sensorlux_c);         //读串口中的传感器数据
        sensor_time = millis();
        shoujid();
     }  
     if (net_time1 > millis()) 
     {
     net_time1 = millis();
     }
     if (millis() - net_time1 > 1000)                  //发送数据时间间隔
    {                
       fasongshuju();                                 //将数据上传到服务器的函数  
       fasongd();                           //将数据上传到服务器的函数
       net_time1 = millis();
    }
  }
void fasongshuju() {
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) 
  {                                             //建立TCP连接，如果失败，不能发送该数据
    Serial.print("tcp ok\r\n");
    jsonToSend="{\"Temperature\":";
    dtostrf(sensortem,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+=",\"Humidity\":";
    dtostrf(sensorhum,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+=",\"Light\":";
    dtostrf(sensorlux,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+="}";
    postString="POST /devices/";
    postString+=DEVICEID;
    postString+="/datapoints?type=3 HTTP/1.1";
    postString+="\r\n";
    postString+="api-key:";
    postString+=apiKey;
    postString+="\r\n";
    postString+="Host:api.heclouds.com\r\n";
    postString+="Connection:close\r\n";
    postString+="Content-Length:";
    postString+=jsonToSend.length();
    postString+="\r\n";
    postString+="\r\n";
    postString+=jsonToSend;
    postString+="\r\n";
    postString+="\r\n";
    postString+="\r\n";
  const char *postArray = postString.c_str();                 //将str转化为char数组、
  wifi.send((const uint8_t*)postArray, strlen(postArray));    //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
  Serial.println(postArray);
  Serial.println("send success");   
     if (wifi.releaseTCP()) 
       {                                 //释放TCP连接
        Serial.print("ok\r\n");
        } 
     else 
       {
        Serial.print("err\r\n");
        }
      postArray = NULL;                                       //清空数组，等待下次传输数据
  
  }
  else
  {
    Serial.print("tcp err\r\n");
  }
}
void shoujid()
{
  sensor_d=digitalRead(d_pin);
  dtostrf(sensor_d,2,1,sensor_d_c);
}
void fasongd()
{
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
    Serial.print("tcp ok\r\n");
    jsonToSend="{\"d\":";
    dtostrf(sensor_d,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+="}";
    postString="POST /devices/";
    postString+=DEVICEID;
    postString+="/datapoints?type=3 HTTP/1.1";
    postString+="\r\n";
    postString+="api-key:";
    postString+=apiKey;
    postString+="\r\n";
    postString+="Host:api.heclouds.com\r\n";
    postString+="Connection:close\r\n";
    postString+="Content-Length:";
    postString+=jsonToSend.length();
    postString+="\r\n";
    postString+="\r\n";
    postString+=jsonToSend;
    postString+="\r\n";
    postString+="\r\n";
    postString+="\r\n";
  const char *postArray = postString.c_str();                 //将str转化为char数组、
  Serial.println(postArray);
  wifi.send((const uint8_t*)postArray, strlen(postArray));    //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
  Serial.println(postArray);
  Serial.println("send success");   
     if (wifi.releaseTCP()) 
       {                                 //释放TCP连接
        Serial.print("ok\r\n");
        } 
     else 
       {
        Serial.print("err\r\n");
        }
      postArray = NULL;                                       //清空数组，等待下次传输数据
  
  }
  else
  {
    Serial.print("tcp err\r\n");
  }
}

