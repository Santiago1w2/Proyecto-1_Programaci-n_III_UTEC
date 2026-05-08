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
Como se especifico anteriormente el plot presenta una particularidad en cuanto a la manera en como está organizado el texto. En este sentido, el contenido puede estar distribuido en uno o más párrafos separados por saltos de linea. Asimismo, el contenido de un mismo párrafo puede estar dividido por diferentes saltos de linea.
Al realizar una lectura simple, es decir usando un solo `getline()` no se estaría tomando en cuenta estas limitaciones, por lo cual la lectura no sería apropiada. Consecuentemente, este error genería una mala lectura de los siguientes atributos para las peliculas posteriores.

Ante esta situación se propone un flujo de funciones que generalicen la lectura de los diferentes campos del csv, considerando las salvedades mencionadas anteriormente.
La función principal de este flujo es `leerPeliculas(const string& csv)`. Esta función va a recibir, por referencia, el csv que se quiere leer y va a crear un unordered_map donde se almacenarán las peliculas leidas.
La justificación en la elección de este tipo de contenedor se basa en que necesitamos almacenar las peliculas, pero mantenerlas mapeadas a traves de un id. Asimismo, es importante resaltar la baja complejidad algorítmica que tiene para la inserción y búsqueda, a comparación de un mapa simple.

Esta función, a su vez, se apoya de la función `leerFilaCSV(archivo,linea)`, la cual se encarga de almacenar en una sola línea, la información correspondiente a una misma película. Para ello, la función trabaja baja una simple lógica en el conteo de comillas en la linea.
En este sentido, para que toda la información de la película sea leida debe existir una cantidad par de comillas en la linea. En el caso no se cumpla esto, se infiere que la información no se ha cerrado, por lo que la función lee e integra la línea siguiente, y vuelve a verificar si la cantidad de comillas es par.

Cuando esta función finaliza, obtenemos un string (una sola línea) con toda la información de la película, lo cual facilita la posterior interacción con la data.
Continuando con el flujo se utiliza la funcion `parseCSVLine(linea)`, la cual se encarga de 'clasificar' la información de la pelicula en la cantidad de atributos requeridos. Como aspecto fundamental en esta función, se valida continuamente el uso de comillas en el texto.
Para ello, es importante recordar que un archivo '.csv' utiliza comillas cuando se usan 'comas' como parte del texto y no como separador de columnas. Asimismo, el formato implica que se utilicen comillas dobles cuando una palabra o frase requiera comillas.
Tomando esto en cuenta, la función verifica este tipo de singularidades y permite que el campo correspondiente se asimile a un texto 'normal', es decir que solo aparecen las comillas propias del texto.

Ahondando más en el apartado técnico de esta última función, podemos ver que crea y retorna un vector de strings donde se almacena el contenido para cada atributo de la pelicula.
Asimismo, cuenta con una variable `encomillas` de tipo 'bool', la cual se utiliza como un 'recordatorio' para cuando el texto ingresa o sale de un apartado con comillas. Luego se recorre la linea de texto, caracter por caracter, y se valida si el valor corresponde a una comilla o una coma (fin del atributo).
En caso no se cumpla ninguna condición el caracter se agrega a un nuevo string donde se almacenerá el texto 'limpio'. Para el caso de la validación de comillas, se verifica si el siguiente caracter tambien es una comilla, es decir es una palabra que utiliza comillas dobles. En el caso se cumpla esta condición, el texto 'limpio' solo registra una comilla y el flujo sigue.
Para la caso contrario, se asume que se esta ingresando o saliendo de una parte de texto que usa comillas, por lo cual no se registra ese caracter y el valor de la variable `encomillas` cambia.
Por otra parte, para el caso que el caracter sea una coma y el valor de `encomillas` sea false, se asume que el contenido de una columna finalizó. Por tanto, el texto limpio almacenado hasta el momento se agrega al vector de atributos y se restea para ser un string vacio.

El proceso mencionado se realiza hasta leer la linea completa y se retorna el vector con los atributos obtenidos. Regresando a la función principal, se utilizan los índices de los atributos (como se muestra en la tabla anterior) para crear y registrar variables que correspondan a cada atributo.
Finalmente, se inserta esta información en el unordered_map inicial, se aumenta el numero de Id y el proceso se repite para la siguiente linea (pelicula).
Cuando todas las lineas han sido procesadas, como buena practica de programación se cierra el archivo y la funcion retorna el mapa.



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
# Complejidad Algorítmica de `insertarPalabra`

La función `insertarPalabra` implementa un índice textual basado en un **Trie (Prefix Tree)**.  
El algoritmo realiza dos operaciones principales:

1. Inserción de la palabra completa.
2. Inserción de subcadenas/sufijos limitados por una longitud máxima (`MAX_LEN`).

---

## 1. Inserción de la palabra completa

El algoritmo recorre cada carácter de la palabra exactamente una vez:

```cpp
PARA CADA caracter c EN palabra
```

