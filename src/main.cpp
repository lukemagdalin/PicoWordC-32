#include <Arduino.h>

const int NUM_DIGITS = 10;

// Control pins for switch registers
const int PIN_DATA_IN = 4;
const int PIN_LATCH_IN = 5;
const int PIN_CLK_IN = 6;

// Control pins for display registers
const int PIN_DATA_OUT = 7;
const int PIN_LATCH_OUT = 8;
const int PIN_CLK_OUT = 9;

uint32_t read_register(void);
void write_register(uint8_t segmentValues[NUM_DIGITS]);
void convertToDecimalArray(uint32_t readBytes, uint8_t decimalDigitArray[10]);
void convertToSegmentValues(uint8_t decimalDigitArray[10], uint8_t segmentValues[NUM_DIGITS]);
// Setup pins
void setup() 
{
    pinMode(PIN_DATA_IN, INPUT);
    pinMode(PIN_LATCH_IN, OUTPUT);
    pinMode(PIN_CLK_IN, OUTPUT);

    pinMode(PIN_DATA_OUT, OUTPUT);
    pinMode(PIN_LATCH_OUT, OUTPUT);
    pinMode(PIN_CLK_OUT, OUTPUT);
}

void loop() 
{
    // Get 32-bit number from register
    uint32_t readBytes = read_register();
    
    // convert bytes to decimal, then seperate each digit
    uint8_t decimalDigitArray[10];
    convertToDecimalArray(readBytes, decimalDigitArray);

    // Convert each decimal number into the seven segment code
    uint8_t segmentValues[NUM_DIGITS];
    convertToSegmentValues(decimalDigitArray, segmentValues);

    // write all 80 bits to the seven segment display registers
    write_register(segmentValues); 
}

void convertToSegmentValues(uint8_t decimalDigitArray[10], uint8_t segmentValues[NUM_DIGITS])
{
    // Each segment code packs 8 segments into a single byte (MSB to LSB)
    uint8_t segmentCodes[10] = 
    {
        0b00111111,   // 0
        0b00000110,   // 1
        0b01011011,   // 2
        0b01001111,   // 3
        0b01100110,   // 4
        0b01101101,   // 5
        0b01111101,   // 6
        0b00000111,   // 7
        0b01111111,   // 8
        0b01100111    // 9
    };

    for(int i = 0; i < NUM_DIGITS; i++)
    {
        segmentValues[i] = segmentCodes[decimalDigitArray[i]];
    }
}

void convertToDecimalArray(uint32_t readBytes, uint8_t decimalDigitArray[10])
{
    // Convert 32-bit number to decimal value, and seperate into single digit 
    for(int i = 0; i < 10; i++)
    {
        decimalDigitArray[i] = readBytes % 10;
        readBytes = readBytes / 10;
    }
}
// Read 32 bits from shift register, return 32-bit unsigned value
uint32_t read_register(void)
{
    uint32_t readBytes = 0;
    digitalWrite(PIN_LATCH_IN, HIGH); // let registers get values from switches
    delayMicroseconds(5);
    digitalWrite(PIN_LATCH_IN, LOW); // lock registers to the values
    for(int i = 0; i < 32; i++)
    {
        int bit = digitalRead(PIN_DATA_IN); // get the bit value of the current iteration
        readBytes |= ((uint32_t)bit << i); // Add bit to readBytes
        digitalWrite(PIN_CLK_IN, HIGH); // Iterate to next bit
        digitalWrite(PIN_CLK_IN, LOW); 
    }
    return readBytes;
}

// Write segment values to display shift registers
void write_register(uint8_t segmentValues[NUM_DIGITS])
{
    digitalWrite(PIN_LATCH_OUT, LOW);
    for(int i = NUM_DIGITS - 1; i >= 0; i--)
    {
        for(int j = 7; j >= 0; j--)
        {
            digitalWrite(PIN_DATA_OUT, (segmentValues[i] >> j) & 1);
            digitalWrite(PIN_CLK_OUT, HIGH);
            digitalWrite(PIN_CLK_OUT, LOW);
        }
    }
    digitalWrite(PIN_LATCH_OUT, HIGH);
}