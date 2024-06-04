#include <Adafruit_NeoPixel.h>

#define LED_COUNT 60
#define PIN 7
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN_top   6     
#define LED_PIN_0  5
#define LED_PIN_4 3
#define LED_PIN_8 9

// pin for smoke machine
#define SMOKE_PIN 10

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT_top  24
#define LED_COUNT_sides 8


// NeoPixel brightness, 0 (min) to 255 (max)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip_top = Adafruit_NeoPixel(LED_COUNT_top, LED_PIN_top, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip_0 = Adafruit_NeoPixel(LED_COUNT_sides, LED_PIN_0, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip_4 = Adafruit_NeoPixel(LED_COUNT_sides, LED_PIN_4, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip_8 = Adafruit_NeoPixel(LED_COUNT_sides, LED_PIN_8, NEO_GRBW + NEO_KHZ800);

Adafruit_NeoPixel strip_bottom = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRBW + NEO_KHZ800);


// Randomly selected starting points (where our 3 side strips reside) - currently used for thruderstrom
const int startPoints[3] = {0, 19, 39};


uint32_t baseColor = strip_bottom.Color(0, 0, 10, 0); // Example base color
uint32_t waveColor = strip_bottom.Color(10, 10, 50, 0); // Example wave color
int rippleLengthDefault = 15;

void setup() 
{
  strip_0.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_0.show();            // Turn OFF all pixels ASAP
  
  strip_4.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_4.show();            // Turn OFF all pixels ASAP
  
  strip_8.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_8.show();            // Turn OFF all pixels ASAP
  
  strip_bottom.begin();
  strip_bottom.show(); // Initialize all pixels to 'off'
  randomSeed(analogRead(0)); // Seed for random number generator
}

void loop() 
{
  strip_bottom.fill(baseColor);
  strip_bottom.show();
  int stripNumber = random(0, 3);
  int size = random(0, 10);
  int startPixel = startPoints[stripNumber];
  Adafruit_NeoPixel* mainStrip;
  
  switch (stripNumber)
  {
      case 0:
          mainStrip = &strip_0;     
          break;
      case 1:
          mainStrip = &strip_4;
          break;
      case 2:
          mainStrip = &strip_8;
          break;
  }
  raindrop(mainStrip, strip_bottom.Color(0, 0, 100, 0),strip_bottom.Color(0, 0, 0, 0));
  delay(25);
  strip_bottom.setPixelColor(startPixel, strip_bottom.Color(0, 0, 100, 0));
  strip_bottom.show();
  delay(25);
  if (size < 5)
  {
      createTinyRipple(5, startPixel);
  } 
  else if (size < 8)
  { 
      createSmallRipple(7, startPixel);
  }
  else if (size < 9)
  {
      createMediumRipple(12, startPixel);
  }
  else
      createRipple(random(15,20), startPixel);
  
  delay(0); // Wait for 2 seconds before creating another ripple
}


void raindrop(Adafruit_NeoPixel* mainStrip, uint32_t dropColor, uint32_t baseColor)
{
  // Light up the main strip from 0 to 7 pixels with accelerating speed
  for (int i = LED_COUNT_sides - 1; i >= 0; i--)
  {
    mainStrip->setPixelColor(i, dropColor);
    mainStrip->show();
    delay(30 - (i * 3)); // Accelerating delay
    mainStrip->setPixelColor(i, baseColor);
  }

  // Turn off the main strip after blinking
  mainStrip->clear();
  mainStrip->show();
}


