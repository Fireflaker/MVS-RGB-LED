/* fade w/o pwm

  Fades an LED that is not connected to a PWM pin
  This was modified from the Blink without Delay code

  The circuit:
   LED attached from pin 13 to ground.
   Note: on most Arduinos, there is already an LED on the board
  that's attached to pin 13, so no hardware is needed for this example.

*/

// constants won't change. Used here to  set pin numbers:
const int b1 =  5;      // the number of the LED pin
const int g1 =  4;
const int r1 =  6;

const int b2 =  7;      // the number of the LED pin
const int g2 =  8;
const int r2 =  9;

const int b3 =  10;      // the number of the LED pin
const int g3 =  11;
const int r3 =  12;

#include <ClickEncoder.h>

// ----------------------------------------------------------------------------
// Button configuration (values for 1ms timer service calls)
//
#define ENC_BUTTONINTERVAL    10  // check button every x milliseconds, also debouce time
#define ENC_DOUBLECLICKTIME  500  // second click within 600ms
#define ENC_HOLDTIME        800  // report held button after 0.8s(modified 2017 10 18 allen.)

// ----------------------------------------------------------------------------
// Acceleration configuration (for 1000Hz calls to ::service())
//
#define ENC_ACCEL_TOP      3072   // max. acceleration: *12 (val >> 8)
#define ENC_ACCEL_INC        25
#define ENC_ACCEL_DEC         2

// ----------------------------------------------------------------------------
#include <TimerOne.h>

ClickEncoder *encoder;
int16_t last, value;

void timerIsr() {
  encoder->service();
}

#include <IRremote.h>
int RECV_PIN = A3;
decode_results results;
IRrecv irrecv(RECV_PIN);

int TurnOn = 1;

int var = 1;
int mode = 1;


int  Acceptoff1 = 0;
int  Acceptoff2 = 0;



int change = 0;
int Toggle = 0;


int PanEn1 = 1;
int PanWait1 = 5;
int PanOn1 = 10;
int PanOff1 = 10;

int PanEn2 = 1;
int PanWait2 = 5;
int PanOn2 = 10;
int PanOff2 = 10;

int TestEn = 0;
int  NowPanic = 0;

unsigned long time1 = millis();
unsigned long time2 = millis();

int rv1 = 1;
int rv2 = 0;
int rv3 = 1;
int gv1 = 1;
int gv2 = 1;
int gv3 = 1;
int bv1 = 0;
int bv2 = 1;
int bv3 = 0;


// Variables will change:
int ledState = LOW;             // ledState used to set the LED
long previousMicros = 0;        // will store the last time the blink cycle occured

// the follow variables is a long because the time, measured in microseconds,
// will quickly become a bigger number than can be stored in an int.
long intervalLength = 50;     // interval at which to blink (microseconds)
long onInterval = 20;          // on or HIGH interval length
long currentInterval;          // length of the current interval
int  fadeLength = 20;
int  fadeCount = 0;
int  intervalDirection = 1;


void setup() {

  Serial.begin(9600);
  encoder = new ClickEncoder(2, 3, A0, 4);                       //(R1,R2,Button,Steps Per Nutch)
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP); //Enc
  pinMode(A0, INPUT_PULLUP); //Button
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  // set the digital pin as output:
  pinMode(g1, OUTPUT);
  pinMode(g2, OUTPUT);
  pinMode(g3, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(b3, OUTPUT);

}

