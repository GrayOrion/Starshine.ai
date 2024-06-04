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

// Define colors for rain - TODO: add these to the array above and change rain functions
uint32_t lightBlue = strip_bottom.Color(173, 216, 230); // Light blue
uint32_t darkBlue = strip_bottom.Color(0, 0, 255); // Full blue

// Randomly selected starting points (where our 3 side strips reside) - currently used for thruderstrom
const int startPoints[3] = {0, 19, 39};


/// Definitions and Variables/constants for the snake ///////////
/// Path going around and up and down through the side strips ///
/////////////////////////////////////////////////////////////////

// Define the path as a sequence of strips and pixel ranges
struct PathSegment
{
    Adafruit_NeoPixel *strip;
    int startPixel;
    int endPixel;
};

PathSegment path[] =
{
    {&strip_bottom, 0, 19},
    {&strip_4, 0, 7},
    {&strip_top, 15, 7},
    {&strip_8, 7, 0},
    {&strip_bottom, 39, 59},
    {&strip_0, 0, 7},
    {&strip_top, 23, 15},
    {&strip_4, 7, 0},
    {&strip_bottom, 19, 39},
    {&strip_8, 0, 7},
    {&strip_top, 7, 0},
    {&strip_0, 7, 0}
};


// Length of the path in terms of segments
const int PATH_LENGTH = sizeof(path) / sizeof(path[0]);

// Tail fade length
int tailLength = 5;


/////////////////////// Variables for including all strips as one ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

#define NUM_STRIPS 5
#define NUM_LEDS (LED_COUNT_bottom + LED_COUNT_top + 3 * LED_COUNT_sides)

// Array to hold all the strips
Adafruit_NeoPixel* strips[NUM_STRIPS] = { &strip_bottom, &strip_top, &strip_0, &strip_4, &strip_8 };

// Array to hold the count of LEDs in each strip
int ledCounts[NUM_STRIPS] = { LED_COUNT_bottom, LED_COUNT_top, LED_COUNT_sides, LED_COUNT_sides, LED_COUNT_sides };





///////////////////// Variables for Serial Communication /////////////////////
//////////////////////////////////////////////////////////////////////////////

// Define a buffer size large enough to hold your input string
const int bufferSize = 100;
char inputString[bufferSize];

// For parsing info from serial commands
char parsedString[50]; // Assuming max length of string part is 50
int parsedIntegers[5] = {0}; // Initialize with zeros

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//////////////////// Setup ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

void setup() 
{
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  // Serial communication bit:
  Serial.begin(9600);//115200);
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


/////////////////////////////////////////////////////////////////////////////////
////////////////////////// For ChatGPT //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

/**
I have 5 neopixel strips, 
All strips are rgbw neopixel strips
a 60 pixel strip called strip_bottom with LED_COUNT_bottom as the variable for the number of neopixels
a 24 pixel strip called strip_top with LED_COUNT_top as the variable for the number of neopixels
and three 8 pixel strips called strip_0, strip_4, and strip_8 with LED_COUNT_sides as the variable for the number of neopixels

Please write a function for my arduino that runs a color changing 5 pixel snake with a fading tail that follow this path:
From pixel 0 to pixel 19 of strip_bottom, from pixel 0 to pixel 7 of strip_4, then from pixel 15 to pixel 7 of strip_top, then from pixel 7 to pixel 0 of strip_8, then from pixel 39 to pixel 59 of strip_bottom, 
then from  to pixel 0 to pixel 7 of strip_0, from pixel 23 to pixel 15 of strip_top, then from pixel 7 to pixel 0 of strip_4, from pixel 19 to pixel 39 of strip_bottom, from pixel 0 to pixel 7 of strip_8, from pixel 7 to pixel 0 of strip_top, from pixel 7 to pixel 0 of strip_0 

The length of the tail fade should be controlled with a variable 




I have a rgbw neopixel strip that is 60 pixel long called strip_bottom on pin 7. This strip is a circle so that pixel 59 is connected to and is followed by pixel 0
The base color of the ring is given by the variable baseColor and the wave head color is give by the variable waveColor.
I would like you to create a function that starts at a random pixel on the strip.
From that pixel I would like you to create a riplle effect using the wave color. 
The ripple goes from the random pixel outwards for rippleLength number of pixels to either side of the starting pixel without returning to the center.
The ripple profile should be look like this:
The head of the ripple, the leading edge of the wave: waveColor. Following depending on how far the wave head has moved away from the starting pixel: waveColor 30% fade to baseColor followed by waveColor 60% fade to baseColor followed by waveColor 80% fade to the baseColor followed by baseColor.
If the rippleLength is longer than this ripple profile, start another one behind it as soon as you can.


From that pixel I would like you to propagate a wave for waveLenth pixels around the ring in either direction.
The wave head is one pixel of the color waveColor and followed by a pixel of 30% fade from waveColor to baseColor, a pixel of 60% fade from waveColor to baseColor, a pixel of 80% fade from waveColor to baseColor and then a couple of pixels of baseColor.
One the wave is far enough from origin to accomodate a 7th pixel, a sedond wave propagate through. 
The wave head stops at the waveLenth pixel mark, but the pixels behind it (and the second wave behind it) continue to move towards the waveLenth pixel till the whole trail is faded back to baseColor
  
**/


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
    if (runSequenceFromSerialInput())
      gradualChangeToColor(strip_bottom.Color(0,0,0), 1000);
  }
  //runSnake(5,100);
  Sparkle(255, 0, 255, 30);
}

