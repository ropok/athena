<<<<<<< HEAD
/* testing motor
  looping : forward - backward - right - left - stop

*/

int i;
/* motor */
int motor_left[] = {11, 10};
int motor_right[] = {12, 13};

/* setup */
void setup() {
  Serial.begin(9600);

  // Setup motors
=======
/* Program pertama untuk uji coba LF
  looping : forward - backward - left - right - stop
  with - : delay
*/

/* Motor */
int motor_left[] = {11, 10};
int motor_right[] = {12, 13};
int i;

/* Initialize - Setup */
void setup() {
  Serial.begin(9600);

  //Setup Motor
>>>>>>> dev-jalerse
  for (i = 0; i < 2; i++) {
    pinMode(motor_left[i], OUTPUT);
    pinMode(motor_right[i], OUTPUT);
  }

}

<<<<<<< HEAD
/* loop */
=======
/* Looping begin */
>>>>>>> dev-jalerse
void loop() {

  drive_forward();
  delay(1000);
  motor_stop();
  Serial.println("1");

  drive_backward();
  delay(1000);
  motor_stop();
  Serial.println("2");

  turn_left();
  delay(500);
  motor_stop();
  Serial.println("3");

  turn_right();
  delay(500);
  motor_stop();
  Serial.println("4");

  motor_stop();
<<<<<<< HEAD
  delay(1000);
=======
  delay(1000 );
>>>>>>> dev-jalerse
  motor_stop();
  Serial.println("5");
}

<<<<<<< HEAD

/* fungsi */
=======
/* Void stop, forward, backward, left, right */

>>>>>>> dev-jalerse
void motor_stop() {
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_left[1], LOW);

  digitalWrite(motor_right[0], LOW);
  digitalWrite(motor_right[1], LOW);
  delay(25);
}

void drive_forward() {
  analogWrite(motor_left[0], 255);
  analogWrite(motor_left[1], 0);

  analogWrite(motor_right[0], 255);
  analogWrite(motor_right[1], 0);
}

void drive_backward() {
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_left[1], HIGH);

  digitalWrite(motor_right[0], LOW);
  digitalWrite(motor_right[1], HIGH);
}

void turn_left() {
  analogWrite(motor_left[0], 0);
  analogWrite(motor_left[1], 255);

  analogWrite(motor_right[0], 255);
  analogWrite(motor_right[1], 0);
}

void turn_right() {
  digitalWrite(motor_left[0], HIGH);
  digitalWrite(motor_left[1], LOW);

  digitalWrite(motor_right[0], LOW);
  digitalWrite(motor_right[1], HIGH);
}

