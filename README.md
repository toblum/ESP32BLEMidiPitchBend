# ESP32BLEMidiPitchBend
[![Open in Visual Studio Code](https://open.vscode.dev/badges/open-in-vscode.svg)](https://open.vscode.dev/toblum/ESP32BLEMidiPitchBend)
[![GitHub license](https://img.shields.io/github/license/Naereen/StrapDown.js.svg)](https://github.com/Naereen/StrapDown.js/blob/master/LICENSE)

A pitch bend wheel based on ESP32 via Bluetooth MIDI.

## Hardware
Connect the middle pin of a 10k potentiometer to GPIO 13 / A14. The left pin (seen from the top) to 5V and the right pin to GND.

## Used libraries
- dxinteractive/ResponsiveAnalogRead @ ^1.2.1
- max22/ESP32-BLE-MIDI @ ^0.2.2

Thanks to the authors!

## Building
Best option is to use platform.io extension in Visual Studio Code. It will take care of the dependencies.

## Licence
Published under the MIT License.