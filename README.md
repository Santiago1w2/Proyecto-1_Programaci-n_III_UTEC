COSAS QUE FALTA


Poner la opcion de solo seleccionar la opcion de usuairo y poner la contraseña

Encontrar una amenra de busqueda mas rapida

Si se encuentra una mejor forma de hacer le arbol y buscar chevere
(creo que se podria hacer juntando palbara comunes por ejemplo, "run" y mas en vez de hacer un arbol de chars hacer un arrbol de string  chars, osea en ves de poner c-a- se divede en r-r-o y s-a, como la palabra es comun solo poner car se divie en r-r-o y s-a, ahi entraria templates y soncept solo pa poner string y char, ahi lo checan


para que funcione tiene que poner los archivos con estos nombres en la carpeta de cmake-build-debug:
wiki_movie_plots_deduped.csv == peliculas.csv


## NUEVA MODIFICACION:
- se agrego una sugerencia inicial de peliculas de manera aleatoria que solo aplica cuando el usuario no ha tenido ninguna busqueda

El formato de la tabla de impresion se puede generalizar y que solo se modifique los ID's de las peliculas que se quieren mostrar. Lo ideal seria que para usuarios que ya han tenido busquedas se pueda mostrar peliculas con un genero similar, peliculas del mismo director o continuaciones de sus peliculas.
Tambien se podria agregar un espacio de 'volver a ver', para lo cual en txt de registro se agregó un espacio para el historial de peliculas. Lo ideal seria que solo se guarden ID's para no saturar el registro

punto adicional:
- para el cuadro de peliculas,hay que considerar una limitacion en el titlo de las peliculas, ya que se esta trabajando con setw() para reservar un espacio pero hay peliculas que exceden este espacio. Tambien se podrian considerar otros atributos como el plot, pero reduciendo su tamaño.
- Como parte del procesamiento de datos, hay casos en los que por ejemplo, una pelicula con mas de un genero, se separa con '/' y otros con ','. Se debería generalizar el almacenamiento.
- El proceso de verificacion de iniciar sesion o registrarse necesita mejoras. Las funciones me parece que estan bien, pero la implementacion en el main necesita mejoras.
- Si es una cuenta privada no creo que sea necesario mostrar los usuarios registrados
- Creo que no hay problema en que el nombre de usuario se repita, siempre y cuando el correo sea diferente.
