#include <BLEMIDI_Transport.h>

#include <hardware/BLEMIDI_ESP32_NimBLE.h>
//#include <hardware/BLEMIDI_ESP32.h>
//#include <hardware/BLEMIDI_nRF52.h>
//#include <hardware/BLEMIDI_ArduinoBLE.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif


const int LOOP_DELAY = 4; //ms

const int BT_LED = LED_BUILTIN;
const int BT_PAIR_DELAY = 300; //ms

const int PEDAL_PIN = 4;
const int LED_PEDAL = 5;
bool pedalState;
bool prevPedalState;

bool isConnectedBT = false;

const byte MIDI_CH = 1;
BLEMIDI_CREATE_INSTANCE("ble-midi-box", MIDI)

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void setup()
{
  MIDI.begin();

  pinMode(BT_LED, OUTPUT);
  digitalWrite(BT_LED, LOW);

  pinMode(PEDAL_PIN, INPUT);
  pinMode(LED_PEDAL, OUTPUT);
  digitalWrite(LED_PEDAL, LOW);

  BLEMIDI.setHandleConnected(OnConnected);
  BLEMIDI.setHandleDisconnected(OnDisconnected);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void loop()
{
  MIDI.read();

  if (isConnectedBT)
  {
    pedalState = isPedalActive();
    if (pedalState != prevPedalState) 
    {
      setPedal(pedalState, MIDI_CH);
      prevPedalState = pedalState;
    }
    
    delay(LOOP_DELAY);
  }
  else
  {
    blinkLED(BT_LED, BT_PAIR_DELAY);
  }
}

// ====================================================================================

void blinkLED(int ledPin, int blinkDelay) {
  digitalWrite(ledPin, LOW);
  delay(blinkDelay);
  digitalWrite(ledPin, HIGH);
  delay(blinkDelay);
}

bool isPedalActive() {
  return digitalRead(PEDAL_PIN) == HIGH;
}

void setPedal(bool active, byte midiChannel) {
  if (active) {
    MIDI.sendControlChange(64, 127, midiChannel);
    digitalWrite(LED_PEDAL, HIGH);
  } else {
    MIDI.sendControlChange(64, 0, midiChannel);
    digitalWrite(LED_PEDAL, LOW);
  }
}

// ====================================================================================
// Event handlers for incoming MIDI messages
// ====================================================================================

// -----------------------------------------------------------------------------
// Device connected
// -----------------------------------------------------------------------------
void OnConnected() {
  isConnectedBT = true;
  digitalWrite(BT_LED, HIGH);

  setPedal(isPedalActive(), MIDI_CH);
}

// -----------------------------------------------------------------------------
// Device disconnected
// -----------------------------------------------------------------------------
void OnDisconnected() {
  isConnectedBT = false;
  digitalWrite(BT_LED, LOW);
}
