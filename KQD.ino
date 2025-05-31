#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trigPin = D5;
const int echoPin = D6;
const int potPin = A0;
const int limitSwitchPin = D3;

const int potThreshold = 10;
const int tankMinHeight = 10;
int tankMaxHeight = 250;

void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(limitSwitchPin, INPUT_PULLUP);
  
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("Tank Level KQD");
  delay(1000);
}

long measureDistanceCM() {
  const int samples = 5;
  long readings[samples];
  
  for (int i = 0; i < samples; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000);
    readings[i] = (duration == 0) ? -1 : (duration * 0.034 / 2);
    delay(20);
  }

  for (int i = 0; i < samples-1; i++) {
    for (int j = i+1; j < samples; j++) {
      if (readings[j] < readings[i]) {
        long temp = readings[i];
        readings[i] = readings[j];
        readings[j] = temp;
      }
    }
  }
  return readings[samples/2];
}

void calibrateTank() {
  lcd.clear();
  lcd.print("Calibrating...");
  
  long measuredHeight = measureDistanceCM();
  if (measuredHeight == -1) {
    lcd.clear();
    lcd.print("Sensor Error!");
    delay(2000);
    return;
  }

  tankMaxHeight = measuredHeight;
  lcd.clear();
  lcd.print("Calibrated!");
  lcd.setCursor(0, 1);
  lcd.print("Max: ");
  lcd.print(tankMaxHeight);
  lcd.print("cm");
  delay(2000);
}

void loop() {
  int potValue = analogRead(potPin);
  long distance = measureDistanceCM();

  if (potValue >= potThreshold && 
      map(potValue, potThreshold, 1023, tankMinHeight, tankMaxHeight) == tankMaxHeight &&
      digitalRead(limitSwitchPin) == LOW) {
    calibrateTank();
    delay(500);
  }

  lcd.clear();

  if (distance == -1) {
    lcd.setCursor(0, 0);
    lcd.print("No sensor data");
    lcd.setCursor(0, 1);
    lcd.print("Check wiring");
  } 
  else if (potValue < potThreshold) {
    lcd.setCursor(0, 0);
    lcd.print("Normal Mode KQD");
    lcd.setCursor(0, 1);
    lcd.print("Dist: ");
    lcd.print(distance);
    lcd.print(" cm");
  } 
  else {
    int tankHeight = map(potValue, potThreshold, 1023, tankMinHeight, tankMaxHeight);
    tankHeight = constrain(tankHeight, tankMinHeight, tankMaxHeight);

    if (distance > tankHeight) distance = tankHeight;
    int waterLevel = tankHeight - distance;
    waterLevel = constrain(waterLevel, 0, tankHeight);
    int percentFull = (waterLevel * 100) / tankHeight;

    lcd.setCursor(0, 0);
    lcd.print("Tank: ");
    lcd.print(tankHeight);
    lcd.print("cm KQD");

    lcd.setCursor(0, 1);
    lcd.print("Lvl: ");
    lcd.print(waterLevel);
    lcd.print("cm (");
    lcd.print(percentFull);
    lcd.print("%)");

    if (percentFull < 10) lcd.print(" LOW!");
    else if (percentFull > 90) lcd.print(" FULL!");
  }

  delay(1000);
}