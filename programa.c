#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h> //INSTALAR LA LIBRERÍA EN EL PROYECTO PARA QUE FUNCIONE

struct  {
  int id;
  char nombre; // por que nombre es un puntero?  por que es un string largo y no se sabe el tamaño de memoria que va a ocupar
  char autor; // por que autor no es un puntero?  por que es un string corto y se sabe el tamaño de memoria que va a ocupar
  int año_publicacion; // por que año_publicacion no es un puntero?  por que es un entero y se sabe el tamaño de memoria que va a ocupar
  char genero; // por que es un puntero?  por que es un string largo y no se sabe el tamaño de memoria que va a ocupar 
  char resumen; // por que es un puntero?  por que es un string largo y no se sabe el tamaño de memoria que va a ocupar
  int disponible; // por que es booleano?  por que es un valor que solo puede ser verdadero o falso
}Ejemplar;

// Estructura de Libro 
typedef struct Libro  {
    int identificador;
    char* nombre;
    char* autor;
    int anioPublicacion;
    char* genero;
    char* resumen;
    int cantidad;
} Libro;

// Estructura de Usuario
typedef struct Usuario  {
    char *identificacion;
    char *nombre;
    char *direccion;
} Usuario;

//Estructura de Prestamo
typedef struct Prestamo  {
    char identificador[50];
    char usuario[50]; //identificación del usuario
    char estado[50];
    char nombre[50];
    char idEjemplar[50]; //identificador del libro
    char fecha_devolucion[50];
    int tardia; //true para tardía
} Prestamo;

// Declaración de funciones
void devolucionesEjemplar();
void opcionesOperativas();
void opcionesGenerales();
void gestionCatalogo();
void gestionUsuarios();
void historialPrestamos();
void vencimientoPrestamos();
void estadisticas();
void busquedaSimple();
void busquedaAvanzada();
void prestamoEjemplar();
void IngresarLibroTxt();
void devolucionesEjemplar_aux(char *id_prestamo, char *fecha_devolucion);
int buscar_prestamo(struct Prestamo *prestamos, int n, char *id_prestamo);

int main() {
    int opcion;
    
    do {
	printf("\n*************************************\n");
        printf("\n*** Menu Principal ***\n");
        printf("1. Opciones Operativas\n");
        printf("2. Opciones Generales\n");
        printf("3. Salir\n");
        printf("Ingrese su opcion: ");
        scanf("%d", &opcion);
        
        switch (opcion) {
            case 1:
                opcionesOperativas();
                break;
            case 2:
                opcionesGenerales();
                break;
            case 3:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción inválida. Intente nuevamente.\n");
        }
    } while (opcion != 3);
    
    return 0;
}

void opcionesOperativas() {
    int opcion;
    
    do {
	printf("\n*************************************\n");
        printf("\n*** Opciones Operativas ***\n");
        printf("1. Gestión de Catálogo\n");
        printf("2. Gestión de Usuarios\n");
        printf("3. Historial de Préstamos\n");
        printf("4. Vencimiento de Préstamos\n");
        printf("5. Estadísticas\n");
        printf("6. Volver al menu principal\n");
        printf("Ingrese su opción: ");
        scanf("%d", &opcion);
        
        switch (opcion) {
            case 1:
                gestionCatalogo();
                break;
            case 2:
                gestionUsuarios();
                break;
            case 3:
                historialPrestamos();
                break;
            case 4:
                vencimientoPrestamos();
                break;
            case 5:
                estadisticas();
                break;
            case 6:
                printf("Volviendo al Menú Principal...\n");
                break;
            default:
                printf("Opción inválida. Intente nuevamente.\n");
        }
    } while (opcion != 6);
}

void opcionesGenerales() {
    int opcion;
    
    do {
	printf("\n*************************************\n");
        printf("\n*** Opciones Generales ***\n");
        printf("1. Búsqueda Simple\n");
        printf("2. Búsqueda Avanzada\n");
        printf("3. Préstamo de Ejemplar\n");
        printf("4. Devolución de Ejemplar\n");
        printf("5. Volver\n");
        printf("Ingrese su opción: ");
        scanf("%d", &opcion);
        
        switch (opcion) {
            case 1:
                busquedaSimple();
                break;
            case 2:
                busquedaAvanzada();
                break;
            case 3:
                prestamoEjemplar();
                break;
            case 4:
                devolucionesEjemplar();
                break;
            case 5:
                printf("Volviendo al Menú Principal...\n");
                break;
            default:
                printf("Opción inválida. Intente nuevamente.\n");
        }
    } while (opcion != 5);
}

