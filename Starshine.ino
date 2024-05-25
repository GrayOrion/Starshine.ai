// NeoPixel test program showing use of the WHITE channel for RGBW
// pixels only (won't look correct on regular RGB NeoPixel strips).

#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN_bottom     7
#define LED_PIN_top   6     
#define LED_PIN_0  5
#define LED_PIN_4 3
#define LED_PIN_8 9

// pin for smoke machine
#define SMOKE_PIN 10

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT_bottom  60
#define LED_COUNT_top  24
#define LED_COUNT_sides 8


// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 50 // Set BRIGHTNESS to about 1/5 (max = 255)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip_bottom = Adafruit_NeoPixel(LED_COUNT_bottom, LED_PIN_bottom, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip_top = Adafruit_NeoPixel(LED_COUNT_top, LED_PIN_top, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip_0 = Adafruit_NeoPixel(LED_COUNT_sides, LED_PIN_0, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip_4 = Adafruit_NeoPixel(LED_COUNT_sides, LED_PIN_4, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip_8 = Adafruit_NeoPixel(LED_COUNT_sides, LED_PIN_8, NEO_GRBW + NEO_KHZ800);

// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// Define an array of well-saturated RGBW colors
const uint32_t colors[] = {
  strip_bottom.Color(255, 0, 0, 0),      // Red
  strip_bottom.Color(0, 255, 0, 0),      // Green
  strip_bottom.Color(0, 0, 255, 0),      // Blue
  strip_bottom.Color(255, 255, 0, 0),    // Yellow
  strip_bottom.Color(0, 255, 255, 0),    // Cyan
  strip_bottom.Color(255, 0, 255, 0),    // Magenta
  strip_bottom.Color(255, 165, 0, 0),    // Orange
  strip_bottom.Color(75, 0, 130, 0),     // Indigo
  strip_bottom.Color(238, 130, 238, 0),  // Violet
  strip_bottom.Color(255, 255, 255, 0),  // White (RGB)
  strip_bottom.Color(0, 0, 0, 255)       // White (White LED only)
};

const int numColors = sizeof(colors) / sizeof(colors[0]);


// Define a buffer size large enough to hold your input string
const int bufferSize = 100;
char inputString[bufferSize];

// For parsing info from serial commands
char parsedString[50]; // Assuming max length of string part is 50
int parsedIntegers[5] = {0}; // Initialize with zeros


///////////////////////////////////////////////////////////////////////////////////////
//////////////////// Setup ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  // Serial communication bit:
  Serial.begin(115200);
  while (!Serial){}

  // Initializing all of our strips:
  strip_bottom.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_bottom.show();            // Turn OFF all pixels ASAP
  strip_bottom.setBrightness(BRIGHTNESS);

  strip_top.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_top.show();            // Turn OFF all pixels ASAP
  strip_top.setBrightness(BRIGHTNESS);

  strip_0.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_0.show();            // Turn OFF all pixels ASAP
  strip_0.setBrightness(BRIGHTNESS);

  strip_4.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_4.show();            // Turn OFF all pixels ASAP
  strip_4.setBrightness(BRIGHTNESS);

  strip_8.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_8.show();            // Turn OFF all pixels ASAP
  strip_8.setBrightness(BRIGHTNESS);

  // setup the smoke pin
  pinMode(SMOKE_PIN, OUTPUT);

  // Initialize random number generator
  randomSeed(analogRead(0)); 
}


//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// Neopixel Strip Info ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

// strip.setPixelColor(n, red, green, blue, white);  // set single pixel pattern rgb values
// strip.setPixelColor(n, color); // set single pixel pattern color name
// uint32_t magenta = strip.Color(255, 0, 255); // get uint32 of a color from the strip for use later
// uint32_t greenishwhite = strip.Color(0, 64, 0, 64); //
// strip.fill(color, first, count); // fill a chunk of the strip
// strip.numPixels() - 1     // is the last pixel
// uint16_t n = strip.numPixels(); // number of pixels
// strip.clear(); // set all to black
// uint32_t color = strip.getPixelColor(11); // get color at pixel 12
// strip.setBrightness(64);

