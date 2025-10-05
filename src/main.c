#include <Arduino.h>

// Определение пина для светодиода (PB5 - пин 13 на STM8S103)
#define LED_PIN PB5

void setup() {
  // Настройка пина светодиода как выход
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // Включить светодиод
  digitalWrite(LED_PIN, HIGH);
  delay(500);  // Пауза 500 мс
  
  // Выключить светодиод
  digitalWrite(LED_PIN, LOW);
  delay(500);  // Пауза 500 мс
}