void gestionCatalogo() {
    printf("\n*************************************\n");
    printf("\n*** Gestión de Catálogo ***\n");

    char rutaTxt[100];
    char *rutaJSON = "datosLibros.json";
    
    //solicitar ruta
    printf("Ingrese la dirección del archivo .txt: ");
    scanf("%s", rutaTxt);        
    
    IngresarLibroTxt(rutaTxt, rutaJSON);   
}

// Función para validar el formato de una línea de texto
int validarFormatoLinea(const char *linea) {
    // Implementa aquí tus propias validaciones de formato
    // En este ejemplo, verificaremos que haya 6 campos separados por '|'
    char *token = strtok((char *)linea, "|");
    int campos = 0;
    while (token != NULL) {
        campos++;
        token = strtok(NULL, "|");
    }
    return campos == 6;
}

char *leerArchivo(char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s.\n", nombreArchivo);
        return NULL;
    }

    fseek(archivo, 0, SEEK_END);
    long tamañoArchivo = ftell(archivo);
    rewind(archivo);

    char *contenido = (char *)malloc((tamañoArchivo + 1) * sizeof(char));
    if (contenido == NULL) {
        printf("Error al asignar memoria para el contenido del archivo.\n");
        fclose(archivo);
        return NULL;
    }

    fread(contenido, sizeof(char), tamañoArchivo, archivo);
    contenido[tamañoArchivo] = '\0'; // Agregar el carácter nulo al final

    fclose(archivo);

    return contenido;
}

// Función para ingresar un libro al archivo JSON
void ingresarLibroJSON(struct Libro libro) {
    printf("Ingresando los ejemplares en el archivo JSON...\n");
    //leer el contenido del archivojson
    char* contenido = leerArchivo("./datosLibros.json");
    // Crear un objeto JSON para el libro
    struct json_object *array = json_object_new_array();

    //hacer validacion en caso de que el archivos json de libros este vacio

    struct json_object *libros = json_tokener_parse(contenido);
    printf("LLega");
    for (int i = 0; i < json_object_array_length(libros); i++) {
        struct json_object *libro = json_object_array_get_idx(libros, i);
        json_object_array_add(array, libro);
    }

    libro.identificador = json_object_array_length(libros) + 1;

    struct json_object *jobj = json_object_new_object();
    json_object_object_add(jobj, "identificador", json_object_new_int(libro.identificador));
    json_object_object_add(jobj, "nombre", json_object_new_string(libro.nombre));
    json_object_object_add(jobj, "autor", json_object_new_string(libro.autor));
    json_object_object_add(jobj, "anioPublicacion", json_object_new_int(libro.anioPublicacion));
    json_object_object_add(jobj, "genero", json_object_new_string(libro.genero));
    json_object_object_add(jobj, "resumen", json_object_new_string(libro.resumen));
    json_object_object_add(jobj, "cantidad", json_object_new_int(libro.cantidad));

    // Agregar el objeto JSON al arreglo de libros
    json_object_array_add(array, jobj);
    // Agregar el objeto JSON al archivo JSON
    json_object_to_file("./datosLibros.json", array);
    
}

