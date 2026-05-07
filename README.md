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
    ├─ Cast         → limpiarCast()      + normalizarYLimpiar()
    ├─ Genre        → normalizarYLimpiar()
    └─ Plot         → normalizarYLimpiar() → filtrarStopwords()
            │
            ▼
    String unificada: "año titulo origen director cast genero plot"
            │
            ▼
    Inserción al Trie  +  Exportación a datosLimpios.csv
```

### Funciones de limpieza detalladas

#### `limpiarTextoAvanzado(s, parentesisProhibidos, palabrasProhibidas)` — Función maestra

Es el núcleo del sistema de limpieza. Trabaja en dos fases:

**Fase 1 — Eliminación de corchetes y paréntesis:**
- `[texto]`: si el contenido son solo números o referencias de Wikipedia (`[1]`, `[2]`...), se elimina por completo. Si el contenido es texto real (p. ej. `[also known as]`), se conserva el texto sin los corchetes.
- `(texto)`: si el contenido en minúsculas contiene alguna palabra de `parentesisProhibidos` (como `"film"` o `"part"`), se descarta todo el bloque. En caso contrario, se conserva el texto interior.

**Fase 2 — Procesamiento carácter a carácter:**
- Caracteres ASCII alfanuméricos: se normalizan a minúsculas.
- Caracteres UTF-8 multibyte: se buscan en el mapa `accents` (más de 80 caracteres acentuados mapeados a su equivalente ASCII). Si el carácter no existe en el mapa (cirílico, chino, árabe, etc.), la **palabra completa** se descarta.
- Separadores y puntuación: disparan el guardado de la palabra acumulada.
- Palabras en `palabrasProhibidas`: se filtran exactamente (p. ej. `"director"` en campos de cast).

#### `limpiarTitulo(s)`
Llama a `limpiarTextoAvanzado` prohibiendo `"film"` y `"part"` dentro de paréntesis.  
Caso real: `"Spider-Man (film)"` → `"spider man"`.

#### `limpiarDirector(s)`
Llama a `limpiarTextoAvanzado` sin bloqueos en paréntesis, pero filtrando la palabra exacta `"director"` para no indexarla como término de búsqueda.  
Caso real: `"James Cameron (director)"` → `"james cameron"`.

#### `limpiarOrigen(s)`
Lógica propia más rápida: solo acepta letras (convierte a minúsculas) y descarta todo lo demás.  
Caso real: `"American/British"` → `"american british"`.

#### `limpiarCast(s)`
Llama a `limpiarTextoAvanzado` filtrando las palabras `"director"` y `"screenplay"` que a veces aparecen en la columna de reparto de Wikipedia.

#### `normalizarYLimpiar(s)`
Limpieza genérica para los campos Genre y Plot. Aplica el mismo mapa de acentos, maneja corchetes de Wikipedia (`[citation needed]`, `[dead link]`, etc.) y normaliza el resto a minúsculas con espacios como separadores.

#### `filtrarStopwords(textoLimpio)`
Se aplica exclusivamente al Plot después de `normalizarYLimpiar`. Elimina las ~80 palabras de ruido del inglés (`"the"`, `"and"`, `"was"`, etc.) para reducir el tamaño del índice y mejorar la precisión de las búsquedas.

---

## Pseudo-código de inserción al Trie

Cada película genera una cadena unificada de palabras limpias. Esa cadena se tokeniza y cada token se descompone en **trigramas** antes de insertarse.

```
FUNCIÓN insertarTodasLasPeliculas(peliculas: Map<id, Movie>, trie: Trie):
    PARA CADA (id, movie) EN peliculas:

        textoLimpio ← limpiarTitulo(movie.title)     [peso: alto]
                    + limpiarOrigen(movie.origin)     [peso: medio]
                    + limpiarDirector(movie.director) [peso: alto]
                    + limpiarCast(movie.cast)         [peso: medio]
                    + normalizarYLimpiar(movie.genre) [peso: medio]
                    + filtrarStopwords(normalizarYLimpiar(movie.plot)) [peso: bajo]

        trie.insertarCompleto(textoLimpio, id, pesoCampo)

FIN FUNCIÓN


FUNCIÓN insertarCompleto(texto: string, id: int, pesoCampo: int):
    tokens ← tokenizar(texto)   // split por espacios

    PARA CADA palabra EN tokens:
        insertarpalabraYTrigramas(palabra, id, pesoCampo)

        SI palabra NO ha sido vista en este documento antes:
            docFreq[palabra] += 1     // para calcular IDF luego
            seenInDoc[palabra].agregar(id)

FIN FUNCIÓN


FUNCIÓN insertarpalabraYTrigramas(palabra: string, id: int, peso: int):
    n ← largo(palabra)

    // Inserta todos los sufijos de longitud ≥ 3 como caminos en el Trie
    // Ejemplo: "action" → inserta "action", "ction", "tion", "ion"
    PARA i DESDE 0 HASTA n-3:
        nodoActual ← raíz

        PARA j DESDE i HASTA n-1:
            c ← palabra[j]
            SI c NO existe en nodoActual.hijos:
                nodoActual.hijos[c] ← nuevo Nodo()
            nodoActual ← nodoActual.hijos[c]
            nodoActual.freq[id] += peso    // acumular peso en cada nodo del camino

        nodoActual.esFinDePalabra ← true   // marcar fin de sufijo