void createRipple(int rippleLength, int startPixel) 
{
  uint32_t fadeColor1 = blendColors(waveColor, baseColor, 70);
  uint32_t fadeColor2 = blendColors(waveColor, baseColor, 40);
  uint32_t fadeColor3 = blendColors(waveColor, baseColor, 20);

  strip_bottom.fill(baseColor);
  strip_bottom.show();

  for (int pos = 0; pos <= rippleLength; pos++) 
  {
    int placeInWave = 1;
    // Update ripple effect
    for (int i = pos; i >=0; i--) 
    {
      int pixelIndexForward = (startPixel + i) % LED_COUNT;
      int pixelIndexBackward = (startPixel - i + LED_COUNT) % LED_COUNT;

      if (placeInWave == 1 || placeInWave == 6 || placeInWave == 11) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, waveColor); // Ripple head forward
        strip_bottom.setPixelColor(pixelIndexBackward, waveColor); // Ripple head backward
      } 
      else if (placeInWave == 2 || placeInWave == 7 || placeInWave == 12) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, fadeColor1); // 30% fade forward
        strip_bottom.setPixelColor(pixelIndexBackward, fadeColor1); // 30% fade backward
      } 
      else if (placeInWave == 3 || placeInWave == 8 || placeInWave == 13) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, fadeColor2); // 60% fade forward
        strip_bottom.setPixelColor(pixelIndexBackward, fadeColor2); // 60% fade backward
      } 
      else if (placeInWave == 4 || placeInWave == 9 || placeInWave == 14) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, fadeColor3); // 80% fade forward
        strip_bottom.setPixelColor(pixelIndexBackward, fadeColor3); // 80% fade backward
      }
      else if (placeInWave == 5 || placeInWave == 10 || placeInWave == 15) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, baseColor); // back to base
        strip_bottom.setPixelColor(pixelIndexBackward, baseColor); // back to base
      }

      placeInWave++;
    }

    strip_bottom.show();
    delay(35); // Adjust delay for ripple speed
  }

  for (int fadeSteps = 0; fadeSteps < 6; fadeSteps++)
  {
    int moveSteps = 0;
    for (int i = rippleLength; i > 0; i--)
    {
      int pixelIndexForward = (startPixel + i) % LED_COUNT;
      int pixelIndexBackward = (startPixel - i + LED_COUNT) % LED_COUNT;

      if (fadeSteps == 0 && strip_bottom.getPixelColor(pixelIndexForward) == fadeColor3)
      {
        moveSteps++;
      }
      else if (fadeSteps == 1 && strip_bottom.getPixelColor(pixelIndexForward) == fadeColor2)
      {
        moveSteps++;
      }
      else if (fadeSteps == 2 && strip_bottom.getPixelColor(pixelIndexForward) == fadeColor1)
      {
        moveSteps++;
      }
      else if (fadeSteps == 3 && strip_bottom.getPixelColor(pixelIndexForward) == fadeColor1)
      {
        moveSteps++;
      }
      else if (fadeSteps == 4 && strip_bottom.getPixelColor(pixelIndexForward) == baseColor)
      {
        moveSteps++;
      }

      if (moveSteps > 0)
      {
        strip_bottom.setPixelColor(pixelIndexForward, strip_bottom.getPixelColor((pixelIndexForward - moveSteps + LED_COUNT) % LED_COUNT));
        strip_bottom.setPixelColor(pixelIndexBackward, strip_bottom.getPixelColor((pixelIndexBackward + moveSteps) % LED_COUNT));
      }

    }
    strip_bottom.show();
    delay(30); // Adjust delay for ripple speed
  }

  for (int j = 0; j < 5 + 1;j++)
  {
    for (int i = rippleLength; i > 0; i--)
    {
      int pixelIndexForward = (startPixel + i) % LED_COUNT;
      int pixelIndexBackward = (startPixel - i + LED_COUNT) % LED_COUNT;

      if (strip_bottom.getPixelColor(pixelIndexForward) != baseColor && strip_bottom.getPixelColor((pixelIndexForward - 1 + LED_COUNT) % LED_COUNT) == baseColor)
      {
        strip_bottom.setPixelColor(pixelIndexForward, strip_bottom.getPixelColor((pixelIndexForward - 1 + LED_COUNT) % LED_COUNT));
        strip_bottom.setPixelColor(pixelIndexBackward, strip_bottom.getPixelColor((pixelIndexBackward + 1) % LED_COUNT));
      }
    }
    strip_bottom.show();
    delay(20); // Adjust delay for ripple speed
  }
}


