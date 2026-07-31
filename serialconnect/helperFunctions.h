#include <string>
#include <cstdint>

void showDisplayLine(uint8_t *data[]);
uint8_t checkDisplayBit(uint8_t displaybit);
uint8_t checkBitValue(uint8_t data);
char checkCharValue(char value);
char checkCharValueNoNull(char value);

uint8_t getNullBit();

void showDisplayInternal(uint8_t data[]);