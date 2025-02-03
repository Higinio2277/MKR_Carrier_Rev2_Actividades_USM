#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier;

// Constantes
const unsigned long UPDATE_INTERVAL = 100;
const unsigned long DEBOUNCE_DELAY = 50;

// Variables globales
int r = 0, g = 0, b = 0;
unsigned long lastUpdate = 0;
bool isMeasuringProximity = false;
bool isReadingColor = false;
int dist = 0;

// Funciones auxiliares
void clearScreen() {
  carrier.display.fillScreen(ST77XX_BLACK);
}

void setText(int x, int y, uint16_t color, int size, const char *text) {
  carrier.display.setCursor(x, y);
  carrier.display.setTextColor(color);
  carrier.display.setTextSize(size);
  carrier.display.println(text);
}

void toggleState(bool &state) {
  state = !state;
  if (!state) {
    clearScreen();
    carrier.leds.clear();
    carrier.leds.show();
  }
}

bool isWhite(int r, int g, int b) {
  return r > 200 && g > 200 && b > 200;
}

void measureProximity() {
  if (carrier.Light.proximityAvailable()) {
    dist = carrier.Light.readProximity();
  }

  updateDisplay("MIDIENDO PROXIMIDAD", 70, 100, ST77XX_WHITE);
  carrier.display.setCursor(70, 130);
  carrier.display.println(dist);

  if (dist >= 200) {
    carrier.leds.setPixelColor(4, 0, 255, 0); // Verde
  } else if (dist >= 120 && dist < 200) {
    carrier.leds.setPixelColor(4, 255, 255, 0); // Amarillo
  } else {
    carrier.leds.setPixelColor(4, 255, 0, 0); // Rojo
  }
  carrier.leds.show();
}

void displayColorDetection(int r, int g, int b) {
  char buffer[20];
  snprintf(buffer, sizeof(buffer), "R:%d G:%d B:%d", r, g, b);
  updateDisplay(buffer, 80, 200, ST77XX_BLUE);

  if (isWhite(r, g, b)) {
    updateDisplay("COLOR BLANCO DETECTADO", 70, 100, ST77XX_GREEN);
  } else {
    updateDisplay("NO DETECTADO", 70, 100, ST77XX_RED);
  }
}

void updateDisplay(const char *message, int x, int y, uint16_t color) {
  clearScreen();
  setText(x, y, color, 2, message);
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  carrier.noCase();
  if (!carrier.begin()) {
    Serial.println("Error inicializando MKR IoT Carrier");
    while (true);
  }
}

void loop() {
  // Mostrar pantalla inicial
  updateDisplay("LEYENDO MOVIMIENTO", 70, 100, ST77XX_WHITE);

  // Procesar gestos
  if (carrier.Light.gestureAvailable()) {
    int gesture = carrier.Light.readGesture();
    switch (gesture) {
      case GESTURE_UP:
        Serial.println("Detected UP gesture");
        break;
      case GESTURE_DOWN:
        Serial.println("Detected DOWN gesture");
        break;
      case GESTURE_LEFT:
        updateDisplay(" <---", 80, 80, ST77XX_WHITE);
        delay(1000);
        clearScreen();
        break;
      case GESTURE_RIGHT:
        Serial.println("Detected RIGHT gesture");
        break;
      default:
        break;
    }
  }

  // Actualizar botones
  carrier.Buttons.update();

  if (carrier.Buttons.onTouchUp(TOUCH2)) {
    toggleState(isReadingColor);
  }

  if (carrier.Buttons.onTouchUp(TOUCH4)) {
    toggleState(isMeasuringProximity);
  }

  // Medir proximidad si está activado
  if (isMeasuringProximity && (millis() - lastUpdate >= UPDATE_INTERVAL)) {
    lastUpdate = millis();
    measureProximity();
  }

  // Medir color si está activado
  if (isReadingColor && (millis() - lastUpdate >= UPDATE_INTERVAL)) {
    lastUpdate = millis();
    if (carrier.Light.colorAvailable()) {
      carrier.Light.readColor(r, g, b);
    }
    displayColorDetection(r, g, b);
  }
}
