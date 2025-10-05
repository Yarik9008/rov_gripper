#include <Arduino.h>

// Определение пина для светодиода (PC13 - встроенный светодиод на STM32F103 BluePill)
#define LED_PIN PC13


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