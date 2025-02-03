#include <Arduino_MKRIoTCarrier.h>  // Incluye la librería para usar el MKR IoT Carrier.

MKRIoTCarrier carrier;  // Declara un objeto para manejar el Carrier.

unsigned long debounceDelay = 50;  // Define un retraso para evitar el efecto de rebote en los botones (50 ms).
unsigned long lastDebounceTime = 0;  // Almacena el tiempo de la última interacción con un botón.

void setup() {
  Serial.begin(9600);  // Inicializa la comunicación serial a 9600 bps.
  carrier.begin();  // Inicializa el Carrier.
  carrier.noCase();  // Configura el Carrier para que no use una carcasa física.
  delay(1000);  // Espera un segundo para asegurarse de que el Carrier esté listo.
  Serial.println("Temperature(C)\tHumidity\tPressure(KPa)\tCO2(ppm)");  
  // Muestra un encabezado para datos en la consola serial.
}

void esperarBoton(int currentButton) {
  // Espera hasta que un botón diferente al actual sea presionado.
  while (true) {
    carrier.Buttons.update();  
    // Actualiza el estado de los botones táctiles.

    if (millis() - lastDebounceTime > debounceDelay) {  
      // Verifica si ha pasado suficiente tiempo desde la última interacción.
      if ((carrier.Buttons.onTouchUp(TOUCH0) && currentButton != TOUCH0) ||
          (carrier.Buttons.onTouchUp(TOUCH1) && currentButton != TOUCH1) ||
          (carrier.Buttons.onTouchUp(TOUCH2) && currentButton != TOUCH2) ||
          (carrier.Buttons.onTouchUp(TOUCH3) && currentButton != TOUCH3) ||
          (carrier.Buttons.onTouchUp(TOUCH4) && currentButton != TOUCH4)) {
        lastDebounceTime = millis();  
        // Actualiza el tiempo del último rebote.
        menu();  
        // Regresa al menú principal.
        return;
      }
    }
    delay(50);  // Pequeño retraso para evitar comprobaciones frecuentes.
  }
}

void temperatura() {
  // Muestra la temperatura en Celsius, Fahrenheit y Kelvin.
  while (true) {
    float temperature = carrier.Env.readTemperature();  // Lee la temperatura en Celsius.
    float tempF = temperature * 9.0 / 5.0 + 32.0;  // Convierte la temperatura a Fahrenheit.
    float tempK = temperature + 273.15;  // Convierte la temperatura a Kelvin.
    carrier.display.fillScreen(ST77XX_BLACK);  
    // Limpia la pantalla.
    carrier.display.setCursor(70, 40);  
    carrier.display.setTextColor(ST77XX_WHITE);  
    carrier.display.setTextSize(2);  
    carrier.display.println("TEMPERATURA: ");
    carrier.display.setCursor(70, 100);
    carrier.display.setTextColor(ST77XX_RED);
    carrier.display.print("T_Celsius:");
    carrier.display.setCursor(70, 145);
    carrier.display.print("T_Fahrenheit:");
    carrier.display.setCursor(70, 190);
    carrier.display.print("T_Kelvin:");
    carrier.display.setTextColor(ST77XX_WHITE);
    carrier.display.setCursor(80, 120);
    carrier.display.println(String(temperature) + " C");
    carrier.display.setCursor(80, 165);
    carrier.display.println(String(tempF) + " F");
    carrier.display.setCursor(80, 210);
    carrier.display.println(String(tempK) + " K");

    esperarBoton(TOUCH0);  
    // Espera que se presione un botón diferente al actual.
    break;
  }
}

void humedad() {
  // Muestra la humedad relativa.
  while (true) {
    float humidity = carrier.Env.readHumidity();  // Lee la humedad relativa.
    carrier.display.fillScreen(ST77XX_BLACK);  
    carrier.display.setCursor(70, 40);
    carrier.display.setTextColor(ST77XX_WHITE);
    carrier.display.setTextSize(3);
    carrier.display.println("HUMEDAD: ");
    carrier.display.setCursor(70, 100);
    carrier.display.setTextColor(ST77XX_BLUE);
    carrier.display.println(String(humidity) + "%");
    esperarBoton(TOUCH1);  
    // Espera que se presione un botón diferente.
    return;
  }
}

