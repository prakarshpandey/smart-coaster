#include <OneWire.h>
#include <DallasTemperature.h>

// define constants and variables
#define SENSOR_PORT 2
#define LED_PORT 13
float tempInput

OneWire oneWire(SENSOR_PORT);

DallasTemperature sensors(&oneWire);

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
  pinMode(LED_PORT, OUTPUT); 
}

void loop(void)
{
  digitalWrite(LED_PORT, LOW);
  delay(1000);
  digitalWrite(LED_PORT, HIGH);
  
  sensors.requestTemperatures();
  Serial.println(sensors.getTempCByIndex(0));
  tempInput = sensors.getTempCByIndex(0);
  delay(1000);
}


