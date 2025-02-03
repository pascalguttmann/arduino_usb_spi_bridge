#include "SoftwareSPI.h"

#define FALSE (0)
#define TRUE (!FALSE)
#if TRUE == FALSE
#error "Preprocessor definition inconsistent: TRUE == FALSE"
#endif

#define LEN(x) (sizeof(x) / sizeof(x[0]))
#define UINT8_MAX (256U)

#define SERIAL_IN_BUF_SIZE (2*UINT8_MAX)
#define SPI_DATA_BUF_SIZE (UINT8_MAX)

#define DEBUG_PRINT_HUMAN_READABLE FALSE

const uint8_t cs_pin = 10;
extern const uint8_t copi_pin;
extern const uint8_t cipo_pin;
extern const uint8_t sck_pin;

uint8_t hexstring[SERIAL_IN_BUF_SIZE] = {};
uint32_t hexstring_len = SERIAL_IN_BUF_SIZE;
uint8_t data[SPI_DATA_BUF_SIZE] = {};
uint32_t data_len = SPI_DATA_BUF_SIZE;

void setup() {
  Serial.begin(115200);
  digitalWrite(cs_pin, HIGH);
  pinMode(cs_pin, OUTPUT);
  swspi_begin();
#if DEBUG_PRINT_HUMAN_READABLE == TRUE
  Serial.println("Setup finished.");
#endif /* #if DEBUG_PRINT_HUMAN_READABLE == TRUE */
}

void loop() {
  memset(hexstring, 0x00, SERIAL_IN_BUF_SIZE);
  hexstring_len = SERIAL_IN_BUF_SIZE;
  memset(data, 0x00, SPI_DATA_BUF_SIZE);
  data_len = SPI_DATA_BUF_SIZE;

  read_serial_line(hexstring, &hexstring_len);
  hexstring_to_uint8(hexstring, hexstring_len, data, &data_len);
  
#if DEBUG_PRINT_HUMAN_READABLE == TRUE
  Serial.print("TX: ");
  print_buf_as_hexstring(data, data_len);
#endif /* #if DEBUG_PRINT_HUMAN_READABLE == TRUE */

  digitalWrite(cs_pin, LOW);
  delayMicroseconds(1);
  swspi_transfer_buf(data, data_len);
  delayMicroseconds(1);
  digitalWrite(cs_pin, HIGH);
  
#if DEBUG_PRINT_HUMAN_READABLE == TRUE
  Serial.print("\tRX: ");
#endif /* #if DEBUG_PRINT_HUMAN_READABLE == TRUE */
  print_buf_as_hexstring(data, data_len);
  Serial.print("\n");
}

uint8_t ascii_hex_to_nibble(uint8_t ascii_hex){
  if (ascii_hex >= '0' && ascii_hex <= '9') {
    return ascii_hex - '0';
  }
  else if (ascii_hex >= 'a' && ascii_hex <= 'f') {
    return ascii_hex - 'a' + 10;
  }
  else if (ascii_hex >= 'A' && ascii_hex <= 'F') {
    return ascii_hex - 'A' + 10;
  }
  else {
    return 0xFF;
  }
}

/*
 * take as input an array `hexstring` of length `hexstring_len`. `hexstring_len` must be a even.
 * Each uint8_t contains a ascii hexadecimal digit, that shall be converted to its binary
 * representation (e.g. 'c' to 1100, '3' to 0011, etc).
 * The binary representation shall be stored in the output array `arr` and the amount of bytes 
 * written to `arr` shall be stored in `arr_len`.
 * `arr_len` specifies the maximum write size of `arr`.
 */
void hexstring_to_uint8(uint8_t* hexstring, uint32_t hexstring_len, uint8_t* arr, uint32_t* arr_len) {
  if (2 * (*arr_len) < hexstring_len) return;
  
  uint32_t bytes_written = 0;
  uint8_t out_byte = 0;
  uint8_t nibble = 0;

  for (uint32_t i = 0; i < hexstring_len; i++) {
      nibble = ascii_hex_to_nibble(hexstring[i]);

      if (i % 2 == 0) {
          out_byte = nibble << 4;
      } else {
          out_byte |= nibble;
          arr[bytes_written++] = out_byte;
      }
  }

  if (hexstring_len % 2 != 0) {
      arr[bytes_written++] = out_byte;
  }

  *arr_len = bytes_written;
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
#if DEBUG_PRINT_HUMAN_READABLE == TRUE
  Serial.print("0x");
#endif /* #if DEBUG_PRINT_HUMAN_READABLE == TRUE */
  for (uint8_t i = 0; i < data_len; i++) {
    if (data[i] < 16){
      Serial.print("0");
    }
    Serial.print(data[i], HEX);
  }
  return;
}
