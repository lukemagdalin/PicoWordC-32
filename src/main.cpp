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
void write_register(int segmentValues[NUM_DIGITS][8]);
void convertToDecimalArray(uint32_t readBytes, int decimalDigitArray[10]);
void convertToSegmentValues(int decimalDigitArray[10], int segmentValues[NUM_DIGITS][8]);
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
    int decimalDigitArray[10];
    convertToDecimalArray(readBytes, decimalDigitArray);

    // Convert each decimal number into the seven segment code
    int segmentValues[10][8];
    convertToSegmentValues(decimalDigitArray, segmentValues);

    // write all 80 bits to the seven segment display registers
    write_register(segmentValues); 
}

void convertToSegmentValues(int decimalDigitArray[10], int segmentValues[NUM_DIGITS][8])
{
        /* List of all the configurations for the seven segment display in order.               
        index0 - Binary code for displaying 0
        index1 - Binary code for displaying 1
        ... so on...
        index 10 ... 10
    */
    int segmentCodes[10][8] = 
    {
        {0, 0, 1, 1, 1, 1, 1, 1},   // 0
        {0, 0, 0, 0, 0, 1, 1, 0},   // 1
        {0, 1, 0, 1, 1, 0, 1, 1},   // 2
        {0, 1, 0, 0, 1, 1, 1, 1},   // 3
        {0, 1, 1, 0, 0, 1, 1, 0},   // 4
        {0, 1, 1, 0, 1, 1, 0, 1},   // 5
        {0, 1, 1, 1, 1, 1, 0, 1},   // 6
        {0, 0, 0, 0, 0, 1, 1, 1},   // 7
        {0, 1, 1, 1, 1, 1, 1, 1},   // 8
        {0, 1, 1, 0, 0, 1, 1, 1}    // 9
    };

    // For each number in decimalDigitArray, get the corresponding segment code, and add to segmentValues.
    for(int i = 0; i < 10; i++)
    {
        int decimalNumber = decimalDigitArray[i]; 
        for(int j = 0; j < 8; j++)
        {
            segmentValues[i][j] = segmentCodes[decimalNumber][j];
        }
    }
}

void convertToDecimalArray(uint32_t readBytes, int decimalDigitArray[10])
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
void write_register(int segmentValues[NUM_DIGITS][8])
{
    digitalWrite(PIN_LATCH_OUT, LOW);
    for(int i = NUM_DIGITS - 1; i >= 0; i--)
    {
        for(int j = 7; j >= 0; j--)
        {
            digitalWrite(PIN_DATA_OUT, segmentValues[i][j]);
            digitalWrite(PIN_CLK_OUT, HIGH);
            digitalWrite(PIN_CLK_OUT, LOW);
        }
    }
    digitalWrite(PIN_LATCH_OUT, HIGH);
}