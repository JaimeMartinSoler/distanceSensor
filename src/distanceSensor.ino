/*
    // ----------------------------------------------------------------------
    // --- distanceSensor.ino -----------------------------------------------
    // ----------------------------------------------------------------------
    // --- Distance Sensor Detector, suited for cars, etc -------------------
    // --- Author: Jaime Martin Soler ---------------------------------------
    // --- Date  : 31/08/2016 -----------------------------------------------
    // ----------------------------------------------------------------------
*/
 
// INCLUDES -------------------------------------------------------------
#include <LiquidCrystal.h>
#include <C:\Users\Jaime\Documents\Arduino\test\distanceSensor\Ultrasonic.h>
#include <C:\Users\Jaime\Documents\Arduino\test\distanceSensor\signalFunction.h>




// PARAMETERS -----------------------------------------------------------
 
// PINs: Ultrasonic
//const int PIN_ULTRAS_GND = GND;
const int PIN_ULTRAS_ECH = 13;
const int PIN_ULTRAS_TRI = 12;
const int PIN_ULTRAS_VCC = 8;   // BROKEN, ALWAYS VCC
// PINs: Buzzer
const int PIN_BUZZER_IN = 10;
// PINs: LED
const int PIN_LED_IN = 9; // PWM
// PINs: Button
const int PIN_BUTTON_IN = 11;
// PINs: LCD
const int PIN_LCD_RS = 7;
const int PIN_LCD_EN = 6;
const int PIN_LCD_D4 = 5;
const int PIN_LCD_D5 = 4;
const int PIN_LCD_D6 = 3;
const int PIN_LCD_D7 = 2;

// LCD properties
const int LCD_ROWS = 2;
const int LCD_COLS = 16;

// button parameters
int button_last = LOW;

// centimeters measured
const int rangesSize = 4;
const short int ranges[rangesSize]    = {10, 30, 100, 200};  // cm
const short int rangeOffset = 1;                             // cm
const short int periods[rangesSize-1] = {200, 600, 1500};    // ms
int rangeCurrent = -1; // ranges idx [0,rangesSize]
int cm;                // cm measured

// time
const unsigned long loopTime = 20;          // ms
const unsigned long LCDrefreshTime = 200;   // ms
unsigned long LCDrefreshLast = 0;           // ms

// objects
LiquidCrystal lcd = LiquidCrystal(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);
Ultrasonic us = Ultrasonic(PIN_ULTRAS_TRI, PIN_ULTRAS_ECH); // (Trig PIN,Echo PIN)
SignalFunction sfBuzzer = SignalFunction();
SignalFunction sfLED = SignalFunction();




// SETUP ----------------------------------------------------------------
void setup() {
  
  //Serial.begin(9600);
          
  // set LCD
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.setCursor(0, 0);
  lcd.print("DISTNACE");
  lcd.setCursor(0, 1);
  lcd.print("SENSOR");
  lcd.display();
  delay(1000);
  
  // set Ultrasonic VCC, GND pins
  pinMode(PIN_ULTRAS_VCC, OUTPUT);
  digitalWrite(PIN_ULTRAS_VCC, HIGH);
    //pinMode(PIN_ULTRAS_GND, OUTPUT);
    //digitalWrite(PIN_ULTRAS_GND, LOW);
          
  // set sfBuzzer
  int buz_enable = ENABLE_NORMAL;
  int buz_type = TYPE_DIGITAL;
  int buz_period = 1000;
  const int buz_VTsize = 3;
  short int buz_Vn[buz_VTsize] = {  0, VnMAX, 0};
  short int buz_Tn[buz_VTsize] = {0, 60,    80};
  sfBuzzer.set(buz_enable, PIN_BUZZER_IN, buz_type, buz_period, buz_Vn, buz_Tn, buz_VTsize);
  
  // set sfLED
  int led_enable = ENABLE_NORMAL;
  int led_type = TYPE_ANALOG;
  int led_period = 1000;
  const int led_VTsize = 3;
  short int led_Vn[led_VTsize] = {0,  0, VnMAX};
  short int led_Tn[led_VTsize] = {0, 60,    80};
  sfLED.set(led_enable, PIN_LED_IN, led_type, led_period, led_Vn, led_Tn, led_VTsize);
  
  // set button pin
  pinMode(PIN_BUTTON_IN, INPUT);
}




// LOOP -----------------------------------------------------------------
void loop()
{
  // t0 for delay
  unsigned long t0 = millis();

  // check button pause
  int button_current = digitalRead(PIN_BUTTON_IN);
  if (button_last==LOW && button_current==HIGH) {
    button_last=HIGH;
  } else if (button_last==HIGH && button_current==LOW) {
    // pause: manage outputs
    sfBuzzer.enable = ENABLE_ALWAYS_LOW;
    sfLED.enable = ENABLE_ALWAYS_LOW;
    sfBuzzer.refresh();
    sfLED.refresh();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PAUSED");
    // pause: manage states
    button_last=LOW;
    while(true) {
      delay(5);
      button_current = digitalRead(PIN_BUTTON_IN);
      if (button_last==LOW && button_current==HIGH) {
        button_last=HIGH;
      } else if (button_last==HIGH && button_current==LOW) {
        // exit pause
        button_last=LOW;
        rangeCurrent=-1;  // force to update rangeCurrent
        break;
  } } }
  
  // get cm
  cm = us.Ranging(CM); // CM or INC

  // get rangeCurrent, ranges[], periods[]
  if (cm<=ranges[0]) {
    if ((rangeCurrent<0) || (rangeCurrent>0 && abs(cm-ranges[0])>=rangeOffset)) {
      rangeCurrent = 0;
      sfBuzzer.enable = ENABLE_ALWAYS_HIGH;
      sfLED.enable = ENABLE_ALWAYS_HIGH;
    }
  } else if (cm>ranges[rangesSize-1]) {
    if ((rangeCurrent<0) || (rangeCurrent<rangesSize && abs(cm-ranges[rangesSize-1])>=rangeOffset)) {
      rangeCurrent = rangesSize;
      sfBuzzer.enable = ENABLE_ALWAYS_LOW;
      sfLED.enable = ENABLE_ALWAYS_LOW;
    }
  } else {
    for (int i=1; i<rangesSize; i++) {
      if (cm>ranges[i-1] && cm<=ranges[i]) {
        if ((rangeCurrent<0) || (rangeCurrent<i && abs(cm-ranges[i])>=rangeOffset) || (rangeCurrent>i && abs(cm-ranges[i])>=rangeOffset)) {
            rangeCurrent = i;
            sfBuzzer.enable = ENABLE_NORMAL;
            sfLED.enable = ENABLE_NORMAL;
            sfBuzzer.period = periods[i-1];
            sfLED.period = periods[i-1];
            break;
  } } } }

  // refresh signalFunctions
  sfBuzzer.refresh();
  sfLED.refresh();

  // refresh LCD
  if ((t0-LCDrefreshLast)>=LCDrefreshTime) {
    LCDrefreshLast = t0;
    String line0(""); line0+="Dist : " ; line0+=cm ; line0+=" cm";
    String line1(""); line1+="Range: " ; line1+=rangeCurrent;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line0);
    lcd.setCursor(0, 1);
    lcd.print(line1);
  }
  
  // delay
  unsigned long tProcess = millis() - t0;
  if (tProcess<loopTime) {
    delay(loopTime-tProcess);
  }

  //Serial.print(tProcess); Serial.println(" us" );
  //Serial.println();
}

