// Librerias para el modulo de temperatura y humedad
#include <DHT.h>
#include <DHT_U.h>

// Libreria para la pantalla LCD
#include <LiquidCrystal.h>

// Librerias para el modulo RTC
#include <Wire.h>
#include <RTClib.h>

const int sensorPinDht = 13;     // Número del pin de entrada del sensor
int temperature;                 // Variable para guardar la temperatura
int humidity;                    // Variable para guardar la humedad

// Iniciamos la libreria del sensor, le pasamos el pin y tipo de sensor
DHT dht(sensorPinDht, DHT22);

// Inicializar la librería con los números de los pines de comunicación
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Inicializamos el objecto del modulo RTC
RTC_DS3231 rtc;

byte customChar[] = {
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

void setup() {
  Serial.begin(9600);                             // Abrimos un puerto para ver la salida por consola
  dht.begin();

  lcd.begin(16, 2);                               // Inicia un LCD 16x02 (columnas, filas)
  lcd.createChar(0, customChar);

  if (!rtc.begin()) {                             // Si el modulo no está conectado, retornamos un mensaje.
    Serial.println("Módulo RTC no encontrado!");
    while(1);                                     // Finalizamos el loop
  }
  rtc.adjust(DateTime(__DATE__, __TIME__));           // Configuramos la fecha y hora una sola vez
}

void loop() {
  DateTime date = rtc.now();                      // Obtenemos la fecha y hora del modulo
  Serial.println(date.toString("DDD, DD MMM YYYY hh:mm:ss"));

  temperature = dht.readTemperature();            // Obtenemos la temperatura
  humidity = dht.readHumidity();                  // Obtenemos la humedad
  
  lcd.setCursor(0, 0);                            // Coloca el cursor en la primera fila
  lcd.print("Temp.: ");
  lcd.print(temperature);
  lcd.write(byte(0));
  lcd.print("C");
  lcd.setCursor(0, 1);                            // Coloca el cursor en la segunda fila
  lcd.print("Humedad: ");
  lcd.print(humidity);
  lcd.print("%");

  delay(1000);
}