//////////////////////////////////////////////////////



/**
* This method read serial port and runs functions according to input.
* It parses a string and 5 integers and replaces empty integers with 0's for robustness.
* @returns true if managed to find the right function. 
**/
boolean runSequenceFromSerialInput()
{
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
      rain(parsedIntegers[0], parsedIntegers[1]);
      //meteorRain(200,200,255,3, 32, true, 30,10);
      //colorWipTopAndBottom(parsedIntegers[0], parsedIntegers[1]);
    }
    else if (strcmp(parsedString, "fire") == 0) 
    {
      fire(parsedIntegers[0], parsedIntegers[1], parsedIntegers[2]);
    }
    else if (strcmp(parsedString, "flood") == 0)
    {
      solid(strip_bottom.Color(parsedIntegers[0], parsedIntegers[1], parsedIntegers[2]), parsedIntegers[3], parsedIntegers[4]);
    }
    else if (strcmp(parsedString, "clouds") == 0)
    {
      thunderstorm(parsedIntegers[0], parsedIntegers[1]);
    }
    else if (strcmp(parsedString, "test") == 0) 
    {
      test(strip_bottom.Color(parsedIntegers[0], parsedIntegers[1], parsedIntegers[2]));
    }
    else
    {
      Serial.print("Unknown function name: ");
      Serial.println(parsedString);
      // Clear the input buffer
      memset(inputString, 0, bufferSize);
      return false;
    }
  } 
  else 
  {
    Serial.println("Failed to parse input.");
    // Clear the input buffer
    memset(inputString, 0, bufferSize);
    return false;
  }  
  Serial.println("Thanks for playing with me, py!\n");
  // Clear the input buffer
  memset(inputString, 0, bufferSize);
  return true;
}




/////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////// High Level Light Functions /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////
/// Sparkle everything ///
//////////////////////////


// Call with Sparkle(0xff, 0xff, 0xff, 0);
// This might be the best function to run as in between calls, but might needto add a before dimming

void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int pixel1 = random(NUM_LEDS);
  int pixel2 = random(NUM_LEDS);
  int pixel3 = random(NUM_LEDS);
  
  setPixel(pixel1, red, green, blue);
  setPixel(pixel2, red, green, blue);
  setPixel(pixel3, red, green, blue);
  showStrip();
  delay(SpeedDelay);
  setPixel(pixel1, 0, 0, 0);
  setPixel(pixel2, 0, 0, 0);
  setPixel(pixel3, 0, 0, 0);
}


