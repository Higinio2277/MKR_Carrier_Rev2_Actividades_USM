ACTIVIDAD 5
Link de funcionamiento: https://youtube.com/shorts/O4ggwSSEq38 

Especificaciones de la quinta y última actividad: Sensores externos

Hardware Utilizado:
Sensor de humedad del suelo en el pin A0.
Sensor PIR en el pin Digital.

Funcionamiento General:
Menú principal para elegir entre:
Sensor de Humedad del Suelo.
Sensor PIR.
Regreso al menú enviando 'r' por el teclado serial.

Modo Sensor de Humedad del Suelo:
Calibración inicial:
Colocar el sensor en suelo seco y húmedo para registrar valores.
Lecturas cada 10 segundos.
Indica si el suelo está seco o húmedo y calcula el porcentaje (0% a 100%).

Modo Sensor PIR:
Detecta movimiento.
Lecturas cada 2 segundos.
Indica si hay movimiento o ninguno en el monitor serial.

Control y Navegación:
Seleccionar sensores con:
'1': Sensor de Humedad.
'2': Sensor PIR.
Volver al menú con 'r'.

Requisitos Adicionales:
Monitor serial configurado a 9600 baudios.
Verificar la correcta conexión de los sensores.

Calibración del Sensor de Humedad:
Manual al ingresar al modo, registrando valores mínimos y máximos.

Frecuencia de Muestreo:
Humedad: 10 segundos.
PIR: 2 segundos.

Plataforma de Desarrollo:
Arduino IDE con soporte para Arduino IoT Explore Kit Rev2.

Formato de Datos:
Resultados en monitor serial con indicaciones claras (menú, lecturas y estados).
Recuerden ir enviando sus actividades por aquí
