#include <Arduino_MKRIoTCarrier.h>

MKRIoTCarrier carrier;  // Constructor del carrier
const int pir = A6;     // Pin del sensor PIR
const int sensorPin = A0;  // Pin del sensor de humedad
int sensorValueDry = 924;    // Valor del sensor para suelo seco
int sensorValueWet = 320; // Valor del sensor para suelo húmedo
unsigned long debounceDelay = 100;  // Retardo para antirrebote
unsigned long previousMillis = 0; // Almacena el tiempo previo
const long interval = 10000; // Intervalo de 1000 ms (1 segundo)
const long interval1 = 2000; // Intervalo de 1000 ms (1 segundo)

void setup() {
  Serial.begin(9600);
  pinMode(pir, INPUT);
  pinMode(sensorPin, INPUT);
  carrier.begin();
  delay(1000);  // Esperar para asegurar la inicialización del carrier
}

void senPir() { 
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setCursor(70, 70);
  carrier.display.println("Activando...");
  while (true) {
    unsigned long currentMillis = millis(); // Obtiene el tiempo actual
    // Verifica si ha pasado el intervalo
    if (currentMillis - previousMillis >= interval1) {
      previousMillis = currentMillis; // Actualiza el tiempo previo
      bool trigger = digitalRead(pir);
      Serial.print("\t Estado de movimiento: ");
      Serial.println(trigger);
      carrier.display.fillScreen(ST77XX_BLACK);
      carrier.display.setTextColor(ST77XX_WHITE);
      carrier.display.setTextSize(2);
      carrier.display.setCursor(70, 70);
      carrier.display.println("MOVIMIENTO");
      carrier.display.setCursor(100, 100);
      carrier.display.println(trigger ? "SI" : "NO");

      if (trigger) {
        carrier.display.fillRect(70, 130, 160, 20, ST77XX_BLACK);
        carrier.display.setCursor(70, 130);
        carrier.display.println("Moviendose...");
      } else {
        carrier.display.fillRect(70, 130, 160, 20, ST77XX_BLACK);
        carrier.display.setCursor(70, 130);
        carrier.display.println("Tranquilo...");
      }
    }
    carrier.Buttons.update();
    if (Serial.available() > 0 && Serial.read() == 'r') {
      break;
    } 
    if (millis() - previousMillis > debounceDelay) {
      if (carrier.Buttons.onTouchUp(TOUCH3)) {
        Serial.println("Saliendo con boton");
        break;}
    }
  }
}

void senHumedad() {
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setCursor(70, 70);
  carrier.display.println("Activando...");
  while (true) {
    unsigned long currentMillis = millis(); // Obtiene el tiempo actual
    // Verifica si ha pasado el intervalo
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis; // Actualiza el tiempo previo
      int sensorValue = analogRead(sensorPin);
      int moisturePercent = map(sensorValue, sensorValueDry, sensorValueWet, 0, 100);
      moisturePercent = constrain(moisturePercent, 0, 100);

      Serial.print("Humedad: ");
      Serial.print(moisturePercent);
      Serial.println("%");

      carrier.display.fillScreen(ST77XX_BLACK);
      carrier.display.setTextColor(ST77XX_WHITE);
      carrier.display.setTextSize(3);
      carrier.display.setCursor(68, 90);
      carrier.display.println("HUMEDAD(%)");
      carrier.display.setCursor(100, 150);
      carrier.display.println(String(moisturePercent) + "%");
    }

    carrier.Buttons.update();
    if (Serial.available() > 0 && Serial.read() == 'r') {
      return;
    } if (millis() - previousMillis > debounceDelay) {
      if (carrier.Buttons.onTouchUp(TOUCH3)) {
      Serial.println("Saliendo con boton");
      break;}
    }
  }
}

void loop() {
  Serial.println("\nMENU\n1.Sensor de humedad\n2.Sensor de movimiento\n3.Salir\nElija una opcion:");
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setCursor(70, 60);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(3);
  carrier.display.println("MENU");
  carrier.display.setTextSize(2);
  carrier.display.setCursor(70, 100);
  carrier.display.println("1.Humedad suelo");
  carrier.display.setCursor(70, 120);
  carrier.display.println("2.Movimiento");
  carrier.display.setCursor(70, 140);
  carrier.display.println("3.Salir");

  while (true) { 
    carrier.Buttons.update();

    // Control por botones táctiles
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
        while (true);  // Salida infinita (detener loop)
      }
      previousMillis = millis();
    }

    
  }
}
