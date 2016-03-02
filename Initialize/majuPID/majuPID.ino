/* PID
  maju PID, test track lurus

*/

int sensPinsPID[8] = {A8, A7, A6, A5, A4, A3, A2, A0};
int readings[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int threshold, i;
/* PID */
int deltaE, errorDulu, error, sensor;
int setPoint = 0;
long prosesKi, ePID;
float Kp, Ki, Kd;
unsigned long deltaT, waktuSekarang, waktuDulu;
/* PID */

/* Motor */
int pwmKiri[2];
int pwmKanan[2];
int motorKiri[] = {11, 10}; //11 maju, 10 mundur
int motorKanan[] = {12, 13}; //12 maju, 13 mundur
int kecepatan = 85;
int pwm1, pwm2, selisihPwm1, selisihPwm2;
/* Motor */


void setup() {

  /* PID set awal */
  Kp = 1.2;
  Ki = 0;
  Kd = 0.2;
  /* PID set awal */

  /********** Init Sensor **********/
  for ( i = 0; i < 8; i++) {
    pinMode(sensPinsPID[i], INPUT);
  }
  /********** Init Sensor **********/

  /********** Init Motor **********/
  for (i = 0; i < 2; i++) {
    pinMode(motorKiri[i], OUTPUT);
    pinMode(motorKanan[i], OUTPUT);
  }
  /********** Init Motor **********/

}

void loop() {
  bacaSensor();
  tabelSensing();
  pid();
  maju();
}

void bacaSensor() {
  for (i = 0; i < 8; i++) {
    readings[i] = analogRead(sensPinsPID[i]);
    if (readings[i] < threshold) {
      readings[i] = 1;
    } else readings[i] = 0;
  }
}

void pid() {
  errorDulu = error;
  error = setPoint - sensor;

  waktuDulu = waktuSekarang;
  waktuSekarang = millis();

  deltaT = waktuSekarang - waktuDulu;
  deltaE = error - errorDulu;

  prosesKi = prosesKi + (Ki * error * deltaT);
  ePID = (Kp * error) + prosesKi + ((Kd * deltaE) / deltaT);
}


void maju() {
  pwm1 = kecepatan + ((ePID * kecepatan) / 20);
  pwm2 = kecepatan - ((ePID * kecepatan) / 20);

  if (pwm1 > 255) {
    selisihPwm1 = pwm1 - 255;
    pwm1 = 255;
  } else {
    selisihPwm1 = 0;
  }

  if (pwm2 > 255) {
    selisihPwm2 = pwm2 - 255;
    pwm2 = 255;
  } else {
    selisihPwm2 = 0;
  }

  if (selisihPwm1 > selisihPwm2) {
    pwm2 =  pwm2 + selisihPwm1;
  } else if (selisihPwm1 < selisihPwm2) {
    pwm1 = pwm1 + selisihPwm2;
  }

  analogWrite(motorKiri[0], pwm1);
  analogWrite(motorKiri[1], 0);

  analogWrite(motorKanan[0], pwm2);
  analogWrite(motorKanan[1], 0);
}

void tabelSensing() {
  //set 1 nilai sensor
  //1000 0000 (20)
  if (readings[0] == 1 && readings[1] == 0 && readings[2] == 0
      && readings[3] == 0 && readings[4] == 0 && readings[5] == 0
      && readings[6] == 0 && readings[7] == 0) sensor = 20;

  //0100 0000 (10)
  if (readings[0] == 0 && readings[1] == 1 && readings[2] == 0
      && readings[3] == 0 && readings[4] == 0 && readings[5] == 0
      && readings[6] == 0 && readings[7] == 0) sensor = 10;

  //0010 0000 (5)
  if (readings[0] == 0 && readings[1] == 0 && readings[2] == 1
      && readings[3] == 0 && readings[4] == 0 && readings[5] == 0
      && readings[6] == 0 && readings[7] == 0) sensor = 5;

  //0001 0000 = 0 (Set Point)
  if (readings[0] == 0 && readings[1] == 0 && readings[2] == 0
      && readings[3] == 1 && readings[4] == 0 && readings[5] == 0
      && readings[6] == 0 && readings[7] == 0) sensor = 0; //kecepatan = 200;}

  // 0000 1000 (0)
  if (readings[0] == 0 && readings[1] == 0 && readings[2] == 0
      && readings[3] == 0 && readings[4] == 1 && readings[5] == 0
      && readings[6] == 0 && readings[7] == 0) sensor = 0;

  //0000 0100 (-5)
  if (readings[0] == 0 && readings[1] == 0 && readings[2] == 0
      && readings[3] == 0 && readings[4] == 0 && readings[5] == 1
      && readings[6] == 0 && readings[7] == 0) sensor = -5;

  //0000 0010 (-10)
  if (readings[0] == 0 && readings[1] == 0 && readings[2] == 0
      && readings[3] == 0 && readings[4] == 0 && readings[5] == 0
      && readings[6] == 1 && readings[7] == 0) sensor = -10;

  //0000 0001 (-20)
  if (readings[0] == 0 && readings[1] == 0 && readings[2] == 0
      && readings[3] == 0 && readings[4] == 0 && readings[5] == 0
      && readings[6] == 0 && readings[7] == 1) sensor = -20;

  //Set 2 nilai sensor
  //1100 0000 (16)
  if (readings[0] == 1 && readings[1] == 1 && readings[2] == 0
      && readings[3] == 0 && readings[4] == 0 && readings[5] == 0
      && readings[6] == 0 && readings[7] == 0) sensor = 16;

  //0110 0000 (8)
  if (readings[0] == 0 && readings[1] == 1 && readings[2] == 1
      && readings[3] == 0 && readings[4] == 0 && readings[5] == 0
      && readings[6] == 0 && readings[7] == 0) sensor = 8;

  //0011 0000 (2)
  if (readings[0] == 0 && readings[1] == 0 && readings[2] == 1
      && readings[3] == 1 && readings[4] == 0 && readings[5] == 0
      && readings[6] == 0 && readings[7] == 0) sensor = 2;

  //0001 1000 (0)
  if (readings[0] == 0 && readings[1] == 0 && readings[2] == 0
      && readings[3] == 1 && readings[4] == 1 && readings[5] == 0
      && readings[6] == 0 && readings[7] == 0) sensor = 0;

  //0000 1100 (-2)
  if (readings[0] == 0 && readings[1] == 0 && readings[2] == 0
      && readings[3] == 0 && readings[4] == 1 && readings[5] == 1
      && readings[6] == 0 && readings[7] == 0) sensor = -2;

  //0000 0110 (-8)
  if (readings[0] == 0 && readings[1] == 0 && readings[2] == 0
      && readings[3] == 0 && readings[4] == 0 && readings[5] == 1
      && readings[6] == 1 && readings[7] == 0) sensor = -8;

  //0000 0011 (-16)
  if (readings[0] == 0 && readings[1] == 0 && readings[2] == 0
      && readings[3] == 0 && readings[4] == 0 && readings[5] == 0
      && readings[6] == 1 && readings[7] == 1) sensor = -16;
}