Si `n` representa la longitud de la palabra, entonces el recorrido ejecuta `n` iteraciones.

Las operaciones internas:

```cpp
nodo.hijos[c]
nodo.freq[id] += pesoCampo
```

corresponden a accesos e inserciones sobre una tabla hash (`unordered_map`), cuyo costo promedio es `O(1)`.

Según *Introduction to Algorithms* de Cormen et al.:

> “Under the assumption of simple uniform hashing, hash-table operations take expected O(1) time.”

Por tanto, la complejidad de esta sección es:

```txt
O(n)
```

---

## 2. Inserción de subcadenas limitadas

Posteriormente, el algoritmo genera subcadenas desde cada posición de la palabra:

```cpp
PARA i DESDE 0 HASTA n-1
```

Sin embargo, el ciclo interno se encuentra limitado por:

```cpp
MAX_LEN = 6
```

```cpp
PARA j DESDE i HASTA min(n-1, i + MAX_LEN - 1)
```

Esto implica que el número máximo de iteraciones internas es constante:

```txt
6 → O(1)
```

Por ello, aunque el ciclo externo recorra `n` posiciones, el costo total permanece lineal:

```txt
O(n) * O(1) = O(n)
```

Según *The Algorithm Design Manual* de Steven Skiena:

> “Bounding search depth or substring length is a common optimization technique to reduce quadratic behavior in text processing.”

---

# Complejidad Temporal Total

La complejidad total del algoritmo es:

```txt
O(n) + O(n) = O(n)
```

Por tanto:

```txt
O(n)
```

---

# Optimización Importante

Si `MAX_LEN` no estuviera acotado, el ciclo interno recorrería aproximadamente `n-i` posiciones por iteración externa, generando la siguiente sumatoria:

```txt
Σ(i=0 → n-1) (n-i)
```

lo cual equivale a:

```txt
n(n+1)/2
```

y produce una complejidad:

```txt
O(n²)
```

Gracias al límite constante `MAX_LEN`, el algoritmo evita crecimiento cuadrático y mantiene complejidad lineal.

---

# Complejidad Espacial

Cada carácter puede generar nuevos nodos dentro del Trie.

Según *Open Data Structures* de Pat Morin:

> “The space used by a trie is proportional to the total number of stored character nodes.”

Dado que:

- la palabra completa inserta `n` caracteres,
- y las subcadenas poseen longitud máxima constante,

el crecimiento espacial también es lineal:

```txt
O(n)
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
# Complejidad Algorítmica de `buscar`

La función `buscar` implementa un sistema de recuperación de información basado en puntuación TF-IDF sobre un índice invertido almacenado en un Trie.  
El algoritmo realiza cuatro procesos principales:

1. Tokenización y normalización de la consulta.
2. Búsqueda de coincidencias por token.
3. Cálculo de puntuaciones TF-IDF.
4. Ordenamiento de resultados por relevancia.

---
## 1. Tokenización de la consulta

El algoritmo recorre cada token generado desde la consulta:

```cpp
PARA CADA token EN tokenizar(query)
```

Si:

```txt
t = número de tokens de la consulta
```

entonces el ciclo externo ejecuta `t` iteraciones.

Las operaciones:

```cpp
convertirMinusculas(token)
largo(token)
```

dependen únicamente del tamaño del token, el cual suele ser pequeño respecto al total de documentos indexados.

Por ello, el costo promedio de esta etapa es:

```txt
O(t)
```

---

## 2. Búsqueda en el Trie

Cada token válido ejecuta:

```cpp
resultados ← buscarNodo(token)
```

La búsqueda en un Trie depende linealmente de la longitud de la cadena buscada.

Según *Open Data Structures* de Pat Morin:

> “Trie operations run in O(k) time, where k is the length of the string.”

Si:

```txt
k = longitud del token
```

entonces:

```txt
buscarNodo(token) = O(k)
```

Como `k` suele ser pequeño y acotado para palabras normales, el costo promedio por token permanece cercano a constante.

---

## 3. Cálculo de TF-IDF

Posteriormente, el algoritmo recorre todos los documentos asociados al token:

```cpp
PARA CADA (id, freq) EN resultados
```

Las operaciones:

```cpp
tf ← freq
add ← tf * idf
score[id] += add
matchCount[id] += 1
```

corresponden a operaciones sobre tablas hash (`unordered_map`), cuyo costo promedio es `O(1)`.

Según *Introduction to Algorithms* de Cormen et al.:

> “Under the assumption of simple uniform hashing, hash-table operations take expected O(1) time.”

Si:

```txt
r = cantidad de documentos que contienen el token
```

entonces esta etapa posee complejidad:

```txt
O(r)
```

Por tanto, considerando todos los tokens:

```txt
O(t * r)
```

---

## 4. Penalización de coincidencias incompletas

El algoritmo recorre todos los documentos puntuados:

```cpp
PARA CADA (id, sc) EN score
```

Si:

```txt
m = cantidad de documentos con score
```

entonces esta sección cuesta:

```txt
O(m)
```

---

## 5. Ordenamiento de resultados

Finalmente, los resultados se ordenan por relevancia:

```cpp
ordenar(score DESCENDENTE)
```

Ordenar `m` elementos requiere:

```txt
O(m log m)
```

Según *Introduction to Algorithms*:

> “Comparison sorting algorithms have a lower bound of Ω(n log n) in the worst case.”

---

# Complejidad Temporal Total

La complejidad total del algoritmo es:

```txt
O(t * r + m log m)
```

donde:

- `t` = número de tokens en la consulta.
- `r` = cantidad promedio de documentos asociados por token.
- `m` = cantidad total de documentos puntuados.

En escenarios reales, como únicamente se retornan los 5 mejores resultados y las consultas suelen contener pocos tokens, el rendimiento práctico es cercano a lineal respecto al número de coincidencias relevantes.

---

# Complejidad Espacial

El algoritmo utiliza estructuras auxiliares:

```cpp
score ← Map<id, double>
matchCount ← Map<id, int>
```

Ambas almacenan información únicamente para documentos coincidentes.

Si:

```txt
m = cantidad de documentos relevantes
```

entonces el espacio utilizado es:

```txt
O(m)
```

---

# Optimización Implementada

El algoritmo incorpora una penalización para documentos que no contienen todos los tokens de la consulta:

```cpp
SI matchCount[id] < totalTokens:
    score[id] *= 0.5
