int counter = 0;
  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Serial.setTimeout(100); // default 1000 ms, can use 10-100
  // but not needed if waiting for a new line...
  while (!Serial){}
}


void loop() {
  delay(1000); //in ms I believe
  if (Serial.available()>0)
  {
    String message = Serial.readStringUntil('\n');
    message = message + " " + String(counter);
    counter++;
    Serial.println("Hello from Arduino " + message);  
  }
}
