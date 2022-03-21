/* 
 *  Blink
 */
 const int ledPin = 23;
 
void setup() {
  // Initialize digital pin ledPin as an output
  pinMode(ledPin, OUTPUT);

}

void loop() {
  // the loop function runs over and over again forever
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
  

}