void setPixel(int Pixel, byte red, byte green, byte blue) {
  int stripIndex = 0;
  int pixelIndex = Pixel;

  // Find which strip the pixel belongs to
  while (pixelIndex >= ledCounts[stripIndex]) {
    pixelIndex -= ledCounts[stripIndex];
    stripIndex++;
  }

  // Set the pixel color in the correct strip
  strips[stripIndex]->setPixelColor(pixelIndex, strips[stripIndex]->Color(red, green, blue, 0));
}


void showStrip() {
  for (int i = 0; i < NUM_STRIPS; i++) {
    strips[i]->show();
  }
}


////////////////////////////
///////// Snake ////////////
////////////////////////////


void runSnake(int snakeLength, int delayTime)
{
    uint32_t currentColor = strip_bottom.Color(255, 0, 0, 0); // Start with red
    uint32_t nextColor = strip_bottom.Color(0, 0, 255, 0); // Change to blue

    int currentPathPosition = 0;
    while (true)
    {
        // Calculate the blend between the current and next color
        currentColor = blendColors(currentColor, nextColor, 5); // Change gradually

        // Move the snake along the path
        for (int i = 0; i < PATH_LENGTH; i++)  // segment
        {
            PathSegment segment = path[i]; // grab next segment
            int step = (segment.startPixel < segment.endPixel) ? 1 : -1;
            for (int j = segment.startPixel; j != segment.endPixel + step; j += step) // go through the pixels in the segment
            {
                // Set the color of the head
                setPixelColorOnMultiSegmentPath(segment, j, currentColor);
              
                // Fade the tail
                for (int k = 1; k <= tailLength; k++) // for each pixel in the tail k = tail pixel
                {
                    int tailPos = j - (k * step);
                    if (segment.strip == &strip_bottom)
                    {
                        if (tailPos >= 0 && tailPos < LED_COUNT_bottom) // at strip bottom and within 0 - max
                        {
                            uint32_t fadedColor = blendColors(currentColor, strip_bottom.Color(0, 0, 0, 0), k * 100 / tailLength);
                            setPixelColorOnMultiSegmentPath(segment, tailPos, fadedColor);
                        }
                        else if (i > 0) // not first segment
                        {
                            if (tailPos < 0)
                                setPixelColorOnMultiSegmentPathByStepback(path[i-1], k);
                            else
                                setPixelColorOnMultiSegmentPathByStepback(path[i-1], k);
                        }
                    }
                    else if (segment.strip == &strip_top)
                    {
                        if (tailPos >= 0 && tailPos < LED_COUNT_top) // at strip top and within 0 - max
                        {
                            uint32_t fadedColor = blendColors(currentColor, strip_top.Color(0, 0, 0, 0), k * 100 / tailLength);
                            setPixelColorOnMultiSegmentPath(segment, tailPos, fadedColor);
                        }
                        else if (i > 0) // not first segment
                        {
                            if (tailPos < 0)
                                setPixelColorOnMultiSegmentPathByStepback(path[i-1], k);
                            else
                                setPixelColorOnMultiSegmentPathByStepback(path[i-1], k);
                        }
                    }
                    else if (segment.strip == &strip_0 || segment.strip == &strip_4 || segment.strip == &strip_8)
                    {
                        if (tailPos >= 0 && tailPos < LED_COUNT_sides)
                        {
                            uint32_t fadedColor = blendColors(currentColor, strip_0.Color(0, 0, 0, 0), k * 100 / tailLength);
                            setPixelColorOnMultiSegmentPath(segment, tailPos, fadedColor);
                        }
                        else if (i > 0) // not first segment
                        {
                            if (tailPos < 0)
                                setPixelColorOnMultiSegmentPathByStepback(path[i-1], k);
                            else
                                setPixelColorOnMultiSegmentPathByStepback(path[i-1], k);
                        }
                    }
                }

                // Show the updates
                segment.strip->show();
                delay(delayTime);

                // Clear the pixel behind the tail
                int clearPos = j - (snakeLength * step);
                if (clearPos >= 0 && clearPos < LED_COUNT_bottom && segment.strip == &strip_bottom)
                {
                    setPixelColorOnMultiSegmentPath(segment, clearPos, strip_bottom.Color(0, 0, 0, 0));
                }
                else if (clearPos >= 0 && clearPos < LED_COUNT_top && segment.strip == &strip_top)
                {
                    setPixelColorOnMultiSegmentPath(segment, clearPos, strip_top.Color(0, 0, 0, 0));
                }
                else if (clearPos >= 0 && clearPos < LED_COUNT_sides && (segment.strip == &strip_0 || segment.strip == &strip_4 || segment.strip == &strip_8))
                {
                    setPixelColorOnMultiSegmentPath(segment, clearPos, strip_0.Color(0, 0, 0, 0));
                }
            }
        }

        // Switch to the next color
        uint32_t tempColor = currentColor;
        currentColor = nextColor;
        nextColor = tempColor;
    }
}

