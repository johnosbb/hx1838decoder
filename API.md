# HX1838Decoder Library API Guide

## Overview

The **HX1838Decoder** library provides an easy-to-use interface for decoding NEC infrared (IR) remote control signals using an interrupt-capable pin. It captures and processes incoming IR pulses, extracting 32-bit NEC command codes and identifying repeated key presses.

## Requirements

- The specified **IR receiver pin must support hardware interrupts**.
- Works with NEC protocol-based remotes.
- Uses an **interrupt-driven approach** to capture IR signals with minimal CPU overhead.

---

## **Class: `IRDecoder`**

### **Constructor**

```cpp
IRDecoder(uint8_t pin);
```

### Description

Creates an instance of the IRDecoder and assigns the specified IR receiver pin.

### Arguments

The pin (uint8_t): The digital pin where the IR receiver is connected. __Note__: This pin must support external interrupts.

### Example

```cpp
IRDecoder irDecoder(4);  // Create an instance using pin 4
```

## Function: void begin()

```cpp
void begin();
```

### Description

Initializes the IR decoder and attaches an interrupt to the specified pin.

### Usage Notes

This function must be called in setup() before using the decoder. It enables an interrupt to detect IR signal changes.


### Example

```cpp
void setup() {
    irDecoder.begin();
}
```

## Function: bool available()

```cpp
bool available();
```

### Description

Checks if a new IR signal has been received and processed.

### Return Value

- true → A new IR signal is available.
- false → No new signal has been received.

### Example

```cpp
if (irDecoder.available()) {
    Serial.println("New IR signal detected!");
}
```


## Function: uint32_t getDecodedData()

```cpp
uint32_t getDecodedData();
```

### Description

Retrieves the last decoded NEC 32-bit command code.

### Return Value

A uint32_t representing the decoded IR command. If no valid signal was received, the return value is the last stored signal. (To check if new data is available, use available() before calling this function.)

### Example

```cpp

if (irDecoder.available()) {
    uint32_t code = irDecoder.getDecodedData();
    Serial.print("Received NEC Code: 0x");
    Serial.println(code, HEX);
}
```

## Function: bool isRepeatSignal()

```cpp

bool isRepeatSignal();
```

### Description
Indicates if the last received signal was a repeat command (i.e., the user held the button down).

### Return Value

- true → The last command was a repeat.
- false → A new keypress was detected.

### Example

```cpp

if (irDecoder.available()) {
    if (irDecoder.isRepeatSignal()) {
        Serial.println("Repeat signal detected");
    } else {
        Serial.println("New key press");
    }

}
```
