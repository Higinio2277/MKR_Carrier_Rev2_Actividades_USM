ACTIVIDAD 3
Link de funcionamiento: https://youtube.com/shorts/UPEJ6wGEYig

Especificaciones Tercera Actividad:

Estado Base:
Mostrar en el display LEYENDO MOVIMIENTO. 
Acciones posibles:
Detectar gestos.
Ir a detección de color (TOUCH2) o proximidad (TOUCH4).

Detección de Gestos:
Gestos: arriba, abajo, izquierda, derecha.
Mostrar flecha en el display y emitir un zumbido (100 ms).
Volver al estado base tras 1 segundo.

Detección de Color (TOUCH2):
Transición al estado con TOUCH2.
Mostrar MUESTRE COLOR BLANCO.
Leer RGB del sensor y detectar blanco (valores altos y similares en R, G, y B).
Mostrar COLOR BLANCO DETECTADO al identificar blanco.
Volver al estado BASE si:
Se detecta el color blanco.
Se presiona nuevamente TOUCH2.

Detección de Proximidad (TOUCH4):
Transición al estado con TOUCH4.
Mostrar MIDENDO PROXIMIDAD y la distancia detectada (mm).
Indicar proximidad con LEDs:
Verde: > 200 mm.
Amarillo: 120 - 200 mm.
Rojo: < 120 mm.
Volver al estado BASE con TOUCH4.

Anti-rebote:
Implementar un sistema anti-rebote entre lecturas válidas de botones táctiles.

Buzzer:
Emitir un zumbido breve al detectar gestos o condiciones críticas (e.g., proximidad extrema). (pueden emplear zumbidos diferentes para mayor virtuosismo, pero con que suene el mismo es suficiente)

Display:
Usar el display para mostrar:
Estados del sistema: Gestos, Color, Proximidad.
Información: Valores RGB, distancia, etc.

Código Modular:
Se recomienda trabajar con máquinas de estados.

Librerías:
Se puede usar la librería del carrier directamente o la del propio sensor.

Sensibilidad:
Recuerden calibrar la sensibilidad de los sensores que así lo requieran.

Lectura Continua:
Los sensores deben leer continuamente si dicho sensor se requiere en el estado en que se encuentre el dispositivo.
Les recomiendo indagar sobre la ubicación del sensor y la orientación que tiene para medir, además de como mide color y distancia. Consideren que los gestos se realizan en el mismo plano que los caracteres del display, por lo que si van desde abajo de los caracteres  hacia arriba, la flecha mostrada debe indicar un gesto hacia arriba (extrapolar a los demás).
Recuerden que el IDE tiene ejemplos incorporados de como usar cada función, con métodos ya implementados en códigos funcionales.