void setPixelColorOnMultiSegmentPathByStepback(PathSegment segment, int clearStep)
{
  int step = (segment.startPixel < segment.endPixel) ? 1 : -1;
  
  setPixelColorOnMultiSegmentPath(segment, segment.endPixel - (clearStep * step), strip_0.Color(0, 0, 0, 0));
}
/**
/ This method changes the color of a pixel in a given segment.
/ It figures wich strip the segment belongs to and does all the work.
/
**/
void setPixelColorOnMultiSegmentPath(PathSegment segment, int pixelIndex, uint32_t color)
{
    if (segment.strip == &strip_bottom && pixelIndex >= 0 && pixelIndex < LED_COUNT_bottom)
    {
        strip_bottom.setPixelColor(pixelIndex, color);
    }
    else if (segment.strip == &strip_top && pixelIndex >= 0 && pixelIndex < LED_COUNT_top)
    {
        strip_top.setPixelColor(pixelIndex, color);
    }
    else if (segment.strip == &strip_0 && pixelIndex >= 0 && pixelIndex < LED_COUNT_sides)
    {
        strip_0.setPixelColor(pixelIndex, color);
    }
    else if (segment.strip == &strip_4 && pixelIndex >= 0 && pixelIndex < LED_COUNT_sides)
    {
        strip_4.setPixelColor(pixelIndex, color);
    }
    else if (segment.strip == &strip_8 && pixelIndex >= 0 && pixelIndex < LED_COUNT_sides)
    {
        strip_8.setPixelColor(pixelIndex, color);
    }
}


//////////////////////////
//////// Rainbow /////////
//////////////////////////


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
  int defaultLoops = 5;

  if(rainbowLoops > -1 && rainbowLoops < 11)
  {
    Serial.print("rainbow loops: ");
    Serial.println(rainbowLoops);
  }
  else
    rainbowLoops = defaultLoops; // no value, give it default
  
  int delayMs = 30; // Base/Default

  if(rainbowLoops > -1 && rainbowLoops < 11)
  {
    delayMs = delayMs * (speed/10); 
    Serial.print("delay (ms): ");
    Serial.println(delayMs);
  }

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
    delay(delayMs);

    // if(firstPixelHue < 65536) {                              // First loop,
    //   if(fadeVal < fadeMax) fadeVal++;                       // fade in
    // } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
    //   if(fadeVal > 0) fadeVal--;                             // fade out
    // } else {
    fadeVal = fadeMax; // Interim loop, make sure fade is at max
    // }
  }

  delay(0); // Pause 1/2 second - delay between loops?!? Not sure if this is ever needed...
}


///////////////////////////
///////// Rain ////////////
///////////////////////////

//// TODO: does not take in intensity or duration, also does not really look like anything!!!


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


void rain(int duration, int intensity)   
{
  gradualChangeToColor(lightBlue, 1000); // Gradually change to light blue over 5 seconds
  delay(100); // Wait for a while before the next operation

  for (int i = 0; i < 6; i++)
  {
    // Pick a random start point
    int startIndex = random(0, 3);
    int startPixel = startPoints[startIndex];
    
    createWaveEffect(startPixel, 15, 100); // Wave effect for 15 pixels, 100 ms delay between steps
    delay(100); // Wait for a while before the next operation
  }
}

