#include <Arduino_MKRIoTCarrier.h>

MKRIoTCarrier carrier;

void setup() {
  Serial.begin(9600);
  carrier.begin();
  carrier.noCase();
  // Espera para asegurar que el Carrier está listo
  delay(1000);
  Serial.println("Temperature(C)\tHumidity\tPressure(KPa)\tCO2(ppm)");
}
void temperatura(){
  float temperature = carrier.Env.readTemperature(); // Temperatura en Celsius
  float tempF = temperature * 9.0 / 5.0 + 32.0;
  float tempK = temperature + 273.15;
  carrier.display.fillScreen(ST77XX_BLACK);  // Limpiar pantalla
  carrier.display.setCursor(80, 100);
  carrier.display.setTextColor(ST77XX_RED);
  carrier.display.setTextSize(2);
  carrier.display.println("TEMPERATURA: ");
  carrier.display.print("T_Celsius: ");
  carrier.display.println(String(temperature) + "C");
  carrier.display.print("T_Fahrenheit: ");
  carrier.display.println(String(tempF) + "F");
  carrier.display.print("T_Kelvin: ");
  carrier.display.println(String(tempK) + "K");
}
void humedad(){
  float humidity = carrier.Env.readHumidity(); // Humedad relativa en %
  carrier.display.fillScreen(ST77XX_BLACK);  // Limpiar pantalla
  carrier.display.setCursor(70, 40);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(3);
  carrier.display.println("HUMEDAD: ");
  carrier.display.setCursor(80, 135);
  carrier.display.setTextColor(ST77XX_BLUE);
  carrier.display.println("Humedad:");
  carrier.display.setCursor(80, 160);
  carrier.display.println(String(humidity) + "%");
}
void presion(){
  float pressure = (carrier.Pressure.readPressure())*10;
  carrier.display.fillScreen(ST77XX_BLACK);  // Limpiar pantalla
  carrier.display.setCursor(70, 40);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(3);
  carrier.display.println("PRESION: ");
  carrier.display.setCursor(80, 130);
  carrier.display.setTextColor(ST77XX_MAGENTA);
  carrier.display.println("Presion:");
  carrier.display.setCursor(80, 160);
  carrier.display.setTextSize(2);
  carrier.display.println(String(pressure) + "hPa");
}
void gases(){
  float co2 = carrier.AirQuality.readCO2(); 
  float gasResistor = carrier.AirQuality.readGasResistor();
  float volatileOrganicCompounds = carrier.AirQuality.readVOC();
  carrier.display.fillScreen(ST77XX_BLACK);  // Limpiar pantalla
  carrier.display.setCursor(70, 40);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(3);
  carrier.display.println("GASES: ");
  carrier.display.setCursor(80, 100);
  carrier.display.setTextColor(ST77XX_MAGENTA);
  carrier.display.setTextSize(2);
  carrier.display.println("Resistencia:");
  carrier.display.setCursor(80, 120);
  carrier.display.println(String(gasResistor));
  carrier.display.setCursor(80, 150);
  carrier.display.print("VOC:");
  carrier.display.println(String(volatileOrganicCompounds) + "ppm");
  carrier.display.setCursor(80, 180);
  carrier.display.print("CO2:");
  carrier.display.println(String(co2));  

}
void giroscopio(){
  
  float x, y, z; 

  if (carrier.IMUmodule.gyroscopeAvailable()) {
    carrier.IMUmodule.readGyroscope(x, y, z);

    carrier.display.fillScreen(ST77XX_BLACK);  // Limpiar pantalla
    carrier.display.setCursor(70, 40);
    carrier.display.setTextColor(ST77XX_WHITE);
    carrier.display.setTextSize(3);
    carrier.display.println("GIROSCOPIO: ");
    carrier.display.setCursor(80, 100);
    carrier.display.setTextColor(ST77XX_MAGENTA);
    //carrier.display.setTextSize(2);
    carrier.display.println("X:");
    carrier.display.println(String(x));
    carrier.display.setCursor(80, 135);
    carrier.display.print("Y:");
    carrier.display.println(String(y));
    carrier.display.setCursor(80, 170);
    carrier.display.print("Z:");
    carrier.display.println(String(z));  
  }

}
void menu(){
  carrier.display.fillScreen(ST77XX_BLACK);  // Limpiar pantalla
  carrier.display.setCursor(70, 40);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(3);
  carrier.display.println("MENU: ");
  carrier.display.setTextSize(2);
  carrier.display.setCursor(80, 90);
  carrier.display.println("00_Temperatura:");
  carrier.display.setCursor(80, 125);
  carrier.display.println("01_Humedad:");
  carrier.display.setCursor(80, 150);
  carrier.display.println("02_Presion:");
  carrier.display.setCursor(80, 175);
  carrier.display.println("03_Gases:");
  carrier.display.setCursor(80, 200);
  carrier.display.println("04_Giroscopio:");

  carrier.Buttons.update();
  if (carrier.Buttons.onTouchUp(TOUCH0)) {
        temperatura();
        romper();
    }
    if (carrier.Buttons.onTouchUp(TOUCH1)) {
        humedad();
        romper();
    }
    if (carrier.Buttons.onTouchUp(TOUCH2)) {
        presion();
        romper();
    }
    if (carrier.Buttons.onTouchUp(TOUCH3)) {
        gases();
        romper();
    }
    if (carrier.Buttons.onTouchUp(TOUCH4)) {
        giroscopio();
        romper();
    }
    delay(100); // Pequeño retraso para evitar múltiples lecturas rápidas
}
void romper() {
    if (carrier.Buttons.onTouchUp(TOUCH0) || 
        carrier.Buttons.onTouchUp(TOUCH1) || 
        carrier.Buttons.onTouchUp(TOUCH2) || 
        carrier.Buttons.onTouchUp(TOUCH3) || 
        carrier.Buttons.onTouchUp(TOUCH4)) {
        menu();
    }
}

void loop() {
  menu();
  delay(200);  // Esperar 1 segundo antes de la siguiente lectura
}
