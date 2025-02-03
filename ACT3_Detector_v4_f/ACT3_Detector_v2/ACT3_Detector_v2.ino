
#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier;
unsigned long debounceDelay = 50;  // Retardo para el antirrebote en milisegundos
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for serial monitor to open
  carrier.noCase();
  carrier.begin();
}

void arriba() {
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
}

void abajo() {
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
}

void izq() {
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setCursor(80, 80);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);
  carrier.display.println(" <---");
  delay(1000);
}

void der() {
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setCursor(80, 80);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);
  carrier.display.println("--->");
  delay(1000);
}
void detectarBlanco() {

  // Esperar a que se presione TOUCH2 nuevamente para volver al estado base
  while (true) {
    
    int r = 0, g = 0, b = 0;
    carrier.display.fillScreen(ST77XX_BLACK);
    carrier.display.setCursor(70, 100);
    carrier.display.setTextColor(ST77XX_WHITE);
    carrier.display.setTextSize(2);
    carrier.display.println("MUESTRE COLOR BLANCO");
    
    // Espera para que el usuario muestre el color blanco
    delay(1000);

    if (carrier.Light.colorAvailable()) {
      carrier.Light.readColor(r, g, b);
    }

    // Verificar si el color detectado es blanco
    if (r > 200 && g > 200 && b > 200) {
      carrier.display.fillScreen(ST77XX_BLACK);
      carrier.display.setCursor(70, 100);
      carrier.display.setTextColor(ST77XX_GREEN);
      carrier.display.setTextSize(2);
      carrier.display.println("COLOR BLANCO DETECTADO");
      delay(1500);  // Mostrar mensaje por un tiempo
    } else {
      carrier.display.fillScreen(ST77XX_BLACK);
      carrier.display.setCursor(70, 100);
      carrier.display.setTextColor(ST77XX_RED);
      carrier.display.setTextSize(2);
      carrier.display.println("NO DETECTADO");
    }
    carrier.Buttons.update();
    if (carrier.Buttons.onTouchUp(TOUCH2)) {
      break;  // Volver al estado base si se presiona TOUCH2
    }
  
    // Pequeña espera para evitar sobrecargar el procesador
  }
}

void detectarProximidad() {
  int dist = 0;
  unsigned long lastUpdate = 0;
  const unsigned long updateInterval = 100; // Intervalo de actualización en milisegundos
 
  while (true) {
    unsigned long currentMillis = millis();

    // Verificar si es hora de actualizar la lectura de proximidad
    if (currentMillis - lastUpdate >= updateInterval) {
      lastUpdate = currentMillis;

      // Check if a proximity reading is available
      if (carrier.Light.proximityAvailable()) {
        dist = carrier.Light.readProximity();
      }

      // Actualizar la pantalla solo si es necesario
      carrier.display.fillRect(70, 100, 100, 60, ST77XX_BLACK); // Borrar solo la parte donde se muestra la distancia
      carrier.display.setCursor(70, 100);
      carrier.display.setTextColor(ST77XX_WHITE);
      carrier.display.setTextSize(2);
      carrier.display.println("MIDIENDO PROXIMIDAD");
      carrier.display.setCursor(70, 130);
      carrier.display.println(dist);

      // Verificar proximidad y configurar LEDs y Buzzer
      if (dist >= 200) {
        carrier.leds.setPixelColor(4, 0, 255, 0);  // Verde
        carrier.Buzzer.beep(400, 400);
      } else if (dist >= 120 && dist < 200) {
        carrier.leds.setPixelColor(4, 255, 255, 0);  // Amarillo
        carrier.Buzzer.beep(600, 300);
      } else if (dist < 120) {
        carrier.leds.setPixelColor(4, 255, 0, 0);  // Rojo
        carrier.Buzzer.beep(800, 200);
      }
      carrier.leds.clear();
      carrier.leds.show();
    }
    // Salir del bucle si se presiona el botón TOUCH4
    carrier.Buttons.update(); // Actualizar el estado de los botones
    if (carrier.Buttons.onTouchUp(TOUCH4)) {
      break; // Salir del bucle
    }
  }
}

void menu(){
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setCursor(70, 100);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);
  carrier.display.println("LEYENDO MOVIMIENTO");
  while (true) {
    // check if a gesture reading is available
    if (carrier.Light.gestureAvailable()) {
      int gesture = APDS.readGesture();
      switch (gesture) {
        case UP:
          carrier.Buzzer.beep(800,100);
          abajo();
          Serial.println("Arriba");
          break;
        case DOWN:
          carrier.Buzzer.beep(800,100);
          arriba();
          Serial.println("Abajo");
          break;
        case LEFT:
          carrier.Buzzer.beep(800,100);
          der();
          Serial.println("Izuierda");
          break;
        case RIGHT:
          carrier.Buzzer.beep(800,100);
          izq();
          Serial.println("Derecha");
          break;
        default:
          // ignore
          break;
      }
      
    } 
    carrier.Buttons.update();
    if (millis() - lastDebounceTime > debounceDelay) {
      if (carrier.Buttons.onTouchUp(TOUCH2)) {detectarBlanco();}
      if (carrier.Buttons.onTouchUp(TOUCH4)) {detectarProximidad();}
      lastDebounceTime = millis();
    }
  }
}

void loop() {
  //menu();
  
  while (true) {
    carrier.display.fillScreen(ST77XX_BLACK);
    carrier.display.setCursor(70, 100);
    carrier.display.setTextColor(ST77XX_WHITE);
    carrier.display.setTextSize(2);
    carrier.display.println("LEYENDO MOVIMIENTO");
    // check if a gesture reading is available
    if (carrier.Light.gestureAvailable()) {
      int gesture = APDS.readGesture();
      switch (gesture) {
        case UP:
          carrier.Buzzer.beep(800,100);
          abajo();
          Serial.println("Arriba");
          break;
        case DOWN:
          carrier.Buzzer.beep(800,100);
          arriba();
          Serial.println("Abajo");
          break;
        case LEFT:
          carrier.Buzzer.beep(800,100);
          der();
          Serial.println("Izuierda");
          break;
        case RIGHT:
          carrier.Buzzer.beep(800,100);
          izq();
          Serial.println("Derecha");
          break;
        default:
          // ignore
          break;
      }
      
    } 
    carrier.Buttons.update();
    if (millis() - lastDebounceTime > debounceDelay) {
      if (carrier.Buttons.onTouchUp(TOUCH2)) {detectarBlanco();}
      if (carrier.Buttons.onTouchUp(TOUCH4)) {detectarProximidad();}
      lastDebounceTime = millis();
    }
  }
}