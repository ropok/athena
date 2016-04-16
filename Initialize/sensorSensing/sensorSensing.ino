/* sensorSensing
      Sensor
    /T1\__________---_________/T2\
    |S1A  pid[0 1 2 3 4 5 6 7] S2A|
    |                             |
    |                             |
    |S1           TE           S2B|
*/
#include <LiquidCrystal.h>
LiquidCrystal lcd(48, 46, 44, 42, 40, 38);
//Sensing Sensor
int sensPinsPID[8] = {A9, A8, A7, A6, A5, A4, A3, A2};
int i = 0, j = 0;
int readings[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int threshold = 80;

void setup() {
  Serial.begin(9600);
  while (i < 8) {
    pinMode(sensPinsPID[i], INPUT);
    i++;
  }
  lcd.begin(16, 2);
  lcd.setCursor(2, 0);
  lcd.print("ATHENA");
  lcd.setCursor(3, 1);
  lcd.print("ETOIMOS");
  delay(500);
  lcd.clear();
}

void loop() {
  bacaSensor();
  menuBacaSensor();
  bacaSensorSerial();
  delay(1000);
}

void bacaSensor() {
  for (i = 0; i < 8; i++) {
    readings[i] = analogRead(sensPinsPID[i]);
    if (readings[i] < threshold)readings[i] = 1;
    else readings[i] = 0;
  }
}

void menuBacaSensor() {
  for ( i = 0; i < 8; i++) {
    lcd.setCursor(j, 0);
    lcd.print(readings[i]);
    j++;
    if (j > 11)j = 4;
  }
}

void bacaSensorSerial() {
  for (i = 0; i < 8; i++) {
    Serial.print(readings[i]);
    Serial.print(" ");
    if (i == 7) Serial.println();
  }
}

/*
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

    if (readings[k] < 40)
      readings[k] = 1; else readings[k] = 0;

    Serial.print(readings[k]);
    Serial.print(" ");
  }
  delay(100); //delay?
  }
*/