///// HSV
// hue is expressed as a 16-bit  (red/green/blue)
// Saturation is an 8-bit number (non to much)
// Value - brightness is an 8-bit number
//
// uint32_t rgbcolor = strip.ColorHSV(hue, saturation, value);
// uint32_t rgbcolor = strip.gamma32(strip.ColorHSV(hue, sat, val)); // better!!!
// strip.fill(rgbcolor);


////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// Main Loop //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


void loop() 
{

  if (Serial.available() > 0) 
  {
    // Serial.println("Starting");
    // String incoming = Serial.readStringUntil('\n');  // Read data from serial
    // Read the incoming string
    size_t len = Serial.readBytesUntil('\n', inputString, bufferSize - 1);
    inputString[len] = '\0'; // Null-terminate the string
    
    for (int i = 0; i < 5; ++i) 
    {
      parsedIntegers[i] = 0;
    }
    
    if (parseInput(inputString, parsedString, parsedIntegers)) 
    {
      // Serial.print("String part: ");
      // Serial.println(parsedString);
      // Serial.print("Integer parts: ");
      // for (int i = 0; i < 5; ++i) 
      // {
      //   Serial.print(parsedIntegers[i]);
      //   Serial.print(" ");
      // }
      // Serial.println();

      if (strcmp(parsedString, "rainbow") == 0) 
      {// 5, 30, 0
        rainbowFade(parsedIntegers[0], parsedIntegers[1]);
      } 
      else if (strcmp(parsedString, "rain") == 0) 
      {
        rain();
        //meteorRain(200,200,255,3, 32, true, 30,10);
        //colorWipTopAndBottom(parsedIntegers[0], parsedIntegers[1]);
      }
      else if (strcmp(parsedString, "fire") == 0) 
      {
        fire(parsedIntegers[0], parsedIntegers[1], parsedIntegers[2]);
      }
      else if (strcmp(parsedString, "solid") == 0) 
      {
        solid(strip_bottom.Color(parsedIntegers[0], parsedIntegers[1], parsedIntegers[2]), parsedIntegers[3], parsedIntegers[4]);
      }
      else if (strcmp(parsedString, "test") == 0) 
      {
        test(strip_bottom.Color(parsedIntegers[0], parsedIntegers[1], parsedIntegers[2]));
      }
      else 
      {
        Serial.print("Unknown function name: ");
        Serial.println(parsedString);
      }
    } 
    else 
    {
      Serial.println("Failed to parse input.");
    }

    // Clear the input buffer
    memset(inputString, 0, bufferSize);

    Serial.println("Thanks for playing with me, py!\n");
  }

   rainbowFade(1, 1);
}


/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////// High Level Light Functions /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

/// Look at: sparkle, Running lights, 

///////// Rain ////////////
/**
/ This function is meant to mimic raindrops coming down around our crystal
/ @param duration - this parameter controls the duration of the rain.
/                   valid values are integers between 1 and 10 with 1 being the shortest and 10 being the longest
/ @param intensity - this parameter controls the intensity of the rain.
/                   valid values are integers between 1 and 10 with 1 being the most intense and 10 being the mallowest
**/

/// meteor rain

/**
/  @param meteorSize – the number of LEDs that represent the meteor, not counting the tail of the meteor.
/  @param meteorTrailDecay - sets how fast the meteor tail decays/ disappears. A larger number makes the tail short and/or disappear faster. 
/                            Theoretically a value of 64 should reduce the brightness by 25% for each time the meteor gets drawn.
/  @param meteorRandomDecay
/  @param speedDelay
/
/
/
**/


void rain()
{
  uint32_t targetColor = strip_bottom.Color(173, 216, 230); // Light blue
  gradualChangeToColor(targetColor, 10000); // Gradually change to light blue over 5 seconds
  delay(5000); // Wait for a while before the next operation
}


