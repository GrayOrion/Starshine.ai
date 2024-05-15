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

// Define a buffer size large enough to hold your input string
const int bufferSize = 100;
char inputString[bufferSize];

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
}


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



void loop() {

  if (Serial.available() > 0) 
  {
        // Serial.println("Starting");
        //String incoming = Serial.readStringUntil('\n');  // Read data from serial
        // Read the incoming string
        size_t len = Serial.readBytesUntil('\n', inputString, bufferSize - 1);
        inputString[len] = '\0'; // Null-terminate the string
        
        // Variables to hold the parsed values
        char functionName[20]; // Adjust size as needed
        int var1, var2, var3;

        // Parse the input string
        if (sscanf(inputString, "%19s %d %d %d", functionName, &var1, &var2, &var3) == 4) 
        {
          // Successfully parsed the input
          if (strcmp(functionName, "rainbowFade") == 0) 
          {// 5, 30, 0
            rainbowFade2White(var1, var2, var3);
          } 
          else if (strcmp(functionName, "colorWipe") == 0) 
          {
            colorWipTopAndBottom(strip_bottom.Color(var1, var2, var3), 50);
          } 
          else if (strcmp(functionName, "pulseWhite") == 0) 
          {
            pulseWhite(var1); //, var2, var3);
          } 
          else 
          {
            Serial.print("Unknown function name ");
            Serial.println(functionName);
          }
        } 
        else 
        {
          // Parsing failed
          Serial.println("Failed to parse input");
        }

        // Clear the input buffer
        memset(inputString, 0, bufferSize);

        Serial.println("Thanks for playing py!\n");
  }

  // Fill along the length of the strip in various colors...
  colorWipTopAndBottom(strip_bottom.Color(255,   0,   0)     , 50); // Red
  colorWipTopAndBottom(strip_bottom.Color(  0, 255,   0)     , 50); // Green
  // colorWipTopAndBottom(strip_bottom.Color(  0,   0, 255)     , 5); // Blue
  //colorWipTopAndBottom(strip_bottom.Color(  0,   0,   0, 255), 5); // True white (not RGB white)

  // Fill along the length of the strip in various colors...
  //colorWipe(strip_top.Color(255,   0,   0)     , 50); // Red
  //colorWipe(strip_top.Color(  0, 255,   0)     , 50); // Green
  //colorWipe(strip_top.Color(  0,   0, 255)     , 50); // Blue
  //colorWipe(strip_top.Color(  0,   0,   0, 255), 50); // True white (not RGB white)

  //whiteOverRainbow(75, 5);

  //pulseWhite(5);

  
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




void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal=0, fadeMax=100;

  // Hue of first pixel runs 'rainbowLoops' complete loops through the color
  // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to rainbowLoops*65536, using steps of 256 so we
  // advance around the wheel at a decent clip.
  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
    firstPixelHue += 256) {

    for(int i=0; i<strip_bottom.numPixels(); i++) { // For each pixel in strip...

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
    
    

    for(int i=0; i<strip_top.numPixels(); i++) { // For each pixel in strip...

      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      uint32_t pixelHue = firstPixelHue - (i * 65536L / strip_top.numPixels());

      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      strip_top.setPixelColor(i, strip_top.gamma32(strip_top.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
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
    delay(wait);

    // if(firstPixelHue < 65536) {                              // First loop,
    //   if(fadeVal < fadeMax) fadeVal++;                       // fade in
    // } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
    //   if(fadeVal > 0) fadeVal--;                             // fade out
    // } else {
    fadeVal = fadeMax; // Interim loop, make sure fade is at max
    // }
  }

  // for(int k=0; k<whiteLoops; k++) {
  //   for(int j=0; j<256; j++) { // Ramp up 0 to 255
  //     // Fill entire strip with white at gamma-corrected brightness level 'j':
  //     strip_bottom.fill(strip_bottom.Color(0, 0, 0, strip_bottom.gamma8(j)));
  //     strip_bottom.show();
  //   }
  //   delay(1000); // Pause 1 second
  //   for(int j=255; j>=0; j--) { // Ramp down 255 to 0
  //     strip_bottom.fill(strip_bottom.Color(0, 0, 0, strip_bottom.gamma8(j)));
  //     strip_bottom.show();
  //   }
  // }

  delay(500); // Pause 1/2 second
}








void whiteOverRainbow(int whiteSpeed, int whiteLength) {

  if(whiteLength >= strip_bottom.numPixels()) whiteLength = strip_bottom.numPixels() - 1;

  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 3;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;

  for(;;) { // Repeat forever (or until a 'break' or 'return')
    for(int i=0; i<strip_bottom.numPixels(); i++) {  // For each pixel in strip...
      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
        strip_bottom.setPixelColor(i, strip_bottom.Color(0, 0, 0, 255)); // Set white
      } else {                                             // else set rainbow
        int pixelHue = firstPixelHue + (i * 65536L / strip_bottom.numPixels());
        strip_bottom.setPixelColor(i, strip_bottom.gamma32(strip_bottom.ColorHSV(pixelHue)));
      }
    }

    strip_bottom.show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.

    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= strip_bottom.numPixels()) {      // Advance head, wrap around
        head = 0;
        if(++loopNum >= loops) return;
      }
      if(++tail >= strip_bottom.numPixels()) {      // Advance tail, wrap around
        tail = 0;
      }
      lastTime = millis();                   // Save time of last movement
    }
  }
}

void pulseWhite(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip_top.fill(strip_top.Color(0, 0, 0, strip_top.gamma8(j)));
    strip_top.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    strip_top.fill(strip_top.Color(0, 0, 0, strip_top.gamma8(j)));
    strip_top.show();
    delay(wait);
  }
}



  



// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip_bottom.numPixels(); i++) { // For each pixel in strip...
    strip_bottom.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip_bottom.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
