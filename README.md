# 🎬 UTECFlix

## 1. Portada

| Campo | Información |
|---|---|
| **Proyecto** | UTECFlix |
| **Curso** | Programación III |
| **Universidad** | Universidad de Ingeniería y Tecnología - UTEC |
| **Integrantes** | Jeseph Imanol Burgos Ochoa<br>Jean Piere Milan Arana Chiong<br>Anthony Yonel Rosales Esteban<br>Omar Alonso Guzman Harvey<br>Santiago Cesar Morales Portella |
| **Docente** | José Chavez |

---

## 2. Descripción del proyecto

**UTECFlix** es una aplicación de consola en C++ orientada a la búsqueda, gestión y consulta de películas a partir de un archivo CSV llamado `peliculas.csv`.

El sistema lee un catálogo de películas, limpia sus campos principales, construye un índice de búsqueda basado en `Trie` y `Suffix Trie`, y permite que un usuario interactúe con el catálogo mediante registro, inicio de sesión, búsqueda textual, recomendaciones, favoritos, historial y notificaciones.

El problema principal que resuelve es la consulta eficiente de películas en un conjunto grande de datos textuales. Para ello, el proyecto combina estructuras de datos, patrones de diseño, procesamiento de archivos, programación paralela y una interfaz de consola para Windows.

---

## 3. Características principales

- Registro de nuevos usuarios.
- Inicio de sesión con validación de correo y contraseña.
- Recuperación de contraseña mediante preguntas de seguridad o historial de búsquedas, cuando existe información suficiente.
- Gestión de perfil:
  - cambio de nombre;
  - cambio de fecha de nacimiento;
  - cambio de contraseña.
- Lectura de películas desde `peliculas.csv`.
- Limpieza y normalización de datos de películas.
- Exportación de datos limpios a `datosLimpios.csv`.
- Búsqueda de películas por texto libre.
- Índice de búsqueda con `Trie` y soporte parcial de `Suffix Trie`.
- Ranking de resultados por puntaje.
- Soporte de coincidencias por palabras y frases cortas.
- Recomendaciones aleatorias de películas.
- Filtro de visualización según edad del usuario mediante Proxy.
- Registro de historial de acciones.
- Visualización de favoritos.
- Sistema de likes, favoritos y "ver más tarde"; en el código actual, favoritos y likes comparten la misma lista interna `likes`.
- Notificaciones generadas por acciones del usuario.
- Paginación de resultados.
- Procesamiento paralelo durante la indexación.
- Búsqueda paralela sobre múltiples tries parciales.
- Registro de métricas de tiempo en `metricas_tiempo.txt`.

---

## 4. Arquitectura del proyecto

Estructura real del repositorio en la raíz del proyecto:

```text
Proyecto-1_Programaci-n_III_UTEC/
├── .gitignore
├── CMakeLists.txt
├── Command.cpp
├── Command.h
├── Interfaz.cpp
├── Interfaz.h
├── Iterator.cpp
├── Iterator.h
├── IUsuarios.cpp
├── IUsuarios.h
├── LimPelis.cpp
├── LimPelis.h
├── main.cpp
├── Memento.cpp
├── Memento.h
├── Observer.cpp
├── Observer.h
├── peliculas.csv
├── Procesador.cpp
├── Procesador.h
├── Proxy.cpp
├── Proxy.h
├── RankingStrategy.cpp
├── RankingStrategy.h
├── README.md
├── Singleton.cpp
├── Singleton.h
├── Trie.cpp
├── Trie.h
├── Utilidades.cpp
└── Utilidades.h
```

También existen localmente las carpetas `.idea/` y `cmake-build-debug/`, generadas por el IDE y por CMake. No se detallan porque contienen configuración local y artefactos de compilación.

### Responsabilidades principales

