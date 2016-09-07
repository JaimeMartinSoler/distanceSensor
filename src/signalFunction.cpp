/*
    // ----------------------------------------------------------------------
    // --- signalFunction.c -------------------------------------------------
    // ----------------------------------------------------------------------
    // --- Library for easy custom signal design for LEDs, Buzzers, etc -----
    // --- Author: Jaime Martin Soler ---------------------------------------
    // --- Date  : 31/08/2016 -----------------------------------------------
    // ----------------------------------------------------------------------
*/


#include "signalFunction.h"


// CONSTRUCTORS ---------------------------------------------------------

// Constructor: Default
SignalFunction::SignalFunction()
{
  // set parameters
  this->enable = ENABLE_ALWAYS_LOW; // = 0
  this->pin = -1;
  this->type = TYPE_DIGITAL;        // = false
  this->period = -1;
  this->VTsize = -1;
}

// Constructor: Main
SignalFunction::SignalFunction(int enable, int pin, int type, int period, short int Vn[], short int Tn[], int VTsize)
{
  set(enable, pin, type, period, Vn, Tn, VTsize);
}

// Destructor
SignalFunction::~SignalFunction()
{
  delete []Vn;
  delete []Tn;
}




// FUNCTIONS ------------------------------------------------------------

// set(int,int,int,int,short int[],short int[],int): sets enable, pin, period, Vn[], Tn[], VTsize
void SignalFunction::set(int enable, int pin, int type, int period, short int Vn[], short int Tn[], int VTsize) 
{
  // set parameters
  this->enable = enable;
  this->pin = pin;
  this->type = type;
  this->period = period;
  this->VTsize = VTsize;
  this->Vn = new short int[this->VTsize];
  this->Tn = new short int[this->VTsize];
  for(int i=0; i<this->VTsize; i++) {
    this->Vn[i]=Vn[i];
    this->Tn[i]=Tn[i];
  }
  // set pin
  pinMode(this->pin, OUTPUT);
}


// refresh: calls refresh(int delayMs=0)
void SignalFunction::refresh()
{
  SignalFunction::refresh(0);
}


// refresh(int delayMs): delay(delayMs) and refresh the output signal value
// executionTime=220-240us (in Arduino UNO, if delayMs=0) 
void SignalFunction::refresh(int delayMs)
{
  // add delay
  if (delayMs > 0) {
    delay(delayMs);
  }
  
  // check enable
  if(enable==ENABLE_ALWAYS_LOW) {
    digitalWrite(pin, LOW);
    return;
  } else if (enable==ENABLE_ALWAYS_HIGH) {
    digitalWrite(pin, HIGH);
    return;
  }
  
  // local variables
  int p;
  short int vn,tn,vt0,vt1;
  
  // get p, tn
  unsigned long millisCurrent = millis();
  static unsigned long millis_offset = millisCurrent; // initialized just once
  p = (millisCurrent-millis_offset)%period;
  tn = map(p, 0, period, TnMIN, TnMAX);

  // get vt0 ,vt1
  for (short int i=VTsize-1; i>=0; i--) {
    if (tn >= Tn[i]) {
      vt0 = i;
      vt1 = vt0 + 1;
      if (vt1 >= VTsize) { vt1 = 0; }
      break;
    }
  }
  
  // if (type==TYPE_ANALOG): get vn, and write vn in analog pin
  if (type==TYPE_ANALOG) {
    if (vt1 > 0) {
      vn = map(tn, Tn[vt0], Tn[vt1], Vn[vt0], Vn[vt1]);
    } else {
      vn = map(tn, Tn[vt0],   TnMAX, Vn[vt0], Vn[vt1]);
    }
    analogWrite(pin, map(vn, VnMIN, VnMAX, 0, 255));
  }
  
  // if (type==TYPE_DIGITAL): get vn, and write vn in digital pin
  else if (type==TYPE_DIGITAL) {
    vn = Vn[vt0];
    if (vn<=VnMIN) {
      digitalWrite(pin, LOW);
    } else {
      digitalWrite(pin, HIGH);
    }
  }
}