```

Esto mejora la relevancia semántica de los resultados sin incrementar significativamente la complejidad temporal, ya que la operación se realiza durante un recorrido lineal sobre los documentos puntuados.

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
# Complejidad Algorítmica de `buscarNodo`

La función `buscarNodo` realiza búsquedas dentro de un **Trie (Prefix Tree)** para recuperar documentos asociados a una palabra o prefijo determinado.

El algoritmo realiza tres procesos principales:

1. Recorrido del Trie carácter por carácter.
2. Verificación de coincidencia exacta o parcial.
3. Construcción del mapa de resultados.

---

## 1. Recorrido del Trie

El algoritmo recorre cada carácter de la clave buscada:

```cpp
PARA CADA caracter c EN clave
```

Si:

```txt
k = longitud de la clave
```

entonces el ciclo ejecuta `k` iteraciones.

En cada iteración se realiza:

```cpp
SI c NO existe en nodo.hijos
nodo ← nodo.hijos[c]
```

Estas operaciones corresponden a accesos sobre una tabla hash (`unordered_map`), cuyo costo promedio es `O(1)`.

Según *Introduction to Algorithms* de Cormen et al.:

> “Under the assumption of simple uniform hashing, hash-table operations take expected O(1) time.”

Por ello, el recorrido completo del Trie tiene complejidad:

```txt
O(k)
```

---

## 2. Caso de coincidencia exacta o parcial

Una vez alcanzado el nodo correspondiente, el algoritmo recorre las frecuencias almacenadas:

```cpp
PARA CADA (id, freq) EN nodo.freq
```

Si:

```txt
r = cantidad de documentos asociados al nodo
```

entonces el recorrido cuesta:

```txt
O(r)
```

Las operaciones internas:

```cpp
resultado[id] += freq
resultado[id] += freq * 1.5
```

son operaciones de inserción y actualización sobre tablas hash, cuyo costo promedio es `O(1)`.

---

## 3. Construcción del resultado

El mapa:

```cpp
resultado ← nuevo Map<id, freq>
```

almacena únicamente los documentos encontrados para la clave buscada.

Por tanto, el tamaño del resultado depende directamente de `r`.

---

# Complejidad Temporal Total

La complejidad total del algoritmo es:

```txt
O(k + r)
```

donde:

- `k` = longitud de la palabra buscada.
- `r` = cantidad de documentos asociados al nodo encontrado.

---

# Complejidad Espacial

El algoritmo crea una estructura auxiliar:

```cpp
resultado ← nuevo Map<id, freq>
```

que almacena todos los documentos coincidentes.

Si:

```txt
r = cantidad de resultados encontrados
```

entonces el espacio utilizado es:

```txt
O(r)
```

---

# Optimización Implementada

El algoritmo diferencia entre:

- coincidencias exactas,
- coincidencias parciales.

Cuando existe coincidencia exacta:

```cpp
resultado[id] += freq * 1.5
```

se aplica un incremento de relevancia (*boost*) para priorizar resultados exactos sobre coincidencias parciales.

Esta optimización mejora la calidad del ranking sin alterar la complejidad asintótica del algoritmo, ya que únicamente agrega una operación aritmética constante durante el recorrido lineal de resultados.

Los Tries permiten búsquedas proporcionales a la longitud de la cadena consultada.

Según *Open Data Structures* de Pat Morin:

> “Trie operations run in O(k) time, where k is the length of the string.”

Asimismo, las operaciones promedio sobre tablas hash poseen costo constante esperado.

Según *Introduction to Algorithms*:

> “Hash-table operations take expected O(1) time.”

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