| Archivo | Responsabilidad |
|---|---|
| `main.cpp` | Punto de entrada, carga paralela del catálogo, autenticación inicial y ciclo principal del menú. |
| `Utilidades.h/.cpp` | Modelos base (`Movie`, `DataLimpia`, `Usuario`) y utilidades generales. |
| `LimPelis.h/.cpp` | Lectura del CSV, parseo de filas, limpieza de texto y exportación de datos limpios. |
| `Trie.h/.cpp` | Implementación del Trie, sufijos parciales, búsqueda y cálculo de scores. |
| `Procesador.h/.cpp` | Tokenización, indexación paralela, búsqueda paralela y coordinación del ranking. |
| `Interfaz.h/.cpp` | Interfaz de consola, menús, perfil, búsqueda y detalle de películas. |
| `IUsuarios.h/.cpp` | Registro, login, recuperación de contraseña, historial, favoritos y notificaciones. |
| `Singleton.h/.cpp` | Catálogo global de películas y motor de búsqueda. |
| `RankingStrategy.h/.cpp` | Estrategia de ordenamiento de resultados. |
| `Proxy.h/.cpp` | Control de visualización de películas según edad. |
| `Iterator.h/.cpp` | Paginación de resultados e iteración de historial. |
| `Memento.h/.cpp` | Clases de soporte para snapshots de historial. |
| `Observer.h/.cpp` | Notificaciones ante acciones del usuario. |
| `Command.h/.cpp` | Comandos para modificar datos del perfil. |

### Archivos generados en ejecución

Los siguientes archivos no están incluidos actualmente en la raíz del repositorio, pero el código puede crearlos o leerlos durante la ejecución:

| Archivo | Estado real | Uso |
|---|---|---|
| `datosLimpios.csv` | Generado durante la carga del catálogo. | Guarda campos limpios de películas. |
| `metricas_tiempo.txt` | Generado si se registran métricas. | Guarda tiempos de lectura, limpieza, carga, indexación y búsqueda. |
| `registroUsuarios.txt` | Generado al registrar usuarios. | Persiste credenciales y listas del usuario. |
| `historialUsuarios.txt` | Generado al registrar acciones. | Persiste búsquedas, visualizaciones y acciones. |
| `notificaciones.txt` | Generado por el Observer. | Persiste notificaciones por usuario. |
| `preguntasRecuperacion.csv` | Generado si se configuran preguntas. | Persiste preguntas y respuestas de recuperación. |

---

## 5. Tecnologías utilizadas

| Tecnología | Uso en el proyecto |
|---|---|
| **C++17** | El código usa recursos compatibles con C++17 como STL, `std::thread`, `std::future`, `std::async`, `std::unique_ptr`, lambdas y contenedores estándar. |
| **C++20 configurado en CMake** | El archivo `CMakeLists.txt` establece actualmente `CMAKE_CXX_STANDARD 20`. Si la evaluación exige estrictamente C++17, debe cambiarse a `17` y recompilarse para validar compatibilidad. |
| **STL** | Uso de `vector`, `unordered_map`, `unordered_set`, `stack`, `string`, `stringstream`, algoritmos y utilidades de tiempo. |
| **Threads** | Indexación paralela del catálogo y destrucción paralela de tries en `Procesador`. |
| **Futures / async** | Carga asíncrona de notificaciones en `main.cpp` con `std::async` y `std::future`. |
| **Atomics** | `atomic_bool datosListos` coordina la espera entre la carga del catálogo y la interfaz. |
| **Smart Pointers** | `unique_ptr<Trie>` y `unique_ptr<RankingStrategy>` administran memoria y polimorfismo. |
| **CMake** | Compilación del ejecutable `PROYECTAZO`. |
| **CSV** | Entrada principal `peliculas.csv`; salida generada `datosLimpios.csv`; soporte adicional para `preguntasRecuperacion.csv`. |
| **Windows API** | Uso de `windows.h`, `conio.h`, `SetConsoleOutputCP`, `SetConsoleCP`, `_getch()` y control de consola. |

---

## 6. Patrones de diseño

