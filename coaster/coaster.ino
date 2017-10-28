#include <OneWire.h>
#include <DallasTemperature.h>

// define pins
// temp pin
#define SENSOR_PORT 3 
// color pins
#define RED_PORT 11
#define GREEN_PORT 12
#define BLUE_PORT 13

// declare 'constant' variables
// temp thresholds for color change (range in between upper and lower is green)
double TEMP_LOWER_BOUND = 23.0; // degrees celcius was 5.0
double TEMP_UPPER_BOUND = 28.0; // degrees celcius was 75.0
// color settings 

// i feel like it should be hue with the way we're defining it here...
double HSV_UPPER_BOUND = 240.0; // degrees - color for 'cold' temps 
double HSV_LOWER_BOUND = 0.0; // degrees - color for 'hot' temps
double HSV_SATURATION = 1.0; 
double HSV_VALUE = 1.0;

// declare variables
double tempInput, hsvHue, red, blue, green = 0;

OneWire oneWire(SENSOR_PORT);

DallasTemperature sensors(&oneWire);

// from http://forum.arduino.cc/index.php?topic=44216.0 from mem
void printDouble( double val, unsigned int precision){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)
// used for testing and debugging

   Serial.print (int(val));  //prints the int part
   Serial.print("."); // print the decimal point
   unsigned int frac;
   if(val >= 0)
       frac = (val - int(val)) * precision;
   else
       frac = (int(val)- val ) * precision;
   Serial.println(frac,DEC) ;
} 

void setup(void)
{
  // set up for the temp sensor
  //Serial.begin(9600);
  sensors.begin();

  // set the modes for each of the pins
  pinMode(RED_PORT, OUTPUT); 
  pinMode(GREEN_PORT, OUTPUT);
  pinMode(BLUE_PORT, OUTPUT);
}

void loop(void)
{
  // get temp data
  sensors.requestTemperatures();
  //Serial.println(sensors.getTempCByIndex(0));
  tempInput = sensors.getTempCByIndex(0);

  // convert temp to HSV
  hsvHue = temperatureToHue(tempInput);
  
//  Serial.print("hue : ");
//  printDouble(hsvHue, 100);
//  Serial.print(" (degrees)\n");

  // convert HSV to RGB values
  HSVtoRGB(red, green, blue, hsvHue, HSV_SATURATION, HSV_VALUE);

//  Serial.print("red: ");
//  Serial.print(red);
//  Serial.print(" green: ");
//  Serial.print(green);
//  Serial.print(" blue: ");
//  Serial.print(blue);
//  Serial.print("\n");
 
  delay(1000);

  // set the output for the LEDs
  setColor((int)red, (int)green, (int)blue); 

  //red, green, blue = 0.0;
}

double temperatureToHue(double temp)
{
  if (temp <= TEMP_LOWER_BOUND)
  {
    return HSV_UPPER_BOUND;
  }
  else if (temp >= TEMP_UPPER_BOUND) {
    return HSV_LOWER_BOUND;
  }
  else {
    double tempDiff = TEMP_UPPER_BOUND - TEMP_LOWER_BOUND;
    return HSV_UPPER_BOUND *(1.0 - (temp - TEMP_LOWER_BOUND)/tempDiff);
  }
}

// taken from https://gist.github.com/fairlight1337/4935ae72bcbcc1ba5c72
void HSVtoRGB(double& fR, double& fG, double& fB, double& fH, double& fS, double& fV) {
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

  fR = fR*255;
  fG = fG*255;
  fB = fB*255;
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(RED_PORT, redValue);
  analogWrite(GREEN_PORT, greenValue);
  analogWrite(BLUE_PORT, blueValue);
}

