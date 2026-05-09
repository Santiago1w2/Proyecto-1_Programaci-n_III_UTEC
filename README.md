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
├── IUsuario.h / IUsuario.cpp   ← Interacción con el usuario
├── PLimpieza.h / PLimpieza.cpp ← Toda la lógica de limpieza
├── LPeliculas.h / LPeliculas.cpp ← Lectura CSV, manejo de usuarios
├── Trie.h / Trie.cpp          ← Estructura de datos principal
├── Interfaz.h / Interfaz.cpp  ← UI de consola (Windows)
└── cmake-build-debug/
    ├── peliculas.csv           ← Dataset original (wiki_movie_plots_deduped.csv)
    └── datosLimpios.csv        ← CSV generado tras la limpieza
```

---

## Lectura inicial del CSV

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
Como muestra en la tabla, el plot presenta una particularidad en cuanto a la manera en como está organizado el texto. 
En este sentido, el contenido puede estar distribuido en uno o más párrafos separados por saltos de linea. Asimismo, el contenido de un mismo párrafo puede estar dividido por diferentes saltos de linea.
Al realizar una lectura simple, es decir usando un solo `getline()` no se estaría tomando en cuenta estas limitaciones, por lo cual la lectura no sería apropiada. 
Consecuentemente, este error genería una mala lectura en las peliculas posteriores.

Ante esta situación se propone un flujo de funciones que generalicen la lectura de los diferentes campos del csv, considerando las salvedades mencionadas anteriormente.
La función principal de este flujo es `leerPeliculas(const string& csv)`. Esta función va a recibir, por referencia, el csv que se quiere leer y va a crear un unordered_map donde se almacenarán las peliculas leidas.
La justificación en la elección de este tipo de contenedor se basa en que necesitamos almacenar las peliculas, pero mantenerlas mapeadas a traves de un id. Asimismo, es importante resaltar la baja complejidad algorítmica que tiene para la inserción y búsqueda, a comparación de un mapa simple.

Esta función, a su vez, se apoya de `leerFilaCSV(archivo,linea)`, la cual se encarga de almacenar en una sola línea, la información correspondiente a una misma película. Para ello, la función utiliza una lógica basada en el conteo de comillas en la linea.
Para que toda la información de la película sea leida debe existir una cantidad par de comillas en la linea. En el caso no se cumpla esto, se infiere que la información no se ha cerrado, por lo que la función lee e integra la línea siguiente, y vuelve a verificar si la cantidad de comillas es par.

Continuando con el flujo, se utiliza la funcion `parseCSVLine(linea)`, la cual se encarga de 'clasificar' el contenido de una linea acorde a la cantidad de atributos de la pelicula. 
Como aspecto fundamental en esta función, se valida continuamente el uso de comas y comillas en el texto.
Para ello, es importante recordar que un archivo '.csv' utiliza comillas cuando se usan 'comas' como parte del texto y no como separador de columnas. Asimismo, el formato implica que se utilicen comillas dobles cuando una palabra o frase requiera comillas.
Tomando esto en cuenta, la función verifica este tipo de singularidades y permite que el campo correspondiente se guarde como un texto 'normal'.

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


---

# Estructura de Datos: Suffix Trie parcial con TF-IDF

Se implementó un **Suffix Trie de caracteres con búsqueda parcial y ranking TF-IDF** como motor principal de recuperación de información.

El sistema permite:

- búsquedas exactas,
- búsquedas por prefijo,
- búsquedas desde el medio de una palabra,
- ranking semántico mediante TF-IDF.

---

# ¿Por qué un Suffix Trie?

El Trie permite búsquedas en tiempo proporcional al tamaño de la consulta:

```txt
O(L)
```

donde:

```txt
L = longitud del token buscado
```

Además:

- soporta coincidencias parciales naturalmente,
- evita recorrer todos los documentos,
- permite indexación eficiente carácter por carácter.

---

# Soporte de Subcadenas

El Suffix Trie no solo inserta palabras completas, sino también subcadenas controladas (*suffix / n-grams*).

Por ejemplo:

```txt
"action"
```

genera:

```txt
action
ction
tion
ion
on
n
```

Esto permite consultas como:

```txt
"tion"
```

encontrar:

```txt
action
fiction
nation
```

---

# Estructura del Nodo

```cpp
struct Nodo {

