# 🎬 UTECFlix — Proyecto 1 · Programación III · UTEC

Motor de búsqueda de películas por texto libre, construido sobre un **Trie** de caracteres, con limpieza de datos en varios pasos y una interfaz de consola interactiva para Windows.

---

## Tabla de contenidos

1. [Estructura del proyecto](#estructura-del-proyecto)
2. [Lectura del CSV](#lectura-del-csv)
3. [Pre-procesamiento y limpieza de datos](#pre-procesamiento-y-limpieza-de-datos)
4. [Pseudo-código de inserción al Trie](#pseudo-código-de-inserción-al-trie)
5. [Estructura de datos: Trie](#estructura-de-datos-trie)
6. [Ejemplos de limpieza](#ejemplos-de-limpieza)
7. [Avance de la interfaz](#avance-de-la-interfaz)
8. [Cómo ejecutar](#cómo-ejecutar)

---

## Estructura del proyecto

```
Proyecto-1_Programaci-n_III_UTEC/
├── main.cpp                  ← Punto de entrada
├── CClases.h / CClases.cpp   ← Clases Movie, Usuario, DataLimpia
├── PLimpieza.h / PLimpieza.cpp ← Toda la lógica de limpieza
├── Funciones.h / Funciones.cpp ← Lectura CSV, manejo de usuarios
├── Trie.h / Trie.cpp          ← Estructura de datos principal
├── Interfaz.h / Interfaz.cpp  ← UI de consola (Windows)
└── cmake-build-debug/
    ├── peliculas.csv           ← Dataset original (wiki_movie_plots_deduped.csv)
    └── datosLimpios.csv        ← CSV generado tras la limpieza
```

---

## Lectura del CSV

El dataset fuente es `wiki_movie_plots_deduped.csv` (renombrado a `peliculas.csv`).  
Tiene 8 columnas por fila:

| # | Columna            | Descripción                            |
|---|--------------------|----------------------------------------|
| 0 | Release Year       | Año de estreno                         |
| 1 | Title              | Título de la película                  |
| 2 | Origin/Ethnicity   | Origen / Industria cinematográfica     |
| 3 | Director           | Director(es)                           |
| 4 | Cast               | Reparto                                |
| 5 | Genre              | Género(s)                              |
| 6 | Wiki Page          | URL de Wikipedia                       |
| 7 | Plot               | Sinopsis (puede tener saltos de línea) |

### El problema de los saltos de línea en los plots

Un `getline()` normal rompería la lectura cuando la sinopsis tiene saltos de línea internos envueltos en comillas. Se usaron dos funciones para resolverlo:

**`leerFilaCSV(ifstream& file, string& lineaCompleta)`**  
Lee líneas y las concatena mientras haya un número impar de comillas abiertas. Garantiza que cada llamada devuelve exactamente una fila lógica del CSV, sin importar cuántas líneas físicas ocupe.

**`parseCSVLine(const string& linea)`**  
Parsea la fila lógica campo a campo, respetando las comillas dobles RFC-4180 (incluyendo comillas escapadas como `""`). Devuelve un `vector<string>` con los 8 campos.

Ambas funciones se reutilizan también para la lectura de usuarios.

---

## Pre-procesamiento y limpieza de datos

Una vez leídas las películas en el `unordered_map<int, Movie>`, se invoca `exportarDataLimpiaCSV()`, que aplica una cadena de transformaciones sobre cada campo antes de insertar las palabras al Trie.

### Flujo general por película

```
Movie (raw)
    │
    ├─ Year         → se usa tal cual (solo números)
    ├─ Title        → limpiarTitulo()
    ├─ Origin       → limpiarOrigen()
    ├─ Director     → limpiarDirector()
    ├─ Cast         → limpiarCast()
    ├─ Genre        → normalizarYLimpiar()
    └─ Plot         → normalizarYLimpiar() → filtrarStopwords()
            │
            ▼
    String unificada: "año titulo origen director cast genero plot"
            │
            ▼
    Inserción al Trie  +  Exportación a datosLimpios.csv
```

### Dos salidas del pipeline

`exportarDataLimpiaCSV` hace dos cosas en un solo recorrido del mapa de películas, evitando procesar las ~35,000 entradas dos veces:

1. **Escribe `datosLimpios.csv`** — archivo con los 7 campos limpios por fila, sin comillas, útil para depuración e inspección manual.
2. **Llena `unordered_map<int, DataLimpia>`** — mapa en memoria (pasado por referencia) que asocia cada ID de película con su `DataLimpia` correspondiente. Este mapa es el que usa el resto del sistema para mostrar resultados al usuario.

### `prepararDataLimpia` — Alimentando el Trie

```cpp
unordered_map<int, string> prepararDataLimpia(const unordered_map<int, Movie>& pelis);
```

Esta función aplica el mismo pipeline de limpieza que `exportarDataLimpiaCSV`, pero en lugar de escribir un archivo, devuelve un `unordered_map<int, string>` donde cada valor es una **cadena unificada** con todos los campos de la película concatenados:

```
"1942 casablanca american michael curtiz humphrey bogart ingrid bergman drama rick blaine..."
```

Esa cadena es la que se tokeniza e inserta palabra por palabra al Trie. El ID entero actúa como clave para relacionar cada palabra indexada con su película de origen.

### Estructura `DataLimpia`

`DataLimpia` es un struct definido en `CClases.h` que almacena los campos de una película ya procesados y normalizados:

```cpp
struct DataLimpia {
    string title, release_year, origin, director, cast, genre, plot;
};
```

A diferencia de `Movie` (que guarda los datos crudos del CSV incluyendo `wiki_page`), `DataLimpia` omite la URL de Wikipedia y solo conserva los campos indexables ya limpios. Es el resultado final del pipeline de limpieza.

### Funciones de limpieza detalladas

#### `limpiarTextoAvanzado(s, parentesisProhibidos, palabrasProhibidas)` — Función maestra

Es el núcleo del sistema de limpieza. Trabaja en dos fases:

**Fase 1 — Eliminación de corchetes y paréntesis:**
- `[texto]`: si el contenido son solo números o referencias de Wikipedia (`[1]`, `[2]`...), se elimina por completo. Si el contenido es texto real (p. ej. `[also known as]`), se conserva el texto sin los corchetes.
- `(texto)`: si el contenido en minúsculas contiene alguna palabra de `parentesisProhibidos` (como `"film"` o `"part"`), se descarta todo el bloque. En caso contrario, se conserva el texto interior.

**Fase 2 — Procesamiento carácter a carácter:**
- Caracteres ASCII alfanuméricos: se normalizan a minúsculas.
- Caracteres UTF-8 multibyte: se buscan en el mapa `accents`. Si el carácter no existe en el mapa (cirílico, chino, árabe, etc.), la **palabra completa** se descarta.
- Separadores y puntuación: disparan el guardado de la palabra acumulada.
- Palabras en `palabrasProhibidas`: se filtran exactamente (p. ej. `"director"` en campos de cast).

#### `limpiarTitulo(s)`
Llama a `limpiarTextoAvanzado` prohibiendo `"film"` y `"part"` dentro de paréntesis.  
Caso real: `"Spider-Man (film)"` → `"spider man"`.

#### `limpiarDirector(s)`
Llama a `limpiarTextoAvanzado` sin bloqueos en paréntesis, pero filtrando la palabra exacta `"director"` para no indexarla como término de búsqueda.  
Caso real: `"James Cameron (director)"` → `"james cameron"`.

#### `limpiarOrigen(s)`
Lógica propia más rápida: solo acepta letras (convierte a minúsculas) y descarta todo lo demás. Se usa una lógica independiente porque el campo Origin solo contiene nombres de países e industrias, sin paréntesis ni caracteres especiales relevantes.  
Caso real: `"American/British"` → `"american british"`.

#### `limpiarCast(s)`
Llama a `limpiarTextoAvanzado` filtrando las palabras `"director"` y `"screenplay"` que a veces aparecen mezcladas en la columna de reparto de Wikipedia.  
Caso real: `"Tom Hanks, Robin Wright (screenplay by Eric Roth)"` → `"tom hanks robin wright"`.

#### `normalizarYLimpiar(s)`
Limpieza genérica para los campos Genre y Plot. Aplica el mismo mapa de acentos, maneja corchetes de Wikipedia (`[citation needed]`, `[dead link]`, `[better source needed]`, etc.) y normaliza el resto a minúsculas con espacios como separadores. A diferencia de la función maestra, no descarta palabras por caracteres no mapeados: los reemplaza por espacio, lo que la hace más permisiva pero suficiente para estos campos.

#### `filtrarStopwords(textoLimpio)`
Se aplica exclusivamente al Plot después de `normalizarYLimpiar`. Elimina ~100 palabras de ruido del inglés para reducir el tamaño del índice y mejorar la precisión de las búsquedas.

La lista es en inglés porque el dataset es predominantemente en inglés. Los criterios para incluir una palabra fueron:

- **Alta frecuencia, bajo valor semántico**: artículos, preposiciones, conjunciones y pronombres (`"the"`, `"and"`, `"his"`, `"from"`...).
- **Verbos auxiliares y copulativos**: `"was"`, `"were"`, `"have"`, `"had"`, `"does"`, `"been"`.
- **Adverbios y cuantificadores genéricos**: `"very"`, `"much"`, `"most"`, `"many"`, `"few"`, `"already"`, `"never"`.
- **Palabras narrativas sin valor de búsqueda**: `"going"`, `"found"`, `"saying"`, `"told"`, `"seems"` — aparecen en casi cualquier sinopsis y no distinguen una película de otra.

> La lista **no** se aplica a título, director, cast ni origen, porque en esos campos palabras como `"the"` o `"will"` pueden ser parte de un nombre propio.

### Decisión de diseño: UTF-8 manual sin librerías externas

El sistema procesa texto UTF-8 sin usar ninguna librería de internacionalización (como ICU o `<codecvt>`). En su lugar, detecta la longitud de cada carácter multibyte inspeccionando los bits del primer byte:

| Patrón del primer byte | Longitud del carácter |
|---|---|
| `0xxxxxxx` (< 0x80) | 1 byte — ASCII estándar |
| `110xxxxx` (& 0xE0 == 0xC0) | 2 bytes |
| `1110xxxx` (& 0xF0 == 0xE0) | 3 bytes |
| `11110xxx` (& 0xF8 == 0xF0) | 4 bytes |

Una vez extraído el carácter completo como `string`, se busca en el mapa `accents`. Si existe, se reemplaza por su equivalente ASCII. Si no existe (cirílico, chino, árabe, etc.), la palabra completa se descarta, ya que no puede ser indexada de forma útil en un índice basado en caracteres latinos.

Esta decisión mantiene el proyecto sin dependencias externas y es suficiente dado que el dataset es predominantemente en inglés.

### El mapa `accents` — Por qué `string → string` y no `char → char`

```cpp
unordered_map<string, string> accents = { {"á","a"}, {"ñ","n"}, {"æ","ae"}, ... };
```

Los caracteres acentuados en UTF-8 ocupan 2, 3 o 4 bytes, por lo que no caben en un `char`. La clave del mapa es el carácter multibyte extraído como `string` (por ejemplo, `"á"` son 2 bytes: `0xC3 0xA1`). El valor también es `string` para poder manejar los casos donde un carácter se expande a dos letras ASCII, como `æ → "ae"` y `œ → "oe"`. El mapa cubre más de 80 caracteres entre vocales acentuadas, consonantes especiales europeas y vocales compuestas.

---

## Pseudo-código de inserción al Trie

Cada película genera una cadena unificada de palabras limpias. Esa cadena se tokeniza y cada token se inserta tanto como palabra completa como mediante subcadenas (suffix / n-grams).

```txt
FUNCIÓN insertarTodasLasPeliculas(peliculas: Map<id, Movie>, trie: Trie):

    PARA CADA (id, movie) EN peliculas:

        textoLimpio ← limpiarTitulo(movie.title)      [peso: alto]
                    + limpiarOrigen(movie.origin)     [peso: medio]
                    + limpiarDirector(movie.director) [peso: alto]
                    + limpiarCast(movie.cast)         [peso: medio]
                    + normalizarYLimpiar(movie.genre) [peso: medio]
                    + filtrarStopwords(movie.plot)    [peso: bajo]

        trie.insertarCompleto(textoLimpio, id, pesoCampo)

FIN FUNCIÓN
```

---

## Inserción completa de texto

```txt
FUNCIÓN insertarCompleto(texto: string, id: int, pesoCampo: int):

    totalDocs += 1

    tokens ← tokenizar(texto)

    PARA CADA palabra EN tokens:

        insertarpalabra(palabra, id, pesoCampo)

        SI palabra NO ha sido vista antes en este documento:

            seenInDoc[palabra].agregar(id)

            docFreq[palabra] += 1     // usado para IDF

FIN FUNCIÓN
```

---

## Inserción de palabras y subcadenas

La implementación inserta:

1. La palabra completa con boost extra.
2. Subcadenas/sufijos controlados para soportar búsqueda parcial.

```txt
FUNCIÓN insertarpalabra(palabra: string, id: int, pesoCampo: int):

    n ← largo(palabra)

    MAX_LEN ← 6


    ---------------------------------------------------
    // 1. INSERTAR PALABRA COMPLETA
    ---------------------------------------------------

    nodo ← raíz

    PARA CADA caracter c EN palabra:

        SI c NO existe en nodo.hijos:

            nodo.hijos[c] ← nuevo Nodo()

        nodo ← nodo.hijos[c]

        // boost fuerte para coincidencia exacta
        nodo.freq[id] += pesoCampo * 2

    nodo.esFinDePalabra ← true


    ---------------------------------------------------
    // 2. INSERTAR SUBCADENAS / SUFIJOS
    ---------------------------------------------------

    PARA i DESDE 0 HASTA n-1:

        nodo ← raíz

        PARA j DESDE i HASTA min(n-1, i + MAX_LEN - 1):

            c ← palabra[j]

            SI c NO existe en nodo.hijos:

                nodo.hijos[c] ← nuevo Nodo()

            nodo ← nodo.hijos[c]

            nodo.freq[id] += pesoCampo

        nodo.esFinDePalabra ← true

FIN FUNCIÓN
```

---

## Estructura de datos: Trie

Se eligió un **Trie de caracteres con TF-IDF y soporte de subcadenas** como motor de búsqueda.

### ¿Por qué un Trie?

- Búsquedas por prefijo en **O(L)** donde `L` es el tamaño de la query.
- Permite búsqueda parcial naturalmente.
- Las subcadenas permiten encontrar palabras aunque el usuario escriba desde el medio.

Ejemplo:

```txt
"tion" → encuentra:
    "action"
    "fiction"
    "nation"
```

---

## Estructura de un nodo

```cpp
struct Nodo {

    unordered_map<char, Nodo*> hijos;

    unordered_map<int, int> freq;

    bool esFinDePalabra = false;
};
```

Cada nodo acumula:

```cpp
freq[id] += peso
```

De esta manera, los nodos intermedios ya contienen información útil para scoring sin recorrer todo el subárbol.

---

## Inserción de subcadenas

Para la palabra:

```txt
"action"
```

El Trie inserta caminos como:

```txt
raíz → a→c→t→i→o→n
raíz → c→t→i→o→n
raíz → t→i→o→n
raíz → i→o→n
raíz → o→n
raíz → n
```

Todos los nodos acumulan:

```txt
freq[id] += peso
```

Esto permite búsquedas desde cualquier parte de la palabra.

---

## Algoritmo de búsqueda con TF-IDF

```txt
FUNCIÓN buscar(query: string) → Lista<id>

    score ← Map<id, double>

    matchCount ← Map<id, int>

    totalTokens ← 0


    PARA CADA token EN tokenizar(query):

        token ← convertirMinusculas(token)

        SI largo(token) ≤ 2:

            continuar


        totalTokens += 1

        resultados ← buscarNodo(token)


        df ← tamaño(resultados)

        idf ← log(
                1 + (totalDocs / (1 + df))
              )


        PARA CADA (id, freq) EN resultados:

            tf ← freq

            add ← tf * idf

            score[id] += add

            matchCount[id] += 1


    ---------------------------------------------------
    // Penalización por coincidencias incompletas
    ---------------------------------------------------

    PARA CADA (id, sc) EN score:

        SI matchCount[id] < totalTokens:

            score[id] *= 0.5


    ---------------------------------------------------
    // Ordenar resultados
    ---------------------------------------------------

    ordenar(score DESCENDENTE)

    RETORNAR primeros 5 ids

FIN FUNCIÓN
```

---

## Búsqueda dentro del Trie

```txt
FUNCIÓN buscarNodo(clave: string) → Map<id, freq>

    nodo ← raíz


    PARA CADA caracter c EN clave:

        SI c NO existe en nodo.hijos:

            RETORNAR {}

        nodo ← nodo.hijos[c]


    resultado ← nuevo Map<id, freq>


    SI nodo.esFinDePalabra:

        // coincidencia exacta → boost ×1.5

        PARA CADA (id, freq) EN nodo.freq:

            resultado[id] += freq * 1.5


    SINO:

        // coincidencia parcial

        PARA CADA (id, freq) EN nodo.freq:

            resultado[id] += freq


    RETORNAR resultado

FIN FUNCIÓN
```

---

## Liberación de memoria

El Trie libera memoria recursivamente recorriendo todos los nodos hijos.

```txt
FUNCIÓN limpiarNodo(nodo)

    SI nodo ES null:

        RETORNAR


    PARA CADA hijo EN nodo.hijos:

        limpiarNodo(hijo)


    eliminar nodo

FIN FUNCIÓN
```

---

## ¿Qué funciona bien en esta implementación?

- Soporta búsqueda exacta y parcial.
- TF-IDF mejora la relevancia.
- El boost ×1.5 favorece coincidencias exactas.
- La penalización ×0.5 reduce falsos positivos.
- `seenInDoc` mantiene el cálculo correcto de IDF.
- `MAX_LEN` evita explosión excesiva de memoria.

---

## Limitaciones conocidas

- Insertar subcadenas aumenta consumo de memoria.
- Complejidad cercana a **O(L²)** por palabra.
- El sistema relaciona palabras mediante prefijos y subcadenas, pero no mediante análisis lingüístico avanzado (stemming o lemmatization).
- El Trie puede crecer bastante con datasets grandes.
- Insertar todos los sufijos de una palabra de longitud L genera O(L²) nodos, lo que hace que el consumo de memoria crezca rápido con un dataset grande de plots largos.

---

## Ejemplos de limpieza

### Ejemplo 1 — Título con paréntesis y caracteres especiales

| Campo    | Valor original                        | Valor limpio         |
|----------|---------------------------------------|----------------------|
| Title    | `Spider-Man (film)`                   | `spider man`         |
| Title    | `Amélie`                              | `amelie`             |
| Title    | `功夫 (Kung Fu Hustle)`               | `kung fu hustle`     |

### Ejemplo 2 — Director con anotaciones de Wikipedia

| Campo    | Valor original                        | Valor limpio         |
|----------|---------------------------------------|----------------------|
| Director | `James Cameron (director)`            | `james cameron`      |
| Director | `Sergio León [1]`                     | `sergio leon`        |

### Ejemplo 3 — Cast con roles mezclados

| Campo | Valor original                                      | Valor limpio                   |
|-------|-----------------------------------------------------|--------------------------------|
| Cast  | `Tom Hanks, Robin Wright (screenplay by Eric Roth)` | `tom hanks robin wright`       |

### Ejemplo 4 — Plot con stopwords

| Antes de filtrar                                          | Después de filtrar stopwords    |
|-----------------------------------------------------------|---------------------------------|
| `the man was walking and he found a treasure in the cave` | `man walking treasure cave` |

### Ejemplo 5 — Resultado final unificado para el Trie

Para la película *Casablanca* (hipotético):

```
Texto final indexado:
"1942 casablanca american michael curtiz humphrey bogart ingrid bergman drama
world war ii rick blaine cynical american expatriate operates nightclub wartime
casablanca morocco unexpected arrival former lover ilsa lund..."
```

---

## Avance de la interfaz

La interfaz corre en la terminal de Windows usando `conio.h` y la API de consola Win32. Tiene tres pantallas principales:

### Pantalla de bienvenida — Logo ASCII

```
██╗   ██╗████████╗███████╗ ██████╗███████╗██╗     ██╗██╗  ██╗
██║   ██║╚══██╔══╝██╔════╝██╔════╝██╔════╝██║     ██║╚██╗██╔╝
██║   ██║   ██║   █████╗  ██║     █████╗  ██║     ██║ ╚███╔╝
██║   ██║   ██║   ██╔══╝  ██║     ██╔══╝  ██║     ██║ ██╔██╗
╚██████╔╝   ██║   ███████╗╚██████╗██║     ███████╗██║██╔╝ ██╗
 ╚═════╝    ╚═╝   ╚══════╝ ╚═════╝╚═╝     ╚══════╝╚═╝╚═╝  ╚═╝

         🎬 Películas y series
            sin límites y mucho más 🍿
```

### Menú de ingreso — navegable con flechas

```
╭────────────────────────────────────╮
│        * INGRESO AL SISTEMA *      │
├────────────────────────────────────┤
│   -> Iniciar sesión                │   ← opción seleccionada (resaltada en cyan)
│      Registrarse                   │
├────────────────────────────────────┤
│   Usa ↑ ↓ y ENTER                  │
╰────────────────────────────────────╯
```

La selección se mueve con las teclas `↑` `↓` y se confirma con `ENTER`.

### Pantalla principal — Panel de películas

Una vez autenticado, el usuario ve:
- Su nombre de bienvenida.
- Un bloque de 5 películas recomendadas de forma aleatoria (géneros variados, sin repetir).
- Un campo de búsqueda de texto libre conectado al Trie.

```
Bienvenido, [usuario]

RECOMENDACIONES:
[482]  Inception             Sci-Fi   | 2010
[1203] The Godfather         Crime    | 1972
[3891] Spirited Away         Animation| 2001
[621]  Mad Max: Fury Road    Action   | 2015
[2047] Amélie                Romance  | 2001

Buscar: _
```

---

## Cómo ejecutar

1. Colocar el dataset original renombrado como `peliculas.csv` dentro de `cmake-build-debug/`.
2. Compilar con CMake (CLion o línea de comandos).
3. **Opción A — Desde CLion:** ir a `Run → Edit Configurations` y activar *"Emulate terminal in the output console"*, luego ejecutar normalmente.
4. **Opción B — Desde el explorador de archivos:** navegar a `cmake-build-debug/` y ejecutar `PROYECTAZO.exe` directamente.

> Al iniciar aparecerá brevemente un mensaje de sistema en la consola mientras se configura UTF-8; es esperado y no indica un error.

## Complejidad

### Trie (estructura principal)
- Búsqueda e inserción: O(L)
- "A trie supports search and insertion in time proportional to the key length." (Sedgewick & Wayne)

### Hash maps
- O(1) promedio para acceso
- "Hash-table operations take O(1) time on average…" (CLRS)

### Árboles balanceados (referencia teórica)
- O(log n) altura
- "The height of a balanced binary search tree is O(log n)." (CLRS)

### TF-IDF scoring
- O(k) por consulta
- "TF-IDF assigns higher weights to rare terms…" (Manning et al.)

### Inserción con sufijos
- O(L²) por palabra debido a expansión de subcadenas

### Complejidad total de búsqueda
- O(L + k)
  donde:
  L = longitud de query
  k = documentos relevantes encontrados
