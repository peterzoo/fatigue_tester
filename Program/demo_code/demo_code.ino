//------------------------------------------
// These parameters can be changed prior to 
// uploading code for testing mats
// -----------------------------------------
int testSpeed = 255; // enter value between 0 and 255
int testCycles = 3; // enter total cycles for testing
// ------------------------------------------
// MAT TESTER DEMO/LOW LOAD CODE, 100% duty cycle
// ------------------------------------------

// include & libraries
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// global variables
int count = 0; // dont change this
int cumulativeCycles = 1; // dont change this

// motor driver pins
const int R_EN = 3;
const int L_EN = 4;
const int R_PWM = 5;
const int L_PWM = 6;

// limit switches
const int limit1 = A0;
const int limit2 = A1;

//buttons
const int startButton = A2;
const int stopButton = A3;
const int resetButton = 7;

// logic constants
const bool pressed = LOW;
const bool released = HIGH;

// timer
unsigned long startTime = 0;
unsigned long duration = 0;

// soft reset queue
bool finished = false;

// function prototypes
void calibrate();
void calibrateMessage();
void forward(int speed);
void backward(int speed);
void stop();
void checkPause();
void test();
void complete();

// -------------------------------
// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// -------------------------------
void setup() 
{
Serial.begin(9600); // Don't forget this bruh

delay(200); //try to prevent cursed text from spawning
lcd.init();      // initialize the lcd 
lcd.backlight(); // Turn on  Backlight
lcd.clear();

 //pinModes
pinMode(R_EN, OUTPUT);
pinMode(R_PWM, OUTPUT);
pinMode(L_EN, OUTPUT);
pinMode(L_PWM, OUTPUT);
digitalWrite(R_EN, HIGH);
digitalWrite(L_EN, HIGH);

pinMode(limit1, INPUT_PULLUP);
pinMode(limit2, INPUT_PULLUP);
pinMode(startButton, INPUT_PULLUP);
pinMode(stopButton, INPUT_PULLUP);
pinMode(resetButton, INPUT_PULLUP);
}

void loop() 
{
finished = false; // soft reset queue

calibrate(); // calibrate actuator position

test(); // testing using parameters

complete(); // finished screen
}

//------------
// functions
//------------

void calibrate()
{
  calibrateMessage();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("calibrating...");

  while(digitalRead(startButton) == released)
  {}

  while(digitalRead(limit1) == released)
  {
    backward(255);
  }
  
  stop();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("load weight and");
  lcd.setCursor(0,1);
  lcd.print("press start (>)");
  while(digitalRead(startButton) == released)
  {}
}

void calibrateMessage()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("press start (>)");
  lcd.setCursor(0,1);
  lcd.print("to calibrate");

  while(digitalRead(startButton) == released)
  {}
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

void checkPause()
{
  if(digitalRead(stopButton) == pressed)
    {
    stop();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("test paused");
    lcd.setCursor(0,1);
    lcd.print("resume with (>)");

    while(digitalRead(startButton) != pressed)
    {}
  }
}

void test()
{
  while (finished == false)
  {
    for(int i=0; i < testCycles; i++)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("test cycles: ");
        lcd.setCursor(0,1);
        lcd.print(count + 1);
        lcd.setCursor(4,1);
        lcd.print("/");
        lcd.setCursor(6,1);
        lcd.print(testCycles*cumulativeCycles);

        while(digitalRead(limit2) != pressed)
        {
          checkPause();
          forward(testSpeed);
        }

        while(digitalRead(limit1) != pressed)
        {
          checkPause();
          backward(testSpeed);
        }
        count++;
      }
      stop();
      finished = true;
  }
}

void complete()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("test complete");
  lcd.setCursor(0,1);
  lcd.print("total: ");
  lcd.setCursor(8, 1);
  lcd.print(testCycles*cumulativeCycles);

  while(finished == true)
  {
    lcd.backlight();
    delay(1500);
    lcd.noBacklight();
    delay(1500);

    if (digitalRead(resetButton) == pressed)
    {
      finished = false;
    }
  }
  lcd.backlight();
  cumulativeCycles += 1; // increment cumulative cycle counter
}