char *obtenerLinea(const char *texto, int numeroLinea) {
    if (numeroLinea <= 0)
        return NULL;

    const char *inicioLinea = texto;
    const char *finLinea = texto;
    int lineaActual = 1;
    int encontrado = 0;

    while (*finLinea != '\0') {
        if (*finLinea == '\n') {
            if (lineaActual == numeroLinea) {
                encontrado = 1;
                break;  // Hemos encontrado la línea deseada
            }
            lineaActual++;
            inicioLinea = finLinea + 1;
        }
        finLinea++;
    }

    if (encontrado ==0) {
      return NULL;
    }

    // Si el número de línea solicitado es mayor que el número de líneas en el texto, o si el texto está vacío, devolvemos NULL.
    if (lineaActual < numeroLinea || *inicioLinea == '\0') {
        return NULL;
    }

    // Calculamos la longitud de la línea y la copiamos en una nueva cadena.
    size_t longitudLinea = finLinea - inicioLinea;
    char *linea = malloc(longitudLinea + 1);
    if (linea == NULL) {
        perror("Error en la asignación de memoria");
        exit(EXIT_FAILURE);
    }
    strncpy(linea, inicioLinea, longitudLinea);
    linea[longitudLinea] = '\0';  // Añadimos el carácter nulo al final de la línea.

    return linea;
}

Libro* buscarLibro(char* nombre) {
    printf("Buscando libros...\n");
    printf("Leyendo base de datos JSON...\n");
    // Leer el contenido del archivo JSON
    char* contenido = leerArchivo("./datosLibros.json");
    printf("%s",contenido);

    printf("Verificando el contenido del documento JSON...\n");
    if (contenido == NULL){
        printf("El archivo esta vacio..\n");
        return NULL;
    }else{
        if(contenido != NULL && contenido[0]=='\0'){
            printf("El archivo esta vacio..\n");
            return NULL;
        }else{
            printf("Creando arreglo de objetos JSON...\n");
            // Crear un objeto JSON para el libro
            struct json_object *array = json_object_new_array();
            printf("Creando objetos JSON...\n");
            struct json_object *libros = json_tokener_parse(contenido);
        
            for (int i = 0; i < json_object_array_length(libros); i++) {
            struct json_object *libro = json_object_array_get_idx(libros, i);
            json_object_array_add(array, libro);
            }

            for (int i = 0; i < json_object_array_length(array); i++) {
                struct json_object *libro = json_object_array_get_idx(array, i);
                struct json_object *nombreLibro;
                struct json_object *autorLibro;
                struct json_object *anioPublicacionLibro;
                struct json_object *generoLibro;
                struct json_object *resumenLibro;
                struct json_object *cantidadLibro;

                Libro* libroStruct = malloc(sizeof(Libro));

                libroStruct->identificador = json_object_get_int(libro);
                nombreLibro = json_object_object_get(libro, "nombre");
                autorLibro = json_object_object_get(libro, "autor");
                anioPublicacionLibro = json_object_object_get(libro, "anioPublicacion");
                generoLibro = json_object_object_get(libro, "genero");
                resumenLibro = json_object_object_get(libro, "resumen");
                cantidadLibro = json_object_object_get(libro, "cantidad");

                libroStruct->nombre = strdup(json_object_get_string(nombreLibro));
                libroStruct->autor = strdup(json_object_get_string(autorLibro));
                libroStruct->anioPublicacion = json_object_get_int(anioPublicacionLibro);
                libroStruct->genero = strdup(json_object_get_string(generoLibro));
                libroStruct->resumen = strdup(json_object_get_string(resumenLibro));
                libroStruct->cantidad = json_object_get_int(cantidadLibro);

                if (strcmp(libroStruct->nombre, nombre) == 0) {
                    return libroStruct;
                }
            }
        }
    }

    return NULL;
}



