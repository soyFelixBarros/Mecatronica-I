#include <SoftwareSerial.h>
SoftwareSerial BTSerial(9, 8);

// Librerias para el modulo de temperatura y humedad
#include <DHT.h>
#include <DHT_U.h>

// Libreria para la pantalla LCD
#include <LiquidCrystal.h>

// Librerias para el modulo RTC
#include <Wire.h>
#include <RTClib.h>

// Constantes
const int relayPin = 6;
const int sensorPinDht = 13;     // Número del pin de entrada del sensor
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // Númerosde pines del display
const byte customChar[] = {      // Definimos simbolo grado centígrado °
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

// Variables
int temperature;                 // Variable para guardar la temperatura
int humidity;                    // Variable para guardar la humedad
bool relayToggle = false;       // Definimos el relé como apagado

// Iniciamos la libreria del sensor, le pasamos el pin y tipo de sensor
DHT dht(sensorPinDht, DHT22);

// Inicializar la librería con los números de los pines de comunicación
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Inicializamos el objecto del modulo RTC
RTC_DS3231 rtc;
DateTime date;

void setup() {
  Serial.begin(9600);                             // Abrimos un puerto para ver la salida por consola
  BTSerial.begin(9600);
  pinMode(relayPin, OUTPUT);                      // Configura el pin del relé como salida. 
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
  //date = rtc.now();                                   // Obtenemos la fecha y hora del modulo
 
  printDisplay();

  bluetooth();

//  if(date.hour() == 15 && date.minute() == 01){
//    digitalWrite(relayPin, HIGH);
//  } else if(date.hour() == 15 && date.minute() == 02){
//    digitalWrite(relayPin, LOW);
//  }
  //delay(1000);
}

// Función para imprimir en el display
void printDisplay() {
  temperature = getTemperature();                // Obtenemos la temperatura
  humidity = getHumidity();                   // Obtenemos la humedad
  
  lcd.setCursor(0, 0);                                // Coloca el cursor en la primera fila
  lcd.print("  Temp.: ");
  lcd.print(temperature);
  lcd.write(byte(0));
  lcd.print("C");
  lcd.setCursor(0, 1);                                // Coloca el cursor en la segunda fila
  lcd.print("  Humedad: ");
  lcd.print(humidity);
  lcd.print("%");
}

int getTemperature() {
  return dht.readTemperature(); 
}

int getHumidity() {
  return dht.readHumidity(); 
}

DateTime getDate() {
  return rtc.now();
  //return date.timestamp(DateTime::TIMESTAMP_FULL); 
}

// Función para capturar y recibir datos
void bluetooth() {
  if (BTSerial.available()) {
    char data = BTSerial.read();
    if (data == 's') {
      sendDate();
      sendTemperature();
      sendHumedity();
      sendRelay();
    }
    if (data == 'r') {
      relayToggleOnOff();
      sendRelay();
    }
  }
  if (Serial.available()) {
    BTSerial.write(Serial.read());
  }
}

// Método para enviar la temperatura ambiente por bluetooth
void sendTemperature() {
  int temperature = getTemperature();
  char buffer[10] = " ";
  char* format = "Temperatura: %i°C\n";
  sprintf(buffer, format, temperature);
    
  BTSerial.write(buffer);
}

// Método para enviar la humedad ambiente por bluetooth
void sendHumedity() {
  int humidity = getHumidity();
  char buffer[10] = " ";
  char* format = "Humedad: %i%%\n";
  sprintf(buffer, format, humidity);
    
  BTSerial.write(buffer);
}

// Método para enviar la fecha y hora por bluetooth
void sendDate() {
  DateTime date = getDate();
  char buffer[10] = " ";
  char* format = "Fecha: %i/%i/%i\nHora: %i:%i\n";
  sprintf(buffer, format, date.day(), date.month(), date.year(), date.hour(), date.minute());
  BTSerial.write(buffer);
}

// Método para activar o desactivar el relé
void relayToggleOnOff() {
  relayToggle = !relayToggle;
  if (relayToggle) {
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }
}

// Método para retornar el estado del relé
void sendRelay() {
  if(relayToggle) {
    BTSerial.write("Motor: encendido\n");
  } else {
    BTSerial.write("Motor: apagado\n");
  }
}
