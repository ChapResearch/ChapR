
#ifndef VDIPSPI_h
#define VDIPSPI_h

#include <Arduino.h>
#include <stdint.h>

class VDIPSPI
{
 public:

  VDIPSPI(uint8_t clockPin, uint8_t mosiPin, uint8_t misoPin, uint8_t csPin);

  bool recv(unsigned char *data, bool statusByte = false);
  bool send(unsigned char data, bool statusByte = false);

 private:
  uint8_t   _clockPin;
  uint8_t   _mosiPin;
  uint8_t   _misoPin;
  uint8_t   _csPin;

  void _header(bool read, bool statusByte);
  bool _statusbit();

};

#endif /* VDIPSPI_h */





