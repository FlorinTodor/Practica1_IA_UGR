# 🤖 Práctica 1 – Agente Reactivo

**Asignatura:** Inteligencia Artificial – Curso 2023/2024  

---

## 🎯 1. Introducción
Esta práctica se centra en el diseño e implementación de un **agente reactivo** capaz de operar de forma autónoma en el entorno simulado de *Los mundos de BelKan*. El objetivo principal es que el agente explore el mapa, identifique diferentes tipos de terreno, evite obstáculos y se posicione correctamente, con el fin de maximizar el porcentaje del mapa descubierto.

---

## 🗺️ 2. Escenario
El mundo es una matriz de hasta **100x100 celdas** que contiene diversos terrenos y elementos:

-   **Terrenos:**
    -   `B`: Bosque
    -   `A`: Agua
    -   `P`: Precipicio
    -   `T`: Arenoso
    -   `S`: Pedregoso
    -   `M`: Muro

-   **Objetos Especiales:**
    -   `K`: Bikini (permite nadar en Agua)
    -   `D`: Zapatillas (reduce coste en Bosque)
    -   `X`: Recarga de Batería
    -   `G`: Posicionamiento (recalibra sensores)

-   **Agentes Móviles:**
    -   `a`: Aldeanos
    -   `l`: Lobos
    -   `j`: Jugador (nuestro agente)

---

## 💡 3. Características del Agente

### Sensores
El agente percibe el entorno a través de un conjunto de sensores que le informan sobre:
- Terreno, agentes cercanos, colisiones, estado de `reset`.
- Posición, orientación, nivel de batería, nivel actual y tiempo restante.

### Acciones
El agente puede ejecutar las siguientes acciones, cada una con un coste de batería variable:
- `actWALK`: Avanzar 1 casilla.
- `actRUN`: Avanzar 2 casillas.
- `actTURN_SR`: Girar 45° a la derecha.
- `actTURN_L`: Girar 90° a la izquierda.
- `actIDLE`: No hacer nada (coste mínimo).

---

## 💻 4. Implementación
El núcleo del comportamiento se define en los ficheros `jugador.cpp` y `jugador.hpp`, dentro de la clase `ComportamientoJugador`. La lógica se basa en un conjunto de **reglas reactivas** que responden a estímulos inmediatos:

-   🔎 **Detección de Obstáculos:** Lee el vector de terreno para identificar y evitar obstáculos inmediatos (`M`, `P`).
-   🧭 **Estrategia de Exploración:** Prioriza el movimiento hacia casillas desconocidas (`?`) para maximizar la cobertura.
-   🧱 **Evasión de Muros:** Implementa una lógica de seguimiento de paredes para evitar quedarse atascado.
-   🔋 **Gestión de Batería:** Optimiza el uso de bikini y zapatillas para minimizar el consumo de energía en terrenos costosos.
-   📍 **Autocalibración:** En niveles sin sensores de posición/orientación, utiliza las casillas `G` para recalibrar su estado interno.

El agente **no emplea planificación global ni búsqueda heurística**; todas sus decisiones se basan en el estado actual de sus sensores.

---

## 🧪 5. Pruebas y Evaluación
El comportamiento del agente ha sido evaluado en distintos niveles con desafíos incrementales:

-   **Nivel 0:** Exploración sistemática con sensores completos. Se logra una cobertura muy alta.
-   **Nivel 1:** Desafío sin sensor de posición. El agente ajusta su rumbo basándose en referencias visuales y obstáculos.
-   **Nivel 2:** Evasión activa de lobos y aldeanos.
-   **Nivel 3:** Gestión con orientación inicial desconocida y sensores potencialmente defectuosos.

---

## 📊 6. Resultados
El agente logra un **alto porcentaje de exploración** en todos los niveles, demostrando una especial eficacia en los niveles 0 y 1. En escenarios con enemigos, las tácticas de evasión son funcionales, aunque los reinicios forzados por colisiones pueden reducir la eficiencia global.

---

## 🏁 7. Conclusiones
-   El enfoque **reactivo simple** permite un comportamiento ágil y muy adaptable en entornos dinámicos y parcialmente observables.
-   La **ausencia de memoria global** limita la optimización de rutas, pero a cambio proporciona una alta tolerancia a cambios locales inesperados.
-   Como mejora futura, se podría implementar un **mapa auxiliar** simple para recordar zonas ya visitadas y trazar rutas más seguras, especialmente en los niveles más complejos.