void loop()
{

  value += encoder->getValue();                                      //Rotory Encoder values generat

  if (value != last) {
    change = value - last;
    Serial.print("Encoder change: ");
    Serial.println(change);
    last = value;
    Serial.print("Encoder Value: ");
    Serial.println(value);
  }
  else
  {
    change = 0;

  }


  if (irrecv.decode(&results)) {                                                 //IR controller
    Serial.println(results.value);
    irrecv.resume(); // Receive the next value
    if (results.value == 16720605 ) {                                         //A off

      irrecv.resume(); // Receive the next value
    }

    else if (results.value == 16712445  ) {                                        //A on

      irrecv.resume(); // Receive the next value
    }

    else if (results.value == 16769055 ) {                                         //B off

      irrecv.resume(); // Receive the next value
    }

    else if (results.value == 16754775 ) {                                         //B on

      irrecv.resume(); // Receive the next value
    }
    else if (results.value == 16753245 ) {                                         //PSU off

      irrecv.resume(); // Receive the next value
    }
    else if (results.value == 16736925 ) {                                         //PSU on

      irrecv.resume(); // Receive the next value
    }
    else if (results.value == 16738455  ) {                                         //Backlight off

      irrecv.resume(); // Receive the next value
    }
    else if (results.value == 16750695  ) {                                         //Backlight on

      irrecv.resume(); // Receive the next value
    }

  }

  ClickEncoder::Button b = encoder->getButton();                                  //encoder controller
  if (b != ClickEncoder::Open) {
    Serial.print("Button: ");
#define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b) {
      //用法
      //case ClickEncoder::Pressed:
      //Serial.println("ClickEncoder::Pressed");
      // break;

      case ClickEncoder::Pressed:
        //似乎没用！
        Serial.println("ClickEncoder::Pressed");
        break;

      case ClickEncoder::Held:

        Serial.println("ClickEncoder::Held");



        if (change != 0) {                                                          // UNSURE &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
          change = 0;
        }

        break;                                                          // Held Func End

      case ClickEncoder::Released:
        Serial.println("ClickEncoder::Released");

        Toggle++;
        if (Toggle > 1) {
          Toggle = 0;
          Serial.println("ON");
          TurnOn = 1;
        }
        else {
          Serial.println("OFF");
          TurnOn = 0;
        }

        break;


      case ClickEncoder::Clicked:
        var++;
        Serial.print("   ClickEncoder::Clicked");
        break;

      case ClickEncoder::DoubleClicked:
        mode++;
        Serial.println("ClickEncoder::DoubleClicked");
        encoder->setAccelerationEnabled(!encoder->getAccelerationEnabled());
        Serial.print("  Acceleration is ");
        Serial.println((encoder->getAccelerationEnabled()) ? "enabled" : "disabled");
        break;
    }
  }

  //=========================================================================================================================================
  //=========================================================================================================================================
  switch (mode) {
    case 1://$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ MODE 1 set color
      switch (var) {

        case 1:  //=========================================================================================================================== Degit M

          switch (value) {
            //------------------------------------------------------------------------------- Color1
            case 1:
              rv1 = 0;
              gv1 = 0;
              bv1 = 0;

              Serial.println("1,1");
              break;
            //------------------------------------------------------------------------------- Color2
            case 2:
              rv1 = 1;
              gv1 = 0;
              bv1 = 0;
              Serial.println("1,2");
              break;
            //------------------------------------------------------------------------------- Color3
            case 3:
              rv1 = 0;
              gv1 = 1;
              bv1 = 0;

              Serial.println("1,1");
              break;
            //------------------------------------------------------------------------------- Color4
            case 4:
              rv1 = 0;
              gv1 = 0;
              bv1 = 1;
              Serial.println("1,2");
              break;
            //------------------------------------------------------------------------------- Color5
            case 5:
              rv1 = 1;
              gv1 = 1;
              bv1 = 0;

              Serial.println("1,1");
              break;
            //------------------------------------------------------------------------------- Color6
            case 6:
              rv1 = 0;
              gv1 = 1;
              bv1 = 1;
              Serial.println("1,2");
              break;
            //------------------------------------------------------------------------------- Color7
            case 7:
              rv1 = 1;
              gv1 = 0;
              bv1 = 1;

              Serial.println("1,1");
              break;

            //------------------------------------------------------------------------------- Colordefult
            default:
              Serial.println("1,def");
          }
          break;

        case 2: //============================================================================================================================= Degit V

          switch (value) {
            //------------------------------------------------------------------------------- Color1
            case 1:
              rv2 = 0;
              gv2 = 0;
              bv2 = 0;

              Serial.println("2,1");
              break;
            //------------------------------------------------------------------------------- Color2
            case 2:
              rv2 = 1;
              gv2 = 0;
              bv2 = 0;
              Serial.println("2,2");
              break;
            //------------------------------------------------------------------------------- Color3
            case 3:
              rv2 = 0;
              gv2 = 1;
              bv2 = 0;

              Serial.println("2,3");
              break;
            //------------------------------------------------------------------------------- Color4
            case 4:
              rv2 = 0;
              gv2 = 0;
              bv2 = 1;
              Serial.println("2,4");
              break;
            //------------------------------------------------------------------------------- Color5
            case 5:
              rv2 = 1;
              gv2 = 1;
              bv2 = 0;

              Serial.println("2,5");
              break;
            //------------------------------------------------------------------------------- Color6
            case 6:
              rv2 = 0;
              gv2 = 1;
              bv2 = 1;
              Serial.println("2,6");
              break;
            //------------------------------------------------------------------------------- Color7
            case 7:
              rv2 = 1;
              gv2 = 0;
              bv2 = 1;

              Serial.println("2,7");
              break;

            //------------------------------------------------------------------------------- Colordefult
            default:
              Serial.println("2,def");
          }
          break;

        case 3: //============================================================================================================================= Degit S

          switch (value) {
            //------------------------------------------------------------------------------- Color1
            case 1:
              rv3 = 0;
              gv3 = 0;
              bv3 = 0;

              Serial.println("2,1");
              break;
            //------------------------------------------------------------------------------- Color2
            case 2:
              rv3 = 1;
              gv3 = 0;
              bv3 = 0;
              Serial.println("2,2");
              break;
            //------------------------------------------------------------------------------- Color3
            case 3:
              rv3 = 0;
              gv3 = 1;
              bv3 = 0;

              Serial.println("2,3");
              break;
            //------------------------------------------------------------------------------- Color4
            case 4:
              rv3 = 0;
              gv3 = 0;
              bv3 = 1;
              Serial.println("2,4");
              break;
            //------------------------------------------------------------------------------- Color5
            case 5:
              rv3 = 1;
              gv3 = 1;
              bv3 = 0;

              Serial.println("2,5");
              break;
            //------------------------------------------------------------------------------- Color6
            case 6:
              rv3 = 0;
              gv3 = 1;
              bv3 = 1;
              Serial.println("2,6");
              break;
            //------------------------------------------------------------------------------- Color7
            case 7:
              rv3 = 1;
              gv3 = 0;
              bv3 = 1;

              Serial.println("2,7");
              break;

            //------------------------------------------------------------------------------- Colordefult
            default:
              Serial.println("2,def");
          }
          break;
      }
      if (value > 7) {
        value = 1; // used to control color
      }
      if (value < 1) {
        value = 7;
      }
      digitalWrite(r1, rv1);
      digitalWrite(g1, gv1);
      digitalWrite(b1, bv1);
      digitalWrite(r2, rv2);
      digitalWrite(g2, gv2);
      digitalWrite(b2, bv2);
      digitalWrite(r3, rv3);
      digitalWrite(g3, gv3);
      digitalWrite(b3, bv3);
      break;//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ END MODE 1 set color

    case 2://$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ MODE 2 strobe
      unsigned long currentMicros = micros();

      currentInterval = currentMicros - previousMicros;

      if (currentInterval <= onInterval) {
        ledState = HIGH;
      }
      else {
        ledState = LOW;
      }

      if (currentInterval > intervalLength) {
        // save the last time you blinked the LED
        previousMicros = currentMicros;
        fadeCount++;
        if (fadeCount > fadeLength) {
          fadeCount = 0;
          onInterval += (intervalDirection * 20);
          if (onInterval <= 20 || onInterval >= intervalLength) {
            intervalDirection *= -1;  // change the direction of the fade ( positive - brighter / negative - dimmer)
          }
        }
      }

      // set the LED with the ledState of the variable:
      if (rv1 == 0) {
        digitalWrite(r1, ledState);
      }
      if (gv1 == 0) {
        digitalWrite(g1, ledState);
      }
      if (bv1 == 0) {
        digitalWrite(b1, ledState);
      }
      if (rv2 == 0) {
        digitalWrite(r2, ledState);
      }
      if (gv2 == 0) {
        digitalWrite(g2, ledState);
      }
      if (bv2 == 0) {
        digitalWrite(b2, ledState);
      } if (rv3 == 0) {
        digitalWrite(r3, ledState);
      }
      if (gv3 == 0) {
        digitalWrite(g3, ledState);
      }
      if (bv3 == 0) {
        digitalWrite(b3, ledState);
      }


      break;//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ END MODE 2 strobe
  }

  //========================================================================================================================================= "digit"Var END****************

  //*********************************** Value Limits
  if (var > 3) {
    var = 1;
  }
  if (var < 1) {
    var = 3;
  }


  if (mode > 2) {
    mode = 1;
  }

  //*********************************** Value Limits END**************

  if (TurnOn == 0) {
    rv1 = 1;
    gv1 = 1;
    bv1 = 1;
    rv2 = 1;
    gv2 = 1;
    bv2 = 1;
    rv3 = 1;
    gv3 = 1;
    bv3 = 1;

  }





}