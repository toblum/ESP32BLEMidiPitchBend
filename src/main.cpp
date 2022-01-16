/* **************************************************
* ESP32BLEMidiPitchBend
* A pitch bend wheel based on ESP32 via Bluetooth MIDI.
* Author: Tobias Blum <github@tobiasblum.de>
* Licence: MIT
* Version: 0.1.0
* *************************************************** */

#include <Arduino.h>
#include <ResponsiveAnalogRead.h>
#include <BLEMidi.h>
#include <EasyButton.h>

// Potentiometer pin
const uint8_t potipin = POTIPIN;
ResponsiveAnalogRead analog(potipin, true);

// Globals
enum Wheelmode
{
	PITCH,
	MODULATION
};

Wheelmode wheelmode = MODULATION;
uint8_t channel = MIDICHANNEL;
unsigned long lastPitchTime;
bool isConnected = false;
bool modeChanged = true;
int lastAnalogValue;

// Button 1 pin
const uint8_t btn1pin = BTN1PIN;
EasyButton btn1(btn1pin);

void setup()
{
	Serial.begin(115200);
	Serial.println("Initializing bluetooth");
	BLEMidiServer.begin("ESP32BLEMidiPitchBend");
	Serial.println("Waiting for connections...");
	//BLEMidiServer.enableDebugging();  // Uncomment if you want to see some debugging output from the library (not much for the server class...)

	btn1.begin();
	pinMode(potipin, INPUT);
}

void loop()
{
	analog.update();
	btn1.read();

	if (btn1.wasPressed()) {
		Serial.println("Switch to pitch mode!");
		wheelmode = PITCH;
		modeChanged = true; // Make sure value is updated instantly after mode change
	}
	if (btn1.wasReleased()) {
		Serial.println("Switch to modulation mode!");
		wheelmode = MODULATION;
		modeChanged = true; // Make sure value is updated instantly after mode change
	}

	if (BLEMidiServer.isConnected())
	{
		if (!isConnected)
		{
			Serial.println("First device connected!");
			isConnected = true;
		}

		if (lastPitchTime <= millis())
		{
			lastPitchTime = millis() + 20;
			int analogValue = analog.getValue(); // 0 to 1023
			int analogValueChange = abs(analogValue - lastAnalogValue);

			if (analogValueChange > 10 || modeChanged)
			{
				// Serial.println(analogValue);
				lastAnalogValue = analogValue;
				modeChanged = false;

				if (wheelmode == PITCH)
				{
					uint16_t val = map(analogValue, 0, 1023, 0, 16368); // Map to 0 to 16368 range
					BLEMidiServer.pitchBend(channel, val);
				}

				if (wheelmode == MODULATION)
				{
					uint16_t val = map(analogValue, 0, 1023, 0, 127); // Map to 0 to 127 range
					BLEMidiServer.controlChange(channel, 1, val); // Send CC1 (modulation) message
				}
			}
		}
	}
}