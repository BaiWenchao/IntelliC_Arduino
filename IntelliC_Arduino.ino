#include <Servo.h>

#define PIN_SERVO 10
#define sensorPin A0
#define pressureThreshold 450

Servo myservo;

int trigPin = 8;
int echoPin = 7;
int beePin = 3;
float v = 331.5 + 0.6 * 15; // m/s

int pos = 0;
int distance = 100;
int delta = 10;
int delayTime = 500;

bool hasFind = false;
bool hasBee = false;

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

bool hasCup() {
  return analogRead(sensorPin) > pressureThreshold ? true : false;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(beePin, OUTPUT);
  pinMode(echoPin, INPUT);
  myservo.attach(PIN_SERVO);

  digitalWrite(beePin, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!hasCup()) {
    Serial.println("LEAVE!");
    myservo.write(0);
    hasFind = false;
    hasBee = false;
    return;
  } else {
    if (hasFind) {
      Serial.println("Counting!");
      if (hasBee) {
        return;
      }
      bool flag = true;
      for (int i=0; i<10; i++) {
        if (!hasCup()) {
          flag = false;
        }
        delay(500);
      }
      if (flag) {
        digitalWrite(beePin, LOW);
        delay(1000);
        digitalWrite(beePin, HIGH);
        hasBee = true;
        delay(1000);
      }
    } else {
      Serial.println("Searching!");
      for (int i=0; i<180; i+=delta) {
        if (!hasCup()) return;
        Serial.println(analogRead(sensorPin));
        pos = i;
        distance = distanceM();
        myservo.write(pos);
        if (distance <= 15) {
          hasFind = true;
          pos += 60;
          myservo.write(pos);
          delay(delayTime);
          return;
        }
        delay(delayTime);
      }
  
      for (int i=180; i>0; i-=delta) {
        if (!hasCup()) return;
        Serial.println(analogRead(sensorPin));
        pos = i;
        distance = distanceM();
        myservo.write(pos);
  
        if (distance <= 15) {
          hasFind = true;
          pos += 60;
          myservo.write(pos);
          delay(delayTime);
          return;
        }
        delay(delayTime);
      }
    }
  }
}
