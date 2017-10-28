#include <OneWire.h>
#include <DallasTemperature.h>

#define SENSOR_PORT 2

OneWire oneWire(SENSOR_PORT);

DallasTemperature sensors(&oneWire);

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
}

void loop(void)
{
  sensors.requestTemperatures();
  Serial.println(sensors.getTempCByIndex(0));
  delay(1000);
}
