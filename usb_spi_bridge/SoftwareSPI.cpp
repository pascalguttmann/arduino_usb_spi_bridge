#include "SoftwareSPI.h"

#define PORT_COPI PORTB
#define PIN_CIPO PINB
#define PORT_SCK  PORTB

#define BIT_COPI  3
#define BIT_CIPO  4
#define BIT_SCK   5

inline void swspi_write_copi(uint8_t state){
  if (state == SWSPI_LOW){
    PORT_COPI &= ~(0x01 << BIT_COPI);
  }
  else {
    PORT_COPI |= 0x01 << BIT_COPI;
  }
  return;
}

inline void swspi_write_sck(uint8_t state){
  if (state == SWSPI_LOW){
    PORT_SCK &= ~(0x01 << BIT_SCK);
  }
  else {
    PORT_SCK |= 0x01 << BIT_SCK;
  }
  return;
}

inline uint8_t swspi_read_cipo(void){
  if (PIN_CIPO & (0x01 << BIT_CIPO)){
    return 0x01;
  }
  else {
    return 0x00;
  }
}

void swspi_begin(void){
  digitalWrite(copi_pin, LOW);
  digitalWrite(sck_pin, LOW);
  pinMode(copi_pin, OUTPUT);
  pinMode(sck_pin, OUTPUT);
  pinMode(cipo_pin, INPUT);
}

inline uint8_t swspi_transfer(uint8_t octet){
  uint8_t out = 0x00;

  swspi_write_sck(SWSPI_LOW);
  swspi_write_copi(octet & (0x80));
  octet = octet << 1;
  delayMicroseconds(1);
  swspi_write_sck(SWSPI_HIGH);
  out = out << 1;
  out |= swspi_read_cipo();
  delayMicroseconds(1);

  swspi_write_sck(SWSPI_LOW);
  swspi_write_copi(octet & (0x80));
  octet = octet << 1;
  delayMicroseconds(1);
  swspi_write_sck(SWSPI_HIGH);
  out = out << 1;
  out |= swspi_read_cipo();
  delayMicroseconds(1);

  swspi_write_sck(SWSPI_LOW);
  swspi_write_copi(octet & (0x80));
  octet = octet << 1;
  delayMicroseconds(1);
  swspi_write_sck(SWSPI_HIGH);
  out = out << 1;
  out |= swspi_read_cipo();
  delayMicroseconds(1);

  swspi_write_sck(SWSPI_LOW);
  swspi_write_copi(octet & (0x80));
  octet = octet << 1;
  delayMicroseconds(1);
  swspi_write_sck(SWSPI_HIGH);
  out = out << 1;
  out |= swspi_read_cipo();
  delayMicroseconds(1);

  swspi_write_sck(SWSPI_LOW);
  swspi_write_copi(octet & (0x80));
  octet = octet << 1;
  delayMicroseconds(1);
  swspi_write_sck(SWSPI_HIGH);
  out = out << 1;
  out |= swspi_read_cipo();
  delayMicroseconds(1);

  swspi_write_sck(SWSPI_LOW);
  swspi_write_copi(octet & (0x80));
  octet = octet << 1;
  delayMicroseconds(1);
  swspi_write_sck(SWSPI_HIGH);
  out = out << 1;
  out |= swspi_read_cipo();
  delayMicroseconds(1);

  swspi_write_sck(SWSPI_LOW);
  swspi_write_copi(octet & (0x80));
  octet = octet << 1;
  delayMicroseconds(1);
  swspi_write_sck(SWSPI_HIGH);
  out = out << 1;
  out |= swspi_read_cipo();
  delayMicroseconds(1);

  swspi_write_sck(SWSPI_LOW);
  swspi_write_copi(octet & (0x80));
  octet = octet << 1;
  delayMicroseconds(1);
  swspi_write_sck(SWSPI_HIGH);
  out = out << 1;
  out |= swspi_read_cipo();
  
  return out;
}

/* data buffer contains spi transfer data (tx before function call, rx after function call)
 * MSBFIRST
 * data[0] == MSByte, data[data_len-1] == LSByte
 */
void swspi_transfer_buf(uint8_t* data, uint8_t data_len) {
  uint8_t* transfer_byte = data;
  uint8_t transfer_bytes_left = data_len;
  noInterrupts();
  while(transfer_bytes_left--){
    *transfer_byte = swspi_transfer(*transfer_byte);
    transfer_byte++;
  }
  swspi_write_sck(SWSPI_LOW);
  interrupts();
  return;
}
