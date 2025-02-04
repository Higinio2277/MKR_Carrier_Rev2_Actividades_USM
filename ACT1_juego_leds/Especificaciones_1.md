ACTIVIDAD 1
Link de funsionamiento: https://youtube.com/shorts/uL8qoxrH9Zs 

Especificaciones primera actividad: Juego LEDs

El juego consiste en fijar todos los LEDs del carrier en un mismo color (rojo, azul y verde) para acumular puntos. Para ello deben considerar lo siguiente:

Los LEDs deben tener 3 estados: apagado, parpadeando y encendido (los estados deben seguir ese orden en forma de bucle, cambiando de estado cada LED por separado una vez se pulsa el botón asociado a dicho LED).

Durante el estado apagado: los LEDs deben variar internamente el color que mostrarán al ser pulsado (cada 10ms puede ser un tiempo bueno). Esto se puede hacer de forma pseudo-aleatoria para evitar que el juego se vuelva monótono. Pero recomiendo partir con que se haga de forma cíclica.

Durante el estado parpadeante: los LEDs deben parpadear en el color que les fue seleccionado al pulsar el botón a una tasa de parpadeo perceptible por el ojo humano.

Durante el estado encendido: el LED se "fija" en el color seleccionado.

Cada vez que se consiga 1 "punto" debe sonar el buzzer (recuerden calibrar frecuencia, tono y duración).

No se puede conseguir un punto extra con la misma combinación de colores: si ya se ganó el punto por luz verde, no se puede volver a ganar un punto por luz verde.

Una vez se consigan los 3 puntos, debe desplegarse en el centro del display "YOU WIN!" y reiniciar el juego desde 0.

Es recomendable implementar delays en ciertas etapas para no hacer doble toque o implementar un sistema antiresorte.

Con esta actividad probarán:
Inicialización del dispositivo.
Manejo de memoria.
Uso de LEDs del carrier.
Uso de botones del carrier.
Uso del display.
Uso del buzzer.
