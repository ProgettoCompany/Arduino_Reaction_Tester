#include <Wire.h> 
// Liquid Crystal Library can be found here: https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
#include <LiquidCrystal_I2C.h>
#include <String.h>

#define DEBOUNCE_TIME_MS 100
#define BUTTON_PIN 12
#define LED_PIN 13

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Store the last time the button was pressed, used for debouncing
long lastPressTime = 0;

// Store the current time, retrieved using millis()
long curTime;

// Stores whether the reaction test has started, meaning the LED has been turned on
bool testStarted = false;

// Stores the user's calculated reaction time, in milliseconds
long reactionTime;

// Stores the time the test was started
long testStartTime;

void setup() {
  // put your setup code here, to run once:
  // Initialize the I/O pins
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  // Initialize the random number generator
  randomSeed(analogRead(A0));

  // Initialize the LCD, print the start message
  lcd.begin();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Reaction Tester,");
  lcd.setCursor(0, 1);
  lcd.print("Press to Start");
}

void loop() {
  // put your main code here, to run repeatedly:
  curTime = millis();
  
  // Check if the button has been pressed, ensuring enough time has passed to debounce the button
  if (digitalRead(BUTTON_PIN) == HIGH && curTime - lastPressTime > DEBOUNCE_TIME_MS) {
    lastPressTime = curTime;

    // Check if the reaction test has started
    if (testStarted == true) {
      // If it has, turn off the LED, and use the current time and test start time to calculate
      // the user's reaction time, then print it to the LCD
      digitalWrite(LED_PIN, LOW);
      curTime = millis();
      reactionTime = curTime - testStartTime;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Reaction Time:");
      lcd.setCursor(0, 1);
      String reactionTimeStr = String(reactionTime) + "ms";
      lcd.print(reactionTimeStr);
      delay(3000);

      // Reset testStarted, and print the start message to the LCD
      testStarted = false;
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Reaction Tester,");
      lcd.setCursor(0, 1);
      lcd.print("Press to Start");
    }
    else {
      // If it hasn't, print to the LCD, delay 1 to 3 seconds, turn on the LED,
      // store the testStartTime, and update testStarted
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Starting Test...");
      long randDelay = random(1000, 3001);
      delay(randDelay);
      digitalWrite(LED_PIN, HIGH);
      testStartTime = millis();
      testStarted = true;
    }
  }
}
