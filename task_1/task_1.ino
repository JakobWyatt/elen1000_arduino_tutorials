void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void blink(int time) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(time);
  digitalWrite(LED_BUILTIN, LOW);
  delay(time);
}

void morse_s() {
  for(int i = 0; i != 3; ++i) {
    blink(200);
  }
}

void morse_o() {
  for(int i = 0; i != 3; ++i) {
    blink(600);
  }
}

void loop() {
  morse_s();
  morse_o();
  morse_s();
  delay(1000);
}
