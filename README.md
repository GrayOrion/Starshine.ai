So far we have 3 files, two are just to test bidirectional communication between pi and arduino.
The arduino takes in a string that is a function name and 3 integer variables for now
functions available:

//////// Rainbow /////////
/**
/ call string: rainbow
/ This function is meant to add a rainbow swirl about the chamber.
/ @param rainbowLoops - this parameter controls the number of rainbow loops that will happen aruond the chamber.
/                       valid values are integers between 1 and 10
/ @param speed - this parameter indicates the speed, and thus the intensity of the rainbow loops.
/                       valid values are integers between 1 and 10 with 1 being the fastest and 10 the slowest.
**/

///////// Rain ////////////
/**
/ call string: rain
/ This function is meant to mimic raindrops coming down around our crystal
/ @param duration - this parameter controls the duration of the rain.
/                   valid values are integers between 1 and 10 with 1 being the shortest and 10 being the longest
/ @param intensity - this parameter controls the intensity of the rain.
/                   valid values are integers between 1 and 10 with 1 being the most intense and 10 being the mallowest
**/

////////// Fire ////////////
/**
/ call string: fire
/ This function is meant to emote fiery intensity, a burning passion, or general excitement. This is not your sitting by the campfire kinda thing...
/ @param duration - this parameter controls the duration of the fire.
/                   valid values are integers between 1 and 10 with 1 being the shortest and 10 being the longest
/ @param intensity - this parameter controls the intensity of the fire.
/                   valid values are integers between 1 and 10 with 1 being the most intense and 10 being the mallowest
/ @param smokeLevel - the smoke level for this function can be adjusted with this parameter.
/                   valid values are integers between 0 (no smoke) to 10 (maximum smoke)
/                   current min is 0 and max is 4 seconds of smoke
**/

///////// Clouds //////////
/**
/ call string: clouds
/ This function is meant to create a cloudy thunderstorm effect, a moody and intense feeling
/ @param duration - this parameter controls the duration of the thunderstorm.
/                   valid values are integers between 1 and 10 with 1 being the shortest and 10 being the longest
/ @param intensity - this parameter controls the intensity of the thunderstorm.
/                   valid values are integers between 1 and 10 with 1 being the most intense and 10 being the mallowest
**/



/**
/ call string: flood
/ Solid is meant to flood the chanmber with one solid color and give a consistant mood.
/ @param color - a color as translated by as Neopixel strip from a R,G,B 0-255 values
/ @param delayLevel - the runtime of this function can be adjusted with this parameter.
/                     valid values are integers between 0 (minimal time) to 10 (maximum time)
/                     current min is 0 and max is 6 seconds
/ @param smokeLevel - the smoke level for this function can be adjusted with this parameter.
/                     valid values are integers between 0 (no smoke) to 10 (maximum smoke)
/                     current min is 0 and max is 4 seconds of smoke
**/

/**
/ call string: test
/ Test is meant to test the connection of all the strips. 
/ It lights the top strip, the bottom strip, then the noon, 4 o'clock, and 8 o'clock stript
/ It then calls the rainbow function to verify alignment of all 5 strips by slowly rotating the lights 
/ counter clockwise in the chamber.
/ @param color - uint32_t color as it comes from a Neopixel strip color function with R, G, B range of 0 - 255
/
**/



