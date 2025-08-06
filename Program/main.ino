// include & libraries
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 
  // Initialize the LCD
  lcd.init(); 
  // Turn on the backlight
  lcd.backlight();
  // Set the cursor to column 0, row 0 (first position on the first line)
  lcd.setCursor(0, 0);
  // Print a message to the LCD
  lcd.print("Hello, World!");
  
// motor driver pins
const int R_EN = 3;
const int L_EN = 4;
const int R_PWM = 5;
const int L_PWM = 6;

// limit switches
const int limit1 = A0;
const int limit2 = A1;

//button
const int button = A2;

// logic constants
const bool pressed = LOW;
const bool released = HIGH;


// global variables
int testSpeed = 255; // enter value between 0 and 255
int testCycles = 3; // enter total cycles for testing
int count = 0;

// timer
bool isPaused = false;

// function prototypes
void start(String text);
void calibrate();
void forward(int speed);
void backward(int speed);
void stop();
void checkPause();

void setup() 
{
Serial.begin(9600); // Don't forget this bruh


 //pinModes
pinMode(R_EN, OUTPUT);
pinMode(R_PWM, OUTPUT);
pinMode(L_EN, OUTPUT);
pinMode(L_PWM, OUTPUT);
digitalWrite(R_EN, HIGH);
digitalWrite(L_EN, HIGH);

pinMode(limit1, INPUT_PULLUP);
pinMode(limit2, INPUT_PULLUP);
pinMode(button, INPUT_PULLUP);

pinMode(LED_BUILTIN, OUTPUT);

start("press start to calibrate"); //press to calibrate
calibrate(); //calibrate
start("press start to begin test"); //start test

  for(int i=0; i < testCycles + 1; i++)
  {
    Serial.println();
    Serial.print("test cycles: ");
    Serial.print(count);
    Serial.print("/");
    Serial.print(testCycles);

    while(digitalRead(limit2) == released)
    {
      checkPause();
      forward(testSpeed);
    }

    while(digitalRead(limit1) == released)
    {
      checkPause();
      backward(testSpeed);
    }

    count++;
  }
  stop();
  
  Serial.println();
  Serial.println();
  Serial.print("test complete");
  Serial.println();
  Serial.print("total cycles: ");
  Serial.print(count - 1);
}

void loop() 
{
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(500);                      // wait for a second
}

//------------
// functions
//------------
void start(String text)
{
  Serial.println();
  Serial.print(text);
  while(digitalRead(button) == released)
  {}
}

void calibrate()
{
  while(digitalRead(limit1) == released)
  {
    backward(255);
  }
    analogWrite(R_PWM, 0);
    analogWrite(L_PWM, 0);
}

void forward(int speed)
{
  analogWrite(R_PWM, 0);
  analogWrite(L_PWM, speed);
  delay(1);
}

void backward(int speed)
{
  analogWrite(R_PWM, speed);
  analogWrite(L_PWM, 0);
  delay(1);
}

void stop()
{
  analogWrite(R_PWM, 0);
  analogWrite(L_PWM, 0);
}

void checkPause() {
  static bool lastButtonState = released;
  static unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 50;

  bool currentButtonState = digitalRead(button);

  if (currentButtonState != lastButtonState) {
    lastDebounceTime = millis(); // reset debounce timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentButtonState == pressed && lastButtonState == released) {
      isPaused = !isPaused;
      Serial.println(isPaused ? "Paused" : "Resumed");
      stop(); // always stop motor on pause toggle
    }
  }

  lastButtonState = currentButtonState;

  // If paused, loop until resumed
  while (isPaused) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);

    currentButtonState = digitalRead(button);
    if (currentButtonState != lastButtonState) {
      lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (currentButtonState == pressed && lastButtonState == released) {
        isPaused = false;
        Serial.println("Resumed from pause");
        stop(); // optional: reset motor
      }
    }

    lastButtonState = currentButtonState;
  }
}
/*
void checkPause()
{
  static bool lastState = released;
  static unsigned long lastDebounce = 0;
  const unsigned long debounce = 50;

  bool reading = digitalRead(button);

  if(reading != lastState)
    lastDebounce = millis();

  if((millis() - lastDebounce) > debounce && reading == pressed && lastState == released)
  {
    isPaused = !isPaused;
    stop();
    Serial.println(isPaused ? "Paused" : "Resumed");
  }
  lastState = reading;

  while(isPaused == true)
  {
    digitalWrite(LED_BUILTIN, HIGH); // Optional visual
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);

    reading = digitalRead(button);

    if(reading != lastState)
      lastDebounce = millis();

    if ((millis() - lastDebounce) > debounce && reading == pressed && lastState == released)
    {
      isPaused = false;
      Serial.println("Resumed from pause");
    }
    lastState = reading;
  }
}
*/