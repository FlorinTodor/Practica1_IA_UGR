Práctica 1 – Agentes Reactivos
Asignatura: Inteligencia Artificial – Curso 2023/2024
Autor: [Tu nombre]

1. Introducción
Esta práctica consiste en diseñar e implementar un agente reactivo que actúe como jugador en el entorno simulado Los mundos de BelKan. El agente debe explorar el mapa, identificar tipos de terreno, detectar obstáculos y posicionarse correctamente, maximizando el porcentaje de mapa descubierto.

2. Escenario
El mapa es una matriz de hasta 100x100 celdas, con distintos tipos de terreno y elementos:

Terrenos: Bosque (B), Agua (A), Precipicio (P), Arenoso (T), Pedregoso (S), Muro (M).

Objetos especiales: Bikini (K), Zapatillas (D), Recarga (X), Posicionamiento (G).

Agentes móviles: Aldeanos (a), Lobos (l) y el propio jugador (j).

3. Características del Agente
Sensores: Terreno, agentes, colisión, reset, posición, orientación, batería, nivel, tiempo.

Acciones:

actWALK (avanzar 1)

actRUN (avanzar 2)

actTURN_SR (girar 45° dcha)

actTURN_L (girar 90° izq)

actIDLE (no hacer nada)

Costes: Variables según terreno y objetos en posesión.

4. Implementación
El comportamiento se define en jugador.cpp y jugador.hpp, sobre la clase ComportamientoJugador.
Se han implementado reglas reactivas basadas en:

Lectura del vector de terreno y detección de obstáculos inmediatos.

Priorización de movimiento hacia casillas no exploradas (?).

Estrategias de evasión de muros y precipicios.

Gestión de batería optimizando el uso de bikini y zapatillas.

En niveles sin sensor de posición/orientación, uso de casillas de posicionamiento (G) para recalibrar.

El agente no emplea planificación global ni búsqueda heurística; responde a estímulos inmediatos según el estado de los sensores.

5. Pruebas
Se ha evaluado el comportamiento en distintos niveles:

Nivel 0: Exploración sistemática con cobertura alta.

Nivel 1: Estrategia robusta sin sensor de posición, ajustando rumbo por referencias visuales.

Nivel 2: Evitación activa de lobos y aldeanos.

Nivel 3: Gestión con orientación desconocida inicial y sensores defectuosos.

6. Resultados
El agente logra un alto porcentaje de exploración en todos los niveles, con especial eficacia en los niveles 0 y 1. En niveles con enemigos, la evasión es efectiva, aunque los reinicios reducen la eficiencia.

7. Conclusiones
El enfoque reactivo permite un comportamiento ágil y adaptable en entornos parcialmente observables.

La ausencia de memoria global limita la optimización de rutas, pero mejora la tolerancia a cambios locales.

Se podrían mejorar los niveles altos con un mapa auxiliar que recuerde zonas visitadas y rutas seguras.

