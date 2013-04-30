/**********************************************
 * 
 * SMART BIRDHOUSE
 *
 * Sensors: 
 *   1. Light
 *   2. Sound
 *   3. Air quality
 *   4. Temperature and humidity
 *   5. Fine dust (particles)
 *
 * Temp + Humidity: http://www.seeedstudio.com/wiki/Grove-_Temperature_and_Humidity_Sensor
 * Dust sensor: http://www.seeedstudio.com/depot/grove-dust-sensor-p-1050.html http://www.sca-shinyei.com/pdf/PPD42NS.pdf 
 * 
 **********************************************/
 

/** 
 * LIBRARIES
 **/
#include <math.h>
#include "DHT.h"                // 4. Temp and Humidity Sensor

#define FEEDID  78529            // COSM settings: feedID
// SENSORS
//   1. Light
float Rsensor;                   // Light sensor: Resistance of sensor in K
//   2. Sound

//   3. Air quality

//   4. Temperature and humidity
#define DHTPIN A0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
//   5. Fine dust (particles)
int pin = 2;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 10000;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;



/**********************************************
 ** 
 ** SETUP
 ** 
 **********************************************/
void setup() {

  // Start the serial monitor
  Serial.begin(9600); 
  Serial.println("LOG, Birdhouse starting! ");
  
  // Setup the sensors
  //   1. Light
  Serial.println("LOG, Setup sensor 1: Light sensor START ");
  
  Serial.println("LOG, Setup sensor 1: Light sensor COMPLETE");
  //   2. Sound
  Serial.println("LOG, Setup sensor 2: Sound sensor START");
  
  Serial.println("LOG, Setup sensor 2: Sound sensor COMPLETE");
  //   3. Air quality
  Serial.println("LOG, Setup sensor 3: Air quality sensor START");
  
  Serial.println("LOG, Setup sensor 3: Air quality COMPLETE");
  //   4. Temperature and humidity
  Serial.println("LOG, Setup sensor 4: Temp and Humidity sensor START");
  dht.begin();
  Serial.println("LOG, Setup sensor 4: Temp and Humidity sensor COMPLETE");
  //   5. Fine dust (particles)
  Serial.println("LOG, Setup sensor 5: Dust sensor START");
  pinMode(8,INPUT);
  Serial.println("LOG, Setup sensor 5: Dust sensor COMPLETE");
  
  // Define the current timestamp 
  starttime = millis();

}



/**********************************************
 **
 ** MAIN LOOP
 ** 
 **********************************************/
void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("ERROR, Failed to read from DHT");
  } else {
    //Serial.print("Humidity: "); 
    //Serial.print(h);
    //Serial.print(" %\t");
    //Serial.print("Temperature: "); 
    //Serial.print(t);
    //Serial.println(" *C");
  }
  
  
  // Light
  int sensorValue = analogRead(1); 
  Rsensor=(float)(1023-sensorValue)*10/sensorValue;
  //Serial.println("the analog read data is ");
  //Serial.println(sensorValue);
  //Serial.println("the sensor resistance is ");
  //Serial.println(Rsensor,DEC);//show the ligth intensity on the serial monitor;
  
  
  // Dust sensor   
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;

  if ((millis()-starttime) > sampletime_ms)
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    Serial.print(lowpulseoccupancy);
    Serial.print(",");
    Serial.print(ratio);
    Serial.print(",");
    Serial.println(concentration);
    lowpulseoccupancy = 0;
    starttime = millis();
  }
  
  Serial.print("FEED"); 
  Serial.print(","); 
  Serial.print(FEEDID); 
  Serial.print(","); 
  Serial.print("10"); 
  Serial.print(",");
  Serial.println("20"); 
  delay(1000); 
  
}
