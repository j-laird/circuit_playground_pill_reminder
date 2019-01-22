// Reminder for 2x/day medication.  Resets indicators to get attention when medication is due.

#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>

#define SECONDS_PER_MIN 60ul
#define MINUTES_PER_HOUR 60ul
#define SECONDS_PER_HOUR (SECONDS_PER_MIN * MINUTES_PER_HOUR)
#define SECONDS_PER_12H (SECONDS_PER_HOUR * 12ul) 
#define SECONDS_PER_10H (SECONDS_PER_HOUR * 8ul)

// we light one pixel at a time, this is our counter
uint8_t pixeln = 0;
uint8_t colorn = 0;
float x_accel;
float y_accel;
float z_accel;
uint32_t loopsSinceBottleTip = 0;

const uint32_t pill_interval_second = SECONDS_PER_12H;
const uint32_t pill_interval_first = SECONDS_PER_10H;

void setup() {
  //while (!Serial);
  Serial.begin(9600);
  //Serial.println("Circuit Playground test!");
  loopsSinceBottleTip = pill_interval_first - 10;
  CircuitPlayground.begin();
}


void loop() {
  delay(1000);

  x_accel = CircuitPlayground.motionX();
  y_accel = CircuitPlayground.motionY();
  z_accel = CircuitPlayground.motionZ();

  if((fabs(x_accel) > 4.0f) ||
     (fabs(y_accel) > 4.0f) ||
     (fabs(z_accel) < 4.0f))
  {
     loopsSinceBottleTip = 0; 
  }
  else
  {
    loopsSinceBottleTip++;
  }
  
  
  //update neopixels based on medicine state
  if(loopsSinceBottleTip == 0)
  {
    for(pixeln = 0; pixeln < 11; pixeln++)
    {
      CircuitPlayground.setPixelColor(pixeln, 0,0,255);
    }
  }
  else if(loopsSinceBottleTip == 1)
  {
    CircuitPlayground.clearPixels();
  }
  
  if(loopsSinceBottleTip > pill_interval_second)
  {
    CircuitPlayground.clearPixels();
    if(loopsSinceBottleTip % 2)
    {
      for(pixeln = 0; pixeln <= 10; pixeln += 2 )
      {
        CircuitPlayground.setPixelColor(pixeln, 0,128,0);
      }
    }
    else
    {
      for(pixeln = 1; pixeln <= 9; pixeln += 2 )
      {
        CircuitPlayground.setPixelColor(pixeln, 0,128,0);
      }
    }
  }
  else if(loopsSinceBottleTip > pill_interval_first)
  {
    CircuitPlayground.clearPixels();
    if(loopsSinceBottleTip % 2)
    {
      for(pixeln = 0; pixeln <= 10; pixeln += 2)
      {
        CircuitPlayground.setPixelColor(pixeln, 128,0,0);
      }
    }
    else
    {
      for(pixeln = 1; pixeln <= 9; pixeln += 2)
      {
        CircuitPlayground.setPixelColor(pixeln, 128,0,0);
      }
    }
  }

  Serial.print(loopsSinceBottleTip);
  Serial.print("\n");

  /************* TEST ACCEL */
  // Display the results (acceleration is measured in m/s*s)
  /*
  Serial.print(CircuitPlayground.motionX());
  Serial.print(","); 
  Serial.print(CircuitPlayground.motionY());
  Serial.print(","); 
  Serial.print(CircuitPlayground.motionZ());
  Serial.print("\n");
  */
}
