//============ Athena ===========
//++++++++++++ Etoimos ++++++++++++
//|||masANDI|||ADIT|||ERLIANSYAH|||JALER|
//---------------Athena.v02--------------
//tambah EEPROM
#include <EEPROM.h>

/*  Daftar void
 * 1. Setup
 * 2. Loop
 * 3. bacaSensor
 * 4. menuBacaSensor - 0 1
 * 5. majuPID
 * 6. pid (rumus)
 * 7. Maju
 * 8. Mapping
 * 9. Berhenti
 * 10. BelokKanan
 * 11. BelokKiri
 * 12. TabelSensing
 */

/***************** LCD ******************/
#include <LiquidCrystal.h>
LiquidCrystal lcd(44, 42, 40, 38, 36, 34);
/***************** LCD ******************/

/***************************** List Sensor *****************/
//int sensPinsPID[8] = {A0, A1, A2, A3, A4, A5, A6, A7};//8index
int sensPinsPID[8] = {A8, A7, A6, A5, A4, A3, A2, A0};
int sensPinsTanduk[2] = {A9, A1};//2index
int sensPinsSayap[2] = {A11, A10};//2index
//valueSensor
int readings[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int readingsT[2] = {0, 0};
int readingsS[2] = {0, 0};
int threshold ;
//looping Variable
int i, j = 4, k = 5, l = 8 ;
/***************************** List Sensor *****************/

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
int kecepatanBelok = 100;
int rem = 50;
int pwm1, pwm2, selisihPwm1, selisihPwm2;
/* Motor */

/* Mapping */
int count = 0;
int flagBelok = 0;
int n;
int opsiMapping = 0;
const int mapKananFull = 1;
int flagSkip = 0;
int flagInvers = 0;

#define mapKananFULL 50
#define mapKananCP1 51
#define mapKananCP2 52
#define mapKananCP3 53
#define mapKananCP4 54
#define mapKananCP5 55

#define mapKiriFULL 60
#define mapKiriCP1 61
#define mapKiriCP2 62
#define mapKiriCP3 63
#define mapKiriCP4 64
#define mapKiriCP5 65
/* Mapping */

/* Menu */
#define mainMenu 0
#define start 1
#define startKiri 2
#define kiriFull 3
#define kiriCp1 4
#define kiriCp2 5
#define kiriCp3 6
#define kiriCp4 24
#define kiriCp5 25
#define startKanan 7
#define kananFull 8
#define kananCp1 9
#define kananCp2 10
#define kananCp3 11
#define kananCp4 26
#define kananCp5 27
#define menuSetting 12
#define menuSensor 13
#define sensorAnalog 14
#define XX 15
#define analogTS 16
#define sensorDigital 17
#define nilaiThreshold 18
#define menuPID 19
#define pidKp 20
#define pidKi 21
#define pidKd 22
#define menuKecepatan 23
#define menuDelay 28
#define menuDelay0 29
#define menuDelay1 30
#define menuDelay2 31
#define menuDelay3 32
#define menuDelay4 33
#define menuDelay5 34


int btnPlus = 0;
int menuDisp = 0;
int menu = 0;
int flagClear = 0;
//button
const int button1 = 32;
const int button3 = 30;
const int button5 = 28;
const int button6 = 26;
const int button4 = 24;
const int button2 = 22;
/*|||||||||||||||||||||||
**||       LCD         ||
**|||||||||||||||||||||||
**b1  b3  b5  b6  b4  b2
*b1 : Geser Kiri atau --
*b3 : - (nilai kecil)
*b5 : ok
*b6 : back
*b4 : + (nilai kecil)
*b2 : Geser Kanan atau ++
/* Menu */

/* EEPROM List */
const int addrDelay[5] = {4, 5, 6, 7, 8};
const int addrThres = 9;
const int addrKecepatan = 0;
int eeDelay[5] = {0, 0, 0, 0, 0 } ;
int eeThres ;
int Delay[5] = {0, 0, 0, 0, 0 } ;
int eeKecepatan;

/* EEPROM List */

void setup() {
  //Serial.begin(9600);

  /* Inisialisasi EEPROM */
  eeDelay[0] = EEPROM.read(addrDelay[0]);
  eeDelay[1] = EEPROM.read(addrDelay[1]);
  eeDelay[2] = EEPROM.read(addrDelay[2]);
  eeDelay[3] = EEPROM.read(addrDelay[3]);
  eeDelay[4] = EEPROM.read(addrDelay[4]);
  eeThres = EEPROM.read(addrThres);
  eeKecepatan = EEPROM.read(addrKecepatan);

  Delay[0] = eeDelay[0];
  Delay[1] = eeDelay[1];
  Delay[2] = eeDelay[2];
  Delay[3] = eeDelay[3];
  Delay[4] = eeDelay[4];
  threshold = eeThres;
  kecepatan = eeKecepatan;
  /* Inisialisasi EEPROM */

  /* PID set awal */
  Kp = 1.2;
  Ki = 0;
  Kd = 0.2;
  /* PID set awal */

  /********** Init Sensor **********/
  for ( i = 0; i < 8; i++)
    pinMode(sensPinsPID[i], INPUT);
  for ( i = 0; i < 2; i++) {
    pinMode(sensPinsTanduk[i], INPUT);
    pinMode(sensPinsSayap[i], INPUT);
  }
  /********** Init Sensor **********/

  /********** Init Motor **********/
  for (i = 0; i < 2; i++) {
    pinMode(motorKiri[i], OUTPUT);
    pinMode(motorKanan[i], OUTPUT);
  }
  /********** Init Motor **********/

  /* Engine Start */
  lcd.begin(16, 2);
  lcd.setCursor(5, 0);
  lcd.print("ATHENA v02");
  lcd.setCursor(6, 1);
  lcd.print("ETOIMOS");
  delay(500);
  lcd.clear();
  /* Engine Start */
}

void loop() {
  menuLCD();
}

void bacaSensor() {
  for (i = 0; i < 8; i++) {
    readings[i] = analogRead(sensPinsPID[i]);
    if (readings[i] < threshold) {
      readings[i] = 1;
    } else readings[i] = 0;
    //    readings[7] = analogRead(sensPinsPID[7]);
    //    if (readings[7] < 80) {
    //      readings[7] = 1;
    //    } else readings[7] = 0;
  }
}

void bacaSensorTengah() {
  for (i = 3 ; i < 5; i++) {
    readings[i] = analogRead(sensPinsPID[i]);
    if (readings[i] < threshold) {
      readings[i] = 1;
    } else readings[i] = 0;
  }
}

void bacaTanduk() {
  for ( i = 0; i < 2; i++) {
    readingsT[i] = analogRead(sensPinsTanduk[i]);
    if (readingsT[i] < threshold) {
      readingsT[i] = 1;
    } else readingsT[i] = 0;
  }
}

void bacaSayap() {
  for ( i = 0; i < 2; i++) {
    readingsS[i] = analogRead(sensPinsSayap[i]);
    if (readingsS[i] < threshold) {
      readingsS[i] = 1;
    } else readingsS[i] = 0;
  }
}

void menuBacaSensor() {
  for ( i = 0; i < 8; i++) {
    lcd.setCursor(j, 0);
    lcd.print(readings[i]);
    j++;
    if (j > 11)j = 4;
  }

  for (  i = 0; i < 2 ; i++) {
    lcd.setCursor(k, 1);
    lcd.print(readingsT[i]);
    k++;
    if (k > 6)k = 5;
  }

  for (  i = 0; i < 2; i++) {
    lcd.setCursor(l, 1);
    lcd.print(readingsS[i]);
    l++;
    if (l > 9)l = 8;
  }
}

void majuPID() {
  if (flagSkip == 0) {
    bacaSensor();
    if (flagInvers == 1)invers();
    tabelSensing();
    pid();
    maju();
  }

  if (opsiMapping == mapKananFULL) mappingKananFull();
  if (opsiMapping == mapKananCP1) mappingKananCP1();
  if (opsiMapping == mapKananCP2) mappingKananCP2();
  if (opsiMapping == mapKananCP3) mappingKananCP3();
  if (opsiMapping == mapKananCP4) mappingKananCP4();
  if (opsiMapping == mapKananCP5) mappingKananCP5();

  if (opsiMapping == mapKiriFULL) mappingKiriFull();
  if (opsiMapping == mapKiriCP1) mappingKiriCP1();
  if (opsiMapping == mapKiriCP2) mappingKiriCP2();
  if (opsiMapping == mapKiriCP3) mappingKiriCP3();
  if (opsiMapping == mapKiriCP4) mappingKiriCP4();
  if (opsiMapping == mapKiriCP5) mappingKiriCP5();
}

void majuPIDnoMapping() {
  bacaSensor();
  tabelSensing();
  pid();
  maju();
}

/* Kondisi Lurus */
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

/********************** Mapping Kiri **********************/
void mappingKiriFull() {

}
void mappingKiriCP1() {

}
void mappingKiriCP2() {

}
void mappingKiriCP3() {

}
void mappingKiriCP4() {

}
void mappingKiriCP5() {

}
/********************** Mapping Kiri **********************/




/********************** Mapping Kanan **********************/
void mappingKananFull() {

}
void mappingKananCP1() {
  //ketemu tanduk
  bacaTanduk();
  if ((readingsT[0] == 0 && readingsT[1] == 1)
      || (readingsT[0] == 1 && readingsT[1] == 0)
      || (readingsT[0] == 1 && readingsT[1] == 1)
     ) {
    count++;
    if (count == 1) {
      berhenti(); majuPID();
    }
    if (count == 2) {
      berhenti(); belokKanan();
    }
    if (count == 3) {
      berhenti(); belokKiri();
    }
    if (count == 4) {
      berhenti(); belokKiri();
    }
    if (count == 5) {
      berhenti(); belokKanan();
    }
    if (count == 6) {
      berhenti(); belokKanan();
    }
    if (count == 7) {
      berhenti(); belokKiri();
    }
    if (count == 8) {
      berhenti(); belokKanan();
    }
    if (count == 9) {
      berhenti(); belokKiri();
    }
    if (count == 10) {
      berhenti(); belokKiri();
    }
    if (count == 11) {
      berhenti(); belokKanan();
    }
    if (count == 12) {
      berhenti(); belokKanan();
    }
    if (count == 13) {
      berhenti(); belokKiri();
    }
    if (count == 14) {
      berhenti(); belokKiri();
    }
    if (count == 15) {
      berhenti(); belokKanan();
    }
    if (count == 16) {
      berhenti(); belokKanan();
    }
    if (count == 17) {
      berhenti(); belokKiri();
    }
    if (count == 18) {
      berhenti();
    }
    if (count == 19) {
      berhenti(); belokKiri();
    }
    if (count == 20) {
      delay(80); mundur();  belokKanan();
    }
    if (count == 21) {
      majuPIDSayap();
    }
    if (count == 22) {
      berhenti(); belokKanan();
    }
    if (count == 23) {
      berhenti(); belokKanan();
    }
    if (count == 24) {
      berhenti(); belokKiri();
    }
    if (count == 25) {
      berhenti(); belokKanan();
    }
    if (count == 26) {
      berhenti(); belokKiri(); majuPIDCP1();
    }
    if (count == 27) {
      stopJalan();
    }
  }
}

void mappingKananCP2() {
  //ketemu tanduk
  bacaTanduk();
  if ((readingsT[0] == 0 && readingsT[1] == 1)
      || (readingsT[0] == 1 && readingsT[1] == 0)
      || (readingsT[0] == 1 && readingsT[1] == 1)
     ) {
    count++;
    if (count == 1) {
      berhenti(); belokKiri();
    }
    if (count == 2) {
      berhenti(); belokKanan(); majuPIDSayap(); belokKiri(); majuPIDSayap();  belokKanan(); majuPIDSayap(); belokKiri(); majuPIDSayap();
      delay(200); belokKiri(); majuPIDSayap(); belokKanan(); majuPIDSayap(); belokKiri(); majuPIDSayap(); delay(50); belokKanan(); majuPIDSayap();
      belokKanan(); belokKanan();
    }
    if (count == 3) {
      berhenti(); belokKiri();
    }
    if (count == 4) {
      majuPIDSayap(); belokKiri();
    }
    if (count == 5) {
      berhenti(); belokKanan();
    }
  }
}

//mainan disini
void mappingKananCP3() {
  //ketemu tanduk
  bacaTanduk();
  if (flagInvers == 1)inversTanduk();
  if ((readingsT[0] == 0 && readingsT[1] == 1)
      || (readingsT[0] == 1 && readingsT[1] == 0)
      || (readingsT[0] == 1 && readingsT[1] == 1)
     ) {
    count++;
    lcdTampilCount();
    //    masuk Invers
    if (count == 1) {
      flagInvers = 1;
    }
    if (count == 2) {
      berhenti(); belokKanan();
    }
    if (count == 3) {
      berhenti();
    }
    if (count == 4) {
      flagInvers = 0;
    }
    if (count == 5) {
      stopJalan();
    }
    //    if (count == 6) {
    //    }
  }
}
void mappingKananCP4() {
  //ketemu tanduk
  bacaTanduk();
  if ((readingsT[0] == 0 && readingsT[1] == 1)
      || (readingsT[0] == 1 && readingsT[1] == 0)
      || (readingsT[0] == 1 && readingsT[1] == 1)
     ) {
    count++;
    if (count == 1) {
      majuPIDCP3(); belokKiriPoros();
      majuPIDCP3(); belokKananPoros();
      majuPIDCP3(); belokKananPoros();
      majuPIDCP3(); belokKanan();
      majuPIDCP3(); delay(10); belokKiriPoros();
      majuPIDCP3(); belokKiriPoros();
      majuPIDCP3(); belokKanan();
      belokKiriPoros();
    }
    if (count == 2) {
      majuPIDSayap(); belokKiri();
    }
    if (count == 3) {
      kecepatan = 200;
      berhenti(); belokKanan();   belokKanan();
    }
    if (count == 4) {
      kecepatan = 150; rem = 30;
      berhenti(); belokKanan();

    }
    if (count == 5) {
      berhenti(); belokKiri();
    }
    if (count == 6) {
      majuPIDSayap(); belokKanan(); belokKanan(); //majuPIDSayap(); belokKanan(); majuPIDSayap(); belokKanan();
      rem = 50;
    }
    if (count == 7) {
      kecepatan = 200;
      berhenti(); belokKanan();
    }
    if (count == 8) {
      berhenti(); belokKanan();
    }
    if (count == 9) {
      kecepatan = 150;
      berhenti(); belokKiri();
    }
    if (count == 10) {
      berhenti(); belokKiri();
    }
    if (count == 11) {
      majuPIDSayap(); belokKanan();
    }
    if (count == 12) {
      berhenti(); belokKanan();
    }
    if (count == 13) {
      majuPIDSayap();
    }
    if (count == 14) {
      berhenti(); belokKiri();
    }
    if (count == 15) {
      berhenti(); belokKanan();
    }
    if (count == 16) {
      berhenti(); belokKiri();
    }
    if (count == 17) {
      berhenti(); belokKanan();
    }
    if (count == 18) {
      berhenti(); belokKiri(); belokKiri();
    }
    if (count == 19) {
      berhenti(); belokKanan();
    }
    if (count == 20) {
      berhenti(); belokKiri(); belokKiri();
    }
    if (count == 21) {
      majuPIDSayap(); delay(150); stopJalan();
    }

  }
}
void mappingKananCP5() {
  //Finish!
  if ((readingsT[0] == 0 && readingsT[1] == 1)
      || (readingsT[0] == 1 && readingsT[1] == 0)
      || (readingsT[0] == 1 && readingsT[1] == 1)
     ) {
    count++;
    if (count == 1) {
      berhenti(); belokKiri();
    }
    if (count == 1) {
      majuPIDSayap(); belokKanan();
    }
    if (count == 2) {
      berhenti(); belokKanan();
    }
    if (count == 3) {
      majuPIDSayap();
    }
    if (count == 4) {
      berhenti(); belokKiri();
    }
    if (count == 5) {
      berhenti(); belokKanan();
    }
    if (count == 6) {
      berhenti(); belokKiri();
    }
    if (count == 7) {
      berhenti(); belokKanan();
    }
    if (count == 8) {
      berhenti(); belokKiri(); belokKiri();
    }
    if (count == 9) {
      berhenti(); belokKanan();
    }
    if (count == 10) {
      berhenti(); belokKiri(); belokKiri();
    }
    if (count == 11) {
      majuPIDSayap(); delay(150); stopJalan();
    }
  }
}
/********************** Mapping Kanan **********************/



void majuPIDSayap() {
  while (1) {
    bacaSensor();
    tabelSensing();
    pid();
    maju();
    bacaSayap();
    if (
      (readingsS[0] == 0 && readingsS[1] == 1)
      || (readingsS[0] == 1 && readingsS[1] == 0)
      || (readingsS[0] == 1 && readingsS[1] == 1)
    ) break;
  }
}

void majuPIDCP1() {
  while (1) {
    bacaSensor();
    tabelSensing();
    pid();
    maju();

    if ((readings[0] == 0 && readings[1] == 0 && readings[2] == 0
         && readings[3] == 1 && readings[4] == 1 && readings[5] == 1
         && readings[6] == 1 && readings[7] == 1)
        || (readings[0] == 0 && readings[1] == 0 && readings[2] == 0
            && readings[3] == 0 && readings[4] == 1 && readings[5] == 1
            && readings[6] == 1 && readings[7] == 1)
        || (readings[0] == 0 && readings[1] == 0 && readings[2] == 0
            && readings[3] == 0 && readings[4] == 0 && readings[5] == 1
            && readings[6] == 1 && readings[7] == 1)
       )break;
  }
}

void majuPIDCP3() {
  while (1) {
    bacaSensor();
    tabelSensing();
    pid();
    maju();
    /*ke Kiri
    * 2345,   1234,     123,       234,     12,       1,          345
    *01111000 11110000  11100000  01110000  11000000  10000000  00111000
    *   4567 ,    5678,      567,      67   ,      78,        8
    *00011110 00001111   00001110 00000110   00000011  00000001
    */

    //Kiri: 0111 1000   1111 0000    1110 0000    0111 0000    1100 0000    1000 00000    0011 1000
    if ((readings[0] == 0 && readings[1] == 1 && readings[2] == 1 && readings[3] == 1 &&
         readings[4] == 1 && readings[5] == 0 && readings[6] == 0 && readings[7] == 0)

        || (readings[0] == 1 && readings[1] == 1 && readings[2] == 1 && readings[3] == 1 &&
            readings[4] == 0 && readings[5] == 0 && readings[6] == 0 && readings[7] == 0)

        || (readings[0] == 1 && readings[1] == 1 && readings[2] == 1 && readings[3] == 0 &&
            readings[4] == 0 && readings[5] == 0 && readings[6] == 0 && readings[7] == 0)

        || (readings[0] == 0 && readings[1] == 1 && readings[2] == 1 && readings[3] == 1 &&
            readings[4] == 0 && readings[5] == 0 && readings[6] == 0 && readings[7] == 0)

        || (readings[0] == 1 && readings[1] == 1 && readings[2] == 0 && readings[3] == 0 &&
            readings[4] == 0 && readings[5] == 0 && readings[6] == 0 && readings[7] == 0)

        || (readings[0] == 1 && readings[1] == 0 && readings[2] == 0 && readings[3] == 0 &&
            readings[4] == 0 && readings[5] == 0 && readings[6] == 0 && readings[7] == 0)

        || (readings[0] == 0 && readings[1] == 0 && readings[2] == 1 && readings[3] == 1 &&
            readings[4] == 1 && readings[5] == 0 && readings[6] == 0 && readings[7] == 0)

        //Kanan :0001 1110     0000 1111     0000 1110     0000 0110     0000 0011    0000 0001

        || (readings[0] == 0 && readings[1] == 0 && readings[2] == 0 && readings[3] == 1 &&
            readings[4] == 1 && readings[5] == 1 && readings[6] == 1 && readings[7] == 0)

        || (readings[0] == 0 && readings[1] == 0 && readings[2] == 0 && readings[3] == 0 &&
            readings[4] == 1 && readings[5] == 1 && readings[6] == 1 && readings[7] == 1)

        || (readings[0] == 0 && readings[1] == 0 && readings[2] == 0 && readings[3] == 0 &&
            readings[4] == 1 && readings[5] == 1 && readings[6] == 1 && readings[7] == 0)

        || (readings[0] == 0 && readings[1] == 0 && readings[2] == 0 && readings[3] == 0 &&
            readings[4] == 0 && readings[5] == 1 && readings[6] == 1 && readings[7] == 0)

        || (readings[0] == 0 && readings[1] == 0 && readings[2] == 0 && readings[3] == 0 &&
            readings[4] == 0 && readings[5] == 0 && readings[6] == 1 && readings[7] == 1)

        || (readings[0] == 0 && readings[1] == 0 && readings[2] == 0 && readings[3] == 0 &&
            readings[4] == 0 && readings[5] == 0 && readings[6] == 0 && readings[7] == 1)

       )break;
  }
}

void berhenti() {
  while (1) {
    bacaSayap();
    if (flagInvers == 1)inversSayap();
    analogWrite(motorKiri[0], rem);
    analogWrite(motorKiri[1], 0);

    analogWrite(motorKanan[0], rem);
    analogWrite(motorKanan[1], 0);
    //ketemu sayap
    if (
      (readingsS[0] == 0 && readingsS[1] == 1)
      || (readingsS[0] == 1 && readingsS[1] == 0)
      || (readingsS[0] == 1 && readingsS[1] == 1)
    ) break;
  }
}
void majuManual() {
  while (1) {
    bacaSayap();
    analogWrite(motorKiri[0], kecepatanBelok);
    analogWrite(motorKiri[1], 0);

    analogWrite(motorKanan[0], kecepatanBelok);
    analogWrite(motorKanan[1], 0);
    //ketemu sayap
    if (
      (readingsS[0] == 0 && readingsS[1] == 1)
      || (readingsS[0] == 1 && readingsS[1] == 0)
      || (readingsS[0] == 1 && readingsS[1] == 1)
    ) break;
  }
}

void mundur() {
  while (1) {
    bacaSayap();
    analogWrite(motorKiri[0], 0);
    analogWrite(motorKiri[1], kecepatanBelok);

    analogWrite(motorKanan[0], 0);
    analogWrite(motorKanan[1], kecepatanBelok);
    //ketemu sayap
    if (
      (readingsS[0] == 0 && readingsS[1] == 1)
      || (readingsS[0] == 1 && readingsS[1] == 0)
      || (readingsS[0] == 1 && readingsS[1] == 1)
    ) break;
  }
}

void stopJalan() {
  while (1) {
    analogWrite(motorKiri[0], 0);
    analogWrite(motorKiri[1], 0);

    analogWrite(motorKanan[0], 0);
    analogWrite(motorKanan[1], 0);
  }
}

void belokKanan() {
  flagBelok = 0;
  while (1) {
    bacaTanduk();
    bacaSensorTengah();
    if (flagInvers == 1)inversTanduk();
    if (flagInvers == 1)inversTengah();
    analogWrite(motorKiri[0], kecepatanBelok);
    analogWrite(motorKiri[1], 0);

    analogWrite(motorKanan[0], 0);
    analogWrite(motorKanan[1], kecepatanBelok);
    if (readingsT[1] == 1 && readingsT[0] == 0) {
      flagBelok = 1;
    }
    //ketemu salah satu set point (3 case di tengah)
    if ( flagBelok == 1 &&
         ((readings[3] == 0 && readings[4] == 1)
          || (readings[3] == 1 && readings[4] == 0)
          || (readings[3] == 1 && readings[4] == 1)
         )) {
      flagBelok = 0;
      break;
    }
  }
}

void belokKananPoros() {
  flagBelok = 0;
  while (1) {
    bacaTanduk();
    bacaSensorTengah();
    analogWrite(motorKiri[0], kecepatanBelok);
    analogWrite(motorKiri[1], 0);

    analogWrite(motorKanan[0], 0);
    analogWrite(motorKanan[1], 0);
    if (readingsT[1] == 1 && readingsT[0] == 0) {
      flagBelok = 1;
    }
    //ketemu salah satu set point (3 case di tengah)
    if ( flagBelok == 1 &&
         ((readings[3] == 0 && readings[4] == 1)
          || (readings[3] == 1 && readings[4] == 0)
          || (readings[3] == 1 && readings[4] == 1)
         )) {
      flagBelok = 0;
      break;
    }
  }
}

void belokKananTipis() {
  analogWrite(motorKiri[0], kecepatanBelok);
  analogWrite(motorKiri[1], 0);

  analogWrite(motorKanan[0], 0);
  analogWrite(motorKanan[1], kecepatanBelok);
  delay(100);

  belokKiri();
}

void belokKiri() {
  flagBelok = 0;
  while (1) {

    bacaTanduk();
    bacaSensorTengah();
    if (flagInvers == 1)inversTanduk();
    if (flagInvers == 1)inversTengah();
    analogWrite(motorKiri[0], 0);
    analogWrite(motorKiri[1], kecepatanBelok);

    analogWrite(motorKanan[0], kecepatanBelok);
    analogWrite(motorKanan[1], 0);
    if (readingsT[0] == 1 && readingsT[1] == 0) {
      flagBelok = 1;
    }
    //ketemu salah satu set point (3 case di tengah)
    if ( flagBelok == 1 &&
         ((readings[3] == 0 && readings[4] == 1)
          || (readings[3] == 1 && readings[4] == 0)
          || (readings[3] == 1 && readings[4] == 1)
         )) {
      flagBelok = 0;
      break;
    }
  }
}


void belokKiriPoros() {
  flagBelok = 0;
  while (1) {
    bacaTanduk();
    bacaSensorTengah();
    analogWrite(motorKiri[0], 0);
    analogWrite(motorKiri[1], 0);

    analogWrite(motorKanan[0], kecepatanBelok);
    analogWrite(motorKanan[1], 0);
    if (readingsT[0] == 1 && readingsT[1] == 0) {
      flagBelok = 1;
    }
    //ketemu salah satu set point (3 case di tengah)
    if ( flagBelok == 1 &&
         ((readings[3] == 0 && readings[4] == 1)
          || (readings[3] == 1 && readings[4] == 0)
          || (readings[3] == 1 && readings[4] == 1)
         )) {
      flagBelok = 0;
      break;
    }
  }
}

void invers() {
  for (i = 0; i < 8; i++) {
    if (readings[i] == 1)readings[i] = 0;
    else readings[i] = 1;
  }
}


void inversTanduk() {
  for (i = 0; i < 2; i++) {
    if (readingsT[i] == 1)readingsT[i] = 0;
    else readingsT[i] = 1;
  }
}

void inversSayap() {
  for (i = 0; i < 2; i++) {
    if (readingsS[i] == 1)readingsS[i] = 0;
    else readingsS[i] = 1;
  }
}

void inversTengah() {
  for (i = 3; i < 5; i++) {
    if (readings[i] == 1)readings[i] = 0;
    else readings[i] = 1;
  }
}

void menuLCD() {
  if (digitalRead(button1) == 1) {
    while (digitalRead(button1) == 1);
    menu--;
    lcd.clear();
  }
  if (digitalRead(button2) == 1) {
    while (digitalRead(button2) == 1);
    menu++;
    lcd.clear();
  }
  if (digitalRead(button3) == 1) {
    while (digitalRead(button3) == 1);
    lcd.clear();
  }
  if (digitalRead(button4) == 1) {
    while (digitalRead(button4) == 1);
    lcd.clear();
  }
  /* Tampilan Menu Awal */
  if (menuDisp == mainMenu) {
    menu = constrain(menu, 0, 2);
    if (menu == 0) {
      lcd.setCursor(6, 0); lcd.print("ATHENA");
      lcd.setCursor(0, 1); lcd.print(">START  MENU  DLY");
    }
    if (menu == 1) {
      lcd.setCursor(6, 0); lcd.print("ATHENA");
      lcd.setCursor(0, 1); lcd.print(" START >MENU  DLY");
    }
    if (menu == 2) {
      lcd.setCursor(6, 0); lcd.print("ATHENA");
      lcd.setCursor(0, 1); lcd.print(" START  MENU >DLY");
    }
  }
  /* Tampilan Menu Awal */

  /* menu Delay */
  if (digitalRead(button5) == 1 && menu == 2 && menuDisp == mainMenu) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = menuDelay;
    lcd.clear();
  }
  if (menuDisp == menuDelay) {
    menu = constrain(menu, 0, 4);
    if (menu == 0) {
      lcd.setCursor(6, 0); lcd.print("DELAY");
      lcd.setCursor(0, 1); lcd.print("<- DELAY 0 ->");
    }
    if (menu == 1) {
      lcd.setCursor(6, 0); lcd.print("DELAY");
      lcd.setCursor(0, 1); lcd.print("<- DELAY 1 ->");
    }
    if (menu == 2) {
      lcd.setCursor(6, 0); lcd.print("DELAY");
      lcd.setCursor(0, 1); lcd.print("<- DELAY 2 ->");
    }
    if (menu == 3) {
      lcd.setCursor(6, 0); lcd.print("DELAY");
      lcd.setCursor(0, 1); lcd.print("<- DELAY 3 ->");
    }
    if (menu == 4) {
      lcd.setCursor(6, 0); lcd.print("DELAY");
      lcd.setCursor(0, 1); lcd.print("<- DELAY 4 ->");
    }
  }
  //DELAY0
  if (digitalRead(button5) == 1 && menu == 0 && menuDisp == menuDelay) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = menuDelay0;
    lcd.clear();
  }
  // BUTUH EEPROM
  if (menuDisp == menuDelay0) {
    menu = constrain(menu, 0, 0);
    lcd.setCursor(6, 0);
    lcd.print(eeDelay[0]);
    //kurangin nilai
    if (digitalRead(button1) == 1) {
      while (digitalRead(button1) == 1);
      eeDelay[0] -= 10;
    }
    //tambah Nilai
    if (digitalRead(button2) == 1) {
      while (digitalRead(button2) == 1);
      eeDelay[0] += 10;
    }
    //kurangin nilai
    if (digitalRead(button3) == 1) {
      while (digitalRead(button3) == 1);
      eeDelay[0] -= 1;
    }
    //tambah Nilai
    if (digitalRead(button4) == 1) {
      while (digitalRead(button4) == 1);
      eeDelay[0] += 1;
    }
    //tambah Nilai
    if (digitalRead(button5) == 1) {
      while (digitalRead(button5) == 1);
      //EEPROM simpan nilai eeDelay[0]

      lcd.clear();
      EEPROM.update(addrDelay[0], eeDelay[0]);
      Delay[0] = eeDelay[0];
      lcd.setCursor(0, 0); lcd.print("Delay0 Saved!");
    }
  }
  //DELAY1
  if (digitalRead(button5) == 1 && menu == 1 && menuDisp == menuDelay) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = menuDelay1;
    lcd.clear();
  }
  // BUTUH EEPROM
  if (menuDisp == menuDelay1) {
    menu = constrain(menu, 0, 0);
    lcd.setCursor(6, 0);
    lcd.print(eeDelay[1]);
    //kurangin nilai
    if (digitalRead(button1) == 1) {
      while (digitalRead(button1) == 1);
      eeDelay[1] -= 10;
    }
    //tambah Nilai
    if (digitalRead(button2) == 1) {
      while (digitalRead(button2) == 1);
      eeDelay[1] += 10;
    }
    //kurangin nilai
    if (digitalRead(button3) == 1) {
      while (digitalRead(button3) == 1);
      eeDelay[1] -= 1;
    }
    //tambah Nilai
    if (digitalRead(button4) == 1) {
      while (digitalRead(button4) == 1);
      eeDelay[1] += 1;
    }
    //tambah Nilai
    if (digitalRead(button5) == 1) {
      while (digitalRead(button5) == 1);
      //EEPROM simpan nilai eeDelay[0]

      lcd.clear();
      EEPROM.update(addrDelay[1], eeDelay[1]);
      Delay[1] = eeDelay[1];
      lcd.setCursor(0, 0); lcd.print("Delay1 Saved!");
    }
  }

  //DELAY2
  if (digitalRead(button5) == 1 && menu == 2 && menuDisp == menuDelay) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = menuDelay2;
    lcd.clear();
  }
  // BUTUH EEPROM
  if (menuDisp == menuDelay2) {
    menu = constrain(menu, 0, 0);
    lcd.setCursor(6, 0);
    lcd.print(eeDelay[2]);
    //kurangin nilai
    if (digitalRead(button1) == 1) {
      while (digitalRead(button1) == 1);
      eeDelay[2] -= 10;
    }
    //tambah Nilai
    if (digitalRead(button2) == 1) {
      while (digitalRead(button2) == 1);
      eeDelay[2] += 10;
    }
    //kurangin nilai
    if (digitalRead(button3) == 1) {
      while (digitalRead(button3) == 1);
      eeDelay[2] -= 1;
    }
    //tambah Nilai
    if (digitalRead(button4) == 1) {
      while (digitalRead(button4) == 1);
      eeDelay[2] += 1;
    }
    //tambah Nilai
    if (digitalRead(button5) == 1) {
      while (digitalRead(button5) == 1);
      //EEPROM simpan nilai eeDelay[0]

      lcd.clear();
      EEPROM.update(addrDelay[2], eeDelay[2]);
      Delay[2] = eeDelay[2];
      lcd.setCursor(0, 0); lcd.print("Delay2 Saved!");
    }
  }

  //DELAY1
  if (digitalRead(button5) == 1 && menu == 3 && menuDisp == menuDelay) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = menuDelay3;
    lcd.clear();
  }
  // BUTUH EEPROM
  if (menuDisp == menuDelay3) {
    menu = constrain(menu, 0, 0);
    lcd.setCursor(6, 0);
    lcd.print(eeDelay[3]);
    //kurangin nilai
    if (digitalRead(button1) == 1) {
      while (digitalRead(button1) == 1);
      eeDelay[3] -= 10;
    }
    //tambah Nilai
    if (digitalRead(button2) == 1) {
      while (digitalRead(button2) == 1);
      eeDelay[3] += 10;
    }
    //kurangin nilai
    if (digitalRead(button3) == 1) {
      while (digitalRead(button3) == 1);
      eeDelay[3] -= 1;
    }
    //tambah Nilai
    if (digitalRead(button4) == 1) {
      while (digitalRead(button4) == 1);
      eeDelay[3] += 1;
    }
    //tambah Nilai
    if (digitalRead(button5) == 1) {
      while (digitalRead(button5) == 1);
      //EEPROM simpan nilai eeDelay[0]

      lcd.clear();
      EEPROM.update(addrDelay[3], eeDelay[3]);
      Delay[3] = eeDelay[3];
      lcd.setCursor(0, 0); lcd.print("Delay3 Saved!");
    }
  }

  //DELAY4
  if (digitalRead(button5) == 1 && menu == 4 && menuDisp == menuDelay) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = menuDelay4;
    lcd.clear();
  }
  // BUTUH EEPROM
  if (menuDisp == menuDelay4) {
    menu = constrain(menu, 0, 0);
    lcd.setCursor(6, 0);
    lcd.print(eeDelay[4]);
    //kurangin nilai
    if (digitalRead(button1) == 1) {
      while (digitalRead(button1) == 1);
      eeDelay[4] -= 10;
    }
    //tambah Nilai
    if (digitalRead(button2) == 1) {
      while (digitalRead(button2) == 1);
      eeDelay[4] += 10;
    }
    //kurangin nilai
    if (digitalRead(button3) == 1) {
      while (digitalRead(button3) == 1);
      eeDelay[4] -= 1;
    }
    //tambah Nilai
    if (digitalRead(button4) == 1) {
      while (digitalRead(button4) == 1);
      eeDelay[4] += 1;
    }
    //tambah Nilai
    if (digitalRead(button5) == 1) {
      while (digitalRead(button5) == 1);
      //EEPROM simpan nilai eeDelay[0]

      lcd.clear();
      EEPROM.update(addrDelay[4], eeDelay[4]);
      Delay[4] = eeDelay[4];
      lcd.setCursor(0, 0); lcd.print("Delay4 Saved!");
    }
  }


  if (digitalRead(button6) == 1 && (menuDisp == menuDelay0 ||
                                    menuDisp == menuDelay1 ||
                                    menuDisp == menuDelay2 ||
                                    menuDisp == menuDelay3 ||
                                    menuDisp == menuDelay4)) {
    while (digitalRead(button6) == 1);
    menu = 0;
    menuDisp = menuDelay;
    lcd.clear();
  }


  if (digitalRead(button6) == 1 && (menuDisp == menuDelay)) {
    while (digitalRead(button6) == 1);
    menu = 0;
    menuDisp = mainMenu;
    lcd.clear();
  }

  /* menu Delay */

  /* mainMenu ke Start */
  if (digitalRead(button5) == 1 && menu == 0 && menuDisp == mainMenu) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = start;
    lcd.clear();
  }
  if (menuDisp == start) {
    menu = constrain(menu, 0, 2);
    if (menu == 0) {
      lcd.setCursor(6, 0); lcd.print("START");
      lcd.setCursor(0, 1); lcd.print(">KIRI  KANAN  XX");
    }
    if (menu == 1) {
      lcd.setCursor(6, 0); lcd.print("START");
      lcd.setCursor(0, 1); lcd.print(" KIRI >KANAN  XX");
    }
    if (menu == 2) {
      lcd.setCursor(6, 0); lcd.print("START");
      lcd.setCursor(0, 1); lcd.print(" KIRI  KANAN >XX");
    }
  }
  /* mainMenu ke Start */

  /* Start ke XX */
  if (digitalRead(button5) == 1 && menu == 2 && menuDisp == start) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = XX;
    lcd.clear();
  }
  if (menuDisp == XX) {
    majuPIDnoMapping();
    lcd.noDisplay();
  }

  /* Start ke Kiri */
  if (digitalRead(button5) == 1 && menu == 0 && menuDisp == start) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = startKiri;
    lcd.clear();
  }
  if (menuDisp == startKiri) {
    menu = constrain(menu, 0, 5);
    if (menu == 0) {
      lcd.setCursor(2, 0); lcd.print("START KIRI");
      lcd.setCursor(0, 1); lcd.print(">Full  CP1  CP2");
    }
    if (menu == 1) {
      lcd.setCursor(2, 0); lcd.print("START KIRI");
      lcd.setCursor(0, 1); lcd.print(" Full >CP1  CP2");
    }
    if (menu == 2) {
      lcd.setCursor(2, 0); lcd.print("START KIRI");
      lcd.setCursor(0, 1); lcd.print(" Full  CP1 >CP2");
    }
    if (menu == 3) {
      lcd.setCursor(2, 0); lcd.print("START KIRI");
      lcd.setCursor(0, 1); lcd.print(">CP3  CP4  CP5");
    }
    if (menu == 4) {
      lcd.setCursor(2, 0); lcd.print("START KIRI");
      lcd.setCursor(0, 1); lcd.print(" CP3 >CP4  CP5");
    }
    if (menu == 5) {
      lcd.setCursor(2, 0); lcd.print("START KIRI");
      lcd.setCursor(0, 1); lcd.print(" CP3  CP4 >CP5");
    }
  }
  //masuk kiri Full
  if (digitalRead(button5) == 1 && menu == 0 && menuDisp == startKiri) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = kiriFull;
    lcd.clear();
  }
  if (menuDisp == kiriFull) {
    opsiMapping = mapKiriFULL;
    majuPID();
    lcd.noDisplay();
  }
  //masuk kiri CP1
  if (digitalRead(button5) == 1 && menu == 1 && menuDisp == startKiri) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = kiriCp1;
    lcd.clear();
  }
  if (menuDisp == kiriCp1) {
    opsiMapping = mapKiriCP1;
    majuPID();
    lcd.noDisplay();
  }
  //masuk kiri CP2
  if (digitalRead(button5) == 1 && menu == 2 && menuDisp == startKiri) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = kiriCp2;
    lcd.clear();
  }
  if (menuDisp == kiriCp2) {
    opsiMapping = mapKiriCP2;
    majuPID();
    lcd.noDisplay();
  }
  //masuk kiri CP3
  if (digitalRead(button5) == 1 && menu == 3 && menuDisp == startKiri) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = kiriCp3;
    lcd.clear();
  }
  if (menuDisp == kiriCp3) {
    opsiMapping = mapKiriCP3;
    majuPID();
    lcd.noDisplay();
  }
  //masuk kiri CP4
  if (digitalRead(button5) == 1 && menu == 4 && menuDisp == startKiri) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = kiriCp4;
    lcd.clear();
  }
  if (menuDisp == kiriCp4) {
    opsiMapping = mapKiriCP4;
    majuPID();
    lcd.noDisplay();
  }
  //masuk kiri CP5
  if (digitalRead(button5) == 1 && menu == 5 && menuDisp == startKiri) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = kiriCp5;
    lcd.clear();
  }
  if (menuDisp == kiriCp5) {
    opsiMapping = mapKiriCP5;
    majuPID();
    lcd.noDisplay();
  }
  /* Start ke Kiri */

  /* Start ke Kanan */
  if (digitalRead(button5) == 1 && menu == 1 && menuDisp == start) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = startKanan;
    lcd.clear();
  }
  if (menuDisp == startKanan) {
    menu = constrain(menu, 0, 5);
    if (menu == 0) {
      lcd.setCursor(2, 0); lcd.print("START KANAN");
      lcd.setCursor(0, 1); lcd.print(">Full  CP1  CP2");
    }
    if (menu == 1) {
      lcd.setCursor(2, 0); lcd.print("START KANAN");
      lcd.setCursor(0, 1); lcd.print(" Full >CP1  CP2");
    }
    if (menu == 2) {
      lcd.setCursor(2, 0); lcd.print("START KANAN");
      lcd.setCursor(0, 1); lcd.print(" Full  CP1 >CP2");
    }
    if (menu == 3) {
      lcd.setCursor(2, 0); lcd.print("START KANAN");
      lcd.setCursor(0, 1); lcd.print(">CP3  CP4  CP5");
    }
    if (menu == 4) {
      lcd.setCursor(2, 0); lcd.print("START KANAN");
      lcd.setCursor(0, 1); lcd.print(" CP3 >CP4  CP5");
    }
    if (menu == 5) {
      lcd.setCursor(2, 0); lcd.print("START KANAN");
      lcd.setCursor(0, 1); lcd.print(" CP3  CP4 >CP5");
    }
  }
  //masuk Kanan Full
  if (digitalRead(button5) == 1 && menu == 0 && menuDisp == startKanan) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = kananFull;
    lcd.clear();
  }
  if (menuDisp == kananFull) {
    opsiMapping = mapKananFULL;
    majuPID();
    lcd.noDisplay();
  }
  //masuk Kanan CP1
  if (digitalRead(button5) == 1 && menu == 1 && menuDisp == startKanan) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = kananCp1;
    lcd.clear();
  }
  if (menuDisp == kananCp1) {
    opsiMapping = mapKananCP1;
    majuPID();
    lcd.noDisplay();
  }
  //masuk kanan CP2
  if (digitalRead(button5) == 1 && menu == 2 && menuDisp == startKanan) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = kananCp2;
    lcd.clear();
  }
  if (menuDisp == kananCp2) {
    opsiMapping = mapKananCP2;
    majuPID();
    lcd.noDisplay();
  }
  //masuk kanan CP3
  if (digitalRead(button5) == 1 && menu == 3 && menuDisp == startKanan) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = kananCp3;
    lcd.clear();
  }
  if (menuDisp == kananCp3) {
    opsiMapping = mapKananCP3;
    majuPID();
    //    lcd.noDisplay();
    lcdTampilCount();
  }
  //masuk kanan CP4
  if (digitalRead(button5) == 1 && menu == 4 && menuDisp == startKanan) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = kananCp4;
    lcd.clear();
  }
  if (menuDisp == kananCp4) {
    opsiMapping = mapKananCP4;
    majuPID();
    lcd.noDisplay();
  }
  //masuk kanan CP5
  if (digitalRead(button5) == 1 && menu == 5 && menuDisp == startKanan) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = kananCp5;
    lcd.clear();
  }
  if (menuDisp == kananCp5) {
    opsiMapping = mapKananCP5;
    majuPID();
    lcd.noDisplay();
  }
  /* startKanan*/
  /* back kiri ke startKiri*/
  if (digitalRead(button6) == 1 && (menuDisp == kiriFull  ||
                                    menuDisp == kiriCp1   ||
                                    menuDisp == kiriCp2   ||
                                    menuDisp == kiriCp3)) {
    while (digitalRead(button6) == 1);
    menu = 0;
    menuDisp = startKiri;
    lcd.clear();
  }
  /* back kiri ke startKiri*/

  /* back kanan ke startkanan*/
  if (digitalRead(button6) == 1 && (menuDisp == kananFull  ||
                                    menuDisp == kananCp1   ||
                                    menuDisp == kananCp2   ||
                                    menuDisp == kananCp3)) {
    while (digitalRead(button6) == 1);
    menu = 0;
    menuDisp = startKanan;
    lcd.clear();
  }
  /* back kanan ke startkanan*/
  /* back startKanan/Kiri ke menuStart*/
  if (digitalRead(button6) == 1 && (menuDisp == startKanan  ||
                                    menuDisp == startKiri)) {
    while (digitalRead(button6) == 1);
    menu = 0;
    menuDisp = start;
    lcd.clear();
  }
  /* back startKanan/Kiri ke menuStart*/
  /* back start ke mainMenu*/
  if (digitalRead(button6) == 1 && (menuDisp == start)) {
    while (digitalRead(button6) == 1);
    menu = 0;
    menuDisp = mainMenu;
    lcd.clear();
  }
  /* back start ke mainMenu*/
  /* Start ke Kanan */

  /* mainMenu ke Setting */
  if (digitalRead(button5) == 1 && menu == 1 && menuDisp == mainMenu) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = menuSetting;
    lcd.clear();
  }
  if (menuDisp == menuSetting) {
    menu = constrain(menu, 0, 2);
    if (menu == 0) {
      lcd.setCursor(6, 0); lcd.print("SETTING");
      lcd.setCursor(0, 1); lcd.print(">SENS  PID  KEC");
    }
    if (menu == 1) {
      lcd.setCursor(6, 0); lcd.print("SETTING");
      lcd.setCursor(0, 1); lcd.print(" SENS >PID  KEC");
    }
    if (menu == 2) {
      lcd.setCursor(6, 0); lcd.print("SETTING");
      lcd.setCursor(0, 1); lcd.print(" SENS  PID >KEC");
    }
  }
  /* mainMenu ke Setting */

  /* menu Sensor */
  if (digitalRead(button5) == 1 && menu == 0 && menuDisp == menuSetting) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = menuSensor;
    lcd.clear();
  }
  if (menuDisp == menuSensor) {
    menu = constrain(menu, 0, 2);
    if (menu == 0) {
      lcd.setCursor(6, 0); lcd.print("SENSOR");
      lcd.setCursor(0, 1); lcd.print(">ADC  DIG  THRES");
    }
    if (menu == 1) {
      lcd.setCursor(6, 0); lcd.print("SENSOR");
      lcd.setCursor(0, 1); lcd.print(" ADC >DIG  THRES");
    }
    if (menu == 2) {
      lcd.setCursor(6, 0); lcd.print("SENSOR");
      lcd.setCursor(0, 1); lcd.print(" ADC  DIG >THRES");
    }
  }
  //MASUK ADC
  if (digitalRead(button5) == 1 && menu == 0 && menuDisp == menuSensor) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = sensorAnalog;
    lcd.clear();
  }
  if (menuDisp == sensorAnalog) {
    bacaSensor(); bacaTanduk(); bacaSayap();
    menu = constrain(menu, 0, 1);
    // baca ADC sensor Utama
    if (menu == 0) {
      n = 0;
      for (i = 0; i < 4; i++) {
        lcd.setCursor(n, 0); lcd.print(analogRead(sensPinsPID[i]));
        n += 4; if (n > 12)n = 0;
        if (analogRead(sensPinsPID[i]) > 99)
          if (analogRead(sensPinsPID[i]) < 100)lcd.clear();
      }
      for (i = 4; i < 8; i++) {
        lcd.setCursor(n, 1); lcd.print(analogRead(sensPinsPID[i]));
        n += 4; if (n > 12)n = 0;
        if (analogRead(sensPinsPID[i]) > 99)
          if (analogRead(sensPinsPID[i]) < 100)lcd.clear();
      }
    }
    if (menu == 1) {
      n = 4;
      for (i = 0; i < 2; i++) {
        lcd.setCursor(n, 0); lcd.print(analogRead(sensPinsTanduk[i]));
        n += 4; if (n > 8) n = 4;
        if (analogRead(sensPinsTanduk[i]) > 99)
          if (analogRead(sensPinsTanduk[i]) < 100)lcd.clear();
      }
      for (i = 0; i < 2; i++) {
        lcd.setCursor(n, 1); lcd.print(analogRead(sensPinsSayap[i]));
        n += 4; if (n > 8) n = 4;
        if (analogRead(sensPinsSayap[i]) > 99)
          if (analogRead(sensPinsSayap[i]) < 100)lcd.clear();
      }
    }
  }
  if (digitalRead(button5) == 1 && menu == 1 && menuDisp == menuSensor) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = sensorDigital;
    lcd.clear();
  }
  if (menuDisp == sensorDigital) {
    menu = constrain(menu, 0, 0);
    bacaSensor();
    bacaTanduk();
    bacaSayap();
    menuBacaSensor();
  }
  if (digitalRead(button5) == 1 && menu == 2 && menuDisp == menuSensor) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = nilaiThreshold;
    lcd.clear();
  }
  // BUTUH EEPROM
  if (menuDisp == nilaiThreshold) {
    menu = constrain(menu, 0, 0);
    lcd.setCursor(6, 0);
    lcd.print(eeThres);
    //kurangin nilai
    if (digitalRead(button1) == 1) {
      while (digitalRead(button1) == 1);
      eeThres -= 10;
    }
    //tambah Nilai
    if (digitalRead(button2) == 1) {
      while (digitalRead(button2) == 1);
      eeThres += 10;
    }
    //kurangin nilai
    if (digitalRead(button3) == 1) {
      while (digitalRead(button3) == 1);
      eeThres -= 1;
    }
    //tambah Nilai
    if (digitalRead(button4) == 1) {
      while (digitalRead(button4) == 1);
      eeThres += 1;
    }
    //tambah Nilai
    if (digitalRead(button5) == 1) {
      while (digitalRead(button5) == 1);
      //EEPROM simpan nilai eeThres

      lcd.clear();
      EEPROM.update(addrThres, eeThres);
      threshold = eeThres;
      lcd.setCursor(0, 0); lcd.print("Threshold Saved!");
    }
  }

  /* menu Sensor */

  /* back Analog/Dig/Thres ke Sensor*/
  if (digitalRead(button6) == 1 && (menuDisp == sensorAnalog ||
                                    menuDisp == sensorDigital   ||
                                    menuDisp == nilaiThreshold)) {
    while (digitalRead(button6) == 1);
    menu = 0;
    menuDisp = menuSensor;
    lcd.clear();
  }
  /* back Analog/Dig/Thres ke Sensor*/

  /* back menuSetting ke mainMenu*/
  if (digitalRead(button6) == 1 && (menuDisp == menuSetting
                                   )) {
    while (digitalRead(button6) == 1);
    menu = 0;
    menuDisp = mainMenu;
    lcd.clear();
  }
  /* back menuSetting ke mainMenu*/

  /* back menuSensor/PID/Kecepatan ke menuSetting */
  if (digitalRead(button6) == 1 && (menuDisp == menuSensor ||
                                    menuDisp == menuPID   ||
                                    menuDisp == menuKecepatan
                                   )) {
    while (digitalRead(button6) == 1);
    menu = 0;
    menuDisp = menuSetting;
    lcd.clear();
  }
  /* back menuSensor/PID/Kecepatan ke menuSetting */

  /* back Kp/Ki/Kd ke menuPID */
  if (digitalRead(button6) == 1 && (menuDisp == pidKp ||
                                    menuDisp == pidKi   ||
                                    menuDisp == pidKd
                                   )) {
    while (digitalRead(button6) == 1);
    menu = 0;
    menuDisp = menuPID;
    lcd.clear();
  }
  /* back Kp/Ki/Kd ke menuPID */

  /* Menu PID */
  if (digitalRead(button5) == 1 && menu == 1 && menuDisp == menuSetting) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = menuPID;
    lcd.clear();
  }
  // BUTUH EEPROM
  if (menuDisp == menuPID) {
    menu = constrain(menu, 0, 2);
    if (menu == 0) {
      lcd.setCursor(6, 0); lcd.print("PID");
      lcd.setCursor(0, 1); lcd.print(">Kp  Ki  Kd");
    }
    if (menu == 1) {
      lcd.setCursor(6, 0); lcd.print("PID");
      lcd.setCursor(0, 1); lcd.print(" Kp >Ki  Kd");
    }
    if (menu == 2) {
      lcd.setCursor(6, 0); lcd.print("PID");
      lcd.setCursor(0, 1); lcd.print(" Kp  Ki >Kd");
    }
  }
  if (digitalRead(button5) == 1 && menu == 0 && menuDisp == menuPID) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = pidKp;
    lcd.clear();
  }
  // BUTUH EEPROM
  if (menuDisp == pidKp) {
    menu = constrain(menu, 0, 0);
    lcd.setCursor(6, 0); lcd.print("Kp");
    lcd.setCursor(6, 1); lcd.print(Kp);
    //kurangin nilai
    if (digitalRead(button1) == 1) {
      while (digitalRead(button1) == 1);
      Kp -= 1;
    }
    if (digitalRead(button3) == 1) {
      while (digitalRead(button3) == 1);
      Kp -= 0.1;
    }
    //tambah Nilai
    if (digitalRead(button2) == 1) {
      while (digitalRead(button2) == 1);
      Kp += 1;
    }
    if (digitalRead(button4) == 1) {
      while (digitalRead(button4) == 1);
      Kp += 0.1;
    }
    if (digitalRead(button5) == 1) {
      while (digitalRead(button5) == 1);
      //EEPROM simpan nilai Kp
    }
  }
  if (digitalRead(button5) == 1 && menu == 1 && menuDisp == menuPID) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = pidKi;
    lcd.clear();
  }
  // BUTUH EEPROM
  if (menuDisp == pidKi) {
    menu = constrain(menu, 0, 0);
    lcd.setCursor(6, 0); lcd.print("Ki");
    lcd.setCursor(6, 1); lcd.print(Ki);
    //kurangin nilai
    if (digitalRead(button1) == 1) {
      while (digitalRead(button1) == 1);
      Ki -= 1;
    }
    if (digitalRead(button3) == 1) {
      while (digitalRead(button3) == 1);
      Ki -= 0.1;
    }
    //tambah Nilai
    if (digitalRead(button2) == 1) {
      while (digitalRead(button2) == 1);
      Ki += 1;
    }
    if (digitalRead(button4) == 1) {
      while (digitalRead(button4) == 1);
      Ki += 0.1;
    }
    if (digitalRead(button5) == 1) {
      while (digitalRead(button5) == 1);
      //EEPROM simpan nilai Ki
    }
  }
  if (digitalRead(button5) == 1 && menu == 2
      && menuDisp == menuPID) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = pidKd;
    lcd.clear();
  }
  // BUTUH EEPROM
  if (menuDisp == pidKd) {
    menu = constrain(menu, 0, 0);
    lcd.setCursor(6, 0); lcd.print("Kd");
    lcd.setCursor(6, 1); lcd.print(Kd);
    //kurangin nilai
    if (digitalRead(button1) == 1) {
      while (digitalRead(button1) == 1);
      Kd -= 1;
    }
    if (digitalRead(button3) == 1) {
      while (digitalRead(button3) == 1);
      Kd -= 0.1;
    }
    //tambah Nilai
    if (digitalRead(button2) == 1) {
      while (digitalRead(button2) == 1);
      Kd += 1;
    }
    if (digitalRead(button4) == 1) {
      while (digitalRead(button4) == 1);
      Kd += 0.1;
    }
    if (digitalRead(button5) == 1) {
      while (digitalRead(button5) == 1);
      //EEPROM simpan nilai Kd
    }
  }
  /* menu PID */

  /* menu Kecepatan */
  if (digitalRead(button5) == 1 && menu == 2 && menuDisp == menuSetting) {
    while (digitalRead(button5) == 1);
    menu = 0;
    menuDisp = menuKecepatan;
    lcd.clear();
  }
  // BUTUH EEPROM
  if (menuDisp == menuKecepatan) {
    menu = constrain(menu, 0, 0);
    lcd.setCursor(6, 0);
    lcd.print(eeKecepatan);
    //kurangin nilai
    if (digitalRead(button1) == 1) {
      while (digitalRead(button1) == 1);
      eeKecepatan -= 1;
    }
    if (digitalRead(button3) == 1) {
      while (digitalRead(button3) == 1);
      eeKecepatan -= 0.1;
    }
    //tambah Nilai
    if (digitalRead(button2) == 1) {
      while (digitalRead(button2) == 1);
      eeKecepatan += 1;
    }
    if (digitalRead(button4) == 1) {
      while (digitalRead(button4) == 1);
      eeKecepatan += 0.1;
    }
    if (digitalRead(button5) == 1) {
      while (digitalRead(button5) == 1);
      //EEPROM simpan nilai kecepatan

      lcd.clear();
      EEPROM.update(addrKecepatan, eeKecepatan);
      kecepatan = eeKecepatan;
      lcd.setCursor(0, 0); lcd.print("Kecepatan Saved!");
    }
  }
}

void lcdTampilCount() {

  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print(count);
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
