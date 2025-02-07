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