| Patrón | Dónde se usa | Beneficio |
|---|---|---|
| **Singleton** | `CatalogoPeliculas` en `Singleton.h/.cpp`. | Mantiene una única instancia del catálogo, evita duplicar estructuras grandes y centraliza carga, búsqueda y acceso a películas. |
| **Strategy** | `RankingStrategy` y `RankingPorScoreStrategy`. | Permite cambiar el criterio de ordenamiento sin modificar `Procesador` ni `Trie`. |
| **Proxy** | `IPelicula`, `PeliculaReal`, `PeliculaProxy`. | Controla si una película puede mostrarse según la edad del usuario sin alterar la clase `Movie`. |
| **Observer** | `AccionUsuarioSubject`, `ObservadorAccion`, `NotificacionesArchivoObserver`. | Desacopla las acciones del usuario de la escritura de notificaciones en archivo. |
| **Command** | `PerfilCommand`, `CambiarNombreCommand`, `CambiarFechaNacimientoCommand`, `CambiarPasswordCommand`. | Encapsula cambios del perfil como operaciones independientes. |
| **Iterator** | `ResultadosIterator` y `HistorialIterator`. | Permite recorrer resultados por páginas e historial sin exponer directamente la lógica de recorrido. |
| **Memento** | `Memento` y `HistorialCareTaker` están implementados en `Memento.h/.cpp`. | Proveen soporte para guardar y restaurar snapshots de historial; en el flujo principal actual no se instancia `HistorialCareTaker`, por lo que el historial visible se maneja con archivos e `HistorialIterator`. |

---

## 7. Estructuras de datos

| Estructura | Dónde aparece | Motivo de uso |
|---|---|---|
| **Trie** | `Trie`, nodo raíz `raiz`. | Búsqueda eficiente por caracteres y prefijos. |
| **Suffix Trie parcial** | Inserción de subcadenas limitadas y raíz auxiliar `raizSufijos`. | Permite coincidencias parciales y búsqueda de sufijos largos con control de memoria. |
| **`unordered_map`** | Películas por ID, frecuencias por documento, hijos de nodos, usuarios y scores. | Acceso promedio O(1), útil para índices, conteos y asociaciones por clave. |
| **`unordered_set`** | Control de tokens vistos por documento y stopwords. | Evita duplicados y permite búsquedas rápidas de pertenencia. |
| **`vector`** | Resultados, tokens, threads, tries, historial y usuarios. | Contenedor secuencial flexible para recorridos y particionado. |
| **`stack`** | Notificaciones de usuario. | Muestra primero la notificación más reciente. |
| **`queue`** | No se encontró uso real en el código actual. | No aplica. |
| **`priority_queue`** | No se encontró uso real en el código actual. | No aplica. |

---

## 8. Algoritmos

| Algoritmo | Implementación | Complejidad temporal aproximada |
|---|---|---|
| Lectura robusta de CSV | `leerFilaCSV` une líneas hasta equilibrar comillas; `parseCSVLine` separa columnas respetando comillas. | O(n) por fila, donde `n` es la longitud de la fila completa. |
| Limpieza de texto | Normalización a minúsculas, eliminación de caracteres no deseados, manejo básico de UTF-8 y filtrado de stopwords. | O(n) por campo. |
| Tokenización | `tokenizar` separa texto por espacios usando `stringstream`. | O(n). |
| Inserción en Trie | `insertarpalabra` inserta palabra completa y subcadenas limitadas por `MAX_LONG`. | O(L * MAX_LONG) aproximado; con `MAX_LONG = 6`, se comporta lineal respecto a la longitud del token. |
| Búsqueda en Trie | `buscarNodo` recorre caracteres de la consulta. | O(L + R), donde `L` es la longitud del token y `R` la cantidad de resultados asociados. |
| Búsqueda de sufijo largo | Invierte el token y consulta `raizSufijos` cuando la búsqueda normal no encuentra resultados. | O(L + R). |
| Ranking por score | `RankingPorScoreStrategy` ordena resultados por puntaje. | O(M log M), donde `M` es la cantidad de candidatos. |
| Paginación | `ResultadosIterator` divide resultados en páginas de tamaño fijo. | O(k) por página, donde `k` es el tamaño de página. |
| Recomendación aleatoria | Selección aleatoria de películas evitando repeticiones y géneros repetidos dentro de la interacción. | Depende de intentos; el código limita intentos a 100 por recomendación. |

---

## 9. Programación paralela

El proyecto usa paralelismo para reducir el tiempo de carga, indexación y búsqueda del catálogo.

