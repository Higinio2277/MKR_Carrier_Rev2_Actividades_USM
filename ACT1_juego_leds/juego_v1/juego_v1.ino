#include <Arduino_MKRIoTCarrier.h>

MKRIoTCarrier carrier;

// Definir los colores
uint32_t colores[] = {
  carrier.leds.Color(255, 0, 0),   // Rojo
  carrier.leds.Color(255, 255, 0), // amarillo
  carrier.leds.Color(0, 255, 0),   // Verde
  carrier.leds.Color(0, 0, 255),   // Azul
  carrier.leds.Color(255, 255, 255), // Blanco
  carrier.leds.Color(255, 165, 0) // naranja
};

// Bandera para detener parpadeo de cada LED
bool detenerLED[5] = {false, false, false, false, false};
bool coloresCoinciden = false; // Variable para controlar si los colores ya coinciden

// Variables para puntaje y colores usados
int puntos = 0;
uint32_t coloresUsados[6] = {0,0,0, 0, 0, 0}; // Para evitar puntos con el mismo color

void mostrarPuntaje() {
  carrier.display.fillScreen(ST77XX_BLACK);  // Limpiar pantalla
  carrier.display.setCursor(80, 100);         // Posición inicial del texto
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(2);
  carrier.display.print("Puntaje: ");
  carrier.display.println(puntos );
}

void verificarColoresYSumarPuntos() {
  // Compara el color del primer LED con los demás
  bool mismosColores = true;
  uint32_t colorBase = carrier.leds.getPixelColor(0);

  for (int i = 1; i < 5; i++) {
    if (carrier.leds.getPixelColor(i) != colorBase) {
      mismosColores = false;
      break;
    }
  }
  // Verificar si el color ya ha sido utilizado
  bool colorUsado = false;
  for (int i = 0; i < 6; i++) {
    if (coloresUsados[i] == colorBase) {
      colorUsado = true;
      break;
    }
  }

  // Solo aumentar puntos si los colores coinciden y no se ha hecho aún, y el color no ha sido usado
  if (mismosColores && !coloresCoinciden && !colorUsado) {
    puntos++;  // Aumenta el puntaje
    coloresCoinciden = true; // Marcar que los colores ya coincidieron
    // Guardar el color utilizado
    coloresUsados[puntos - 2] = colorBase; // Guardar en la posición correspondiente
    Serial.print("Puntos: ");
    Serial.println(puntos);
    carrier.Buzzer.beep();
    carrier.Buzzer.beep(800, 20); // Sonido de 800 Hz durante 20 ms
    mostrarPuntaje(); // Mostrar el puntaje actualizado en el display
  }

  // Reseteamos la variable para permitir que se vuelva a aumentar el puntaje si los colores cambian
  if (!mismosColores) {
    coloresCoinciden = false;
  }
}

void reiniciarJuego() {
  puntos = 0;
  for (int i = 0; i < 6; i++) {
    coloresUsados[i] = 0; // Resetear colores usados
  }
  mostrarPuntaje();
}

void parpadeoConCambioDeColor(int delayTime) {
  static int colorIndex = 0;

  for (int j = 0; j < 5; j++) {
    if (!detenerLED[j]) {
      carrier.leds.setPixelColor(j, colores[colorIndex]);
    }
  }
  carrier.leds.show();
  delay(delayTime);

  carrier.Buttons.update();

  if (carrier.Buttons.onTouchDown(TOUCH0)) detenerLED[0] = !detenerLED[0];
  if (carrier.Buttons.onTouchDown(TOUCH1)) detenerLED[1] = !detenerLED[1];
  if (carrier.Buttons.onTouchDown(TOUCH2)) detenerLED[2] = !detenerLED[2];
  if (carrier.Buttons.onTouchDown(TOUCH3)) detenerLED[3] = !detenerLED[3];
  if (carrier.Buttons.onTouchDown(TOUCH4)) detenerLED[4] = !detenerLED[4];

  for (int j = 0; j < 5; j++) {
    if (!detenerLED[j]) {
      carrier.leds.setPixelColor(j, 0);
    }
  }
  carrier.leds.show();
  delay(delayTime);

  verificarColoresYSumarPuntos();
  colorIndex = (colorIndex + 1) % 6;
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
    parpadeoConCambioDeColor(100);
  } else {
    carrier.leds.clear(); // Apagar los LEDs
    carrier.display.fillScreen(ST77XX_BLACK);  // Limpiar pantalla
    carrier.display.setCursor(80, 100);         // Posición inicial del texto
    carrier.display.setTextColor(ST77XX_WHITE);
    carrier.display.setTextSize(3);
    carrier.display.print("YOU WIN :) ");
    carrier.Buzzer.beep(800, 100); // Sonido de 800 Hz durante 20 ms
    for (int i = 0; i <= 15; i++) {
    carrier.leds.setPixelColor(i, colores[i%6]); // Encender el LED en el índice i
    carrier.leds.show(); // Actualizar los LEDs para mostrar el cambio
    delay(100);
  }
    delay(3000);
    reiniciarJuego();  // Reinicia el juego después de mostrar la victoria
  }
}
