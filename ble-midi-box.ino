#include <BLEMIDI_Transport.h>

#include <hardware/BLEMIDI_ESP32_NimBLE.h>
//#include <hardware/BLEMIDI_ESP32.h>
//#include <hardware/BLEMIDI_nRF52.h>
//#include <hardware/BLEMIDI_ArduinoBLE.h>

BLEMIDI_CREATE_INSTANCE("ble-midi-box", MIDI)

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

bool isConnected = false;

const int BT_LED = LED_BUILTIN;
const int BT_PAIR_DELAY = 300; //ms

const int pedalPin = 4;

bool pedalState;
bool prevPedalState;


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void setup()
{
  MIDI.begin();

  pinMode(BT_LED, OUTPUT);
  digitalWrite(BT_LED, LOW);

  pinMode(pedalPin, INPUT);

  BLEMIDI.setHandleConnected(OnConnected);
  BLEMIDI.setHandleDisconnected(OnDisconnected);

  MIDI.setHandleNoteOn(OnNoteOn);
  MIDI.setHandleNoteOff(OnNoteOff);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void loop()
{
  MIDI.read();

  if (isConnected)
  {
    pedalState = digitalRead(pedalPin) == HIGH;
    if (pedalState != prevPedalState) {
      if (pedalState == true)
      {
        MIDI.sendControlChange(64, 127, 1);
      }
      else
      {
        MIDI.sendControlChange(64, 0, 1);
      }
    }
    prevPedalState = pedalState;
    delay(4);
  }
  else
  {
    digitalWrite(BT_LED, LOW);
    delay(BT_PAIR_DELAY);
    digitalWrite(BT_LED, HIGH);
    delay(BT_PAIR_DELAY);
  }
}

// ====================================================================================
// Event handlers for incoming MIDI messages
// ====================================================================================

// -----------------------------------------------------------------------------
// Device connected
// -----------------------------------------------------------------------------
void OnConnected() {
  isConnected = true;
  digitalWrite(BT_LED, HIGH);
}

// -----------------------------------------------------------------------------
// Device disconnected
// -----------------------------------------------------------------------------
void OnDisconnected() {
  isConnected = false;
  digitalWrite(BT_LED, LOW);
}

// -----------------------------------------------------------------------------
// Received note on
// -----------------------------------------------------------------------------
void OnNoteOn(byte channel, byte note, byte velocity) {
}

// -----------------------------------------------------------------------------
// Received note off
// -----------------------------------------------------------------------------
void OnNoteOff(byte channel, byte note, byte velocity) {
}
