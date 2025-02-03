#include <Arduino_MKRIoTCarrier.h> // Biblioteca para usar MKRIoTCarrier
MKRIoTCarrier carrier; // Instancia del carrier

void setup() {
  Serial.begin(9600); // Inicia la comunicación serial
  while (!Serial); // Espera a que se abra el monitor serial

  carrier.noCase(); // Desactiva la carcasa (para evitar restricciones físicas)
  if (!carrier.begin()) { // Inicializa el carrier y verifica errores
    Serial.println("Error");
    while (true); // Detiene la ejecución si hay error
  }
}

int r = 0, g = 0, b = 0; // Variables para los colores RGB
unsigned long lastUpdate = 0; // Última actualización
unsigned long debounceDelay = 50; // Retardo para el antirrebote
unsigned long lastDebounceTime = 0; // Última acción con antirrebote
int dist = 0; // Variable para proximidad
const unsigned long updateInterval = 100; // Intervalo de actualización
bool isMeasuringProximity = false; // Estado de medición de proximidad
bool rcolor = false; // Estado de medición de color

void loop() {
  // Mostrar mensaje inicial en pantalla
  carrier.display.setCursor(70, 60);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);
  carrier.display.println("LEYENDO");
  carrier.display.setCursor(70, 80);
  carrier.display.println("MOVIMIENTO");

  // Detectar y manejar gestos
  if (carrier.Light.gestureAvailable()) {
    int gesture = carrier.Light.readGesture();
    switch (gesture) {
      case GESTURE_LEFT: // Gesto "izquierda"
        Serial.println("Detected UP gesture");
        // Dibuja flecha hacia abajo
        carrier.display.fillScreen(ST77XX_BLACK);
        carrier.display.setCursor(70, 40);
        carrier.display.println("\\_____/");
        delay(1000); // Pausa
        carrier.display.fillScreen(ST77XX_BLACK);
        break;
      case GESTURE_RIGHT: // Gesto "derecha"
        Serial.println("Detected DOWN gesture");
        // Dibuja flecha hacia arriba
        carrier.display.fillScreen(ST77XX_BLACK);
        carrier.display.setCursor(70, 40);
        carrier.display.println("   ^");
        delay(1000);
        carrier.display.fillScreen(ST77XX_BLACK);
        break;
      case GESTURE_DOWN: // Gesto "abajo"
        Serial.println("Detected LEFT gesture");
        // Dibuja flecha hacia la derecha
        carrier.display.fillScreen(ST77XX_BLACK);
        carrier.display.setCursor(80, 80);
        carrier.display.println("--->");
        delay(1000);
        carrier.display.fillScreen(ST77XX_BLACK);
        break;
      case GESTURE_UP: // Gesto "arriba"
        Serial.println("Detected RIGHT gesture");
        // Dibuja flecha hacia la izquierda
        carrier.display.fillScreen(ST77XX_BLACK);
        carrier.display.setCursor(80, 80);
        carrier.display.println("<---");
        delay(1000);
        carrier.display.fillScreen(ST77XX_BLACK);
        break;
      default:
        break;
    }
  }

  // Leer colores si están disponibles
  if (carrier.Light.colorAvailable()) {
    carrier.Light.readColor(r, g, b); // Obtiene valores RGB
  }

  // Manejo de botones con antirrebote
  carrier.Buttons.update();
  if (millis() - lastDebounceTime > debounceDelay) {
    if (carrier.Buttons.onTouchUp(TOUCH2)) {
      rcolor = !rcolor; // Alterna estado de medición de color
      if (!rcolor) {
        carrier.display.fillScreen(ST77XX_BLACK); // Limpia pantalla al salir
      }
    }
    if (carrier.Buttons.onTouchUp(TOUCH4)) {
      isMeasuringProximity = !isMeasuringProximity; // Alterna medición de proximidad
      if (!isMeasuringProximity) {
        carrier.display.fillScreen(ST77XX_BLACK); // Limpia pantalla y LEDs al salir
        carrier.leds.clear();
        carrier.leds.show();
      }
    }
    lastDebounceTime = millis(); // Actualiza el tiempo de antirrebote
  }

  // Medir proximidad
  if (isMeasuringProximity) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastUpdate >= updateInterval) {
      lastUpdate = currentMillis;
      if (carrier.Light.proximityAvailable()) {
        dist = carrier.Light.readProximity(); // Obtiene la distancia
      }
      // Muestra proximidad y controla LEDs y zumbador
      carrier.display.fillScreen(ST77XX_WHITE);
      carrier.display.setCursor(70, 100);
      carrier.display.setTextColor(ST77XX_BLACK);
      carrier.display.println("PROXIMIDAD");
      carrier.display.println(dist);
      if (dist >= 200) {
        carrier.leds.setPixelColor(4, 0, 255, 0); // Verde
        carrier.Buzzer.beep(200, 100);
      } else if (dist >= 120) {
        carrier.leds.setPixelColor(4, 255, 255, 0); // Amarillo
        carrier.Buzzer.beep(500, 100);
      } else {
        carrier.leds.setPixelColor(4, 255, 0, 0); // Rojo
        carrier.Buzzer.beep(1000, 100);
      }
      carrier.leds.show();
    }
  }

  // Medir color
  if (rcolor) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastUpdate >= updateInterval) {
      lastUpdate = currentMillis;
      if (carrier.Light.colorAvailable()) {
        carrier.Light.readColor(r, g, b);
      }
      // Muestra valores RGB y detecta color blanco
      carrier.display.fillScreen(ST77XX_WHITE);
      carrier.display.setCursor(90, 150);
      carrier.display.setTextColor(ST77XX_RED);
      carrier.display.println("R:" + String(r));
      carrier.display.setTextColor(ST77XX_GREEN);
      carrier.display.println("G:" + String(g));
      carrier.display.setTextColor(ST77XX_BLUE);
      carrier.display.println("B:" + String(b));
      if (r > 200 && g > 200 && b > 200) {
        carrier.display.setCursor(70, 100);
        carrier.display.setTextColor(ST77XX_GREEN);
        carrier.display.println("COLOR BLANCO DETECTADO");
      } else {
        carrier.display.setCursor(70, 100);
        carrier.display.setTextColor(ST77XX_RED);
        carrier.display.println("NO DETECTADO");
      }
    }
  }
}
