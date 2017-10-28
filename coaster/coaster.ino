#include <OneWire.h>
#include <DallasTemperature.h>

// define constants and variables
#define SENSOR_PORT 2
#define LED_PORT 13
#define TEMP_LOWER_BOUND = 5 // degrees celcius
#define TEMP_UPPER_BOUND = 75 // degrees celcius
#define HSV_UPPER_BOUND = 240 // degrees
#define HSV_SATURATION = 1.0
#define HSV_VALUE = 1.0

float tempInput, hsvHue, red, blue, green

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
  hsvHue = temperatureToHue(tempInput);
  HSVtoRGB(red, blue, green, hsvHue, HSV_SATURATION, HSV_VALUE);
  Serial.println(red + " " + blue + " " + green);
  delay(1000);
}

float temperatureToHue(float temp)
{
  if (temp <= TEMP_LOWER_BOUND)
  {
    return (float) HSV_UPPER_BOUND;
  }
  else if (temp >= TEMP_UPPER_BOUND) {
    return 0.0
  }
  return (1 - temp / (TEMP_UPPER_BOUND - TEMP_LOWER_BOUND)) * HSV_UPPER_BOUND;
}

// taken from https://gist.github.com/fairlight1337/4935ae72bcbcc1ba5c72
void HSVtoRGB(float& fR, float& fG, float& fB, float& fH, float& fS, float& fV) {
  float fC = fV * fS; // Chroma
  float fHPrime = fmod(fH / 60.0, 6);
  float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
  float fM = fV - fC;

  if(0 <= fHPrime && fHPrime < 1) {
    fR = fC;
    fG = fX;
    fB = 0;
  } else if(1 <= fHPrime && fHPrime < 2) {
    fR = fX;
    fG = fC;
    fB = 0;
  } else if(2 <= fHPrime && fHPrime < 3) {
    fR = 0;
    fG = fC;
    fB = fX;
  } else if(3 <= fHPrime && fHPrime < 4) {
    fR = 0;
    fG = fX;
    fB = fC;
  } else if(4 <= fHPrime && fHPrime < 5) {
    fR = fX;
    fG = 0;
    fB = fC;
  } else if(5 <= fHPrime && fHPrime < 6) {
    fR = fC;
    fG = 0;
    fB = fX;
  } else {
    fR = 0;
    fG = 0;
    fB = 0;
  }

  fR += fM;
  fG += fM;
  fB += fM;
}