void createWaveEffect(int startPixel, int waveLength, int delayTime)
{
    for (int step = 0; step < waveLength; step++)
    {
        //int intensity = 255; // Full intensity by default

        // // Reduce intensity for the last three steps
        // if (step - i <= 2)
        // {
        //     intensity = 255 * (step - i + 1) / 3;
        // }  TODO: add intentity after getting the wave going

        // Calculate and set colors for the wave effect
      
        int leftPixel = (startPixel - step + LED_COUNT_bottom) % LED_COUNT_bottom;
        int rightPixel = (startPixel + step) % LED_COUNT_bottom;

        uint32_t currentColorLeft = darkBlue; //blendColors(waveColor, lightBlue, intensity);
        uint32_t currentColorRight = darkBlue; //blendColors(waveColor, lightBlue, intensity);

        strip_bottom.setPixelColor(leftPixel, currentColorLeft);
        strip_bottom.setPixelColor(rightPixel, currentColorRight);
        
        uint32_t fadeColor = lightBlue; //blendColors(waveColor, lightBlue, 30);

        if (step > 0)
        {
          leftPixel = (startPixel - step - 1 + LED_COUNT_bottom) % LED_COUNT_bottom;
          rightPixel = (startPixel + step + 1) % LED_COUNT_bottom;
          strip_bottom.setPixelColor(leftPixel, fadeColor);
          strip_bottom.setPixelColor(rightPixel, fadeColor);
        }

        if (step > 1)
        {
          leftPixel = (startPixel - step - 2 + LED_COUNT_bottom) % LED_COUNT_bottom;
          rightPixel = (startPixel + step + 2) % LED_COUNT_bottom;
          fadeColor = lightBlue; //blendColors(waveColor, lightBlue, 60);
          strip_bottom.setPixelColor(leftPixel, fadeColor);
          strip_bottom.setPixelColor(rightPixel, fadeColor);
        }

        if (step > 2)
        {
          leftPixel = (startPixel - step - 6 + LED_COUNT_bottom) % LED_COUNT_bottom;
          rightPixel = (startPixel + step + 6) % LED_COUNT_bottom;
          fadeColor = lightBlue;
          strip_bottom.setPixelColor(leftPixel, fadeColor);
          strip_bottom.setPixelColor(rightPixel, fadeColor);
        }

        strip_bottom.show();
        delay(delayTime);
    }
}






/// TODO: is not used, would love to use on zigzag path!!!
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



///////////////////////////
////////// Fire ///////////
///////////////////////////


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
  for (int i = 0; i < 1000; i++)
  {
    fireEffect(50); // Run the fire effect with a delay of 50ms between frames
  }
  
}




// Function to set a pixel color using hue, saturation, and brightness
uint32_t ColorHSB(float hue, float sat, float brightness)
{
    hue = fmod(hue, 1.0f); // Wrap around if hue is greater than 1.0
    hue *= 6.0;            // Hue sector 0 to 5
    int i = floor(hue);    // Hue sector
    float f = hue - i;     // Factorial part of hue
    float p = brightness * (1.0 - sat);
    float q = brightness * (1.0 - sat * f);
    float t = brightness * (1.0 - sat * (1.0 - f));

    float r, g, b;
    switch (i)
    {
    case 0:
        r = brightness;
        g = t;
        b = p;
        break;
    case 1:
        r = q;
        g = brightness;
        b = p;
        break;
    case 2:
        r = p;
        g = brightness;
        b = t;
        break;
    case 3:
        r = p;
        g = q;
        b = brightness;
        break;
    case 4:
        r = t;
        g = p;
        b = brightness;
        break;
    default: // case 5:
        r = brightness;
        g = p;
        b = q;
        break;
    }

    return strip_bottom.Color((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255));
}