FIN FUNCIÓN
```

---

## Estructura de datos: Trie

Se eligió un **Trie de caracteres con índice TF-IDF y trigramas** como motor de búsqueda.

### Por qué un Trie

- Búsquedas por prefijo en **O(L)** donde L = largo de la query, sin importar cuántas películas hay indexadas.
- Soporte natural de búsqueda parcial: `"sci"` encuentra `"science"`, `"scientist"`, etc.
- Los sufijos (trigramas) permiten encontrar palabras aunque el usuario escriba desde el medio: `"tion"` encuentra `"action"`, `"nation"`, etc.

### Estructura de un nodo

```cpp
struct Nodo {
    unordered_map<char, Nodo*> hijos;   // hijos por carácter
    unordered_map<int, int>    freq;    // docID → frecuencia acumulada
    bool esFinDePalabra = false;
};
```

Cada nodo del camino acumula `freq[id] += peso`, de modo que nodos intermedios ya cargan información útil de scoring sin necesidad de recorrer el subárbol completo.

### Algoritmo de inserción — trigramas posicionales

En lugar de insertar solo la palabra completa, se insertan **todos sus sufijos de longitud ≥ 3**. Para la palabra `"action"` de longitud 6 se crean 4 caminos desde la raíz:

```
raíz → a→c→t→i→o→n   (sufijo completo, esFinDePalabra = true)
raíz → c→t→i→o→n
raíz → t→i→o→n
raíz → i→o→n
```

Todos los nodos de todos esos caminos reciben `freq[id] += peso`.

**¿Por qué esto?** Permite buscar subcadenas de palabras. Si el usuario escribe `"tion"`, el Trie lo encuentra directamente sin necesidad de recorrer toda la palabra original.

Complejidad de inserción: **O(L²)** por palabra (L sufijos × L nodos cada uno), que es el costo de los trigramas.

### Algoritmo de búsqueda con TF-IDF

```
FUNCIÓN buscar(query: string) → Lista<id>:
    score     ← Map<id, double>    // puntuación acumulada por película
    matchCount← Map<id, int>       // cuántos tokens de la query aparecen en cada doc
    totalTokens ← 0

    PARA CADA token EN tokenizar(query):
        SI largo(token) ≤ 2: continuar   // tokens muy cortos se ignoran

        totalTokens += 1
        resultados ← buscarNodo(token)   // Map<id, freq> desde el Trie

        df  ← tamaño(resultados)         // en cuántos docs aparece el token
        idf ← log(1 + totalDocs / (1 + df))   // penaliza palabras muy comunes

        PARA CADA (id, freq) EN resultados:
            tf  ← freq                   // frecuencia acumulada en ese doc
            score[id]     += tf * idf
            matchCount[id]+= 1

    // Penalización por incompletitud: docs que no tienen todos los tokens
    PARA CADA (id, sc) EN score:
        SI matchCount[id] < totalTokens:
            score[id] *= 0.5

    // Ordenar por score descendente y retornar los top 5
    ordenar(score, descendente)
    RETORNAR primeros 5 ids

FIN FUNCIÓN


FUNCIÓN buscarNodo(clave: string) → Map<id, freq>:
    nodo ← raíz
    PARA CADA c EN clave:
        SI c NO existe en nodo.hijos: RETORNAR {}
        nodo ← nodo.hijos[c]

    SI nodo.esFinDePalabra:
        // Coincidencia exacta → boost ×1.5
        RETORNAR { id: freq*1.5  PARA CADA (id,freq) EN nodo.freq }
    SINO:
        // Coincidencia de prefijo/sufijo → sin boost
        RETORNAR nodo.freq

FIN FUNCIÓN
```

### ¿Funciona bien el Trie implementado?

La implementación es sólida y tiene decisiones de diseño bien pensadas. Algunos puntos a considerar:

**Lo que funciona bien:**
- Los trigramas posicionales resuelven búsquedas parciales sin necesidad de un algoritmo extra (como KMP o Aho-Corasick).
- El scoring TF-IDF penaliza términos muy comunes (como `"love"` o `"man"` que aparecen en miles de plots) y favorece términos discriminativos.
- El boost ×1.5 para coincidencia exacta premia queries precisas sobre prefijos genéricos.
- La penalización del ×0.5 por incompletitud reduce los falsos positivos cuando solo uno de varios tokens buscados hace match.
- `seenInDoc` garantiza que `docFreq` se incremente solo una vez por documento, manteniendo el IDF correcto.

**Limitaciones conocidas:**
- Insertar todos los sufijos de una palabra de longitud L genera O(L²) nodos, lo que hace que el consumo de memoria crezca rápido con un dataset grande de plots largos. Con el CSV de ~35k películas esto puede ser significativo.
- Tokens de 1 o 2 caracteres se ignoran en la búsqueda (`if (token.size() <= 2) continue`), lo que es correcto para el inglés pero podría omitir siglas o años de 2 dígitos.
- `totalDocs` se inicializa en 0 y no se incrementa automáticamente al insertar; debe actualizarse manualmente desde el código que llama a `insertarCompleto`, o el IDF calculará siempre con denominador incorrecto.

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
| `the man was walking and he found a treasure in the cave` | `man walking found treasure cave` |

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

