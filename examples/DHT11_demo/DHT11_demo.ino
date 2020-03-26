#include <DHT11.h>

DHT11 dht11{2}; // pin 2
// port manipulation:
// attiny           : PORTB
// UNO / atmega328p : PORTD (pin 0 to 7)  

void setup() {
  Serial.begin(9600);

  dht11.begin();
}

void loop() {
  Serial.print("reading dht11.. ");
  if (dht11.read()){
    Serial.print("success: humidity: ");
    Serial.print(dht11.humidity);
    Serial.print(", temperatur: ");
    Serial.println(dht11.temperature);
  } else {
    Serial.println("failed");
  }
  
  delay(4000);
}
