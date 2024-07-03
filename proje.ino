#include <LiquidCrystal.h>

#define REDPin 2
#define GREENPin 3

#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define E 8
#define RS 9

#define trigPin 10
#define echoPin 11

#define buzzerPin 12
#define magneticSensorPin 13

#define max 400
#define min 2
#define speedOfSoundCmPerMicrosecond 29

void alarm();
void triggerUltrasonicSensor();
void calculateDistance();

int duration = 0;
int distance = 0;
bool state = 1;

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);


void setup() {
  Serial.begin(9600);

  pinMode(magneticSensorPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(REDPin, OUTPUT);
  digitalWrite(REDPin, HIGH);
  pinMode(GREENPin, OUTPUT);
  digitalWrite(GREENPin, HIGH);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, HIGH);
  Serial.println("Pins set.");
  
  lcd.begin(16, 2);
  lcd.print("Distance:           ");

  Serial.println("Ready!");
}

void loop() {
  state = digitalRead(magneticSensorPin);
  if (!state) {
    Serial.println("Door open");
    alarm();
    triggerUltrasonicSensor();
    calculateDistance();

    if (distance>max || distance<min) {
      lcd.setCursor(0, 1);
      lcd.print("Out of Range    ");
      Serial.println("Out of Range");
    }
    else {
      lcd.setCursor(0, 1);
      lcd.print(distance);
      lcd.print(" cm             ");
      Serial.print("Distance: ");
      Serial.println(distance);
    }
  }
  else {
    // Reset
    digitalWrite(REDPin, HIGH);
    digitalWrite(GREENPin, LOW);
    noTone(buzzerPin);
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }
  
  delay(1000);
}


void alarm() {
  digitalWrite(REDPin, LOW);
  digitalWrite(GREENPin, HIGH);
  tone(buzzerPin, 1760);
}

void triggerUltrasonicSensor() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
}

void calculateDistance() {
  distance = duration/speedOfSoundCmPerMicrosecond ;
  distance = distance/2;
}
