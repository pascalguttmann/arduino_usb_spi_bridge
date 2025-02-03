#ifndef SoftwareSPI_h
#define SoftwareSPI_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif

#define SWSPI_LOW   (0x00)
#define SWSPI_HIGH  (0x01)

const uint8_t copi_pin = 11;
const uint8_t cipo_pin = 12;
const uint8_t sck_pin = 13;

void swspi_begin(void);
inline uint8_t swspi_transfer(uint8_t octet);
void swspi_transfer_buf(uint8_t* data, uint8_t data_len);

#endif
