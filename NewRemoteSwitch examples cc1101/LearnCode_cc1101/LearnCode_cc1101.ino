/*
 * Demo for RF remote switch receiver. 
 * This example is for the new KaKu / Home Easy type of remotes!
 *
 * For details, see NewRemoteReceiver.h!
 *
 * With this sketch you can control a LED connected to digital pin 13,
 * after the sketch learned the code. After start, the LED starts to blink,
 * until a valid code has been received. The led stops blinking. Now you
 * can control the LED with the remote.
 *
 * Note: only unit-switches are supported in this sketch, no group or dim.
 *
 *  https://github.com/1technophile/NewRemoteSwitch
 *  https://github.com/LSatan/RCSwitch-CC1101-Driver-Lib
 *  ----------------------------------------------------------
 *  Mod by Little Satan. Have Fun!
 *  ----------------------------------------------------------
 *
 */
#include <ELECHOUSE_CC1101_RCS_DRV.h>
#include <NewRemoteReceiver.h>

#define esp 0 // for ESP8266 set 1, for Arduino set 0.

int pin; // int for Receive pin.
int led; // pin for Led.

boolean codeLearned = false;
unsigned long learnedAddress;
byte learnedUnit;

void setup() {
    
  if (esp == 1){pin = 4; led = 2;} // for esp8266! Receiver on pin 4 = D2. Led pin 2 = D4.
  if (esp == 0){pin = 0; led = 4;} // for Arduino! Receiver on interrupt 0 => that is pin #2. Led pin = 4;

//CC1101 Settings:                (Settings with "//" are optional!)
  ELECHOUSE_cc1101.setESP8266(esp); // esp8266 & Arduino SPI pin settings. Don´t change this line!
//ELECHOUSE_cc1101.setRxBW(16);  // set Receive filter bandwidth (default = 812khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
//ELECHOUSE_cc1101.setChannel(175);// set channel 80khz steps.0 - 255 default channel number is 175 = 433,92Mhz / 315Mhz 
//ELECHOUSE_cc1101.setChsp(80); // set Channle spacing (default = 80khz) you can set 25,50,80,100,150,200,250,300,350,405.
//ELECHOUSE_cc1101.freq2(0x10); //set your own frequency Parameters Freq2. From Smart RF Studio. Must set Frequency settings F_xxx.
//ELECHOUSE_cc1101.freq1(0x27); //set your own frequency Parameters Freq1. From Smart RF Studio. Must set Frequency settings F_xxx.
//ELECHOUSE_cc1101.freq0(0x62); //set your own frequency Parameters Freq0. From Smart RF Studio. Must set Frequency settings F_xxx.
  ELECHOUSE_cc1101.Init(F_433); //Must set! set frequency - F_315, F_433, F_868, F_965 MHz. custom = F_xxx. set TxPower  PA10, PA7, PA5, PA0, PA_10, PA_15, PA_20, PA_30. example: F_433_PA10. (default = PA10)
  ELECHOUSE_cc1101.SetRx();    // set Recive on
  
  // LED-pin as output
  pinMode(led, OUTPUT);

  // Init a new receiver on interrupt pin 0, minimal 2 identical repeats, and callback set to processCode.
  NewRemoteReceiver::init(pin, 2, processCode);
}

void loop() {
  // Blink led until a code has been learned
  if (!codeLearned) {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
  }
}

// Callback function is called only when a valid code is received.
void processCode(NewRemoteCode receivedCode) {
  // A code has been received.
  // Do we already know the code?
  if (!codeLearned) {
    // No! Let's learn the received code.
    learnedAddress = receivedCode.address;
    learnedUnit = receivedCode.unit;
    codeLearned = true;    
  } else {
    // Yes!    
    // Is the received code identical to the learned code?
    if (receivedCode.address == learnedAddress && receivedCode.unit == learnedUnit) {
      // Yes!
      // Switch the LED off if the received code was "off".
      // Anything else (on, dim, on_with_dim) will switch the LED on.
      if (receivedCode.switchType == NewRemoteCode::off) {
        digitalWrite(13, LOW);
      } else {
        digitalWrite(13, HIGH);
      }
    }
  }
}