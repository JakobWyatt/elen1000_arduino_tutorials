void setup () {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void blink(int time) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(time);
  digitalWrite(LED_BUILTIN, LOW);
  delay(time);
}

void loop() {
  for (int n = 0; n != 31; ++n) {
    Serial.println(n);
    delay(1000);
  }
  Serial.println("Time is up!");
  while (true) {
    blink(1000);
  }
}

