/* deadZoneMotor test
*/
int pwmKiri[2];
int pwmKanan[2];
char chrMsk;
int i;
int motorKiri[] = {11, 10}; //11 maju, 10 mundur
int motorKanan[] = {12, 13}; //12 maju, 13 mundur

void setup()
{
  Serial.begin(9600);
  for (i = 0; i < 2; i++) {
    pinMode(motorKiri[i], OUTPUT);
    pinMode(motorKanan[i], OUTPUT);
  }
}

void loop() {
  while (Serial.available()) {
    chrMsk = Serial.read();
    if (chrMsk == 'q') pwmKiri[0]++;
    if (chrMsk == 'a') pwmKiri[0]--;
    if (chrMsk == 'w') pwmKiri[1]++;
    if (chrMsk == 's') pwmKiri[1]--;
    if (chrMsk == 'e') pwmKanan[0]++;
    if (chrMsk == 'd') pwmKanan[0]--;
    if (chrMsk == 'r') pwmKanan[1]++;
    if (chrMsk == 'f') pwmKanan[1]--;

    Serial.print("PWM Kiri 1 = ");
    Serial.print(pwmKiri[0]);
    Serial.print(", PWM Kiri 2 = ");
    Serial.print(pwmKiri[1]);
    Serial.print(", PWM Kanan 1 = ");
    Serial.print(pwmKanan[0]);
    Serial.print(", PWM Kanan 2 = ");
    Serial.println(pwmKanan[1]);
  }
  analogWrite(motorKiri[0], pwmKiri[0]);
  analogWrite(motorKiri[1], pwmKiri[1]);
  analogWrite(motorKanan[0], pwmKanan[0]);
  analogWrite(motorKanan[1], pwmKanan[1]);


}
