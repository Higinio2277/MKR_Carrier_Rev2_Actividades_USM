#include <ArduinoBLE.h>
#include <SPI.h>
#include <WiFiNINA.h>
//WIFI-------------------------------------------------------------
// Variables globales
char ssid[32];        // SSID seleccionado
char pass[32];        // Contraseña de la red
int status = WL_IDLE_STATUS;

// Prototipos de funciones
void listNetworks();
void connectToNetwork();
void printWiFiStatus();
void printMacAddress(byte mac[]);
//BLE ++++++++++++++++++++++++++++++++++++++++++++++
BLEService myService("fff0");
BLEIntCharacteristic myCharacteristic("fff1", BLERead | BLEWrite | BLEBroadcast);

const uint8_t manufactData[4] = {0x01, 0x02, 0x03, 0x04};
const uint8_t serviceData[3] = {0x00, 0x01, 0x02};

bool isBLEConnected = false;  // Indicador para el estado de la conexión BLE

void onBLEConnected(BLEDevice central) {
  Serial.print("Conectado al dispositivo central: ");
  Serial.println(central.address());
  isBLEConnected = true;
}

void onBLEDisconnected(BLEDevice central) {
  Serial.print("Desconectado del dispositivo central: ");
  Serial.println(central.address());
  isBLEConnected = false;
}

void initializeBLE() {
  if (!BLE.begin()) {
    Serial.println("¡Error al inicializar BLE!");
    while (1);
  }
  myService.addCharacteristic(myCharacteristic);
  BLE.addService(myService);

  BLEAdvertisingData scanData;
  scanData.setLocalName("A_Higinio");
  BLE.setScanResponseData(scanData);

  BLEAdvertisingData advData;
  advData.setManufacturerData(0x004C, manufactData, sizeof(manufactData));
  advData.setAdvertisedService(myService);
  BLE.setAdvertisingData(advData);

  BLE.advertise();
  Serial.println("Publicitando ...");
  Serial.println("\nPresiona la letra 'q'para regresar al menu.\n");

  BLE.setEventHandler(BLEConnected, onBLEConnected);
  BLE.setEventHandler(BLEDisconnected, onBLEDisconnected);

  while (true) {
    BLE.poll();
    if (Serial.available()) {
      char input = Serial.read();
      if (input == 'q') {
        BLE.disconnect();
        Serial.println("\nDesconectado manualmente. Volviendo al menú principal...");
        delay(150);
        break;
      }
    }
  }

  BLE.stopAdvertise();
  BLE.disconnect();
  BLE.end();
  delay(1000);  // Aumentado el retraso para estabilizar hardware
}

void resetModule() {
  WiFi.end();
  BLE.end();
  delay(1000); // Esperar un segundo para estabilizar el hardware
}

//WIFI+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void listNetworks() {
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    Serial.println("No se pudo obtener una conexión Wi-Fi.");
    return;
  }

  Serial.println("** Redes disponibles **");
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial.print(thisNet + 1);
    Serial.print(") ");
    Serial.print(WiFi.SSID(thisNet));
    Serial.print("\tSeñal: ");
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(" dBm\tCanal: ");
    Serial.println(WiFi.channel(thisNet));
  }
  Serial.println();
}

void connectToNetwork() {
  Serial.println("Intentando conectar a la red: " + String(ssid));
  status = WiFi.begin(ssid, pass);
  
  if (status == WL_CONNECTED) {
    Serial.println("Conexión exitosa.");
    printWiFiStatus();

    Serial.println("\nIngrese 'm' para desconectar y regresar al menú principal:");
    while (Serial.read() != 'm');
    WiFi.disconnect();
    Serial.println("Desconectado de la red Wi-Fi.");
  } else {
    Serial.println("No se pudo conectar a la red. Regresando al menú principal.");
    WiFi.disconnect();
  }
}

void printWiFiStatus() {
  IPAddress ip = WiFi.localIP();
  Serial.print("Dirección IP: ");
  Serial.println(ip);
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}

void wifi() {
  resetModule();
  Serial.println("Escaneando redes Wi-Fi...");
  listNetworks();

  Serial.println("Ingrese el número de la red para conectarse o 'm' para regresar al menú principal:\n");
  while (true) {
    while (!Serial.available());
    char input = Serial.read();
    if (input == 'm') {
      WiFi.disconnect();
      Serial.println("Saliendo de wifi...");
      return;
    }
    int selectedNetwork = input - '0' - 1;
    if (selectedNetwork >= 0 && selectedNetwork < WiFi.scanNetworks()) {
      strcpy(ssid, WiFi.SSID(selectedNetwork));
      Serial.println("Red seleccionada: " + String(ssid));
      Serial.println("Ingrese la contraseña de la red:");
      delay(500);
      
      int len = 0;
      while (len == 0) {
      if (Serial.available()) {
        len = Serial.readBytesUntil('\n', pass, sizeof(pass));
        }
      }
      pass[len] = '\0';
      connectToNetwork();
      return;
    } else {
      Serial.println("\nSelección inválida. Intente de nuevo o ingrese 'm' para regresar al menú principal:\n");
      } 
  }
}
//MENU----------------------------------------------------
void showMenu() {
  Serial.println("\n=== Menú de opciones ===");
  Serial.println("1. Conectar por BLE");
  Serial.println("2. Conectar Wifi");
  Serial.println("s. Salir del sistema");
  Serial.println("Elige una opción: ");
}

void handleMenuOption(char option) {
  switch (option) {
    case '1': {
      Serial.println("\nInicializando BLE...");
      initializeBLE();
      break;
    }
    case '2': {
      Serial.println("Iniciando WIFI");
      wifi();
      break;
    }
    case 's': {
      Serial.println("Saliendo del sistema...");
      while (true);  // Simulación de cierre de programa
      break;
    }
    default: {
      Serial.println("Opción no válida. Intenta de nuevo.");
      break;
    }
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("¡Fallo en la comunicación con el módulo WiFi!");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Por favor, actualiza el firmware.");
  }
  Serial.println("Sistema iniciado.");
  showMenu();
}

void loop() {
  char input = '\0'; // Variable para almacenar la entrada del usuario
  
  // Espera hasta que haya una entrada válida
  while (input != '1' && input != '2' && input != 's') {
    if (Serial.available() > 0) {
      input = Serial.read();  // Lee la opción del usuario
      // Limpia cualquier carácter sobrante en el búfer
      while (Serial.available() > 0) {
        Serial.read();
      }
      
      // Verifica si la entrada es válida
      if (input != '1' && input != '2' && input != 's') {
        Serial.println(input);
      }
    }
  }
  
  // Ahora procesamos la opción válida
  handleMenuOption(input);  // Llamamos a la función para manejar la opción seleccionada
  
  // Luego mostramos el menú de nuevo
  showMenu();
}

