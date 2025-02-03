#include <Arduino_MKRIoTCarrier.h>

MKRIoTCarrier carrier;

// Definir los colores (solo 3: rojo, verde, azul)
uint32_t colores[] = {
  carrier.leds.Color(255, 0, 0),   // Rojo
  carrier.leds.Color(0, 255, 0),   // Verde
  carrier.leds.Color(0, 0, 255)    // Azul
};

// Bandera para detener parpadeo de cada LED
bool detenerLED[5] = {false, false, false, false, false};

// Variables para puntaje y colores usados
int puntos = 0;
uint32_t coloresUsados[3] = {0, 0, 0}; // Para evitar puntos con el mismo color

void mostrarPuntaje() {
  carrier.display.fillScreen(ST77XX_BLACK);  // Limpiar pantalla
  carrier.display.setCursor(80, 100);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);
  carrier.display.print("Puntaje: ");
  carrier.display.println(puntos);
}

void verificarColoresYSumarPuntos() {
  // Verificar que todos los LEDs estén detenidos
  bool todosDetenidos = true;
  for (int i = 0; i < 5; i++) {
    if (!detenerLED[i]) {
      todosDetenidos = false;
      break;
    }
  }

  if (todosDetenidos) {
    // Verificar que todos los LEDs tienen el mismo color
    bool mismosColores = true;
    uint32_t colorBase = carrier.leds.getPixelColor(0);
    for (int i = 1; i < 5; i++) {
      if (carrier.leds.getPixelColor(i) != colorBase) {
        mismosColores = false;
        break;
      }
    }

    // Verificar si el color ya fue usado
    bool colorUsado = false;
    for (int i = 0; i < 3; i++) {
      if (coloresUsados[i] == colorBase) {
        colorUsado = true;
        break;
      }
    }

    // Si los LEDs cumplen con las condiciones, aumentar el puntaje
    if (mismosColores && !colorUsado) {
      puntos++;
      coloresUsados[puntos - 1] = colorBase; // Guardar el color usado
      Serial.print("Puntos: ");
      Serial.println(puntos);
      carrier.Buzzer.beep();
      mostrarPuntaje();
    }
  }
}

void reiniciarJuego() {
  puntos = 0;
  for (int i = 0; i < 3; i++) {
    coloresUsados[i] = 0; // Resetear colores usados
  }
  mostrarPuntaje();
}

void parpadeoConCambioDeColor(int delayTime) {
  static unsigned long tiempoAnterior[5] = {0, 0, 0, 0, 0};
  static int colorIndex[5] = {0, 0, 0, 0, 0};

  // Parpadeo aleatorio para LEDs no detenidos
  for (int j = 0; j < 5; j++) {
    if (!detenerLED[j]) {
      unsigned long tiempoActual = millis();
      if (tiempoActual - tiempoAnterior[j] >= random(200, 600)) {
        colorIndex[j] = random(0, 3); // Elegir color aleatorio
        carrier.leds.setPixelColor(j, colores[colorIndex[j]]);
        tiempoAnterior[j] = tiempoActual;
      }
    }
  }
  carrier.leds.show();

  // Actualizar botones
  carrier.Buttons.update();
  if (carrier.Buttons.onTouchDown(TOUCH0)) detenerLED[0] = !detenerLED[0];
  if (carrier.Buttons.onTouchDown(TOUCH1)) detenerLED[1] = !detenerLED[1];
  if (carrier.Buttons.onTouchDown(TOUCH2)) detenerLED[2] = !detenerLED[2];
  if (carrier.Buttons.onTouchDown(TOUCH3)) detenerLED[3] = !detenerLED[3];
  if (carrier.Buttons.onTouchDown(TOUCH4)) detenerLED[4] = !detenerLED[4];

  verificarColoresYSumarPuntos();
  delay(delayTime);
}

void setup() {
  Serial.begin(9600);
  carrier.begin();
  carrier.leds.setBrightness(10);
  carrier.display.fillScreen(ST77XX_BLACK);
  mostrarPuntaje();
}

void loop() {
  if (puntos < 3) {
    parpadeoConCambioDeColor(50); // LEDs parpadean más rápido
  } else {
    carrier.leds.clear(); // Apagar los LEDs
    carrier.display.fillScreen(ST77XX_BLACK);  // Limpiar pantalla
    carrier.display.setCursor(80, 100);
    carrier.display.setTextColor(ST77XX_WHITE);
    carrier.display.setTextSize(3);
    carrier.display.print("YOU WIN :) ");
    carrier.Buzzer.beep(800, 100);
    for (int i = 0; i <= 15; i++) {
      carrier.leds.setPixelColor(i, colores[i % 3]); // Ciclo con los 3 colores
      carrier.leds.show();
      delay(100);
    }
    delay(3000);
    reiniciarJuego(); // Reinicia el juego después de mostrar la victoria
  }
}
