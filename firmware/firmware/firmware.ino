#include <Wire.h>
#include <LiquidCrystal_I2C.h>

bool f = false;                              // (unused, kept as in your code)
LiquidCrystal_I2C lcd(0x3F, 16, 2);          // LCD I2C address 0x3F, 16x2

void setup() {
  pinMode(2, INPUT);                         // LO+ from AD8232 (electrode off detect)
  pinMode(3, INPUT);                         // LO- from AD8232
  pinMode(9, OUTPUT);                        // Buzzer
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
}

void loop() {
  int Time = millis();                       // (unused, kept as in your code)

  // If either electrode is off, signal (!) and send marker 350 over serial
  if ((digitalRead(2) == 1) || (digitalRead(3) == 1)) {
    Serial.println(350);
    lcd.clear();
    lcd.print("!");
    delay(200);
    tone(9, 1500);
  } else {
    noTone(9);
    int a = analogRead(A0);                  // ECG analog input
    Serial.println(a);

    // Beep while the signal is above threshold
    while (a >= 380) {
      tone(9, 1500);
      a = analogRead(A0);
    }

    noTone(9);
    lcd.setCursor(0, 0);
    lcd.print("Analyzing...");
    // delay(200);

    // When MATLAB sends BPM back over serial, show it
    if (Serial.available() > 0) {
      lcd.setCursor(0, 1);
      lcd.print("Average BPM:");
      lcd.setCursor(14, 1);
      lcd.print(Serial.readString());
      delay(500);
    }

    lcd.clear();
  }
}
