// Reminder for 2x/day medication.  Resets indicators to get attention when medication is due.

#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>

#define SECONDS_PER_MIN 60ul
#define MINUTES_PER_HOUR 60ul
#define SECONDS_PER_HOUR (SECONDS_PER_MIN * MINUTES_PER_HOUR)
#define SECONDS_PER_12H (SECONDS_PER_HOUR * 12ul) 
#define SECONDS_PER_10H (SECONDS_PER_HOUR * 8ul)
#define SECONDS_PER_PIXEL_24H (SECONDS_PER_HOUR * 24ul / 10ul)

// we light one pixel at a time, this is our counter
uint8_t pixeln = 0;
uint8_t colorn = 0;
float x_accel;
float y_accel;
float z_accel;
uint32_t loopsSinceBottleTip = 0;
uint32_t hoursSinceBottleTip;

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
      CircuitPlayground.setPixelColor(pixeln, 255,255,255);
      delay(100);
      CircuitPlayground.clearPixels();
    }
  }

  // every 10 seconds, show elapsed time since medication by indicating in dim purple (1 pixel = 2.4h elapsed):
  if(!(loopsSinceBottleTip % 10))
  {
    for(pixeln=0; pixeln<10; pixeln++)
      {
        if(pixeln < (loopsSinceBottleTip/SECONDS_PER_PIXEL_24H))
        {
          CircuitPlayground.setPixelColor(pixeln, 16,0,16);
        }
      }
    delay(1000);
    CircuitPlayground.clearPixels();
    // we just burned an extra second, so add to time:
    loopsSinceBottleTip++;
  }

  // show green animation if > 12h:
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
  
  // show amber animation if 10-12h:
  else if(loopsSinceBottleTip > pill_interval_first)
  {
    CircuitPlayground.clearPixels();
    if(loopsSinceBottleTip % 2)
    {
      for(pixeln = 0; pixeln <= 10; pixeln += 2)
      {
        CircuitPlayground.setPixelColor(pixeln, 128,128,0);
      }
    }
    else
    {
      for(pixeln = 1; pixeln <= 9; pixeln += 2)
      {
        CircuitPlayground.setPixelColor(pixeln, 128,128,0);
      }
    }
  }

  
  // Debug elapsed time:
  //Serial.print(loopsSinceBottleTip);
  //Serial.print("\n");

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
