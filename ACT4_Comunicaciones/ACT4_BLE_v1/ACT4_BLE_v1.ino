#include <ArduinoBLE.h>

BLEService myService("fff0");
BLEIntCharacteristic myCharacteristic("fff1", BLERead | BLEWrite | BLEBroadcast);

// Parámetros de publicidad
const uint8_t manufactData[4] = {0x01, 0x02, 0x03, 0x04};
const uint8_t serviceData[3] = {0x00, 0x01, 0x02};

void onBLEConnected(BLEDevice central) {
  // Imprime la dirección del dispositivo central cuando se conecta
  Serial.print("Conectado al dispositivo central: ");
  Serial.println(central.address());
  //Serial.println(myCharacteristic);
  //Serial.println(myService);
}

void onBLEDisconnected(BLEDevice central) {
  // Imprime la dirección del dispositivo central cuando se desconecta
  Serial.print("Desconectado del dispositivo central: ");
  Serial.println(central.address());
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Inicialización del módulo BLE
  if (!BLE.begin()) {
    Serial.println("¡Error al inicializar BLE!");
    while (1);
  }

  // Agregar la característica al servicio BLE
  myService.addCharacteristic(myCharacteristic);
  BLE.addService(myService);

  // Configuración de los datos de respuesta de escaneo
  BLEAdvertisingData scanData;
  scanData.setLocalName("A_Higinio");
  BLE.setScanResponseData(scanData);

  // Configuración de los datos de publicidad
  BLEAdvertisingData advData;
  advData.setManufacturerData(0x004C, manufactData, sizeof(manufactData));
  advData.setAdvertisedService(myService);
  advData.setAdvertisedServiceData(0xfff0, serviceData, sizeof(serviceData));
  BLE.setAdvertisingData(advData);

  // Iniciar la publicidad BLE
  BLE.advertise();
  Serial.println("Publicitando ...");

  // Configurar los manejadores de eventos de conexión y desconexión
  BLE.setEventHandler(BLEConnected, onBLEConnected);
  BLE.setEventHandler(BLEDisconnected, onBLEDisconnected);
}

void loop() {
  BLE.poll();

  // Verificar si el usuario ingresa 'm' para regresar al menú principal
  if (Serial.available()) {
    char input = Serial.read();
    if (input == 'm') {
      BLE.disconnect();
      Serial.println("Desconectado. Regresando al menú principal...");
    }
  }
}
