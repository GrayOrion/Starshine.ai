const int trigPins[] = {9, 11, 13}; // Trig pins for sensors
const int echoPins[] = {10, 12, 14}; // Echo pins for sensors
const int ledPin = 7;  // LED pin
const int numReadings = 10;  // Number of readings to average over 1 second (10 readings per second)
const int thresholdDistance = 200;  // Distance threshold in cm
const int presentDuration = 1000;  // Duration to confirm "present" state in milliseconds
const int notPresentDuration = 60000;  // Duration to confirm "not_present" state in milliseconds (1 minute)

long readings[3][numReadings];  // Array to store the distance readings for each sensor
int readIndex[3] = {0, 0, 0};  // Index of the current reading for each sensor
long total[3] = {0, 0, 0};  // Sum of the readings for each sensor
long average[3] = {0, 0, 0};  // Average of the readings for each sensor

unsigned long presentStartTime = 0;  // Time when "present" condition started
unsigned long notPresentStartTime = 0;  // Time when "not_present" condition started
String currentState = "not_present";  // Initial state

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }
  pinMode(ledPin, OUTPUT);  // Initialize the LED pin as an output
  digitalWrite(ledPin, LOW);  // Ensure the LED is off initially
  Serial.begin(9600);

  // Initialize all readings to 0 for each sensor
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < numReadings; j++) {
      readings[i][j] = 0;
    }
  }
}

void loop() {
  for (int sensorIndex = 0; sensorIndex < 3; sensorIndex++) {
    // Clear the trigPin by setting it LOW
    digitalWrite(trigPins[sensorIndex], LOW);
    delayMicroseconds(2);

    // Set the trigPin HIGH for 10 microseconds
    digitalWrite(trigPins[sensorIndex], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPins[sensorIndex], LOW);

    // Read the echoPin, which returns the duration of the pulse in microseconds
    long duration = pulseIn(echoPins[sensorIndex], HIGH);

    // Calculate the distance in centimeters
    long distance = duration * 0.034 / 2;

    // Subtract the last reading and add the new reading
    total[sensorIndex] = total[sensorIndex] - readings[sensorIndex][readIndex[sensorIndex]];
    readings[sensorIndex][readIndex[sensorIndex]] = distance;
    total[sensorIndex] = total[sensorIndex] + readings[sensorIndex][readIndex[sensorIndex]];

    // Advance to the next position in the array
    readIndex[sensorIndex] = (readIndex[sensorIndex] + 1) % numReadings;

    // Calculate the average distance
    average[sensorIndex] = total[sensorIndex] / numReadings;

    // Delay for 30ms to offset sensors from each other
    delay(30);
  }

  // Determine the state based on the averages
  bool anySensorPresent = false;
  bool allSensorsNotPresent = true;

  for (int i = 0; i < 3; i++) {
    if (average[i] < thresholdDistance) {
      anySensorPresent = true;
      allSensorsNotPresent = false;
      break;
    }
    if (average[i] < thresholdDistance) {
      allSensorsNotPresent = false;
    }
  }

  if (anySensorPresent) {
    if (presentStartTime == 0) {
      presentStartTime = millis();
    } else if (millis() - presentStartTime >= presentDuration && currentState != "present") {
      currentState = "present";
      Serial.println("present");
      digitalWrite(ledPin, HIGH);  // Turn the LED on
    }
    notPresentStartTime = 0;  // Reset the not present timer
  } else if (allSensorsNotPresent) {
    if (notPresentStartTime == 0) {
      notPresentStartTime = millis();
    } else if (millis() - notPresentStartTime >= notPresentDuration && currentState != "not_present") {
      currentState = "not_present";
      Serial.println("not_present");
      digitalWrite(ledPin, LOW);  // Turn the LED off
    }
    presentStartTime = 0;  // Reset the present timer
  }
}
