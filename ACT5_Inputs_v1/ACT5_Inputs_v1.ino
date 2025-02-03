#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier; //Constructor of the carrier maybe we can include it on the library itself
int pir = A6;
bool trigger = true;
const int sensorPin = A0;  // Pin analógico al que está conectado el sensor
int sensorValueDry = 0;    // Valor leído en condiciones de suelo seco
int sensorValueWet = 1023; // Valor leído en condiciones de suelo húmedo

void setup() {
  Serial.begin(9600); // Iniciar la comunicación serial
  pinMode(pir,INPUT);
  pinMode(sensorPin, INPUT); // Configurar el pin del sensor como entrada
  carrier.begin();
}
void senPir(){
  while(true){
    trigger = digitalRead(pir);
    Serial.print("\t Move state: ");
    Serial.println(digitalRead(pir));
    carrier.display.fillScreen(ST77XX_BLACK);
    carrier.display.setTextColor(ST77XX_WHITE);
    carrier.display.setTextSize(2);
    carrier.display.setCursor(70, 70);
    carrier.display.println("MOVIMIENTO");
    carrier.display.setCursor(100, 100);carrier.display.println(trigger);
    if(trigger == HIGH){
      carrier.display.fillRect(70, 130, 160, 20, ST77XX_BLACK); // Asume fondo negro
      carrier.display.setCursor(70, 130);carrier.display.println("Moviendose...");
    } else if( trigger == LOW){
      carrier.display.fillRect(70, 130, 160, 20, ST77XX_BLACK); // Asume fondo negro
      carrier.display.setCursor(70, 130);carrier.display.println("Tranquilo...");
    }
    char sas = Serial.read();
    if (sas == 'r'){
      break;
    }
    delay(2000);
  }
  
}
void senHumedad(){
  while(true){
    int sensorValue = analogRead(sensorPin); // Leer el valor analógico
    // Mapear porcentaje de humedad
    int moisturePercent = map(sensorValue, sensorValueDry, sensorValueWet, 0, 100);
    // Limitar el rango de 0 a 100%
    moisturePercent = constrain(moisturePercent, 0, 100);
    //Serial.print("Valor del sensor: ");
    //Serial.print(sensorValue);
    Serial.print(" - Humedad: ");
    Serial.print(moisturePercent);
    Serial.println("%");
    carrier.display.fillScreen(ST77XX_BLACK);
    carrier.display.setTextColor(ST77XX_WHITE);
    carrier.display.setTextSize(3);
    carrier.display.setCursor(68, 90);carrier.display.println("HUMEDAD(%)");
    carrier.display.setCursor(100, 150);carrier.display.println(String(moisturePercent)+"%");
    char sa = Serial.read();
    if( sa == 'r'){
      break;
    }
    delay(10000); // Esperar 1 segundo antes de la siguiente lectura
  }
}

void loop() {
  Serial.println(  "   \nMENU""\n1.Sensor de humeda en tierra""\n2.Sensor de presencia o movimiento""\n3. salir""\nElija una opcion");
  carrier.display.fillScreen(ST77XX_BLACK);
  carrier.display.setCursor(70, 60);
  carrier.display.setTextColor(ST77XX_WHITE);
  carrier.display.setTextSize(3);
  carrier.display.println("MENU");
  carrier.display.setTextSize(2);
  carrier.display.setCursor(70, 100);carrier.display.println("1.Humedad(suelo");
  carrier.display.setCursor(70, 120);carrier.display.println("2.Movimiento");
  carrier.display.setCursor(70, 140);carrier.display.println("3.Salir");
  
  while(Serial.available()==0);
  int opcion = Serial.parseInt();
  Serial.println("La opcion elegida ingrsada es la"+ String(opcion));
  switch (opcion){
    case 1: 
      Serial.println("Sensor de humedad en el suelo");
      senHumedad();
      break;
    case 2:
      Serial.println("Sensor de movimiento");
      senPir();
      break;
    case 3:
        return;
    default:
      Serial.println("Opcion no valida");
      break;
  }
}