// Función para ingresar libros desde un archivo de texto y almacenarlos en un archivo JSON
void IngresarLibroTxt(char *rutaTxt, const char *rutaJSON) {
    char *contenidoTxt = leerArchivo(rutaTxt);
    char *linea;

    printf("Leyendo archivo txt...\n");
    
    if (contenidoTxt == NULL) {
        printf("No se pudo abrir el archivo de texto.\n");
        return;
    }
    
    char *copiaContenidoTxt = malloc(strlen(contenidoTxt) + 1);
    strcpy(copiaContenidoTxt, contenidoTxt);
    
    linea = obtenerLinea(contenidoTxt, 1);
    int lineaActual = 1;
    printf("Leyendo lineas del archivo.txt....\n");
    struct Libro libro;
    printf("Crando structs para los Libros....\n");

    // Mientras la línea no sea nula
    while (linea != NULL) {
        // Valida el formato de la línea
        char *copiaLinea = malloc(strlen(linea) + 1);
        strcpy(copiaLinea, linea);
        if (validarFormatoLinea(copiaLinea)) {
            // Obtiene el primer token (nombre)
            char *token = strtok(linea, "|");
            // Guarda el nombre en la estructura
            libro.nombre = token;
            // Obtiene el segundo token (autor)
            token = strtok(NULL, "|");
            // Guarda el autor en la estructura
            libro.autor = token;
            // Obtiene el tercer token (año de publicación)
            token = strtok(NULL, "|");
            // Guarda el año de publicación en la estructura
            libro.anioPublicacion = atoi(token);
            // Obtiene el cuarto token (género)
            token = strtok(NULL, "|");
            // Guarda el género en la estructura
            libro.genero = token;
            // Obtiene el quinto token (resumen)
            token = strtok(NULL, "|");
            // Guarda el resumen en la estructura
            libro.resumen = token;
            // Obtiene el sexto token (cantidad)
            token = strtok(NULL, "|");
            // Guarda la cantidad en la estructura
            libro.cantidad = atoi(token);

            printf("Objetos recuperados del archivo txt...\n");

            // Ingresa el libro al archivo JSON
            if (buscarLibro(libro.nombre) == NULL){
                printf("Verificando duplicidad de ejemplares....\n");                
                ingresarLibroJSON(libro);
                printf("Ingresando ejemplar en la base de datos json...\n");
            }
        } else {
            printf("ERROR!! La línea no tiene el formato esperado: %s\n", linea);
        }

        // Lee la siguiente línea
        lineaActual++;
        linea = obtenerLinea(contenidoTxt, lineaActual);
    }
    

    printf("Se ha procesado el archivo .txt con éxito.\n");
}


void devolucionesEjemplar() {
    printf("Realizando Devolución de Ejemplar...\n");
    // Lógica de devolución de ejemplar
}


void IngresarUsuario(struct Usuario usuario) {
    char* contenido = leerArchivo("./datosUsuarios.json");
    // Crear un objeto JSON para el libro
    struct json_object *array = json_object_new_array();
    struct json_object *usuarios = json_tokener_parse(contenido);
    for (int i = 0; i < json_object_array_length(usuarios); i++) {
        struct json_object *usuarioTemp = json_object_array_get_idx(usuarios, i);
        json_object_array_add(array, usuarioTemp);
    }

    struct json_object *jobj = json_object_new_object();
    json_object_object_add(jobj, "identificacion", json_object_new_string(usuario.identificacion));
    json_object_object_add(jobj, "nombre", json_object_new_string(usuario.nombre));
    json_object_object_add(jobj, "direccion", json_object_new_string(usuario.direccion));

    // Agregar el objeto JSON al arreglo de libros
    json_object_array_add(array, jobj);
    // Agregar el objeto JSON al archivo JSON
    json_object_to_file("./datosUsuarios.json", array);
}

Usuario* buscarUsuario(char* identificacion) {
    // Leer el contenido del archivo JSON
    char* contenido = leerArchivo("./datosUsuarios.json");
    
    // Crear un objeto JSON para el usuario
    struct json_object *array = json_object_new_array();
    struct json_object *usuarios = json_tokener_parse(contenido);
    
    for (int i = 0; i < json_object_array_length(usuarios); i++) {
        struct json_object *usuario = json_object_array_get_idx(usuarios, i);
        json_object_array_add(array, usuario);
    }

    for (int i = 0; i < json_object_array_length(array); i++) {
        struct json_object *usuario = json_object_array_get_idx(array, i);
        struct json_object *identificacionUsuario;
        struct json_object *nombreUsuario;
        struct json_object *direccionUsuario;

        Usuario* usuarioStruct = malloc(sizeof(Usuario));

        identificacionUsuario = json_object_object_get(usuario, "identificacion");
        nombreUsuario = json_object_object_get(usuario, "nombre");
        direccionUsuario = json_object_object_get(usuario, "direccion");

        usuarioStruct->identificacion = strdup(json_object_get_string(identificacionUsuario));
        usuarioStruct->nombre = strdup(json_object_get_string(nombreUsuario));
        usuarioStruct->direccion = strdup(json_object_get_string(direccionUsuario));

        if (strcmp(usuarioStruct->identificacion, identificacion) == 0) {
            return usuarioStruct;
        }
    }

    return NULL;
}


