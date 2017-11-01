// digital pin
#define BUTTON_PIN 12

volatile byte interruptCounter = 0;
int numberOfInterrupts = 0;
 
void setup() {
 
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, FALLING);
 
}
 
void handleInterrupt() {
  interruptCounter++;
}
 
void loop() {
 
  if(interruptCounter>0){
 
      interruptCounter--;
      numberOfInterrupts++;
 
      Serial.print("An interrupt has occurred. Total: ");
      Serial.println(numberOfInterrupts);
  }
 
}