void presion() {
  // Muestra la presión atmosférica.
  while (true) {
    float pressure = (carrier.Pressure.readPressure()) * 10;  // Convierte la presión a hectopascales.
    carrier.display.fillScreen(ST77XX_BLACK);
    carrier.display.setCursor(70, 40);
    carrier.display.setTextColor(ST77XX_WHITE);
    carrier.display.setTextSize(3);
    carrier.display.println("PRESION: ");
    carrier.display.setCursor(70, 100);
    carrier.display.setTextSize(2);
    carrier.display.setTextColor(ST77XX_MAGENTA);
    carrier.display.println(String(pressure) + "hPa");
    esperarBoton(TOUCH2);
    return;
  }
}

void gases() {
  // Muestra las lecturas de CO2, resistencia del sensor y compuestos orgánicos volátiles.
  while (true) {
    float co2 = carrier.AirQuality.readCO2();  
    float gasResistor = carrier.AirQuality.readGasResistor();  
    float volatileOrganicCompounds = carrier.AirQuality.readVOC();
    carrier.display.fillScreen(ST77XX_BLACK);
    carrier.display.setCursor(70, 40);
    carrier.display.setTextColor(ST77XX_WHITE);
    carrier.display.setTextSize(3);
    carrier.display.println("GASES:");
    carrier.display.setCursor(70, 100);
    carrier.display.setTextColor(ST77XX_GREEN);
    carrier.display.setTextSize(2);
    carrier.display.println("Resistencia:");
    carrier.display.setCursor(70, 145);
    carrier.display.print("VOC:");
    carrier.display.setCursor(70, 190);
    carrier.display.print("CO2:");
    carrier.display.setTextColor(ST77XX_WHITE);
    carrier.display.setCursor(80, 120);
    carrier.display.println(String(gasResistor));
    carrier.display.setCursor(80, 165);
    carrier.display.println(String(volatileOrganicCompounds) + "ppm");
    carrier.display.setCursor(80, 210);
    carrier.display.println(String(co2));
    esperarBoton(TOUCH3);
    return;
  }
}

void giroscopio() {
  // Muestra los valores del giroscopio en los ejes X, Y y Z.
  while (true) {
    float x, y, z;
    if (carrier.IMUmodule.gyroscopeAvailable()) {  
      // Verifica si el giroscopio está disponible.
      carrier.IMUmodule.readGyroscope(x, y, z);  
      // Lee los valores del giroscopio.
      carrier.display.fillScreen(ST77XX_BLACK);
      carrier.display.setCursor(70, 40);
      carrier.display.setTextColor(ST77XX_WHITE);
      carrier.display.setTextSize(2);
      carrier.display.println("GIROSCOPIO:");
      carrier.display.setCursor(80, 100);
      carrier.display.setTextColor(ST77XX_CYAN);
      carrier.display.print("X: ");
      carrier.display.println(String(x));
      carrier.display.setCursor(80, 135);
      carrier.display.print("Y: ");
      carrier.display.println(String(y));
      carrier.display.setCursor(80, 170);
      carrier.display.print("Z: ");
      carrier.display.println(String(z));
    }
    esperarBoton(TOUCH4);
    return;
  }
}

void menu() {
  // Muestra un menú principal para seleccionar entre las diferentes opciones.
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setCursor(70, 40);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(3);
  carrier.display.println("MENU:");
  carrier.display.setTextSize(2);
  carrier.display.println("00_Temperatura");
  carrier.display.println("01_Humedad");
  carrier.display.println("02_Presion");
  carrier.display.println("03_Gases");
  carrier.display.println("04_Giroscopio");

  while (true) {
    carrier.Buttons.update();
    if (millis() - lastDebounceTime > debounceDelay) {
      if (carrier.Buttons.onTouchUp(TOUCH0)) { temperatura(); }
      if (carrier.Buttons.onTouchUp(TOUCH1)) { humedad(); }
      if (carrier.Buttons.onTouchUp(TOUCH2)) { presion(); }
      if (carrier.Buttons.onTouchUp(TOUCH3)) { gases(); }
      if (carrier.Buttons.onTouchUp(TOUCH4)) { giroscopio(); }
      lastDebounceTime = millis();
    }
    delay(100);
  }
}

void loop() {
  menu();  
  // Inicia siempre mostrando el menú.
}