/*
2.	Implementar una función llamada showHistorial() que permita al usuario ingresar un
 rango de fechas (inicio y fin) y muestre todos los préstamos generados en ese período.
*/

void showHistorial(char* rutaArchivo){

    char fechaInicio[50];
    char fechaFin[50];

    printf("Ingrese la fecha de inicio: ");
    scanf("%s", fechaInicio);
    printf("Ingrese la fecha de fin: ");
    scanf("%s", fechaFin);

    FILE *archivo;
    archivo = fopen(rutaArchivo,"r");
    if(archivo == NULL){
        printf("Error al abrir el archivo");
    }
    else{
        char linea[100];
        while(fgets(linea, 100, archivo) != NULL){
            char *token = strtok(linea, " ");
            while(token != NULL){
                if(strcmp(token, fechaInicio) == 0){
                    printf("%s", linea);
                }
                token = strtok(NULL, " ");
            }
        }
        fclose(archivo);
    }
}


void gestionUsuarios() {
    printf("\n\nRealizando Gestión de Usuarios...\n");
    char identificacion[100];
    char nombre[100];
    char direccion[100];

    printf("Ingrese el número de identificación: ");
    scanf("%s", identificacion);

    printf("Ingrese el nombre: ");
    scanf("%s", nombre);

    printf("Ingrese la dirección: ");
    scanf("%s", direccion);

    //validar que no haya usuario con la cedula ingresada
    if (buscarUsuario(identificacion) != NULL) {
      printf("Ya existe un usuario con la cédula ingresada.");
      return;
    }

    //generar el objeto usuario
    struct Usuario usuarioNuevo;
    usuarioNuevo.identificacion = malloc(strlen(identificacion) + 1);
    strcpy(usuarioNuevo.identificacion, identificacion);
    usuarioNuevo.nombre = malloc(strlen(nombre) + 1);
    strcpy(usuarioNuevo.nombre, nombre);
    usuarioNuevo.direccion = malloc(strlen(direccion) + 1);
    strcpy(usuarioNuevo.direccion, direccion);

    IngresarUsuario(usuarioNuevo);
    printf("Usuario agregado exitosamente");
    return;
}

void historialPrestamos() {
    printf("Mostrando Historial de Préstamos...\n");
    // Lógica de historial de préstamos
}

void vencimientoPrestamos() {
    printf("Verificando Vencimiento de Préstamos...\n");
    // Lógica de vencimiento de préstamos
}

void estadisticas() {
    printf("Generando Estadísticas...\n");
    // Lógica de generación de estadísticas
}

Libro *obtenerLibros() {
    char* contenido = leerArchivo("./datosLibros.json");
    // Crear un objeto JSON para el libro
    struct json_object *libros = json_tokener_parse(contenido);
    int cantidadLibros = json_object_array_length(libros);
    Libro *librosStruct = malloc(sizeof(Libro) * cantidadLibros);
    
    for (int i = 0; i < cantidadLibros; i++) {
        struct json_object *libro = json_object_array_get_idx(libros, i);
        json_object *id = json_object_object_get(libro, "identificador");
        struct json_object *nombreLibro;
        struct json_object *autorLibro;
        struct json_object *anioPublicacionLibro;
        struct json_object *generoLibro;
        struct json_object *resumenLibro;
        struct json_object *cantidadLibro;

        Libro libroActual;

        libroActual.identificador = json_object_get_int(id);
        nombreLibro = json_object_object_get(libro, "nombre");
        autorLibro = json_object_object_get(libro, "autor");
        anioPublicacionLibro = json_object_object_get(libro, "anioPublicacion");
        generoLibro = json_object_object_get(libro, "genero");
        resumenLibro = json_object_object_get(libro, "resumen");
        cantidadLibro = json_object_object_get(libro, "cantidad");

        libroActual.nombre = malloc(strlen(json_object_get_string(nombreLibro)) + 1);
        strcpy(libroActual.nombre, json_object_get_string(nombreLibro));
        
        libroActual.autor = malloc(strlen(json_object_get_string(autorLibro)) + 1);
        strcpy(libroActual.autor, json_object_get_string(autorLibro));

        libroActual.anioPublicacion = json_object_get_int(anioPublicacionLibro);

        libroActual.genero = malloc(strlen(json_object_get_string(generoLibro)) + 1);
        strcpy(libroActual.genero, json_object_get_string(generoLibro));

        libroActual.resumen = malloc(strlen(json_object_get_string(resumenLibro)) + 1);
        strcpy(libroActual.resumen, json_object_get_string(resumenLibro));

        libroActual.cantidad = json_object_get_int(cantidadLibro);

        librosStruct[i] = libroActual;
    }

    return librosStruct;
}

