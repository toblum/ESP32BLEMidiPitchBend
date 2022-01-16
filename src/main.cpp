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

// Potentiometer pin
const uint8_t potipin = POTIPIN;
ResponsiveAnalogRead analog(potipin, true);

// Globals
uint8_t channel = MIDICHANNEL;
unsigned long lastPitchTime;
bool isConnected = false;
int lastAnalogValue;


void setup()
{
	Serial.begin(115200);
	Serial.println("Initializing bluetooth");
	BLEMidiServer.begin("ESP32BLEMidiPitchBend");
	Serial.println("Waiting for connections...");
	//BLEMidiServer.enableDebugging();  // Uncomment if you want to see some debugging output from the library (not much for the server class...)

	pinMode(potipin, INPUT);
}

void loop()
{
	analog.update();

	if (BLEMidiServer.isConnected())
	{
		if (!isConnected) {
			Serial.println("First device connected!");
			isConnected = true;
		}

		if (lastPitchTime <= millis())
		{
			lastPitchTime = millis() + 20;
			int analogValue = analog.getValue(); // 0 to 1023
			int analogValueChange = abs(analogValue - lastAnalogValue);

			if (analogValueChange > 10)
			{
				// Serial.println(analogValue);
				lastAnalogValue = analogValue;

				uint16_t val = analogValue * 16; // 0 to 16368
				BLEMidiServer.pitchBend(channel, val); // Expects values from 0 to 16383, map to complete range
			}
		}
	}
}