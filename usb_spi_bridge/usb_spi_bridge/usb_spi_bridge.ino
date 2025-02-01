#include <SPI.h>

#define LEN(x) (sizeof(x) / sizeof(x[0]))
#define UINT8_MAX (256U)

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
  uint8_t data[2*UINT8_MAX] = {};
  uint32_t data_len = LEN(data);

  read_serial_line(data, &data_len);
  Serial.print("Read: ");
  print_buf_as_hexstring(data, data_len);
  Serial.print("\n");

  // TODO: hexstring_to_uint8arr();
  
  Serial.print("TX: ");
  print_buf_as_hexstring(data, data_len);
  
  spi_transfer(cs_pin, data, data_len);
  
  Serial.print("\tRX: ");
  print_buf_as_hexstring(data, data_len);
  Serial.print("\n");
}

// buf is overwritten by serial data (excluding newline), sets buf_len to the number of bytes read
void read_serial_line(uint8_t* buf, uint32_t* buf_len) {
  uint32_t i = 0;
  while (i < *buf_len) {
    spinlock_for_serial_input();
    uint8_t c = Serial.read();
    if (c == '\n') {
      break;
    }
    else {
      buf[i++] = c;
    }
  }
  *buf_len = i;
}

void spinlock_for_serial_input(void){
  while (Serial.available() == 0) {}
  return;
}

void print_buf_as_hexstring(uint8_t* data, uint8_t data_len) {
  Serial.print("0x");
  for (uint8_t i = 0; i < data_len; i++) {
    Serial.print(data[i], HEX);
  }
  return;
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