    unordered_map<char, Nodo*> hijos;

    unordered_map<int, int> freq;

    bool esFinDePalabra = false;
};
```

---

# Explicación de la Estructura

Cada nodo almacena:

| atributo | función |
|---|---|
| `hijos` | conexiones hacia otros caracteres |
| `freq` | TF acumulado por película |
| `esFinDePalabra` | indica coincidencia exacta |

---

# Frecuencias Acumuladas

Cada nodo almacena:

```cpp
freq[id] += peso
```

donde:

- `id` = identificador de la película,
- `peso` = relevancia del campo.

---

# Pesos por Campo

El sistema asigna distintos pesos según el origen del token:

| campo | peso |
|---|---|
| título | alto |
| director | alto |
| cast | medio |
| género | medio |
| plot | bajo |

Esto mejora la relevancia semántica de los resultados.

---

# Estructura General del Trie

```txt
ESTRUCTURA Trie

    raíz:
        Nodo

    docFreq:
        Map<token, DF>

    totalDocs:
        int

FIN ESTRUCTURA
```

---

# Variables Globales del Trie

## `totalDocs`

Cantidad total de películas indexadas.

---

## `docFreq`

Almacena:

```txt
token → cantidad de documentos
```

Ejemplo:

```txt
docFreq["spider"] = 120
```

---

# Inserción en el Trie

El algoritmo inserta:

1. palabra completa,
2. subcadenas limitadas.

---

# Inserción de Palabra Completa

Para:

```txt
spider
```

se genera:

```txt
s → p → i → d → e → r
```

---

# Inserción de Subcadenas

Para:

```txt
spider
```

también se insertan:

```txt
pider
ider
der
er
r
```

---

# Pseudocódigo de Inserción

```txt
FUNCIÓN insertarPalabra(
    palabra,
    movieID,
    pesoCampo
)

    n ← longitud(palabra)

    MAX_LEN ← 6

    // caso 1: INSERTAR PALABRA COMPLETA
    nodo ← raíz

    PARA CADA caracter c EN palabra

        SI c NO existe EN nodo.hijos

            nodo.hijos[c]
                ← nuevo Nodo()

        FIN SI

        nodo ← nodo.hijos[c]

        nodo.freq[movieID]
            += pesoCampo * 2

    FIN PARA

    nodo.esFinDePalabra ← verdadero

    // CASO 2: INSERTAR SUBCADENAS

    PARA i DESDE 0 HASTA n-1

        nodo ← raíz

        PARA j DESDE i
                HASTA min(n-1, i + MAX_LEN - 1)

            c ← palabra[j]

            SI c NO existe EN nodo.hijos

                nodo.hijos[c]
                    ← nuevo Nodo()

            FIN SI

            nodo ← nodo.hijos[c]

            nodo.freq[movieID]
                += pesoCampo

        FIN PARA

        nodo.esFinDePalabra ← verdadero

    FIN PARA

