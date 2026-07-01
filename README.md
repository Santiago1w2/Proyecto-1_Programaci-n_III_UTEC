# UTECFlix - Proyecto 1 Programacion III

Motor de busqueda de peliculas por texto libre construido en C++20. El proyecto
lee el dataset `wiki_movie_plots_deduped.csv`, limpia los campos relevantes,
construye un indice tipo Suffix Trie y permite buscar peliculas por titulo,
anio, origen, director, reparto, genero o sinopsis.

## Integrantes

- Jeseph Imanol Burgos Ochoa
- Jean Piere Milan Arana Chiong
- Anthony Yonel Rosales Esteban
- Omar Alonso Guzman Harvey
- Santiago Cesar Morales Perlaza

## Arquitectura

El proyecto mantiene una arquitectura modular:

| Modulo | Responsabilidad |
|---|---|
| `main.cpp` | Punto de entrada, argumentos, benchmark y flujo interactivo |
| `Utilidades.*` | Clases de dominio: `Movie`, `DataLimpia`, `Usuario` |
| `LimPelis.*` | Lectura CSV, parseo con comillas/multilinea y limpieza de texto |
| `Trie.*` | Arbol Suffix Trie usado como indice principal |
| `Procesador.*` | Tokenizacion, pesos por campo, indexacion secuencial/paralela |
| `Singleton.*` | Fachada unica del motor de busqueda |
| `Proxy.*` | Control de acceso a peliculas restringidas |
| `Memento.*` | Snapshots del historial de sesion |
| `Iterator.*` | Recorrido del historial |
| `Benchmark.h` | Utilidades genericas con templates para medir tiempos |
| `Interfaz.*`, `IUsuarios.*` | Interfaz de consola y funciones de usuario/recomendacion |

## Dataset

El programa busca el dataset en este orden:

1. Ruta pasada como argumento.
2. `peliculas.csv`.
3. `wiki_movie_plots_deduped.csv`.
4. `wiki_movie_plots_deduped (2).csv`.

Ejemplos:

```bash
PROYECTAZO.exe wiki_movie_plots_deduped.csv
PROYECTAZO.exe "wiki_movie_plots_deduped (2).csv" --benchmark
```

El archivo limpio `datosLimpios.csv` solo se exporta si se usa:

```bash
PROYECTAZO.exe --export-clean
```

## Flujo De Ejecucion

1. `Singleton` carga el dataset con `leerPeliculas`.
2. `LimPelis` transforma cada fila CSV en `Movie`.
3. `exportarDataLimpiaCSV` limpia los campos y llena `DataLimpia`.
4. `Procesador` tokeniza cada pelicula con pesos por campo:
   - titulo: 5
   - plot: 2
   - anio, origen, director, cast, genero: 1
5. `Trie` inserta palabras completas y sufijos limitados por `MAX_LONG`.
6. Las busquedas consultan los Tries, calculan score con frecuencia, IDF y
   cobertura de consulta, y devuelven el top 5.

## Estructura De Datos Principal

La estructura principal es un Suffix Trie:

- Cada nodo tiene 256 hijos posibles.
- Cada nodo mantiene un mapa `movieID -> frecuencia/peso`.
- Se insertan palabras completas con mayor peso.
- Tambien se insertan sufijos de hasta `MAX_LONG` caracteres para permitir
  coincidencias parciales.
- El Trie libera su memoria recursivamente al finalizar.

## Programacion Paralela

`Procesador` crea varios Tries internos y divide las peliculas entre threads.
Cada thread indexa su bloque de peliculas en su propio Trie, evitando escritura
compartida sobre la misma estructura. Luego se combinan las frecuencias globales
para calcular IDF.

Tambien existe `procesarSecuencial`, usado como linea base para benchmark.

## Programacion Generica

`Benchmark.h` implementa funciones template reutilizables:

- `medirTiempo<Funcion>` mide cualquier algoritmo invocable.
- `tomarTopN<T, Comparador>` ordena y recorta colecciones genericas.

Estas utilidades usan templates de C++ para trabajar con distintos tipos y
funciones sin duplicar logica.

## Patrones De Diseno

El proyecto implementa cuatro patrones:

| Patron | Archivos | Uso |
|---|---|---|
| Singleton | `Singleton.*` | Una unica instancia del motor de busqueda |
| Proxy | `Proxy.*` | Control de acceso a peliculas restringidas por edad |
| Memento | `Memento.*` | Snapshots del historial de busqueda |
| Iterator | `Iterator.*` | Recorrido y filtrado del historial |

## Manejo De Errores

El proyecto usa excepciones para errores recuperables:

- dataset inexistente o no legible;
- limpieza sin datos indexables;
- intento de busqueda antes de inicializar el motor;
- edad invalida en `--age`;
- error al crear `datosLimpios.csv`.

`main` captura `std::exception` y muestra un mensaje claro sin abortar de forma
inesperada.

## Benchmark

Ejecutar:

```bash
PROYECTAZO.exe "wiki_movie_plots_deduped (2).csv" --benchmark
```

Formato de salida:

| Algoritmo | Peliculas | Tiempo(ms) |
|---|---:|---:|
| Indexacion secuencial | dataset completo | generado en ejecucion |
| Indexacion paralela | dataset completo | generado en ejecucion |

La tabla impresa puede copiarse a este README luego de ejecutar en la maquina
de entrega.

## Compilacion

Con CMake:

```bash
cmake -S . -B build
cmake --build build
```

El proyecto usa C++20 y APIs de consola de Windows.

## Estado Final Esperado

- Arbol funcional: Suffix Trie.
- Templates: utilidades genericas de benchmark/top-N.
- Paralelismo: indexacion y busqueda con threads.
- Patrones: Singleton, Proxy, Memento, Iterator.
- Dataset grande: lectura multilinea, limpieza e indexacion paralela.
- Excepciones: errores de archivo, inicializacion y argumentos.
- Arquitectura modular: lectura, limpieza, indice, UI y patrones separados.