void fireEffect(int delayTime)
{
    static byte heat[LED_COUNT_bottom];

    // Step 1. Cool down every cell a little
    for (int i = 0; i < LED_COUNT_bottom; i++)
    {
        heat[i] = qsub8(heat[i], random(0, ((55 * 10) / LED_COUNT_bottom) + 2));
    }

    // Step 2. Heat from each cell drifts 'up' and diffuses a little
    for (int i = LED_COUNT_bottom - 1; i >= 2; i--)
    {
        heat[i] = (heat[i - 1] + heat[i - 2] + heat[i - 2]) / 3;
    }

    // Step 3. Randomly ignite new 'sparks' near the bottom
    if (random(255) < 120)
    {
        int y = random(7);
        heat[y] = qadd8(heat[y], random(160, 255));
    }

    // Step 4. Convert heat to LED colors
    for (int i = 0; i < LED_COUNT_bottom; i++)
    {
        strip_bottom.setPixelColor(i, HeatColor(heat[i]));
    }

    strip_bottom.show();
    delay(delayTime);
}

uint32_t HeatColor(byte temperature)
{
    // Scale 'heat' down from 0-255 to 0-191
    byte t192 = scale8_video(temperature, 191);

    // Calculate ramp up from
    byte heatramp = t192 & 0x3F; // 0..63
    heatramp <<= 2; // scale up to 0..252

    // Figure out which third of the spectrum we're in:
    if (t192 & 0x80)
    { // hottest
        return strip_bottom.Color(255, 255, heatramp);
    }
    else if (t192 & 0x40)
    { // middle
        return strip_bottom.Color(255, heatramp, 0);
    }
    else
    { // coolest
        return strip_bottom.Color(heatramp, 0, 0);
    }
}

// Utility function to subtract with saturation at 0
byte qsub8(byte i, byte j)
{
    return (i > j) ? i - j : 0;
}

// Utility function to add with saturation at 255
byte qadd8(byte i, byte j)
{
    return (i + j > 255) ? 255 : i + j;
}

// Utility function to scale down a byte by a percentage (0-255)
byte scale8_video(byte i, byte scale)
{
    return ((uint16_t)i * (uint16_t)scale) >> 8;
}




///////// Clouds //////////
/**
/ This function is meant to create a cloudy thunderstorm effect, a moody and intense feeling
/ @param duration - this parameter controls the duration of the thunderstorm.
/                   valid values are integers between 1 and 10 with 1 being the shortest and 10 being the longest
/ @param intensity - this parameter controls the intensity of the thunderstorm.
/                   valid values are integers between 1 and 10 with 1 being the most intense and 10 being the mallowest
**/
void thunderstorm(int duration, int smokeLevel)
{
  // fade to black
  gradualChangeToColor(strip_bottom.Color(0,0,0), 1000);

  // Start smoke
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
  
  performEffect();

  if (smokeDelay > 0 )
  {
    digitalWrite(SMOKE_PIN, HIGH);
    // wait
    delay(smokeDelay);
  }
  /// do some flashing 
  performEffect();
  // Stop smoke
  digitalWrite(SMOKE_PIN, LOW);
  // do some flashing
  for (int i = 0;i < 10; i++)
  {
    performEffect();
  }
}


void performEffect()
{
    // Pick a random strip as the main strip
    int mainStripIndex = random(0, 3);
    Adafruit_NeoPixel* mainStrip;
    Adafruit_NeoPixel* stripA;
    Adafruit_NeoPixel* stripB;
    int bottom_pixel;
    switch (mainStripIndex)
    {
        case 0:
            mainStrip = &strip_0;
            stripA = &strip_4;
            stripB = &strip_8;
            bottom_pixel = 0;     
            break;
        case 1:
            mainStrip = &strip_4;
            stripA = &strip_0;
            stripB = &strip_8;
            bottom_pixel = 19;
            break;
        case 2:
            mainStrip = &strip_8;
            stripA = &strip_0;
            stripB = &strip_4;
            bottom_pixel = 40;
            break;
    }
    uint32_t color = mainStrip->Color(150, 150, 150, 0); // Full white

    // blink top strip
    strip_top.fill(color);
    strip_top.show();
    delay(100);
    strip_top.clear();
    strip_top.show();

    // Light up the main strip from 0 to 7 pixels with accelerating speed
    for (int i = LED_COUNT_sides - 1; i >= 0; i--)
    {
        mainStrip->setPixelColor(i, color);
        mainStrip->show();
        delay(100 - (i * 10)); // Accelerating delay
    }

    // Turn off the main strip after blinking
    mainStrip->clear();
    mainStrip->show();

    // blink top strip
    strip_bottom.setPixelColor(bottom_pixel, color);
    strip_bottom.show();
    delay(100);
    strip_bottom.setPixelColor(bottom_pixel+1, color);
    strip_bottom.setPixelColor((LED_COUNT_bottom+bottom_pixel-1)%LED_COUNT_bottom, color);
    strip_bottom.show();
    delay(100);
    strip_bottom.clear();
    strip_bottom.show();

    // Blink stripA and stripB in sequence
    blinkStrip(stripA);
    delay(300); // Short delay between strips
    blinkStrip(stripB);
}