| Clase / archivo | Recurso usado | Tarea paralela |
|---|---|---|
| `main.cpp` | `std::thread`, `std::atomic_bool` | Carga del catálogo en segundo plano mientras se muestra el flujo inicial de la interfaz. |
| `main.cpp` | `std::async`, `std::future` | Carga de notificaciones del usuario autenticado. |
| `Procesador.cpp` | `std::thread` | División del catálogo entre `NUM_THREADS` workers para construir tries parciales. |
| `Procesador.cpp` | `std::thread` | Búsqueda paralela sobre cada Trie parcial y combinación posterior de scores. |
| `Procesador.cpp` | `std::thread` | Liberación paralela de memoria en el destructor de `Procesador`. |

La constante `NUM_THREADS` está definida en `Utilidades.h` con valor `8`.

### Mediciones reales

Las mediciones se tomaron ejecutando los binarios desde terminal con el `PATH` de MinGW configurado y usando el archivo real `peliculas.csv` del repositorio. El programa registra tiempos en `metricas_tiempo.txt` mediante `std::chrono::high_resolution_clock`.

Las mediciones paralelas corresponden al promedio de 5 ejecuciones completas de inicialización del catálogo con 34 886 películas cargadas. Las mediciones secuenciales corresponden a 1 ejecución completa usando un `Trie` único sin hilos. Los valores están en milisegundos.

| Operación | Secuencial | Paralelo | Mejora |
|:---|:---:|:---:|:---:|
| Lectura de `peliculas.csv` | 6 718 ms | 3 174 ms | 2.1x más rápido |
| Limpieza y exportación a `datosLimpios.csv` | 13 289 ms | 10 463 ms | 1.27x más rápido |
| Inserción del índice de búsqueda | 305 847 ms | 57 699 ms | 5.3x más rápido |
| Carga total del catálogo | 326 148 ms | 71 647 ms | 4.55x más rápido |

Detalle de ejecuciones:

| Tipo | Ejecución | Lectura CSV | Limpieza | Inserción | Carga total |
|:---|:---:|:---:|:---:|:---:|:---:|
| Paralela | 1 | 3 422 ms | 14 054 ms | 62 216 ms | 80 189 ms |
| Paralela | 2 | 3 212 ms | 14 240 ms | 68 507 ms | 86 398 ms |
| Paralela | 3 | 4 470 ms | 12 894 ms | 59 624 ms | 77 174 ms |
| Paralela | 4 | 2 394 ms | 6 304 ms | 38 907 ms | 47 782 ms |
| Paralela | 5 | 2 372 ms | 4 824 ms | 59 243 ms | 66 691 ms |
| Secuencial | 1 | 6 718 ms | 13 289 ms | 305 847 ms | 326 148 ms |

Las mediciones secuenciales se obtuvieron con un benchmark dedicado (`BENCHMARK.exe`) que construye un solo `Trie` sin uso de hilos, procesando el mismo dataset de 34 886 películas. El benchmark incluyó serialización del `Trie` a un archivo temporal (`trie_temp.bin`, ~653 MB), su carga y posterior eliminación, como comprobación de persistencia en disco sin programación paralela.

---

## 10. Manejo de errores y validaciones

Validaciones implementadas en el código:

- Verificación de apertura de archivos:
  - `peliculas.csv`;
  - `registroUsuarios.txt`;
  - `preguntasRecuperacion.csv`;
  - `notificaciones.txt`;
  - `datosLimpios.csv`;
  - `metricas_tiempo.txt`.
