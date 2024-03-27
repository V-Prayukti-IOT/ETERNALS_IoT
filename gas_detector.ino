
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3oR0QPV_h"
#define BLYNK_TEMPLATE_NAME "fire alert"
#define BLYNK_AUTH_TOKEN "lxBrxRNUpX2FUXCzA0F6uhjb6kkpEEjD"

#include <ThingSpeak.h> 
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include<WiFiClient.h>

unsigned long ChannelNumber=2486072;
const char* apiKey ="T81V1FR32LRG5KBB";
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Nikash";
char pass[] = "nikash24";
char server[]="api.thingspeak.com";

int pinValue = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
#define SCL D1
#define SDA D2
#define Green D3
#define Yellow D4
#define Red D5
#define Buzzer D6
#define relay D7
#define Sensor A0

BlynkTimer timer;
WiFiClient client;

int lcdcontrol=0;

void setup() 
{

  Serial.begin(115200);
  ThingSpeak.begin(client);
  Blynk.begin(auth, ssid, pass);
  lcd.init();

  pinMode(Sensor, INPUT);
  pinMode(Green, OUTPUT);
  pinMode(Yellow,OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(relay,OUTPUT);

  timer.setInterval(2500L, notifiaction);
}

BLYNK_WRITE(V0) 
{
  pinValue = param.asInt();
}


void notifiaction() {
  float sensor = analogRead(Sensor);
  Serial.println(sensor);
  sensor = map(sensor, 0, 1024, 0, 100);
    Blynk.virtualWrite(V1, sensor);
  if (pinValue == 1) {
    if (sensor < 25)
     {
      digitalWrite(Green, HIGH);
      digitalWrite(Yellow,LOW);
      digitalWrite(Red, LOW);
      digitalWrite(Buzzer, LOW);
      digitalWrite(relay,LOW);

      lcd.setCursor(0, 1);
      lcd.print("Gas value:Normal");
    } 
    else if (sensor > 26 && sensor < 70)
     { 
      digitalWrite(relay,HIGH);
      digitalWrite(Green, LOW);
      digitalWrite(Yellow,HIGH);
      digitalWrite(Red, LOW);
      Blynk.logEvent("fire_alert","Danger!!! High Gas Leakage detected");
      lcd.setCursor(0, 1);
      lcd.print("Gas value:Medium");
    }
    else
    {
     
      Blynk.logEvent("fire_alert","Danger!!! High Gas Leakage detected");
      digitalWrite(Green,LOW);
      digitalWrite(relay,HIGH);
      digitalWrite(Yellow,LOW);
      digitalWrite(Red,HIGH);
      digitalWrite(Buzzer,HIGH);
      lcd.setCursor(0,1);
      lcd.print("Gas value:Danger");
    }
     lcd.setCursor(0, 0);
     lcd.print("Value : ");
     lcd.print(sensor);
  }
   else 
   {
    digitalWrite(Red, LOW);
    digitalWrite(Yellow,LOW);
    digitalWrite(Buzzer, LOW);
    digitalWrite(Green, LOW);
    digitalWrite(relay,LOW);
    lcd.clear();
  }
}

void loop() 
{
  float sensor =analogRead(Sensor);
  if(pinValue == 0)
  {
    ThingSpeak.writeField(ChannelNumber,1,0,apiKey);
  }
  else
  {
  delay(1000);
  ThingSpeak.writeField(ChannelNumber,1,sensor,apiKey);
  delay(1600);
  }
  Blynk.run();
  timer.run();
}