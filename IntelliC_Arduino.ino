#include <Servo.h>

#define PIN_SERVO 10

Servo myservo;

int trigPin = 8;
int echoPin = 7;
float v = 331.5 + 0.6 * 15; // m/s

int pos = 0;
int distance = 100;
int delta = 10;
int delayTime = 500;

bool hasFind = false;

float distanceM() {
  // send sound pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(3);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin,LOW);

  // listen for echo
  float tUs = pulseIn(echoPin, HIGH);   // us
  float t = tUs / 1000.0 / 1000.0 / 2;  // s
  float d = t * v;                      // m
  return d * 100;                       // cm
}

void spinningLog(int pos, int distance) {
  Serial.println("===== SPINNING LOG =====");
  Serial.println(pos, DEC);
  Serial.println(distance, DEC);
  Serial.println("========================");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myservo.attach(PIN_SERVO);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (hasFind) {
    return;
  } else {
    for (int i=0; i<180; i+=delta) {
      pos = i;
      distance = distanceM();
      myservo.write(pos);
      spinningLog(pos, distance);
      if (distance <= 15) {
        Serial.println("##### DIST ACHIEVED! (A) #####");
        hasFind = true;
        pos += 60;
        myservo.write(pos);
        spinningLog(pos, distance);
        Serial.println("##############################");
        delay(delayTime);
        return;
      }
      delay(delayTime);
    }

    for (int i=180; i>0; i-=delta) {
      pos = i;
      distance = distanceM();
      myservo.write(pos);
      spinningLog(pos, distance);
      if (distance <= 15) {
        Serial.println("##### DIST ACHIEVED! (B) #####");
        hasFind = true;
        pos += 60;
        myservo.write(pos);
        spinningLog(pos, distance);
        Serial.println("##############################");
        delay(delayTime);
        return;
      }
      delay(delayTime);
    }
  }
}