FIN FUNCIÓN
```

---

# Optimización de Memoria

Se utiliza:

```txt
MAX_LEN = 6
```

para evitar crecimiento cuadrático.

Sin este límite, insertar todos los suffix completos produciría:

```txt
O(n²)
```

en tiempo y memoria.

Con la restricción:

```txt
O(n)
```

---

# Complejidad de Inserción

## Complejidad Temporal

La inserción completa cuesta:

```txt
O(n)
```

donde:

```txt
n = longitud de la palabra
```

---

## Complejidad Espacial

Cada carácter puede generar un nodo:

```txt
O(n)
```

---

# Búsqueda en el Suffix Trie

La búsqueda recorre el Trie carácter por carácter.

---

# Pseudocódigo de Búsqueda de Nodo

```txt
FUNCIÓN buscarNodo(clave)

    nodo ← raíz

    PARA CADA caracter c EN clave

        SI c NO existe EN nodo.hijos

            RETORNAR vacío

        FIN SI

        nodo ← nodo.hijos[c]

    FIN PARA

    resultado ← nuevo Map<movieID, TF>

    //CASO 1: SE ENCUENTRA UNA PALABRA COMPLETA

    SI nodo.esFinDePalabra

        PARA CADA (id, freq) EN nodo.freq

            resultado[id]
                += freq * 1.5

        FIN PARA


    //CASO 2: SE ENCUENTRA UN SUFIJO O PREFIJO DE LA PALABRA

    SINO

        PARA CADA (id, freq) EN nodo.freq

            resultado[id]
                += freq

        FIN PARA

    FIN SI


    RETORNAR resultado

FIN FUNCIÓN
```

---

# Boost de Coincidencia Exacta

Cuando existe coincidencia exacta:

```txt
freq × 1.5
```

Esto prioriza:

```txt
spider
```

sobre:

```txt
spiderman
```

---

# Complejidad de `buscarNodo`

## Temporal

```txt
O(k + r)
```

donde:

| símbolo | significado |
|---|---|
| `k` | longitud de la palabra |
| `r` | cantidad de documentos encontrados |

---

## Espacial

```txt
O(r)
```

---

# Ranking TF-IDF

El sistema utiliza:

```txt
TF-IDF
```

para calcular relevancia semántica.

---

# TF (Term Frequency)

Representa:

```txt
cuántas veces aparece el token
```

dentro de una película.

---

# IDF (Inverse Document Frequency)

Representa:

```txt
qué tan raro es el token
```

en toda la colección.

---

# Fórmula de IDF

```txt
IDF = log(1 +(totalDocs / (1 + df) ))
```

donde:

| símbolo | significado |
|---|---|
| `totalDocs` | películas totales |
| `df` | documentos que contienen el token |

---

# Fórmula TF-IDF

```txt
TF-IDF = TF × IDF
```

---

# Ejemplo

| token | TF | IDF | score |
|---|---|---|---|
| spider | 10 | 1 | 10 |
| multiverse | 6 | 5 | 30 |

Aunque:

```txt
spider
```

aparece más veces,:

```txt
multiverse
```

es más relevante por ser más raro.

---

# Pseudocódigo de Búsqueda Completa

```txt
FUNCIÓN buscar(query)

    score ← nuevo Map<movieID, double>

    matchCount ← nuevo Map<movieID, int>

    tokens ← tokenizar(query)

    totalTokens ← 0

    // RECORRER TOKENS

    PARA CADA token EN tokens

        SI longitud(token) ≤ 2

            CONTINUAR

        FIN SI


        totalTokens += 1

        resultados ← buscarNodo(token)

        // CALCULAR IDF

        SI token existe EN docFreq

            df ← docFreq[token]

        SINO

            df ← 1

        FIN SI


        idf ← log(1 +(totalDocs / (1 + df) ))

        // TF-IDF

        PARA CADA (id, freq) EN resultados

            tf ← freq

            add ← tf * idf

            score[id]
                += add

            matchCount[id]
                += 1

        FIN PARA

    FIN PARA


    // PENALIZACIÓN POR NO SER UNA PALBRA COMPLETA
    

    PARA CADA (id, sc) EN score

        SI matchCount[id] < totalTokens

            score[id]
                *= 0.5

        FIN SI

    FIN PARA


    
    // ORDENAR DE MAYOR A MENOR SEGUN SCORE CALCULA CON TF-IDF
    

    ordenar score DESCENDENTE

    // TOP 5 PARA RESULTADO FINAL

    retornar primeros 5 ids

