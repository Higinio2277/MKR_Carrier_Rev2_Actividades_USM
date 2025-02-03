
#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier;
unsigned long debounceDelay = 50;  // Retardo para el antirrebote en milisegundos
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for serial monitor to open

  carrier.noCase();
  if (!carrier.begin()) {
    Serial.println("Error");
    while (true); // Stop forever
  }
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
  int r = 0, g = 0, b = 0;
  const int threshold = 200; // Umbral para considerar blanco

  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setCursor(70, 50);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);
  carrier.display.println("Muestre color blanco");

  while (true) {
    if (carrier.Light.colorAvailable()) {
      carrier.Light.readColor(r, g, b); // Leer valores RGB
    }

    // Mostrar valores RGB en pantalla
    carrier.display.fillRect(70, 100, 200, 60, ST77XX_BLACK); // Limpia el área del texto
    carrier.display.setCursor(70, 100);
    carrier.display.print("R: "); carrier.display.print(r);
    carrier.display.print(" G: "); carrier.display.print(g);
    carrier.display.print(" B: "); carrier.display.print(b);

    // Verificar si el color detectado es blanco
    if (r > threshold && g > threshold && b > threshold) {
      carrier.display.fillRect(70, 160, 200, 30, ST77XX_BLACK); // Limpia el área del mensaje
      carrier.display.setCursor(70, 160);
      carrier.display.setTextColor(ST77XX_GREEN);
      carrier.display.println("Blanco Detectado");
    } else {
      carrier.display.fillRect(70, 160, 200, 30, ST77XX_BLACK); // Limpia el área del mensaje
      carrier.display.setCursor(70, 160);
      carrier.display.setTextColor(ST77XX_RED);
      carrier.display.println("No Detectado");
    }

    // Salir si se presiona TOUCH2
    carrier.Buttons.update();
    if (carrier.Buttons.onTouchUp(TOUCH2)) {
      break;
    }

    delay(200); // Evitar sobrecarga de bucle
  }
}


void detectarProximidad() {
  int dist = 0;
  unsigned long lastUpdate = 0;
  const unsigned long updateInterval = 100; // Intervalo de actualización en ms

  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setCursor(70, 70);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);
  carrier.display.println("Proximidad Activa");

  while (true) {
    unsigned long currentMillis = millis();

    // Actualizar lectura de proximidad
    if (currentMillis - lastUpdate >= updateInterval) {
      lastUpdate = currentMillis;

      // Leer proximidad
      if (carrier.Light.proximityAvailable()) {
        dist = carrier.Light.readProximity();
      }

      // Mostrar distancia actualizada
      carrier.display.fillRect(70, 100, 200, 30, ST77XX_BLACK); // Limpia el área del texto
      carrier.display.setCursor(70, 100);
      carrier.display.print("Dist: ");
      carrier.display.println(dist);

      // Configurar LEDs y buzzer según la distancia
      if (dist >= 200) {
        carrier.leds.setPixelColor(4, 0, 255, 0);  // Verde
        carrier.Buzzer.noSound();
        carrier.Buzzer.beep(400, 400); // Tonalidad más baja
      } else if (dist >= 120 && dist < 200) {
        carrier.leds.setPixelColor(4, 255, 255, 0);  // Amarillo
        carrier.Buzzer.noSound();
        carrier.Buzzer.beep(600, 300); // Tonalidad media
      } else if (dist < 120) {
        carrier.leds.setPixelColor(4, 255, 0, 0);  // Rojo
        carrier.Buzzer.noSound();
        carrier.Buzzer.beep(800, 200); // Tonalidad más alta
      }

      carrier.leds.show(); // Refrescar LEDs
    }

    // Verificar botón TOUCH4 para salir
    carrier.Buttons.update();
    if (carrier.Buttons.onTouchUp(TOUCH4)) {
      carrier.Buzzer.noSound(); // Apagar el buzzer al salir
      break;
    }
  }
}


void menu(){
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

void loop() {
  menu();
}