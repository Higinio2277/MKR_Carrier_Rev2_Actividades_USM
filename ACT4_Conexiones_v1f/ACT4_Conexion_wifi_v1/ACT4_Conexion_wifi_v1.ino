#include <SPI.h>
#include <WiFiNINA.h>

// Variables globales
char ssid[32];        // SSID seleccionado
char pass[32];        // Contraseña de la red
int status = WL_IDLE_STATUS;

// Prototipos de funciones
void listNetworks();
void connectToNetwork();
void printWiFiStatus();
void printMacAddress(byte mac[]);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // esperar a que el puerto serial esté listo
  }

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("¡Fallo en la comunicación con el módulo WiFi!");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Por favor, actualiza el firmware.");
  }
}

void loop() {
  Serial.println("Escaneando redes Wi-Fi...");
  listNetworks();

  Serial.println("Ingrese el número de la red para conectarse o 'm' para regresar al menú principal:");
  while (!Serial.available());

  char input = Serial.read();
  if (input == 'm') {
    return;
  }

  int selectedNetwork = input - '0' - 1;
  if (selectedNetwork >= 0 && selectedNetwork < WiFi.scanNetworks()) {
    strcpy(ssid, WiFi.SSID(selectedNetwork));
    Serial.println("Red seleccionada: " + String(ssid));

    Serial.println("Ingrese la contraseña de la red:");
    int len = 0;
    while (len == 0) {
      while (!Serial.available()); // Espera hasta que haya algo en el buffer serie
      len = Serial.readBytesUntil('\n', pass, sizeof(pass));
      if (len == 0) {
        Serial.println("La contraseña no puede estar vacía. Por favor, ingrese la contraseña:");
      }
    }
  pass[len] = '\0'; // Añadir terminador nulo

    connectToNetwork();
  } else {
    Serial.println("Selección inválida. Regresando al menú principal.");
  }
}

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

    Serial.println("Ingrese 'm' para desconectar y regresar al menú principal:");
    while (Serial.read() != 'm');
    WiFi.disconnect();
    Serial.println("Desconectado de la red Wi-Fi.");
  } else {
    Serial.println("No se pudo conectar a la red. Regresando al menú principal.");
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
