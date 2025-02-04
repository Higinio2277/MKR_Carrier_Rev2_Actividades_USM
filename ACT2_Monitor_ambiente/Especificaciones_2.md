ACTIVIDAD 2
Link de funcionamiento: https://youtube.com/shorts/HtnQESOn0tc

Especificaciones segunda actividad: Monitor de ambiente

El monitor hará uso constante del display para mostrar diferentes estados del dispositvo:

El estado DEFAULT: Debe ser un menú inicial que indique qué botón debe ser pulsado para tomar una medición con el dispositivo. 00 para Temperatura, 01 para Humedad, 02 para Presión, 03 para Gases y 04 para el Giroscópio.

00 Temperatura:
El display debe mostrar un menú que se actualice periódicamente con la temperatura. El formato debe ser:
Temperatura:
"temperatura_celsius" C
"temperatura_fahrenheit" F
"temperatura_kelvin" K

01 Humedad:
El display debe mostrar un menú que se actualice periódiacmente con la humedad del aire. El formato debe ser:
Humedad:
"humedad" %

02 Presión:
El display debe mostrar un menú que se actualice periódiacmente con la presión del ambiente. El formato debe ser:
Presion:
"presion" hPa (hectopascales)

03 Gases:
El display debe mostrar un menú que se actualice periódiacmente con los gases del ambiente. El formato debe ser:
Gases:
Resistencia:
"resistencia"
VOC:
"volatileorganiccompounds"
CO2:
"co2"

04 Giroscópio:
El display debe mostrar las variaciones en los ejes cartesianos (x, y, z) que ha sufrido el dispositivo como valores flotantes. El formato debe ser:
Giroscopio:
x: "x"
y: "y"
z: "z"

Si se entra a un submenú (00, 01, ... , 04) y luego se presiona cualquier botón, se debe regresar al menú principal.

En esta actividad probarán principalmente el sensor ambiental del carrier y el IMU, ya que tanto los botones como el display ya los deben haber manejado en la actividad anterior. 
