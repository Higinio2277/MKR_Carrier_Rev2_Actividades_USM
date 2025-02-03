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
  // Se muestra el puntaje en el display y se actualiza con la variable puntos
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
  // Alternar el cambio de color y el encendido de los leds 
  static int colorIndex = 0; // Índice para seleccionar el color actual de la lista de colores
  static unsigned long tiempoAnterior[5] = {0, 0, 0, 0, 0}; // Arreglo para registrar el último tiempo de cambio para cada LED
  static int estadoParpadeo[5] = {0, 0, 0, 0, 0}; // Arreglo para almacenar el estado de encendido/apagado de cada LED
  // Iterar sobre los 5 LEDs
  for (int j = 0; j < 5; j++) {
    if (!detenerLED[j]) { // Verificar si el LED no está detenido
      unsigned long tiempoActual = millis(); // Obtener el tiempo actual en milisegundos
      // Verificar si ha transcurrido un tiempo aleatorio entre 200 y 600 ms
      if (tiempoActual - tiempoAnterior[j] >= random(200, 600)) {
        estadoParpadeo[j] = !estadoParpadeo[j]; // Alternar el estado de encendido/apagado del LED
        tiempoAnterior[j] = tiempoActual; // Actualizar el tiempo de cambio del LED
        if (estadoParpadeo[j]) { 
          carrier.leds.setPixelColor(j, colores[colorIndex]); // Encender el LED con el color actual
        } else {
          carrier.leds.setPixelColor(j, 0); // Apagar el LED
        }
      }
    }
  }
  carrier.leds.show();

  carrier.Buttons.update();
  // Detectar si se ha presionado alguno de los botones táctiles y alternar el estado de detenerLED para el LED correspondiente
  if (carrier.Buttons.onTouchDown(TOUCH0)) detenerLED[0] = !detenerLED[0];
  if (carrier.Buttons.onTouchDown(TOUCH1)) detenerLED[1] = !detenerLED[1];
  if (carrier.Buttons.onTouchDown(TOUCH2)) detenerLED[2] = !detenerLED[2];
  if (carrier.Buttons.onTouchDown(TOUCH3)) detenerLED[3] = !detenerLED[3];
  if (carrier.Buttons.onTouchDown(TOUCH4)) detenerLED[4] = !detenerLED[4];

  verificarColoresYSumarPuntos();
  colorIndex = (colorIndex + 1) % 3; // Cambia al siguiente color
  delay(delayTime);
}

void setup() {
  Serial.begin(9600); //Se inicializa la comunicacion 
  carrier.begin(); //Se inicializa el carrier 
  carrier.leds.setBrightness(10); //Ajuste de brillo de los leds
  carrier.display.fillScreen(ST77XX_BLACK); //Limpia el display
  mostrarPuntaje(); // Se muestra el menu
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