void createTinyRipple(int rippleLength, int startPixel)   // 1 - 6
{
  uint32_t fadeColor1 = blendColors(waveColor, baseColor, 70);
  uint32_t fadeColor2 = blendColors(waveColor, baseColor, 40);
  uint32_t fadeColor3 = blendColors(waveColor, baseColor, 20);

  strip_bottom.fill(baseColor);
  strip_bottom.show();

  for (int pos = 0; pos <= rippleLength; pos++) 
  {
    int placeInWave = 1;
    // Update ripple effect
    for (int i = pos; i >=0; i--) 
    {
      int pixelIndexForward = (startPixel + i) % LED_COUNT;
      int pixelIndexBackward = (startPixel - i + LED_COUNT) % LED_COUNT;

      if (placeInWave == 1) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, waveColor); // Ripple head forward
        strip_bottom.setPixelColor(pixelIndexBackward, waveColor); // Ripple head backward
      } 
      else if (placeInWave == 2) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, fadeColor1); // 30% fade forward
        strip_bottom.setPixelColor(pixelIndexBackward, fadeColor1); // 30% fade backward
      } 
      else if (placeInWave == 3) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, fadeColor2); // 60% fade forward
        strip_bottom.setPixelColor(pixelIndexBackward, fadeColor2); // 60% fade backward
      } 
      else if (placeInWave == 4) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, fadeColor3); // 80% fade forward
        strip_bottom.setPixelColor(pixelIndexBackward, fadeColor3); // 80% fade backward
      }
      else if (placeInWave == 5) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, baseColor); // back to base
        strip_bottom.setPixelColor(pixelIndexBackward, baseColor); // back to base
      }

      placeInWave++;
    }

    strip_bottom.show();
    delay(35); // Adjust delay for ripple speed
  }

  for (int fadeSteps = 0; fadeSteps < 6; fadeSteps++) // removing all middle colors and staying only with wave color and base color, all wave color in front
  {
    int moveSteps = 0;
    for (int i = rippleLength; i > 0; i--)
    {
      int pixelIndexForward = (startPixel + i) % LED_COUNT;
      int pixelIndexBackward = (startPixel - i + LED_COUNT) % LED_COUNT;

      if (fadeSteps == 0 && strip_bottom.getPixelColor(pixelIndexForward) == fadeColor3)
      {
        moveSteps++;
      }
      else if (fadeSteps == 1 && strip_bottom.getPixelColor(pixelIndexForward) == fadeColor2)
      {
        moveSteps++;
      }
      else if (fadeSteps == 2 && strip_bottom.getPixelColor(pixelIndexForward) == fadeColor1)
      {
        moveSteps++;
      }
      else if (fadeSteps == 3 && strip_bottom.getPixelColor(pixelIndexForward) == fadeColor1)
      {
        moveSteps++;
      }
      else if (fadeSteps == 4 && strip_bottom.getPixelColor(pixelIndexForward) == baseColor)
      {
        moveSteps++;
      }

      if (moveSteps > 0)
      {
        strip_bottom.setPixelColor(pixelIndexForward, strip_bottom.getPixelColor((pixelIndexForward - moveSteps + LED_COUNT) % LED_COUNT));
        strip_bottom.setPixelColor(pixelIndexBackward, strip_bottom.getPixelColor((pixelIndexBackward + moveSteps) % LED_COUNT));
      }

    }
    strip_bottom.show();
    delay(30); // Adjust delay for ripple speed
  }

  for (int j = 0; j < 4 + 1;j++) // removing all wave colors one by one
  {
    for (int i = rippleLength; i > 0; i--)
    {
      int pixelIndexForward = (startPixel + i) % LED_COUNT;
      int pixelIndexBackward = (startPixel - i + LED_COUNT) % LED_COUNT;

      if (strip_bottom.getPixelColor(pixelIndexForward) != baseColor && strip_bottom.getPixelColor((pixelIndexForward - 1 + LED_COUNT) % LED_COUNT) == baseColor)
      {
        strip_bottom.setPixelColor(pixelIndexForward, strip_bottom.getPixelColor((pixelIndexForward - 1 + LED_COUNT) % LED_COUNT));
        strip_bottom.setPixelColor(pixelIndexBackward, strip_bottom.getPixelColor((pixelIndexBackward + 1) % LED_COUNT));
      }
    }
    strip_bottom.show();
    delay(20); // Adjust delay for ripple speed
  }
}


