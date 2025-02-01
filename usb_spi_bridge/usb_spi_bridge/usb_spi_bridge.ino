#include <SPI.h>

#define LEN(x) (sizeof(x) / sizeof(x[0]))

const uint8_t cs_pin = 10;
const uint8_t copi_pin = 11;
const uint8_t cipo_pin = 12;
const uint8_t sck_pin = 13;

void setup() {
  Serial.begin(115200);
  pinMode(cs_pin, OUTPUT);
  SPI.begin();
  Serial.println("Setup finished.");
}

void loop() {
  uint8_t data[] = {0x01, 0xaa, 0xF1};
  uint8_t data_len = LEN(data);
  
  Serial.print("TX: ");
  print_buf_as_hexstring(data, data_len);
  
  spi_transfer(cs_pin, data, data_len);
  
  Serial.print("\tRX: ");
  print_buf_as_hexstring(data, data_len);
  Serial.print("\n");

  delay(1000);
}

void print_buf_as_hexstring(uint8_t* data, uint8_t data_len) {
  Serial.print("0x");
  for (uint8_t i = 0; i < data_len; i++) {
    Serial.print(data[i], HEX);
  }
}

/* data buffer contains spi transfer data (tx before function call, rx after function call)
 * MSBFIRST
 * data[0] == MSByte, data[data_len-1] == LSByte
 */
void spi_transfer(uint8_t cs_pin, uint8_t* data, uint8_t data_len) {
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(cs_pin, LOW);
  delayMicroseconds(10);

  for (uint8_t i = 0; i < data_len; i++) {
    data[i] = SPI.transfer(data[i]);
  }

  delayMicroseconds(10);
  digitalWrite(cs_pin, HIGH);
  SPI.endTransaction();
  return;
}
