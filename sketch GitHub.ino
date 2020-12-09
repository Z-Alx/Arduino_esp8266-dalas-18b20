#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D2

uint32_t Timer;
int period = 900000; // 15 min между уведомлениями

float temp, temp1;

String message, theme;
String gradus = "°C";

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

char auth[] = "4gq-zwm08RB2FYH9AAM0DNuyPgncN1Jd";
char ssid[] = "ist";
char pass[] = "qwerty123";

void setup() 
{
Serial.begin(115200);
Blynk.begin(auth, ssid, pass);
sensors.begin();
}

void sendTemps()
{
sensors.requestTemperatures();
temp = sensors.getTempCByIndex(0); 
temp1 = sensors.getTempCByIndex(1); 
Serial.println(temp);
Blynk.virtualWrite(V1, temp);
Blynk.virtualWrite(V2, temp);
}

void sendAlarm()
{
  if (temp < 14) {
   if (millis() - Timer >= period) {
    Timer = millis(); 
    message = String("Температура в доме опустилась ниже установленного предела в 14°C и равна ") + temp + gradus;
    theme = String("Warning sensor t =") + temp + gradus;
    Blynk.email("ifinch.pw@gmail.com", theme, message);
    Blynk.notify (theme);
   }
  }
}

void loop() 
{
Blynk.run();
sendTemps();
sendAlarm();
}