void createSmallRipple(int rippleLength, int startPixel)   // 1 - 6
{
  uint32_t fadeColor1 = blendColors(waveColor, baseColor, 70);
  uint32_t fadeColor2 = blendColors(waveColor, baseColor, 40);
  uint32_t fadeColor3 = blendColors(waveColor, baseColor, 20);

  strip_bottom.fill(baseColor);
  strip_bottom.show();

  for (int pos = 0; pos <= rippleLength; pos++) 
  {
    int placeInWave = 1;
    // Update ripple effect
    for (int i = pos; i >=0; i--) 
    {
      int pixelIndexForward = (startPixel + i) % LED_COUNT;
      int pixelIndexBackward = (startPixel - i + LED_COUNT) % LED_COUNT;

      if (placeInWave == 1 || placeInWave == 6) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, waveColor); // Ripple head forward
        strip_bottom.setPixelColor(pixelIndexBackward, waveColor); // Ripple head backward
      } 
      else if (placeInWave == 2) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, fadeColor1); // 30% fade forward
        strip_bottom.setPixelColor(pixelIndexBackward, fadeColor1); // 30% fade backward
      } 
      else if (placeInWave == 3) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, fadeColor2); // 60% fade forward
        strip_bottom.setPixelColor(pixelIndexBackward, fadeColor2); // 60% fade backward
      } 
      else if (placeInWave == 4) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, fadeColor3); // 80% fade forward
        strip_bottom.setPixelColor(pixelIndexBackward, fadeColor3); // 80% fade backward
      }
      else if (placeInWave == 5 || placeInWave == 7) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, baseColor); // back to base
        strip_bottom.setPixelColor(pixelIndexBackward, baseColor); // back to base
      }

      placeInWave++;
    }

    strip_bottom.show();
    delay(35); // Adjust delay for ripple speed
  }

  for (int fadeSteps = 0; fadeSteps < 6; fadeSteps++) // removing all middle colors and staying only with wave colr and base color, all wave color in front
  {
    int moveSteps = 0;
    for (int i = rippleLength; i > 0; i--)
    {
      int pixelIndexForward = (startPixel + i) % LED_COUNT;
      int pixelIndexBackward = (startPixel - i + LED_COUNT) % LED_COUNT;

      if (fadeSteps == 0 && strip_bottom.getPixelColor(pixelIndexForward) == fadeColor3)
      {
        moveSteps++;
      }
      else if (fadeSteps == 1 && strip_bottom.getPixelColor(pixelIndexForward) == fadeColor2)
      {
        moveSteps++;
      }
      else if (fadeSteps == 2 && strip_bottom.getPixelColor(pixelIndexForward) == fadeColor1)
      {
        moveSteps++;
      }
      else if (fadeSteps == 3 && strip_bottom.getPixelColor(pixelIndexForward) == fadeColor1)
      {
        moveSteps++;
      }
      else if (fadeSteps == 4 && strip_bottom.getPixelColor(pixelIndexForward) == baseColor)
      {
        moveSteps++;
      }

      if (moveSteps > 0)
      {
        strip_bottom.setPixelColor(pixelIndexForward, strip_bottom.getPixelColor((pixelIndexForward - moveSteps + LED_COUNT) % LED_COUNT));
        strip_bottom.setPixelColor(pixelIndexBackward, strip_bottom.getPixelColor((pixelIndexBackward + moveSteps) % LED_COUNT));
      }

    }
    strip_bottom.show();
    delay(30); // Adjust delay for ripple speed
  }

  for (int j = 0; j < 4 + 1;j++) // removing all wave colors one by one
  {
    for (int i = rippleLength; i > 0; i--)
    {
      int pixelIndexForward = (startPixel + i) % LED_COUNT;
      int pixelIndexBackward = (startPixel - i + LED_COUNT) % LED_COUNT;

      if (strip_bottom.getPixelColor(pixelIndexForward) != baseColor && strip_bottom.getPixelColor((pixelIndexForward - 1 + LED_COUNT) % LED_COUNT) == baseColor)
      {
        strip_bottom.setPixelColor(pixelIndexForward, strip_bottom.getPixelColor((pixelIndexForward - 1 + LED_COUNT) % LED_COUNT));
        strip_bottom.setPixelColor(pixelIndexBackward, strip_bottom.getPixelColor((pixelIndexBackward + 1) % LED_COUNT));
      }
    }
    strip_bottom.show();
    delay(20); // Adjust delay for ripple speed
  }
}

