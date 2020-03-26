#include <DHT11.h>

DHT11::DHT11(uint8_t dht11_pin): dht11_pin{dht11_pin} {
}

void DHT11::begin(){
   #ifdef DHT11_DEBUG
   Serial.print("DHT11 setting pinMode: ");
   Serial.println(dht11_pin);  
   #endif

   set_pin_input();
}

void DHT11::start(){
   // wake up dht11: pull low 18 ms 
   //pinMode(dht11_pin, OUTPUT);
   //digitalWrite(dht11_pin, 0);

   #if defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
   DDRB |= 1<<dht11_pin;     // output
   PORTB &= ~(1<<dht11_pin); // low
   #else
   DDRD |= 1<<dht11_pin;     // output
   PORTD &= ~(1<<dht11_pin); // low
   #endif

   delay(18);
   // signal rises quicker by driving high, but it also works without doing this:
   //digitalWrite(dht11_pin, 1); 
   
   set_pin_input();
}

void DHT11::set_pin_input(){
   //pinMode(dht11_pin, INPUT_PULLUP);  

   #if defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
   DDRB &= ~(1<<dht11_pin); // input
   PORTB |= 1<<dht11_pin;   // pullup
   #else
   DDRD &= ~(1<<dht11_pin); // input
   PORTD |= 1<<dht11_pin;   // pullup
   #endif 
}

bool DHT11::read(){

   uint8_t dht11_data[5];

   #ifdef DHT11_DEBUG
   uint8_t bit_cycles[dht11_bits];
   memset(bit_cycles, 0, sizeof(bit_cycles));
   #endif DHT11_DEBUG

   start();

   #if F_CPU == 1000000L
   delayMicroseconds(170);
   # else
   delayMicroseconds(200);
   #endif

   noInterrupts();

   for (uint8_t i = 0; i < dht11_bits; i++){
      uint8_t high_pulse_cycles = count_high_pulse_cycles();
      
      uint8_t current_byte = i / 8; 
      
      // pulse length debugging
      #ifdef DHT11_DEBUG
      bit_cycles[i] = high_pulse_cycles; 
      #endif DHT11_DEBUG
                  
      // always shift all bits by one
      dht11_data[current_byte] <<= 1;
      
      // add the bit if it was a long pulse
      if (high_pulse_cycles > long_cycle_threshold){
         dht11_data[current_byte]++;
      }
   }

   interrupts();

   #ifdef DHT11_DEBUG
   Serial.println();
   for (uint8_t i = 0; i < dht11_bits; i++){
      Serial.print(i);
      Serial.print(": ");
      Serial.print(bit_cycles[i]);
      Serial.println();
   }
   #endif DHT11_DEBUG

   // checksum
   if (((dht11_data[0] + dht11_data[1] + dht11_data[2] + dht11_data[3]) & 0xFF) == dht11_data[4]){
      humidity    = dht11_data[0];
      temperature = dht11_data[2];
      return true;
   }

   return false;
}

bool DHT11::dht11_pin_state() {
   // when using digitalRead() the number of captured cycles decreases: long_cycle_threshold must be lowered
   //return digitalRead(dht11_pin);

   // use better performing PIN register lookup by default: 
   #if defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
      // attiny only has PORTB
      return PINB & 1<<dht11_pin;
   #else
      // atmega: pins 0 to 7: PORTD 
      return PIND & 1<<dht11_pin;
   #endif
}

bool DHT11::dht11_wait_high_signal(){
   // wait for pin to become high

   // ends when i overflows
   for (uint8_t i = 1; i; i++){
       if (dht11_pin_state()){
          return true;
       }
   }

   return false;
}

uint8_t DHT11::count_high_pulse_cycles(){
   // counts number of high cycles
   uint8_t high_cycles = 0;
   
   // returns 0 when no high signal comes within 255 cycles
   if (!dht11_wait_high_signal()){
      return 0;
   }

   // ends when i overflows
   for (uint8_t i = 1; i && dht11_pin_state(); i++, high_cycles++){ }

   return high_cycles;
}

