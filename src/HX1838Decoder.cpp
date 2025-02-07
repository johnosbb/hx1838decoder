#include "HX1838Decoder.h"

volatile unsigned long IRDecoder::pulseTimes[BUFFER_SIZE] = {0};
volatile int IRDecoder::pulseIndex = 0;
volatile unsigned long IRDecoder::lastTime = 0;

IRDecoder::IRDecoder(uint8_t pin) : irPin(pin), lastDecodedData(0), isRepeat(false) {}

void IRDecoder::begin() {
    pinMode(irPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(irPin), handleIRSignal, CHANGE);
}


bool IRDecoder::isRepeatSignal() {
    return isRepeat;
}


bool IRDecoder::available() {
    if (pulseIndex > 0) {
        processSignal();
        return true;
    }
    return false;
}

uint32_t IRDecoder::getDecodedData() {
    return lastDecodedData;
}

void IRAM_ATTR IRDecoder::handleIRSignal() {
    unsigned long currentTime = micros();
    unsigned long pulseDuration = currentTime - lastTime;
    lastTime = currentTime;

    if (pulseDuration < 100000 && pulseIndex < BUFFER_SIZE) {  
        pulseTimes[pulseIndex++] = pulseDuration;
    }
}

void IRDecoder::processSignal() {
    delay(100);  // Ensure full signal capture by waiting for 100ms after the initial interrupt

    noInterrupts();
    int count = pulseIndex;
    pulseIndex = 0;
    interrupts();

    if (count < 4) return;  // Ignore noise, by ignoring cases where count < 4, we ensure we are only processing signals that have at least a minimal structure. The smallest valid IR transmission has at least one mark and one space, forming a single pulse pair or 4 transitions.

    // Check for NEC Repeat Signal (9ms LOW + 2.25ms HIGH)
    if (count >= 4 && pulseTimes[0] > 8500 && pulseTimes[0] < 9500 &&
        pulseTimes[1] > 2000 && pulseTimes[1] < 2500 &&
        pulseTimes[2] > 300 && pulseTimes[2] < 700) {
        isRepeat = true;    
        return;
    }

    // Check NEC Header: 9ms LOW + 4.5ms HIGH
    if (!(pulseTimes[0] > 8500 && pulseTimes[0] < 9500 &&
          pulseTimes[1] > 4000 && pulseTimes[1] < 5000)) {
        return;
    }

    // Decode NEC signal
    uint32_t data = 0;
    int index = 2;

    for (int i = 0; i < 32; i++) {
        if (index + 1 >= count) break;
        unsigned long space = pulseTimes[index + 1];

        if (space > 1200) {  
            data |= (1UL << i);
        }
        index += 2;
    }

    lastDecodedData = reverseBits32(data); // we need to reverse the pulse train to correspond with what the IR transmitter sent
    isRepeat = false;  // Reset repeat flag for new signals
}

uint32_t IRDecoder::reverseBits32(uint32_t num) {
    num = (num & 0xFFFF0000) >> 16 | (num & 0x0000FFFF) << 16;
    num = (num & 0xFF00FF00) >> 8  | (num & 0x00FF00FF) << 8;
    num = (num & 0xF0F0F0F0) >> 4  | (num & 0x0F0F0F0F) << 4;
    num = (num & 0xCCCCCCCC) >> 2  | (num & 0x33333333) << 2;
    num = (num & 0xAAAAAAAA) >> 1  | (num & 0x55555555) << 1;
    return num;
}
