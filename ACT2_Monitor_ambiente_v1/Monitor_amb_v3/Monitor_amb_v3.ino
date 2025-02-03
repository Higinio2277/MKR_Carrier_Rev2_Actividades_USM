#include <Arduino_MKRIoTCarrier.h>

MKRIoTCarrier carrier;
unsigned long debounceDelay = 50;  // Retardo para el antirrebote en milisegundos
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(9600);
  carrier.begin();
  carrier.noCase();
  delay(1000);  // Espera para asegurar que el Carrier está listo
  Serial.println("Temperature(C)\tHumidity\tPressure(KPa)\tCO2(ppm)");
}

void esperarBoton(int currentButton) {
  // Espera hasta que un botón diferente sea presionado y liberado, con antirrebote
  while (true) {
    carrier.Buttons.update();
    if (millis() - lastDebounceTime > debounceDelay) {
      if ((carrier.Buttons.onTouchUp(TOUCH0) && currentButton != TOUCH0) ||
          (carrier.Buttons.onTouchUp(TOUCH1) && currentButton != TOUCH1) ||
          (carrier.Buttons.onTouchUp(TOUCH2) && currentButton != TOUCH2) ||
          (carrier.Buttons.onTouchUp(TOUCH3) && currentButton != TOUCH3) ||
          (carrier.Buttons.onTouchUp(TOUCH4) && currentButton != TOUCH4)) {
        lastDebounceTime = millis();
        menu();
        return;
      }
    }
    delay(50);  // Pequeño retraso para evitar rebotes rápidos
  }
}

void temperatura() {
  while (true) {
    float temperature = carrier.Env.readTemperature(); // Temperatura en Celsius
    float tempF = temperature * 9.0 / 5.0 + 32.0;
    float tempK = temperature + 273.15;
    carrier.display.fillScreen(ST77XX_BLACK);  // Limpiar pantalla
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
    break;
  }
}

void humedad() {
  while (true) {
    float humidity = carrier.Env.readHumidity();
    carrier.display.fillScreen(ST77XX_BLACK);  // Limpiar pantalla
    carrier.display.setCursor(70, 40);
    carrier.display.setTextColor(ST77XX_WHITE);
    carrier.display.setTextSize(3);
    carrier.display.println("HUMEDAD: ");
    carrier.display.setCursor(70, 100);
    carrier.display.setTextColor(ST77XX_BLUE);
    carrier.display.println(String(humidity) + "%");
    esperarBoton(TOUCH1);
    return;
  }
}

void presion() {
  while (true) {
    float pressure = (carrier.Pressure.readPressure())*10;
    carrier.display.fillScreen(ST77XX_BLACK);  // Limpiar pantalla
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
  while (true) {
    float x, y, z;
    if (carrier.IMUmodule.gyroscopeAvailable()) {
      carrier.IMUmodule.readGyroscope(x, y, z);
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
}