- Si no se cargan películas desde el CSV, `CatalogoPeliculas::cargarDatos` lanza un `runtime_error`.
- Si no se construye la data limpia, `CatalogoPeliculas::cargarDatos` lanza un `runtime_error`.
- Si se intenta buscar antes de inicializar el catálogo, `CatalogoPeliculas::buscar` lanza un `logic_error`.
- Lectura de CSV con control de comillas para evitar cortar sinopsis multilínea.
- Ignora filas del CSV con menos de 8 columnas.
- Limpieza de texto con normalización de minúsculas, acentos y caracteres especiales.
- Descarte de tokens de longitud menor o igual a 2 en el Trie.
- Retorno vacío ante búsquedas sin tokens válidos.
- Validación de correo repetido al registrar usuario.
- Validación de credenciales en inicio de sesión.
- Validación de contraseñas repetidas durante registro y cambio de contraseña.
- Validación de contraseña actual antes de modificar datos del perfil.
- Validación de fecha de nacimiento mediante `calcularEdad`.
- Validación de existencia de usuario antes de actualizar datos.
- Evita duplicar IDs en listas de likes, historial y ver más tarde.
- Manejo de notificaciones vacías.
- Manejo de historial vacío.
- Manejo de favoritos vacíos.
- Ignora IDs de películas no encontrados al mostrar listas.
- Control de edad al mostrar películas mediante `PeliculaProxy`.

---

## 11. Requisitos

- **Sistema operativo:** Windows.
- **IDE recomendado:** CLion.
- **Toolchain:** Visual Studio o MinGW compatible con CMake.
- **CMake:** versión 3.20 o superior.
- **Estándar de C++ requerido por el curso:** C++17.
- **Configuración actual del repositorio:** `CMAKE_CXX_STANDARD 20`.
- **Archivo de datos:** `peliculas.csv`.

El proyecto depende explícitamente de:

- `windows.h`: configuración y control de consola en Windows.
- `conio.h`: captura de teclas como flechas y Enter mediante `_getch()`.

Por esas dependencias, el proyecto no es portable directamente a Linux o macOS sin adaptar la interfaz de consola.

---

## 12. Instalación y ejecución

### Opción recomendada: CLion

1. Abrir la carpeta del proyecto en CLion.
2. Verificar que `peliculas.csv` esté en la raíz del proyecto:

```text
Proyecto-1_Programaci-n_III_UTEC/peliculas.csv
```

3. Cargar el proyecto con CMake.
4. Compilar el target `PROYECTAZO`.
5. Ejecutar el programa desde CLion.

El código también intenta resolver `peliculas.csv` en estas rutas:

```text
peliculas.csv
cmake-build-debug/peliculas.csv
../peliculas.csv
../cmake-build-debug/peliculas.csv
```

Por claridad, se recomienda mantener el archivo en la raíz del proyecto. El archivo `datosLimpios.csv` se genera durante la ejecución.

### Compilación con CMake

```bash
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug
```

Luego ejecutar el binario generado por CMake dentro de `cmake-build-debug`.


## 14. Resultados

El proyecto logra integrar un flujo completo de consulta de películas desde datos en CSV:

- carga y parseo de un catálogo grande;
- limpieza de campos textuales;
- construcción de un índice de búsqueda;
- búsqueda textual con ranking;
- interacción de usuario con registro, login, perfil, historial, favoritos y notificaciones;
- aplicación de patrones de diseño vistos en el curso;
- uso de programación paralela para indexación y consulta;
- persistencia básica en archivos `.txt` y `.csv`.

Sus principales ventajas son la separación de responsabilidades por archivos, el uso de estructuras de datos adecuadas para búsqueda textual y la incorporación de patrones de diseño aplicados a casos concretos del sistema.


## 16. Referencias

Gamma, E., Helm, R., Johnson, R., & Vlissides, J. (1994). *Design patterns: Elements of reusable object-oriented software*. Addison-Wesley.

Lippman, S. B., Lajoie, J., & Moo, B. E. (2012). *C++ Primer* (5th ed.). Addison-Wesley.

Meyers, S. (2014). *Effective Modern C++: 42 specific ways to improve your use of C++11 and C++14*. O'Reilly Media.

ISO. (2024). *ISO/IEC 14882:2024: Programming languages - C++*. International Organization for Standardization.

cppreference. (s. f.). *C++ reference*. https://en.cppreference.com/

GeeksforGeeks. (s. f.). *Trie data structure*. https://www.geeksforgeeks.org/trie-insert-and-search/

Sedgewick, R., & Wayne, K. (2011). *Algorithms* (4th ed.). Addison-Wesley.

Williams, A. (2019). *C++ Concurrency in Action* (2nd ed.). Manning Publications.