FIN FUNCIÓN
```

---

# Penalización de Coincidencias Incompletas

Si una película no contiene todos los tokens buscados:

```txt
score *= 0.5
```

Esto reduce falsos positivos.

---

# Complejidad de Búsqueda

## Complejidad Temporal

```txt
O(t * r + m log m)
```

donde:

| símbolo | significado |
|---|---|
| `t` | tokens de la consulta |
| `r` | documentos encontrados por token |
| `m` | documentos puntuados |

---

# Complejidad Espacial

```txt
O(m)
```

---

# Liberación de Memoria

El Trie libera memoria recursivamente.

---

```txt
FUNCIÓN limpiarNodo(nodo)

    SI nodo ES null

        RETORNAR

    FIN SI


    PARA CADA hijo EN nodo.hijos

        limpiarNodo(hijo)

    FIN PARA


    eliminar nodo

FIN FUNCIÓN
```

---

# Flujo Completo del Sistema

```txt
CSV
    ↓

normalización
    ↓

tokenización
    ↓

Trie
    ↓

TF acumulado
    ↓

docFreq
    ↓

búsqueda
    ↓

TF-IDF
    ↓

ranking
    ↓

top resultados
```
## Ventajas de este tipo de implementación

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
## Interaccion con el usuario


Mas allá de la interacción con el usuario a traves de la interfaz, es importante mencionar y enteder algunas validaciones que se realizan.

### Registro historico de usuarios

Cuando el usuario ingresa a la interfaz, se le da la opcion de crear una nueva cuenta (registrarse) o acceder a una cuentaya creada
(inicio de sesion)

Esto se hace con la finalida de tener un mejor control en la validacion de usuarios y mantener un registro histórico de todos los usuarios registrados.
El registro histórico consiste en un archivo .txt donde se guarda de manera secuencial

`correo,password,nombre de usuario,lista de peliculas para ver mas tarde,
lista de peliculas que le gustan, lista de peliculas que no desea ver,  lista con todas las peliculas que ha visto`

Esto se hace con la finalidad de mejorar la interaccion con el usuario, a trvaes de una personalizacion de peliculas recomendadas, basado en las peliculas que le gustan,
peliculas de un genero similar a las que ha visto, etc.


### Validaciones en las opciones registrar e iniciar sesion

Dependiendo de la opcion elegida por el usuario al ingresar a la interfaz, se ejecutaran una serie de validaciones espeficicas.
En el caso que eliga *registrarse*, se utilizan funciones como `validar_correo()` para verificar que este usuario nuevo se esta registrando con un correo que no esta en uso.
Esto se hace porque el correo se ha elegido como clave primaria, similar a la validacion que usan las plataformas de streaming en la actualidad.

De manera similar, cuando el usuario elige la opcion de *iniciar sesion* se utilizan funciones como `validar_info()`, la cual verifica que el correo y contraseña que estan siendo ingresados
corresponden a los que se registraron cuando el usuario creo la cuenta.

### Recomendacion de Peliculas Aleatoria
Como se menciono en la interfaz, cuando el usuario ya valido su identidad, se muestra una recomendacion de 5 peliculas aletorias. Para ello, la funcion `peliculasRecomendadasPanel`
utiliza un algoritmo simple de generacion aleatoria de un numero que corresponde al indice de una pelicula. Con este indice se accede a un mapa que contiene las peliculas, ya leidas,
para extraer la informacion que se mostrar en pantalla (titulo, genero,año de lanzamiento).

Asimismo, se realiza un registro y validacion interna de las pelicula que estan saliendo, con la finalidad de que no se repitan peliculas del mismo genero. Esto se hace para
que las recomendaciones sean lo mas aletorias posibles y el usuario pueda conocer un catálogo diverso de peliculas.

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