void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int speedDelay, int loops) 
{  
  //strip_bottom.fill(0,0,0);
  
  for(int i = 0; i < (LED_COUNT_bottom*loops); i++) 
  {
   
    // fade brightness all LEDs one step
    for(int j=0; j<LED_COUNT_bottom; j++) 
    {
      if( (!meteorRandomDecay) || (random(10)>5) ) 
      {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }

    int useRed = red;
    int useGreen = green;
    int useBlue = blue;

    // draw meteor
    for(int j = 0; j < meteorSize; j++) 
    {
      if( ( i-j <LED_COUNT_bottom) && (i-j>=0) ) 
      {
        strip_bottom.setPixelColor(i-j, useRed, useGreen, useBlue);
      }
      else if ( i-j >= LED_COUNT_bottom)
      {
        if (i%LED_COUNT_bottom-j<0)
          strip_bottom.setPixelColor(i%LED_COUNT_bottom-j+LED_COUNT_bottom, useRed, useGreen, useBlue);
        else
          strip_bottom.setPixelColor(i%LED_COUNT_bottom-j, useRed, useGreen, useBlue);
      }

      useRed = int(useRed * 0.50);
      useGreen = int(useGreen * 0.50);
      useBlue = int(useBlue * 0.50);
    }
   
    strip_bottom.show();
    delay(speedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) 
{
 #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = strip_bottom.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    strip_bottom.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}




// void raindropsEffect() 
// {
//   int numRaindrops = random(2, 6); // Choose between 2 and 5 raindrops
//   int raindropPositions[numRaindrops];
//   uint32_t raindropColors[numRaindrops];
  

//   // Initialize raindrop positions and colors
//   for (int i = 0; i < numRaindrops; i++) 
//   {
//     raindropPositions[i] = random(LED_COUNT_bottom);
//     uint32_t randomColor = colors[random(numColors)];
//     raindropColors[i] = randomColor;
//   }

//   // Propagate the raindrops
//   propagateRaindrops(raindropPositions, raindropColors, numRaindrops, 10, 50); // Adjust steps and delay for effect speed and smoothness
// }

////////// Fire ///////////
/**
/ This function is meant to emote fiery intensity, a burning passion, or general excitement. This is not your sitting by the campfire kinda thing...
/ @param duration - this parameter controls the duration of the fire.
/                   valid values are integers between 1 and 10 with 1 being the shortest and 10 being the longest
/ @param intensity - this parameter controls the intensity of the fire.
/                   valid values are integers between 1 and 10 with 1 being the most intense and 10 being the mallowest
/ @param smokeLevel - the smoke level for this function can be adjusted with this parameter.
/                     valid values are integers between 0 (no smoke) to 10 (maximum smoke)
/                     current min is 0 and max is 4 seconds of smoke
**/
void fire(int duration, int intensity, int smokeLevel)
{
  // find fireeffect https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
  // thatre chase?
}



///////// Clouds //////////
/**
/ This function is meant to create a cloudy thunderstorm effect, a moody and intense feeling
/ @param duration - this parameter controls the duration of the thunderstorm.
/                   valid values are integers between 1 and 10 with 1 being the shortest and 10 being the longest
/ @param intensity - this parameter controls the intensity of the thunderstorm.
/                   valid values are integers between 1 and 10 with 1 being the most intense and 10 being the mallowest
**/
void thunderstorm(int duration, int intensity)
{
  
}



//////// Rainbow /////////
/**
/ This function is meant to add a rainbow swirl about the chamber.
/ @param rainbowLoops - this parameter controls the number of rainbow loops that will happen aruond the chamber.
/                       valid values are integers between 1 and 10
/ @param speed - this parameter indicates the speed, and thus the intensity of the rainbow loops.
/                       valid values are integers between 1 and 10 with 1 being the fastest and 10 the slowest.
**/

void rainbowFade(int rainbowLoops, int speed) 
{
  int fadeVal=100, fadeMax=100;

  // Hue of first pixel runs 'rainbowLoops' complete loops through the color
  // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to rainbowLoops*65536, using steps of 256 so we
  // advance around the wheel at a decent clip.
  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536; firstPixelHue += 256) 
  {

    for(int i=0; i<strip_bottom.numPixels(); i++) 
    { // For each pixel in strip...

      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      uint32_t pixelHue = firstPixelHue + (i * 65536L / strip_bottom.numPixels());

      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      strip_bottom.setPixelColor(i, strip_bottom.gamma32(strip_bottom.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
    }

    for(int i=0; i<strip_top.numPixels(); i++) 
    { // For each pixel in strip...

      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      uint32_t pixelHue = firstPixelHue - (i * 65536L / strip_top.numPixels());

      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      strip_top.setPixelColor(i, strip_top.gamma32(strip_top.ColorHSV(pixelHue, 255, 255 * fadeVal / fadeMax)));
    }

    uint32_t pixelHue = firstPixelHue + (0 * 65536L / 3);
    strip_0.fill(strip_0.gamma32(strip_0.ColorHSV(pixelHue, 255, 255 * fadeVal / fadeMax)));
    pixelHue = firstPixelHue + (1 * 65536L / 3);
    strip_4.fill(strip_4.gamma32(strip_4.ColorHSV(pixelHue, 255, 255 * fadeVal / fadeMax)));
    pixelHue = firstPixelHue + (2 * 65536L / 3);
    strip_8.fill(strip_8.gamma32(strip_8.ColorHSV(pixelHue, 255, 255 * fadeVal / fadeMax)));
    
    strip_top.show();
    strip_bottom.show();
    strip_0.show();
    strip_4.show();
    strip_8.show();
    delay(speed);

    // if(firstPixelHue < 65536) {                              // First loop,
    //   if(fadeVal < fadeMax) fadeVal++;                       // fade in
    // } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
    //   if(fadeVal > 0) fadeVal--;                             // fade out
    // } else {
    fadeVal = fadeMax; // Interim loop, make sure fade is at max
    // }
  }

  delay(0); // Pause 1/2 second
}



////////////////////////
//////// Flood /////////
////////////////////////


/**
/ Solid is meant to flood the chanmber with one solid color and give a consistant mood.
/ @param color - a color as translated by as Neopixel strip from a R,G,B 0-255 values
/ @param delayLevel - the runtime of this function can be adjusted with this parameter.
/                     valid values are integers between 0 (minimal time) to 10 (maximum time)
/                     current min is 0 and max is 6 seconds
/ @param smokeLevel - the smoke level for this function can be adjusted with this parameter.
/                     valid values are integers between 0 (no smoke) to 10 (maximum smoke)
/                     current min is 0 and max is 4 seconds of smoke
**/
void solid(uint32_t color, int delayLevel, int smokeLevel)
{
    strip_bottom.fill(color); 
    strip_top.fill(color);
    strip_0.fill(color);
    strip_4.fill(color);
    strip_8.fill(color);

    strip_top.show();
    strip_bottom.show();                          //  Update strip to match
    strip_0.show();
    strip_4.show();
    strip_8.show();

    uint32_t smokeDelay = 4000;
    uint32_t howLong = 6000;

    if(smokeLevel > -1 && smokeLevel < 11)
    {
      smokeDelay = smokeDelay * (smokeLevel/10); 
      Serial.print("smoke delay: ");
      Serial.println(smokeDelay);
    }
    else
      smokeDelay = smokeDelay / 2; // no value, give it halfpoint
    
    digitalWrite(SMOKE_PIN, HIGH);
    delay(smokeDelay);
    digitalWrite(SMOKE_PIN, LOW);

    //TODO: add some fading or blinking here...
    if(delayLevel > -1 && delayLevel < 11)
    {
      howLong = howLong * (delayLevel/10);
      Serial.print("how long: ");
      Serial.println(howLong);
    }
    else
      howLong = howLong/2; // no value, give it halfpoint

    delay(howLong);
}




/**
/ Test is meant to test the connection of all the strips. 
/ It lights the top strip, the bottom strip, then the noon, 4 o'clock, and 8 o'clock stript
/ It then calls the rainbow function to verify alignment of all 5 strips by slowly rotating the lights 
/ counter clockwise in the chamber.
/ @param color - uint32_t color as it comes from a Neopixel strip color function with R, G, B range of 0 - 255
/
**/
void test(uint32_t color)
{
    strip_bottom.setPixelColor(0,color); 
    strip_top.setPixelColor(0,color); 
    strip_0.setPixelColor(0,color); 
    strip_4.setPixelColor(0,color); 
    strip_8.setPixelColor(0,color); 

    strip_top.show();
    strip_bottom.show();                          //  Update strip to match
    strip_0.show();
    delay(2000);
    strip_4.show();
    delay(2000);
    strip_8.show();

    digitalWrite(SMOKE_PIN, HIGH);
    delay(3000);
    digitalWrite(SMOKE_PIN, LOW);
    

    delay(1000);
    rainbowFade(5,10);
}



/////////////////////////////////////////////////////////////////////
////////////////// Other light functions ////////////////////////////
/////////////////////////////////////////////////////////////////////




/**
/ Wave is meant to have a whimzical running wave through the chamber, a fleeting feeling.
/ @param color - a color as translated by as Neopixel strip from a R,G,B 0-255 values
/ @param delayLevel - the runtime of this function can be adjusted with this parameter.
/                     valid values are integers between 0 (minimal time) to 10 (maximum time)
/                     current min is 0 and max is 6 seconds
**/
void wave()
{
  // Choose a random pixel and color
  int randomPixel = random(LED_COUNT_bottom);
  //int red = random(256);
  //int green = random(256);
  //int blue = random(256);
  //uint32_t randomColor = strip_bottom.Color(red, green, blue, 0);
  uint32_t randomColor = colors[random(numColors)];
  // Change the color of the chosen pixel
  strip_bottom.setPixelColor(randomPixel, randomColor);
  strip_bottom.show();
  delay(100); // Slight delay for visibility

  // Propagate the color around the ring in a wave
  propagateWave(randomPixel, randomColor, 1, 5, LED_COUNT_bottom); // Adjust delay and steps for speed of propagation
}



void propagateWave(int startPixel, uint32_t color, int delayTime, int steps, int numberOfPixels) 
{
  for (int i = 0; i < numberOfPixels / 2; i++) {
    int leftPixel = (startPixel - i + numberOfPixels) % numberOfPixels;
    int rightPixel = (startPixel + i) % numberOfPixels;

    //fadePixelToColor(leftPixel, color, delayTime);
    //fadePixelToColor(rightPixel, color, delayTime);
    fadePixelsToColor(leftPixel, rightPixel, color, steps);
    delay(delayTime);
  }
}

////////////////////////////////////////////////////////////////

void gradualChangeToColor(uint32_t targetColor, int duration) {
  int steps = 100; // Number of steps in the transition
  int delayTime = duration / steps; // Time to wait between each step

  for (int step = 0; step <= steps; step++) {
    updateStripColors(strip_bottom, targetColor, step, steps);
    updateStripColors(strip_top, targetColor, step, steps);
    updateStripColors(strip_0, targetColor, step, steps);
    updateStripColors(strip_4, targetColor, step, steps);
    updateStripColors(strip_8, targetColor, step, steps);
    
    // Show the updated colors
    strip_bottom.show();
    strip_top.show();
    strip_0.show();
    strip_4.show();
    strip_8.show();
    
    delay(delayTime);
  }
}

void updateStripColors(Adafruit_NeoPixel& strip, uint32_t targetColor, int step, int steps) {
  for (int i = 0; i < strip.numPixels(); i++) {
    uint32_t currentColor = strip.getPixelColor(i);
    
    uint8_t r_current = (currentColor >> 16) & 0xFF;
    uint8_t g_current = (currentColor >> 8) & 0xFF;
    uint8_t b_current = currentColor & 0xFF;
    
    uint8_t r_target = (targetColor >> 16) & 0xFF;
    uint8_t g_target = (targetColor >> 8) & 0xFF;
    uint8_t b_target = targetColor & 0xFF;
    
    uint8_t r_new = r_current + ((r_target - r_current) * step / steps);
    uint8_t g_new = g_current + ((g_target - g_current) * step / steps);
    uint8_t b_new = b_current + ((b_target - b_current) * step / steps);
    
    strip.setPixelColor(i, strip.Color(r_new, g_new, b_new));
  }
}

//////////////////////////////////////////////////////////////////////////


void fadePixelsToColor(int pixel1, int pixel2, uint32_t targetColor, int steps) 
{
  uint32_t startColor1 = strip_bottom.getPixelColor(pixel1);
  uint32_t startColor2 = strip_bottom.getPixelColor(pixel2);

  int startR1 = (startColor1 >> 24) & 0xFF;
  int startG1 = (startColor1 >> 16) & 0xFF;
  int startB1 = (startColor1 >> 8) & 0xFF;
  int startW1 = startColor1 & 0xFF;

  int startR2 = (startColor2 >> 24) & 0xFF;
  int startG2 = (startColor2 >> 16) & 0xFF;
  int startB2 = (startColor2 >> 8) & 0xFF;
  int startW2 = startColor2 & 0xFF;

  int targetR = (targetColor >> 24) & 0xFF;
  int targetG = (targetColor >> 16) & 0xFF;
  int targetB = (targetColor >> 8) & 0xFF;
  int targetW = targetColor & 0xFF;

  for (int i = 0; i <= steps; i++) {
    int currentR1 = startR1 + ((targetR - startR1) * i / steps);
    int currentG1 = startG1 + ((targetG - startG1) * i / steps);
    int currentB1 = startB1 + ((targetB - startB1) * i / steps);
    int currentW1 = startW1 + ((targetW - startW1) * i / steps);

    int currentR2 = startR2 + ((targetR - startR2) * i / steps);
    int currentG2 = startG2 + ((targetG - startG2) * i / steps);
    int currentB2 = startB2 + ((targetB - startB2) * i / steps);
    int currentW2 = startW2 + ((targetW - startW2) * i / steps);

    strip_bottom.setPixelColor(pixel1, strip_bottom.Color(currentR1, currentG1, currentB1, currentW1));
    strip_bottom.setPixelColor(pixel2, strip_bottom.Color(currentR2, currentG2, currentB2, currentW2));
    strip_bottom.show();
    delay(10);
  }
}








void colorWipTopAndBottom(uint32_t color, int wait)
{
  int j = 23;
  for(int i=0; i<strip_bottom.numPixels(); i++) { // For each pixel in strip...
    strip_bottom.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip_top.setPixelColor(j,color);
    if ((i+1)%3 == 0 || (i+1)%10 == 0)
    {
      j--;
      strip_top.setPixelColor(j,color);
    }
    if (i ==0)
    {
      strip_0.fill(color);
    }
    if (i == 19)
    {
      strip_4.fill(color);
    }
    if (i == 39)
    {
      strip_8.fill(color);
    }
    strip_top.show();
    strip_bottom.show();                          //  Update strip to match
    strip_0.show();
    strip_4.show();
    strip_8.show();
    
    delay(wait);                           //  Pause for a moment
  }
}


//////////////////////////////////////////////////////////////////////
//////////////////// Other Utilities /////////////////////////////////
//////////////////////////////////////////////////////////////////////


bool parseInput(const char* input, char* str, int integers[5]) 
{
    int strIndex = 0;
    int intIndex = 0;
    bool inString = true;
    
    for (int i = 0; input[i] != '\0'; ++i) 
    {
        if (inString) 
        {
            if (input[i] == ' ') 
            {
                inString = false;
                str[strIndex] = '\0'; // Null-terminate the string
            } else {
                str[strIndex++] = input[i];
            }
        } 
        else // integers
        {
            if (input[i] >= '0' && input[i] <= '9') 
            {
                integers[intIndex] = integers[intIndex] * 10 + (input[i] - '0');
            } else if (input[i] == ' ') 
            {
                if (++intIndex >= 5) 
                {
                    break;
                }
            }
        }
    }
    // Null-terminate the string just in case it was a single word without integers
    if (inString) {
        str[strIndex] = '\0';
    }
    
    // Fill remaining integers with 0 if less than 5 found
    for (int i = intIndex + 1; i < 5; ++i) {
        integers[i] = 0;
    }

    return true; // Assuming the input is always well-formed for simplicity
}