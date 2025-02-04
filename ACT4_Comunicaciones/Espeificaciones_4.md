ACTIVIDAD 4
Link de funcionamiento: https://youtu.be/c7dMKmoqzms

Especificaciones actividad 4:

Menú Inicial:
El programa debe mostrar un menú inicial con dos opciones disponibles:
Conexión Wi-Fi.
Bluetooth Low Energy (BLE).

Conexión Wi-Fi:
Escanear redes Wi-Fi disponibles de 2.4 GHz.
Mostrar las redes encontradas, indicando el SSID, la intensidad de la señal (RSSI), y el canal.
Permitir seleccionar una red Wi-Fi o regresar al menú principal ingresando 'm'.
Solicitar la contraseña para la red seleccionada.
Intentar la conexión Wi-Fi y:
Indicar si la conexión fue exitosa, mostrando la dirección IP asignada.
En caso de error, regresar automáticamente al menú principal.
Si la conexión es exitosa, permitir desconectarse ingresando 'm' para regresar al menú principal y desconectar el dispositivo de la red Wi-Fi.

Bluetooth Low Energy (BLE):
Activar el módulo BLE y publicar un servicio genérico con una característica de lectura y escritura.
Indicar en el monitor serial que el dispositivo está disponible para conexiones BLE.
Detectar y mostrar información sobre el dispositivo central conectado.
Durante el estado de conexión, permitir regresar al menú principal ingresando 'm', cerrando las conexiones BLE activas.
Para probar la conexion, debe usar alguna aplicación de conexión por medio de BLE.

Interacción con el Usuario:
Garantizar que el monitor serial proporcione retroalimentación clara y concisa para cada acción del programa.
Permitir al usuario volver al menú principal desde cualquier estado mediante la entrada 'm'.

Estabilidad y Robustez:
Implementar validaciones para entradas inválidas del usuario.
Prevenir bucles infinitos en estados de espera mediante controles adecuados.
nRF Connect (Android/iOS)
LightBlue Explorer (iOS)
Configuración de Bluetooth en macOS o Windows.