void createMediumRipple(int rippleLength, int startPixel)   // 1 - 6
{
  uint32_t fadeColor1 = blendColors(waveColor, baseColor, 70);
  uint32_t fadeColor2 = blendColors(waveColor, baseColor, 40);
  uint32_t fadeColor3 = blendColors(waveColor, baseColor, 20);

  strip_bottom.fill(baseColor);
  strip_bottom.show();

  for (int pos = 0; pos <= rippleLength; pos++) 
  {
    int placeInWave = 1;
    // Update ripple effect
    for (int i = pos; i >=0; i--) 
    {
      int pixelIndexForward = (startPixel + i) % LED_COUNT;
      int pixelIndexBackward = (startPixel - i + LED_COUNT) % LED_COUNT;

      if (placeInWave == 1 || placeInWave == 6 || placeInWave == 11) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, waveColor); // Ripple head forward
        strip_bottom.setPixelColor(pixelIndexBackward, waveColor); // Ripple head backward
      } 
      else if (placeInWave == 2 || placeInWave == 7) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, fadeColor1); // 30% fade forward
        strip_bottom.setPixelColor(pixelIndexBackward, fadeColor1); // 30% fade backward
      } 
      else if (placeInWave == 3 || placeInWave == 8) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, fadeColor2); // 60% fade forward
        strip_bottom.setPixelColor(pixelIndexBackward, fadeColor2); // 60% fade backward
      } 
      else if (placeInWave == 4 || placeInWave == 9) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, fadeColor3); // 80% fade forward
        strip_bottom.setPixelColor(pixelIndexBackward, fadeColor3); // 80% fade backward
      }
      else if (placeInWave == 5 || placeInWave == 10 || placeInWave == 12) 
      {
        strip_bottom.setPixelColor(pixelIndexForward, baseColor); // back to base
        strip_bottom.setPixelColor(pixelIndexBackward, baseColor); // back to base
      }

      placeInWave++;
    }

    strip_bottom.show();
    delay(35); // Adjust delay for ripple speed
  }

  for (int fadeSteps = 0; fadeSteps < 6; fadeSteps++) // removing all middle colors and staying only with wave colr and base color, all wave color in front
  {
    int moveSteps = 0;
    for (int i = rippleLength; i > 0; i--)
    {
      int pixelIndexForward = (startPixel + i) % LED_COUNT;
      int pixelIndexBackward = (startPixel - i + LED_COUNT) % LED_COUNT;

      if (fadeSteps == 0 && strip_bottom.getPixelColor(pixelIndexForward) == fadeColor3)
      {
        moveSteps++;
      }
      else if (fadeSteps == 1 && strip_bottom.getPixelColor(pixelIndexForward) == fadeColor2)
      {
        moveSteps++;
      }
      else if (fadeSteps == 2 && strip_bottom.getPixelColor(pixelIndexForward) == fadeColor1)
      {
        moveSteps++;
      }
      else if (fadeSteps == 3 && strip_bottom.getPixelColor(pixelIndexForward) == fadeColor1)
      {
        moveSteps++;
      }
      else if (fadeSteps == 4 && strip_bottom.getPixelColor(pixelIndexForward) == baseColor)
      {
        moveSteps++;
      }

      if (moveSteps > 0)
      {
        strip_bottom.setPixelColor(pixelIndexForward, strip_bottom.getPixelColor((pixelIndexForward - moveSteps + LED_COUNT) % LED_COUNT));
        strip_bottom.setPixelColor(pixelIndexBackward, strip_bottom.getPixelColor((pixelIndexBackward + moveSteps) % LED_COUNT));
      }

    }
    strip_bottom.show();
    delay(30); // Adjust delay for ripple speed
  }

  for (int j = 0; j < 4 + 1;j++) // removing all wave colors one by one
  {
    for (int i = rippleLength; i > 0; i--)
    {
      int pixelIndexForward = (startPixel + i) % LED_COUNT;
      int pixelIndexBackward = (startPixel - i + LED_COUNT) % LED_COUNT;

      if (strip_bottom.getPixelColor(pixelIndexForward) != baseColor && strip_bottom.getPixelColor((pixelIndexForward - 1 + LED_COUNT) % LED_COUNT) == baseColor)
      {
        strip_bottom.setPixelColor(pixelIndexForward, strip_bottom.getPixelColor((pixelIndexForward - 1 + LED_COUNT) % LED_COUNT));
        strip_bottom.setPixelColor(pixelIndexBackward, strip_bottom.getPixelColor((pixelIndexBackward + 1) % LED_COUNT));
      }
    }
    strip_bottom.show();
    delay(20); // Adjust delay for ripple speed
  }
}

uint32_t blendColors(uint32_t color1, uint32_t color2, int percentage)
{
    uint8_t r1 = (color1 >> 24) & 0xFF;
    uint8_t g1 = (color1 >> 16) & 0xFF;
    uint8_t b1 = (color1 >> 8) & 0xFF;
    uint8_t w1 = color1 & 0xFF;

    uint8_t r2 = (color2 >> 24) & 0xFF;
    uint8_t g2 = (color2 >> 16) & 0xFF;
    uint8_t b2 = (color2 >> 8) & 0xFF;
    uint8_t w2 = color2 & 0xFF;

    uint8_t r_blend = ((r1 * (100 - percentage)) + (r2 * percentage)) / 100;
    uint8_t g_blend = ((g1 * (100 - percentage)) + (g2 * percentage)) / 100;
    uint8_t b_blend = ((b1 * (100 - percentage)) + (b2 * percentage)) / 100;
    uint8_t w_blend = ((w1 * (100 - percentage)) + (w2 * percentage)) / 100;

    return (r_blend << 24) | (g_blend << 16) | (b_blend << 8) | w_blend;
}
