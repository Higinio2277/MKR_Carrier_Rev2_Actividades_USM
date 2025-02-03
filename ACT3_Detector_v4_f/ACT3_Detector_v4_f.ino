#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier;

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for serial monitor to open

  carrier.noCase();
  if (!carrier.begin()) {
    Serial.println("Error");
    while (true); // Stop forever
  }
}


int r = 0, g = 0, b = 0;
unsigned long lastUpdate = 0;
unsigned long debounceDelay = 50;  // Retardo para el antirrebote en milisegundos
unsigned long lastDebounceTime = 0;
int dist = 0;
const unsigned long updateInterval = 100;
bool isMeasuringProximity = false;
bool rcolor = false;
void loop() {
  //carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setCursor(70, 60);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);
  carrier.display.println("LEYENDO");
  carrier.display.setCursor(70, 80);
  carrier.display.println("MOVIMIENTO");
  
  // Procesar gestos
  if (carrier.Light.gestureAvailable()) {
    int gesture = carrier.Light.readGesture();
    switch (gesture) {
      case GESTURE_LEFT:
        Serial.println("Detected UP gesture");
        carrier.display.fillScreen(ST77XX_BLACK);
        carrier.display.setCursor(70, 40);
        carrier.display.setTextColor(ST77XX_WHITE);
        carrier.display.setTextSize(2);
        carrier.display.println("\\_____/");
        carrier.display.setCursor(70, 60);
        carrier.display.println(" \\   /");
        carrier.display.setCursor(70, 80);
        carrier.display.println("  \\ /");
        carrier.display.setCursor(70, 100);
        carrier.display.println("   v");
        delay(1000);
        carrier.display.fillScreen(ST77XX_BLACK);
        break;
      case GESTURE_RIGHT:
        Serial.println("Detected DOWN gesture");
        carrier.display.fillScreen(ST77XX_BLACK);
        carrier.display.setCursor(70, 40);
        carrier.display.setTextColor(ST77XX_WHITE);
        carrier.display.setTextSize(2);
        carrier.display.println("   ^");
        carrier.display.setCursor(70, 60);
        carrier.display.println("  / \\");
        carrier.display.setCursor(70, 80);
        carrier.display.println(" /   \\");
        carrier.display.setCursor(70, 100);
        carrier.display.println("/_____\\");
        delay(1000);
        carrier.display.fillScreen(ST77XX_BLACK);
        break;
      case GESTURE_DOWN:
        Serial.println("Detected LEFT gesture");
        carrier.display.fillScreen(ST77XX_BLACK);
        carrier.display.setCursor(80, 80);
        carrier.display.setTextColor(ST77XX_WHITE);
        carrier.display.setTextSize(4);
        carrier.display.println("--->");
        delay(1000);
        carrier.display.fillScreen(ST77XX_BLACK);
                break;
      case GESTURE_UP:
        Serial.println("Detected RIGHT gesture");
        carrier.display.fillScreen(ST77XX_BLACK);
        carrier.display.setCursor(80, 80);
        carrier.display.setTextColor(ST77XX_WHITE);
        carrier.display.setTextSize(4);
        carrier.display.println(" <---");
        delay(1000);
        carrier.display.fillScreen(ST77XX_BLACK); // Borrar pantalla al salir
        break;
      default:
        break;
    }
  }

  // Leer colores si están disponibles
  if (carrier.Light.colorAvailable()) {
    carrier.Light.readColor(r, g, b);
  }

  carrier.Buttons.update();
  if (millis() - lastDebounceTime > debounceDelay) {
      if (carrier.Buttons.onTouchUp(TOUCH2)) {
        rcolor = !rcolor;  // Alternar estado de medición
        if (!rcolor) {
          carrier.display.fillScreen(ST77XX_BLACK); // Borrar pantalla al salir
        }
      }
      if (carrier.Buttons.onTouchUp(TOUCH4)) {
        isMeasuringProximity = !isMeasuringProximity;  // Alternar estado de medición
        if (!isMeasuringProximity) {
          carrier.display.fillScreen(ST77XX_BLACK); // Borrar pantalla al salir
          carrier.leds.clear();                    // Apagar LEDs
          carrier.leds.show();
        }
      }
      lastDebounceTime = millis();
    }
    
  // Medir proximidad si está activado
  if (isMeasuringProximity) {
    unsigned long currentMillis = millis();

    if (currentMillis - lastUpdate >= updateInterval) {
      lastUpdate = currentMillis;

      if (carrier.Light.proximityAvailable()) {
        dist = carrier.Light.readProximity();
      }

      // Actualizar pantalla y LEDs
      carrier.display.fillScreen(ST77XX_WHITE);
      carrier.display.setCursor(70, 100);
      carrier.display.setTextColor(ST77XX_BLACK);
      //carrier.display.setTextSize(2);
      carrier.display.println("MIDIENDO");
      carrier.display.setCursor(70, 120);
      carrier.display.println("PROXIMIDAD");
      carrier.display.setCursor(70, 140);
      carrier.display.println(dist);

      if (dist >= 200) {
        carrier.leds.setPixelColor(4, 0, 255, 0);  // Verde
        carrier.Buzzer.beep(200,100);
      } else if (dist >= 120 && dist < 200) {
        carrier.leds.setPixelColor(4, 255, 255, 0);  // Amarillo
        carrier.Buzzer.beep(500,100);
      } else if (dist < 120) {
        carrier.leds.setPixelColor(4, 255, 0, 0);  // Rojo
        carrier.Buzzer.beep(1000,100);
      }
      carrier.leds.show();
    }
  }

  // Medir COLOR si está activado
  if (rcolor) {
    unsigned long currentMillis = millis();

    if (currentMillis - lastUpdate >= updateInterval) {
      lastUpdate = currentMillis;

      if (carrier.Light.colorAvailable()) {
        carrier.Light.readColor(r, g, b);
      }
      carrier.display.fillScreen(ST77XX_WHITE);
      carrier.display.setCursor(90,150);
      carrier.display.setTextColor(ST77XX_RED);
      //carrier.display.setTextSize(2);
      carrier.display.println("R:" + String(r));
      carrier.display.setCursor(90,180);
      carrier.display.setTextColor(ST77XX_GREEN);
      carrier.display.println("G:" + String(g));
      carrier.display.setTextColor(ST77XX_BLUE);
      carrier.display.setCursor(90,210);
      carrier.display.println("B:" + String(b));
      // Verificar si el color detectado es blanco
      if (r > 200 && g > 200 && b > 200) {
        carrier.display.setCursor(70, 100);
        carrier.display.setTextColor(ST77XX_GREEN);
        //carrier.display.setTextSize(2);
        carrier.display.println("COLOR BLANCO");
        carrier.display.setCursor(70, 120);
        carrier.display.println("DETECTADO");
        delay(3000);  // Mostrar mensaje por un tiempo
        carrier.display.fillScreen(ST77XX_BLACK);
      } else {
        carrier.display.setCursor(70, 100);
        carrier.display.setTextColor(ST77XX_RED);
        //carrier.display.setTextSize(2);
        carrier.display.println("NO DETECTADO");
      }
    }
  }
}
