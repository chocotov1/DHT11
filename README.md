# DHT11

Arduino library to read temperatur and humidity of DHT11 sensor. This library improves on my previous Arduino sketch based solutions
[DHT11Light](https://github.com/chocotov1/DHT11Light) and [DHT11Tiny](https://github.com/chocotov1/DHT11Tiny).

## Working principle and 1 MHz challenge solved
The DHT11 sends a sequence of long and short high signal pulses. Long means 1, short means 0.

This library is able to get correct readings when running the AVR chip at 1 MHz. In my previous sketches this proved to be a challenge.

## 1 MHz optimizations
- turn off interrupts during reading
- count the number of cycles of the high signal in loop cycles
- poll pin state with register (PINB / PIND) lookup instead of digitalRead()

_General note on running the chip at 1 MHz: To save power it's wiser to run the chip faster (for instance 8 MHz) and use the sleep modes whenever possible._

## Debugging
Debugging of the registered high signal lengths can be turned by on uncommenting this line in DHT11.h:
```
#define DHT11_DEBUG
```
