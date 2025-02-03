#include <Arduino_MKRIoTCarrier.h>

MKRIoTCarrier carrier;
unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  Serial.begin(9600);
  carrier.begin(); // Esto inicializa el Carrier incluyendo la pantalla
  delay(1000);
  Serial.println("Temperature(C)\tHumidity\tPressure(KPa)\tCO2(ppm)");
  carrier.display.setRotation(2);
}

void displayData(String title, String label, String value, uint16_t color) {
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setCursor(70, 40);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(3);
  carrier.display.println(title);
  carrier.display.setCursor(80, 100);
  carrier.display.setTextColor(color);
  carrier.display.setTextSize(2);
  carrier.display.println(label);
  carrier.display.println(value);
  waitForButtonPress();  // Esperar a que se presione un botón para regresar al menú
}

void waitForButtonPress() {
  while (true) {
    carrier.Buttons.update();
    if (carrier.Buttons.onTouchUp(TOUCH0) || carrier.Buttons.onTouchUp(TOUCH1) ||
        carrier.Buttons.onTouchUp(TOUCH2) || carrier.Buttons.onTouchUp(TOUCH3) ||
        carrier.Buttons.onTouchUp(TOUCH4)) {
      break;
    }
    delay(100);  // Pequeño retraso para evitar múltiples lecturas rápidas
  }
}

void displayTemperature() {
  float tempC = carrier.Env.readTemperature();
  String data = "T_Celsius: " + String(tempC) + "C\nT_Fahrenheit: " + String(tempC * 9.0 / 5.0 + 32.0) + "F\nT_Kelvin: " + String(tempC + 273.15) + "K";
  displayData("TEMPERATURA", "Temperatura", data, ST77XX_RED);
}

void displayHumidity() {
  float humidity = carrier.Env.readHumidity();
  displayData("HUMEDAD", "Humedad: " + String(humidity) + "%", "", ST77XX_BLUE);
}

void displayPressure() {
  float pressure = carrier.Pressure.readPressure() * 10;
  displayData("PRESION", "Presión: " + String(pressure) + " hPa", "", ST77XX_MAGENTA);
}

void displayGases() {
  float co2 = carrier.AirQuality.readCO2();
  float voc = carrier.AirQuality.readVOC();
  String data = "CO2: " + String(co2) + " ppm\nVOC: " + String(voc) + " ppm";
  displayData("GASES", "Gases", data, ST77XX_MAGENTA);
}

void displayGyroscope() {
  float x, y, z;
  if (carrier.IMUmodule.gyroscopeAvailable()) {
    carrier.IMUmodule.readGyroscope(x, y, z);
    String data = "X: " + String(x) + "\nY: " + String(y) + "\nZ: " + String(z);
    displayData("GIROSCOPIO", "Giroscopio", data, ST77XX_MAGENTA);
  }
}

void displayMenu() {
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setCursor(70, 40);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(3);
  carrier.display.println("MENU:");
  carrier.display.setTextSize(2);
  carrier.display.setCursor(80, 90);
  carrier.display.println("0: Temperatura");
  carrier.display.setCursor(80, 125);
  carrier.display.println("1: Humedad");
  carrier.display.setCursor(80, 160);
  carrier.display.println("2: Presión");
  carrier.display.setCursor(80, 195);
  carrier.display.println("3: Gases");
  carrier.display.setCursor(80, 230);
  carrier.display.println("4: Giroscopio");
}

void handleButtonPress(int option) {
  switch (option) {
    case 0: displayTemperature(); break;
    case 1: displayHumidity(); break;
    case 2: displayPressure(); break;
    case 3: displayGases(); break;
    case 4: displayGyroscope(); break;
    default: displayData("ERROR", "", "", ST77XX_RED); break;
  }
}

void checkButtons() {
  if (carrier.Buttons.onTouchUp(TOUCH0)) handleButtonPress(0);
  if (carrier.Buttons.onTouchUp(TOUCH1)) handleButtonPress(1);
  if (carrier.Buttons.onTouchUp(TOUCH2)) handleButtonPress(2);
  if (carrier.Buttons.onTouchUp(TOUCH3)) handleButtonPress(3);
  if (carrier.Buttons.onTouchUp(TOUCH4)) handleButtonPress(4);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    displayMenu();
    checkButtons();
  }
}
