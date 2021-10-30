#include <LiquidCrystal.h>

// Inicializar la librería con los números de los pines de comunicación
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  lcd.begin(16, 2);              // Inicia un LCD 16x02 (columnas, filas)
  lcd.setCursor(0, 0);           // Coloca el cursor en la primera fila
  lcd.print("Hola Mundo!");      // Imprime un mensaje en la pantalla LCD.
}

void loop() {
  lcd.setCursor(0, 1);           // Coloca el cursor en la segunda fila
  lcd.print(millis() / 1000);    // Imprime el número de segundos desde el reinicio
}
