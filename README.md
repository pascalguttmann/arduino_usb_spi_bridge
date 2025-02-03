# Arduino USB to SPI Bridge

Use your Arduino as a USB to SPI Bridge.

## Installation

1. Install [Arduino IDE][arduino-download]
2. Open the file `.usb_spi_bridge/usb_spi_bridge.ino`
3. Click "Upload" to compile and load the program to your connected ArduinoUno.  
   [Arduino Support - Upload a sketch in Arduino IDE][arduino-support-upload]

[arduino-download]: https://www.arduino.cc/en/software
[arduino-support-upload]: https://support.arduino.cc/hc/en-us/articles/4733418441116-Upload-a-sketch-in-Arduino-IDE

## Usage

To transfer data via the spi interface connect the peripheral to your Arduino
board and the Arduino to the PC. From the PC send an ascii encoded hexstring to the USB
interface of the Arduino to transfer. Transfers are separated by a single
newline character `\n`. The transfer is performed as soon as the newline
character is received and your data is transmitted with the below preconfigured settings.
The received data is sent from the Arduino to the PC via the USB interface
after the transfer is completed as an ascii encoded hexstring.

The length of the ascii hexstring determines the length of the SPI transfer and
must specify an equal number of nibbles to transfer. E.g.: `8f\n`, `1234\n`,
... but *not* `123\n`.

| Description | Value |
| :---------- | :---- |
| SPI_MODE | 0 (CPHA=0, CPOL=0) |
| $f_{SCKL}$ | $\approx 300 kHz$ |
| Bitorder | MSBit first |
| Byteorder | MSByte first |

### Example

1. Send `8f02\n` via USB from PC to Arduino
2. Arduino transfers bits `1000 1111 0000 0010` via SPI to peripheral  
   During the SPI transfer a response of the peripheral is received, e.g.: `1101 1010 0001 1010`
3. Receive `DA1A\n` via USB from Arduino to PC

## Pin out

| Pin description | Arduino pin number |
| :-- | :----------------- |
| $\overline{CS}$ (chipselect) | 10 |
| COPI (controller out, peripheral in) | 11 |
| CIPO (controller in, peripheral out) | 12 |
| SCKL (serial clock) | 13 |

## Configuration

### DEBUG_PRINT_HUMAN_READABLE

Preprocessor definition `DEBUG_PRINT_HUMAN_READABLE` can be used to change the
USB communication between the PC and the Arduino to be more human readable for
debug sessions.

| Value | Effect |
| :---- | :----- |
| FALSE | default, USB communication as described in the [usage section](#usage) |
| TRUE  | print human readable summary for every SPI transfer to USB interface |

Example output for sending `601234\n30fff0\n` with DEBUG_PRINT_HUMAN_READABLE == TRUE:

```
Setup finished.
TX: 0x601234 RX: 0xFFFFFF
TX: 0x30FFF0 RX: 0x000000
```