int obtenerTotalLibros() {
    char* contenido = leerArchivo("./datosLibros.json");
    // Crear un objeto JSON para el libro
    struct json_object *libros = json_tokener_parse(contenido);
    int cantidadLibros = json_object_array_length(libros);
    return cantidadLibros;
}

void busquedaSimple() {
  printf("Realizando Búsqueda Simple...\n");
  
  char textoBuscar[250];
  
    printf("Ingrese el texto a buscar: ");
    scanf(" %[^\n]s", textoBuscar);
    
    // Lógica de búsqueda simple
    Libro *libros = obtenerLibros();
    int totalLibros = obtenerTotalLibros();
    for(int i = 0; i < totalLibros; i++) {
        if (strstr(libros[i].nombre, textoBuscar) != NULL || strstr(libros[i].autor, textoBuscar) != NULL || strstr(libros[i].resumen, textoBuscar) != NULL) {
            printf("\n\n_______________\n");
            printf("Identificador: %d\n", libros[i].identificador);
            printf("Nombre: %s\n", libros[i].nombre);
            printf("Resumen: %s\n", libros[i].resumen);
            printf("Estado: %s\n", libros[i].cantidad > 0 ? "Disponible" : "No disponible");
        }
    }
}

// Busca un préstamo por su ID
int buscar_prestamo(struct Prestamo *prestamos, int n, char* id_prestamo) {
  int i;

  // Recorre los préstamos
  for (i = 0; i < n; i++) {
    // Si el identificador coincide con el buscado retorna el índice (ambos son char [50])
    if (strcmp(prestamos[i].identificador, id_prestamo) == 0) {
      return i;
    }
  }

  // Si no se encontró el préstamo
  return -1;
}

void devolucionesEjemplar_aux(char* id_prestamo, char *fecha_devolucion) {

  struct Prestamo *prestamos;
  //hay que cargar todos los prestamos que estén en los archivos json
  int n = 0;

  // Valida la fecha de devolución
  if (fecha_devolucion == NULL || strlen(fecha_devolucion) == 0) {
    printf("Debe ingresar una fecha de devolución válida.\n");
    return;
  }

  // Busca el préstamo
  int i = buscar_prestamo(prestamos, n, id_prestamo);

  // Si el préstamo no se encontró
  if (i == -1) {
    printf("No se encontró el préstamo.\n");
    return;
  }

  // Actualiza la fecha de devolución
  //prestamos[i].fecha_devolucion = fecha_devolucion;//funcionará hasta cargar los prestamos

  // Calcula el monto asociado al préstamo
  int dias = 0;
  //dias = (difftime(fecha_devolucion, prestamos[i].fecha_prestamo) / 86400);
  int monto = 0;

  // Si el préstamo es de 1 a 7 días
  if (dias <= 7) {
    monto = dias * 10;
  } else {
    monto = 70;
  }

  // Imprime el monto asociado al préstamo
  printf("El monto a pagar es de $%d.\n", monto);

  // Cambia el estado del préstamo a "devuelto"
//   prestamos[i].estado = "Devuelto";

  // Imprime un mensaje de confirmación
  printf("El ejemplar se ha devuelto correctamente.\n");
}








void busquedaAvanzada() {
    printf("Realizando Búsqueda Avanzada...\n");
    // Lógica de búsqueda avanzada
}

void prestamoEjemplar() {
    printf("Realizando Préstamo de Ejemplar...\n");
    // Lógica de préstamo de ejemplar
}
