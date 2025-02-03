#include <Arduino_MKRIoTCarrier.h>

// Se incluye la librería para controlar el MKR IoT Carrier.

MKRIoTCarrier carrier;  // Constructor del carrier
const int pir = A6;     // Pin al que se conecta el sensor PIR (detección de movimiento)
const int sensorPin = A0;  // Pin al que se conecta el sensor de humedad del suelo
int sensorValueDry = 924;    // Valor de referencia para suelo seco en el sensor de humedad
int sensorValueWet = 320;    // Valor de referencia para suelo húmedo en el sensor de humedad
unsigned long debounceDelay = 100;  // Tiempo de antirrebote para botones táctiles
unsigned long previousMillis = 0;   // Almacena el tiempo anterior para intervalos
const long interval = 10000;        // Intervalo para actualizar datos de humedad (10 segundos)
const long interval1 = 2000;        // Intervalo para verificar el sensor PIR (2 segundos)

void setup() {
  Serial.begin(9600);  // Inicialización del puerto serie para depuración
  pinMode(pir, INPUT); // Configuración del pin del PIR como entrada
  pinMode(sensorPin, INPUT); // Configuración del pin del sensor de humedad como entrada
  carrier.begin();     // Inicialización del MKR IoT Carrier
  delay(1000);         // Retardo para asegurar la correcta inicialización del hardware
}

void senPir() { 
  // Función para manejar la detección de movimiento utilizando el sensor PIR.
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setCursor(70, 70);
  carrier.display.println("Activando...");
  
  while (true) {
    unsigned long currentMillis = millis(); // Tiempo actual en milisegundos

    // Verifica si ha pasado el intervalo establecido
    if (currentMillis - previousMillis >= interval1) {
      previousMillis = currentMillis;  // Actualiza el tiempo previo
      bool trigger = digitalRead(pir); // Lee el estado del sensor PIR
      Serial.print("\t Estado de movimiento: ");
      Serial.println(trigger);
      
      // Actualiza la pantalla con el estado de movimiento
      carrier.display.fillScreen(ST77XX_BLACK);
      carrier.display.setTextSize(2);
      carrier.display.setCursor(70, 70);
      carrier.display.println("MOVIMIENTO");
      carrier.display.setCursor(100, 100);
      carrier.display.println(trigger ? "SI" : "NO");

      // Mensaje adicional según el estado del sensor
      carrier.display.fillRect(70, 130, 160, 20, ST77XX_BLACK);
      carrier.display.setCursor(70, 130);
      carrier.display.println(trigger ? "Moviendose..." : "Tranquilo...");
    }

    carrier.Buttons.update(); // Actualiza el estado de los botones del carrier

    // Opción para salir de la función con entrada desde el puerto serie o botón táctil
    if (Serial.available() > 0 && Serial.read() == 'r') {
      break;
    }
    if (millis() - previousMillis > debounceDelay && carrier.Buttons.onTouchUp(TOUCH3)) {
      Serial.println("Saliendo con botón");
      break;
    }
  }
}

void senHumedad() {
  // Función para medir y mostrar la humedad del suelo.
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setCursor(70, 70);
  carrier.display.println("Activando...");

  while (true) {
    unsigned long currentMillis = millis(); // Tiempo actual en milisegundos

    // Verifica si ha pasado el intervalo establecido
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;  // Actualiza el tiempo previo
      int sensorValue = analogRead(sensorPin); // Lee el valor del sensor de humedad
      int moisturePercent = map(sensorValue, sensorValueDry, sensorValueWet, 0, 100);
      moisturePercent = constrain(moisturePercent, 0, 100); // Constrain asegura valores dentro del rango 0-100

      Serial.print("Humedad: ");
      Serial.print(moisturePercent);
      Serial.println("%");

      // Actualiza la pantalla con el porcentaje de humedad
      carrier.display.fillScreen(ST77XX_BLACK);
      carrier.display.setTextSize(3);
      carrier.display.setCursor(68, 90);
      carrier.display.println("HUMEDAD(%)");
      carrier.display.setCursor(100, 150);
      carrier.display.println(String(moisturePercent) + "%");
    }

    carrier.Buttons.update(); // Actualiza el estado de los botones del carrier

    // Opción para salir de la función con entrada desde el puerto serie o botón táctil
    if (Serial.available() > 0 && Serial.read() == 'r') {
      return;
    }
    if (millis() - previousMillis > debounceDelay && carrier.Buttons.onTouchUp(TOUCH3)) {
      Serial.println("Saliendo con botón");
      break;
    }
  }
}

void loop() {
  // Menú principal que permite seleccionar las funciones disponibles
  Serial.println("\nMENU\n1.Sensor de humedad\n2.Sensor de movimiento\n3.Salir\nElija una opcion:");
  
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setTextSize(3);
  carrier.display.setCursor(70, 60);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.println("MENU");
  carrier.display.setTextSize(2);
  carrier.display.setCursor(70, 100);
  carrier.display.println("1.Humedad suelo");
  carrier.display.setCursor(70, 120);
  carrier.display.println("2.Movimiento");
  carrier.display.setCursor(70, 140);
  carrier.display.println("3.Salir");

  while (true) { 
    carrier.Buttons.update(); // Actualiza los botones táctiles

    // Acciones según la opción seleccionada
    if (millis() - previousMillis > debounceDelay) {
      if (carrier.Buttons.onTouchUp(TOUCH1)) {
        Serial.println("Sensor de humedad activado.");
        senHumedad();
        break;
      } else if (carrier.Buttons.onTouchUp(TOUCH2)) {
        Serial.println("Sensor de movimiento activado.");
        senPir();
        break;
      } else if (carrier.Buttons.onTouchUp(TOUCH3)) {
        Serial.println("Saliendo del programa.");
        while (true);  // Bucle infinito para detener la ejecución
      }
      previousMillis = millis(); // Actualiza el tiempo previo
    }
  }
}
