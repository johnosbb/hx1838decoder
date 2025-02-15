## Introduction

![image](https://github.com/user-attachments/assets/58449a8a-e302-431d-8ae5-4f6692c08533)

I was recently working on an ESP32-based project and wanted to integrate a simple, cheap IR remote control into it. Initially, I assumed this would be straightforward—after all, there are plenty of IR remote libraries available for Arduino.

However, after testing multiple popular libraries, I ran into reliability issues on ESP32 and ESP8266 devices. Some libraries had timing inconsistencies, while others didn’t handle interrupts well on these architectures. Others simply weren’t designed with ESP-specific constraints in mind.

Why HX1838Decoder?
Rather than hacking existing libraries, I decided to build my own lightweight NEC-compatible IR decoder, focusing on:

- ESP32 & ESP8266 Compatibility – Designed with their architecture in mind.
- Interrupt-Based Decoding – Ensures accurate and efficient signal capture.
- Minimal Overhead – No unnecessary dependencies or features.
- Support for Repeat Signals – Properly detects when a key is held down.

## Getting Started
To integrate HX1838Decoder into your project:

- Install the library via the Arduino Library Manager or GitHub.
- Connect an IR receiver module (HX1838 or similar) to an interrupt-capable pin.
- Use the API to capture and decode IR signals.

## Overview

This library provides an Arduino-compatible NEC IR remote decoder for HX1838 and similar IR receivers. It captures and processes infrared signals, extracts 32-bit NEC codes, and provides a simple interface for retrieval.



## NEC Protocol

The NEC IR transmission protocol uses pulse distance encoding of the message bits. Each pulse burst (mark – RC transmitter ON) is 562.5µs in length, at a carrier frequency of 38kHz (26.3µs). Logical bits are transmitted as follows: 

- Logical '0' – a 562.5µs pulse burst followed by a 562.5µs space, with a total transmit time of 1.125ms
- Logical '1' – a 562.5µs pulse burst followed by a 1.6875ms space, with a total transmit time of 2.25ms

When transmitting or receiving remote control codes using the NEC IR transmission protocol, the WB_IRRC performs optimally when the carrier frequency (used for modulation/demodulation) is set to 38.222kHz.


![image](https://github.com/user-attachments/assets/2194b772-ae63-45f3-abe9-25d183b078f0)

_Logical 0 and Logical 1 Sequence Captured on an Oscilloscope_

### Key Press

When a key is pressed on the remote controller, the message transmitted consists of the following, in order:

- a 9ms leading pulse burst (16 times the pulse burst length used for a logical data bit)
- a 4.5ms space
- the 8-bit address for the receiving device
- the 8-bit logical inverse of the address
- the 8-bit command
- the 8-bit logical inverse of the command
- a final 562.5µs pulse burst to signify the end of message transmission.

The four bytes of data bits are each sent least significant bit first.  

![image](https://github.com/user-attachments/assets/25b6e279-3dec-4f91-be2a-9b2d16449fdf)

_Key Press Captured on an Oscilloscope_

### Repeat Codes

If the key on the remote controller is kept depressed, a repeat code will be issued, typically around 40ms after the pulse burst that signified the end of the message. A repeat code will continue to be sent out at 108ms intervals, until the key is finally released. The repeat code consists of the following, in order:

- a 9ms leading pulse burst
- a 2.25ms space
- a 562.5µs pulse burst to mark the end of the space (and hence end of the transmitted repeat code).

![image](https://github.com/user-attachments/assets/21b733ec-a690-4374-aaa3-f16c91d5ce22)

_Key Repeat Sequence on an Oscilloscope_

## How the Library Works


### Interrupt-Driven Signal Capture

An interrupt is attached to the specified IR receiver pin. Each pulse transition (HIGH/LOW) is recorded in pulseTimes[] along with its duration (in microseconds).

### Signal Processing & Noise Filtering

After receiving a signal, we pause briefly (100ms) to ensure full capture. If the signal is too short (<4 transitions), it is discarded as noise. NEC repeat signals (9ms LOW + 2.25ms HIGH) are detected and ignored. Valid NEC signals (9ms LOW + 4.5ms HIGH) proceed to decoding.

### Decoding NEC Signals

The length of each pulse determines whether it is a 1 or 0. The 32-bit NEC code is reconstructed from the captured pulse train. Since the NEC protocol transmits LSB (Least Significant Bit) first, we reverse the bit order for correct decoding.

### Retrieving the Decoded Data

Use available() to check if a valid signal was received. Use getDecodedData() to retrieve the decoded 32-bit NEC command in hexadecimal format.

## Usage Example

```cpp
#include "HX1838Decoder.h"

IRDecoder irDecoder(4);  // IR Receiver on pin 4

void setup() {
    Serial.begin(115200);
    irDecoder.begin();
}

void loop() {
    if (irDecoder.available()) {
        Serial.print("Decoded NEC Data: 0x");
        Serial.print(irDecoder.getDecodedData(), HEX);
        
        if (irDecoder.isRepeatSignal()) {
            Serial.println(" (REPEATED)");
        } else {
            Serial.println(" (NEW PRESS)");
        }
    }
}
```

## Key Features

- Interrupt-based decoding (efficient and accurate).
- Filters out noise and identifies NEC repeat signals.
- Reconstructs 32-bit NEC codes with correct bit order.
- Simple API for integration into Arduino projects.


__Important__: The IR receiver must be connected to an interrupt-capable pin. Check your microcontroller’s documentation for interrupt-capable pins.

- Arduino Uno/Nano: Pins 2, 3
- ESP8266: Pins 0, 2, 4, 5, 12, 13, 14, 15
- ESP32: Almost all GPIO pins (except 6-11)

- ![image](https://github.com/user-attachments/assets/0f10852e-3eb2-495b-8deb-aa0d594b3cb9)

