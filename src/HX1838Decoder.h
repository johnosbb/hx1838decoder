#ifndef HX1838Decoder_H
#define HX1838Decoder_H

#include <Arduino.h>

#define BUFFER_SIZE 100  // Maximum number of pulse entries

// Check if the selected pin is interrupt-capable (only works on AVR-based boards)
#if defined(__AVR__) 
    #define CHECK_INTERRUPT_PIN(pin) static_assert(digitalPinToInterrupt(pin) != NOT_AN_INTERRUPT, "ERROR: Selected pin is NOT interrupt-capable!");
#else
    #define CHECK_INTERRUPT_PIN(pin)
#endif

class IRDecoder {
public:
    IRDecoder(uint8_t pin);
    void begin();
    bool available();
    uint32_t getDecodedData();
    bool isRepeatSignal();

private:
    uint8_t irPin;
    static volatile unsigned long pulseTimes[];
    static volatile int pulseIndex;
    static volatile unsigned long lastTime;
    uint32_t lastDecodedData;
    bool isRepeat;

    static void IRAM_ATTR handleIRSignal();
    void processSignal();
    uint32_t reverseBits32(uint32_t num);
};

#endif
