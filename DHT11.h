#ifndef DHT11_H 
#define DHT11_H

#include <Arduino.h>

//#define DHT11_DEBUG

class DHT11 {
   public:
      DHT11(uint8_t);
      uint8_t dht11_pin;
      void set_pin_input();

      const uint8_t dht11_bits = 40;
      #if F_CPU == 1000000L
      const uint8_t long_cycle_threshold = 2;  // low threshold for 1 MHz
      # else
      const uint8_t long_cycle_threshold = 35; // tested with both 8 MHz and 16 MHz
      #endif

      uint8_t humidity;
      uint8_t temperature;

      void begin();
      void start();
      bool read();

      bool dht11_pin_state();
      uint8_t count_high_pulse_cycles();
      bool dht11_wait_high_signal();
};

#endif