void blinkStrip(Adafruit_NeoPixel* strip)
{
    uint32_t color = strip->Color(100, 100, 100, 0); // Full white
    for (int i = 0; i < 2; i++)
    {
        strip->clear();
        strip->show();
        delay(100);
        for (int j = 0; j < LED_COUNT_sides; j++)
        {
            strip->setPixelColor(j, color);
        }
        strip->show();
        delay(100);
    }
    strip->clear();
    strip->show();
    delay(100);
    for (int j = 0; j < LED_COUNT_sides; j++)
    {
        strip->setPixelColor(j, color);
    }
    strip->show();
    delay(300); // Last 'on' delay is three times as long

    // Turn off the strip after blinking
    strip->clear();
    strip->show();
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
    
    if (smokeDelay > 0 )
    {
      digitalWrite(SMOKE_PIN, HIGH);
      delay(smokeDelay);
      digitalWrite(SMOKE_PIN, LOW);
    }

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
    strip_bottom.clear();
    strip_top.clear();
    strip_0.clear();
    strip_4.clear();
    strip_8.clear();
    strip_bottom.show();
    strip_top.show();
    strip_0.show();
    strip_4.show();
    strip_8.show();
    delay(1000);
    strip_bottom.setPixelColor(0,color); 
    strip_top.setPixelColor(0,color); 
    strip_0.fill(color);
    strip_bottom.show();
    strip_top.show();
    strip_0.show();
    delay(1000);
    strip_bottom.setPixelColor(19,color); 
    strip_top.setPixelColor(7,color); 
    strip_4.fill(color);
    strip_bottom.show();
    strip_top.show();
    strip_4.show();
    delay(1000);
    strip_bottom.setPixelColor(39,color); 
    strip_top.setPixelColor(15,color); 
    strip_8.fill(color);
    strip_bottom.show();
    strip_top.show();
    strip_8.show();
    delay(1000);
    strip_bottom.fill(color); 
    strip_top.fill(color); 
    strip_bottom.show();
    strip_top.show();
    // digitalWrite(SMOKE_PIN, HIGH);
    // delay(3000);
    // digitalWrite(SMOKE_PIN, LOW);
    
    delay(5000);
    //rainbowFade(5,10);
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

// // Blend colors for rgb
// uint32_t blendColors(uint32_t color1, uint32_t color2, int intensity) 
// {
//   // Blend color1 with color2 based on the intensity (0-255)
//   uint8_t r1 = (color1 >> 16) & 0xFF;
//   uint8_t g1 = (color1 >> 8) & 0xFF;
//   uint8_t b1 = color1 & 0xFF;
  
//   uint8_t r2 = (color2 >> 16) & 0xFF;
//   uint8_t g2 = (color2 >> 8) & 0xFF;
//   uint8_t b2 = color2 & 0xFF;

//   uint8_t r_blend = ((r1 * intensity) + (r2 * (255 - intensity))) / 255;
//   uint8_t g_blend = ((g1 * intensity) + (g2 * (255 - intensity))) / 255;
//   uint8_t b_blend = ((b1 * intensity) + (b2 * (255 - intensity))) / 255;

//   return strip_bottom.Color(r_blend, g_blend, b_blend);
// }

// blend colors for rgbw
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