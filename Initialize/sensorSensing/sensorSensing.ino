/* sensorSensing

*/
int sensPins[] = {A0, A1, A2, A3, A4, A5, A6, A7};
int i = 0;
int threshold = 40;
int readings[] = {0, 0, 0, 0, 0, 0, 0};

void setup() {
  Serial.begin(9600);
  while (i < 8) {
    pinMode(sensPins[i], INPUT);
    i++;
  }

}

void loop() {
  Serial.print("\n");
  for (int k = 0; k < 8; k++) {
    readings[k] = analogRead(sensPins[k]);

    if (readings[k] < threshold)
      readings[k] = 1; else readings[k] = 0;

    Serial.print(readings[k]);
    Serial.print(" ");
  }
  delay(100);

}
