#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <json-c/json.h> //INSTALAR LA LIBRERÍA EN EL PROYECTO PARA QUE FUNCIONE
#include <time.h>

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
    int id;
    int identificadorUsuario;
    int estado; //0 para prestado, 1 para devuelto
    int idEjemplar; //identificador del libro
    char fecha_prestamo[50];
    char fecha_devolucion[50];
    char fecha_de_entrega[50];
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
Prestamo* buscarPrestamo(int);
int buscar_prestamo(struct Prestamo *prestamos, int n, char *id_prestamo);

/*
*Verifica que el formato de las fechas sea el adecuado para el programa
*Entradas: Una cadena de caracteres
*Salidas: true para indicar que el formato es el adecuado, false para indicar que no lo es
*Restricciones: ninguna
*/
bool esFormatoFechaValido(const char *fecha) {
    // Verifica si la longitud de la cadena es exactamente 10 caracteres.
    if (strlen(fecha) != 10) {
        return false;
    }

    // Verifica que los caracteres en las posiciones específicas sean números y los separadores sean guiones (-).
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (fecha[i] != '-') {
                return false;
            }
        } else {
            if (fecha[i] < '0' || fecha[i] > '9') {
                return false;
            }
        }
    }
    return true;
}
/*
*Permite al usuario hacer la devolucion de un ejemplar en prestamo
*Entradas: EL usuario debe ingresar el id del prestamo
*Salidas: Realiza la devolucin dle ejemplar
*Restricciones:el id ingresado debe ser el id de un prestamo realizado
*/
void devolucionesEjemplar(){
    int elPrestamo = 0;
    printf("Realizando Devolución de Ejemplar...\n");
    printf("Ingrese el id del prestamo: ");
    scanf("%d", &elPrestamo);

    if(buscarPrestamo(elPrestamo) == NULL){
        printf("No se encontró el prestamo");
        return;
    }

    //leer prestamos y guardar en lista de prestamos
    FILE *archivo;
    char buffer[5000];

    struct json_object *parsed_json;
    struct json_object *obj;

    struct json_object *id;
    struct json_object *identificadorUsuario;
    struct json_object *estado;
    struct json_object *idEjemplar;
    struct json_object *fecha_prestamo;
    struct json_object *fecha_devolucion;
    struct json_object *fecha_entrega;
    struct json_object *tardia;
    size_t n,i;
    archivo = fopen("./prestamos.json", "r"); 
    fread(buffer, 5000, 1, archivo);
    fclose(archivo);

    parsed_json = json_tokener_parse(buffer);
    n = json_object_array_length(parsed_json);

    Prestamo* prestamos[n];
    for(int x = 0; x<n; x++){
        prestamos[x] = malloc(sizeof(Prestamo));
    }
    
    for(i = 0; i < n; i++){
        obj = json_object_array_get_idx(parsed_json, i);
        json_object_object_get_ex(obj, "id", &id); 
        
        json_object_object_get_ex(obj, "identificadorUsuario", &identificadorUsuario); 
        json_object_object_get_ex(obj, "estado", &estado); 
        json_object_object_get_ex(obj, "idEjemplar", &idEjemplar); 
        json_object_object_get_ex(obj, "fecha_prestamo", &fecha_prestamo); 
        json_object_object_get_ex(obj, "fecha_devolucion", &fecha_devolucion); 
        json_object_object_get_ex(obj, "fecha_entrega", &fecha_entrega); 
        json_object_object_get_ex(obj, "tardia", &tardia); 
        
        prestamos[i]->id = json_object_get_int(id);
        prestamos[i]->identificadorUsuario = json_object_get_int(identificadorUsuario);
        prestamos[i]->estado = json_object_get_int(estado);
        prestamos[i]->idEjemplar = json_object_get_int(idEjemplar);
        strcpy(prestamos[i]->fecha_prestamo, json_object_get_string(fecha_prestamo));
        strcpy(prestamos[i]->fecha_devolucion, json_object_get_string(fecha_devolucion));
        strcpy(prestamos[i]->fecha_de_entrega, json_object_get_string(fecha_entrega));
        prestamos[i]->tardia = json_object_get_int(tardia);
    }

    //buscar el prestamo en el array
    int precio = 0;
    for(i = 0; i < n; i++){
        if(prestamos[i]->id == elPrestamo){
            //hacer la edicion 
            time_t t = time(NULL);

            // Convertimos el tiempo a una estructura tm
            struct tm *tm = localtime(&t);

            // Reservamos memoria para la cadena de caracteres
            char *fecha = malloc(sizeof(char) * 20);

            // Formateamos la fecha
            strftime(fecha, 20, "%d/%m/%Y", tm);

            strcpy(prestamos[i]->fecha_de_entrega, fecha);
            prestamos[i]->estado = 0; //entregado
            //ver tardia comparando si la fecha de entrega es mayor a la fecha de devolucion
            //calcular el precio
            int tarde = strcmp(prestamos[i]->fecha_de_entrega, prestamos[i]->fecha_devolucion);
                
            if(tarde > 0){
                prestamos[i]->tardia = 1; //tardia
                if(tarde <= 7){
                    precio += tarde*75;
                }
                else if(tarde <= 15){
                    precio += tarde*50;
                }
                else{
                    precio += tarde*25;
                }
            }
            else{
                prestamos[i]->tardia = 0; //no tardia
            }
            int dias = strcmp(prestamos[i]->fecha_prestamo, prestamos[i]->fecha_devolucion);
            if(dias <= 7){
                precio += dias*150;
            }
            else if(dias <= 15){
                precio += dias*125;
            }
            else{
                precio += dias*100;
            }
            break;
        }
    }

    printf("Se ha realizado la devolución del ejemplar con éxito.\n");
    printf("El precio del préstamo es: %d\n", precio);
    printf("Gracias por usar nuestro servicio.\n");

    //volver a guardar los prestamos en el json
    struct json_object *json_array = json_object_new_array();

    for(int i = 0; i < n; i++){
        struct json_object *obj = json_object_new_object();
        json_object_object_add(obj, "id", json_object_new_int(prestamos[i]->id)); 
        json_object_object_add(obj, "identificadorUsuario", json_object_new_int(prestamos[i]->identificadorUsuario));
        json_object_object_add(obj, "estado", json_object_new_int(prestamos[i]->estado));
        json_object_object_add(obj, "idEjemplar", json_object_new_int(prestamos[i]->idEjemplar));
        json_object_object_add(obj, "fecha_prestamo", json_object_new_string(prestamos[i]->fecha_prestamo));
        json_object_object_add(obj, "fecha_devolucion", json_object_new_string(prestamos[i]->fecha_devolucion));
        json_object_object_add(obj, "fecha_entrega", json_object_new_string(prestamos[i]->fecha_de_entrega));
        json_object_object_add(obj, "tardia", json_object_new_int(prestamos[i]->tardia));

        json_object_array_add(json_array, obj);   
    }
    
    const char *json_str = json_object_to_json_string(json_array);

    FILE *file = fopen("./prestamos.json", "w");
    if (file) {
        fprintf(file, "%s", json_str);
        fclose(file);
    }


}
/*
*Menu de opciones generales, permite al usuario navegar a traves de las opciones generales
*Entradas: un caracter por teclado
*Salidas: Una opcion segun la eleccion del usuario
*Restricciones: EL usuario debe seleccionar una opcion valida*/
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
/*
*Menu de opciones iniciales, permite al usuario navegar a traves de las opciones principales
*Entradas: un caracter por teclado
*Salidas: Una opcion segun la eleccion del usuario
*Restricciones: EL usuario debe seleccionar una opcion valida*/
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
/*
*Retorna el prestamo cuyo id coincide con el id ingresado como argumento
*Entradas: Un entero
*Salidas: El prestamo cuyo id coincide con el argumento de la funcion
*Restricciones: El argumento debe ser un entero*/
struct Prestamo * buscarPrestamo(int id_prestamo) {
    FILE *archivo;
    char buffer[5000];

    struct json_object *parsed_json;
    struct json_object *obj;

    struct json_object *id;
    struct json_object *identificadorUsuario;
    struct json_object *estado;
    struct json_object *idEjemplar;
    struct json_object *fecha_prestamo;
    struct json_object *fecha_devolucion;
    struct json_object *fecha_entrega;
    struct json_object *tardia;
    size_t n,i;
    archivo = fopen("./prestamos.json", "r"); 
    fread(buffer, 5000, 1, archivo);
    fclose(archivo);

    parsed_json = json_tokener_parse(buffer);
    n = json_object_array_length(parsed_json);

    Prestamo* prestamo = malloc(sizeof(Prestamo));
    int encontrado;
    //buscar el prestamo
    for(i = 0; i < n; i++){
        obj = json_object_array_get_idx(parsed_json, i);
        json_object_object_get_ex(obj, "id", &id); 
        
        if(json_object_get_int(id) == id_prestamo){
            json_object_object_get_ex(obj, "identificadorUsuario", &identificadorUsuario); 
            json_object_object_get_ex(obj, "estado", &estado); 
            json_object_object_get_ex(obj, "idEjemplar", &idEjemplar); 
            json_object_object_get_ex(obj, "fecha_prestamo", &fecha_prestamo); 
            json_object_object_get_ex(obj, "fecha_devolucion", &fecha_devolucion); 
            json_object_object_get_ex(obj, "fecha_entrega", &fecha_entrega); 
            json_object_object_get_ex(obj, "tardia", &tardia); 
            
            prestamo->id = json_object_get_int(id);
            prestamo->identificadorUsuario = json_object_get_int(identificadorUsuario);
            prestamo->estado = json_object_get_int(estado);
            prestamo->idEjemplar = json_object_get_int(idEjemplar);
            strcpy(prestamo->fecha_prestamo, json_object_get_string(fecha_prestamo));
            strcpy(prestamo->fecha_devolucion, json_object_get_string(fecha_devolucion));
            strcpy(prestamo->fecha_de_entrega, json_object_get_string(fecha_entrega));
            prestamo->tardia = json_object_get_int(tardia);

            encontrado = 1;
        }
    }
    if(encontrado == 0){
        return NULL;
    }
    return prestamo;
}
/*
*Menu de opciones operativas, permite al usuario navegar a traves de las opciones operativas
*Entradas: un caracter por teclado
*Salidas: Una opcion segun la eleccion del usuario
*Restricciones: EL usuario debe seleccionar una opcion valida
*/
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
/*
*Permite al usuario hacer una inclusion en lote desde un txt en una ruta
*Entradas: UNa ruta de un archivo de texto plano
*Salidas: Se cargan los libros desde el archivo de texto
*Restricciones: LA ruta debe ser valida*/
void gestionCatalogo() {
    printf("Gestionando Catálogo...\n"); 
    // Lógica de gestión de catálogo
    int opcion;
    do {
        printf("\n*************************************\n");
        printf("\n*** Gestión de Catálogo ***\n");
        printf("1. Ingresar Libro\n");
        printf("4. Volver\n");
        printf("Ingrese su opción: ");
        scanf("%d", &opcion);
        
        switch (opcion) {
            case 1:
                char rutaTxt[100];
                char *rutaJSON = "datosLibros.json";
                //solicitar ruta
                printf("Ingrese la dirección del archivo .txt: ");
                scanf("%s", rutaTxt);

                IngresarLibroTxt(rutaTxt, rutaJSON);
                break;
            case 2:
                printf("Modificando Libro...\n");
                break;
            case 3:
                printf("Eliminando Libro...\n");
                break;
            case 4:
                printf("Volviendo al Menú Principal...\n");
                break;
            default:
                printf("Opción inválida. Intente nuevamente.\n");
        }
    } while (opcion != 4);
}
/*
*Verifica que la linea de texto recibida tenga el formato de tokens divididos por pipes
*Entradas: Una linea de caracteres
*Salidas: UN codigo de confirmacion en caso de que la linea tenga el formato deseado
*Restricciones: EL argumento debe ser una cadena de caracteres
*/
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
/*
*Lee el contenido de un archivo en la ruta indicada
*Entradas: Una cadena de caracteres
*Salidas: Retorna una cadena de caracteres con el contenido del archivo de texto en la rita indicada como argumento
*Restricciones: EL argumento debe ser la ruta de un archivo de texto*/
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
/*
* Guarda el struct libro en el archivo json de libros
*Enradas: uN struct de libro
*Salidas: Guarda los datos del struct en el archivo json
*Restricciones: EL argumento debe ser un struct libro
*/
void ingresarLibroJSON(struct Libro libro) {
    //leer el contenido del archivojson
    char* contenido = leerArchivo("./datosLibros.json");
    // Crear un objeto JSON para el libro
    struct json_object *array = json_object_new_array();
    struct json_object *libros = json_tokener_parse(contenido);
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
/*
* Retorna la linea indicada con el segundo argumento en el archivo indiicado como primer argumento
*Enradas: Una cadena de texto y un entero
*Salidas: Del texto recibido devuelve la linea en el numero indicado
*Restricciones: La linea no debe ser mayor al numero de lineas de la cadena de texto
*/
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
/*
* Devuelve el libro con el nombre indicado
*Enradas: UNa cadena de texto con el nombre del libro
*Salidas: EL struct con el libro del nombre buscado
*Restricciones: El texto recibido debe ser el nombre de un libro del sistema
*/
Libro* buscarLibro(char* nombre) {
    // Leer el contenido del archivo JSON
    char* contenido = leerArchivo("./datosLibros.json");
    
    // Crear un objeto JSON para el libro
    struct json_object *array = json_object_new_array();
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

        Libro* libroStruct = malloc(sizeof(Libro)); //  malloc reserva memoria para un tipo de dato

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

        char* identif;
        sprintf(identif, "%d", libroStruct->identificador);
        if (strcmp(libroStruct->nombre, nombre) == 0 || strcmp(identif, nombre)) {
            return libroStruct;
        }
    }

    return NULL;
}
/*
* Función para ingresar libros desde un archivo de texto y almacenarlos en un archivo JSON
*Enradas:Una ruta del archivo de texto y la ruta del json
*Salidas: Gurda los ibros del txt en el json
*Restricciones:
*/
void IngresarLibroTxt(char *rutaTxt, const char *rutaJSON) {
    char *contenidoTxt = leerArchivo(rutaTxt);
    char *linea;
    
    if (contenidoTxt == NULL) {
        printf("No se pudo abrir el archivo de texto.\n");
        return;
    }
    
    char *copiaContenidoTxt = malloc(strlen(contenidoTxt) + 1);
    strcpy(copiaContenidoTxt, contenidoTxt);
    
    linea = obtenerLinea(contenidoTxt, 1);
    int lineaActual = 1;
    struct Libro libro;

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

            // Ingresa el libro al archivo JSON
            if (buscarLibro(libro.nombre) == NULL){
              ingresarLibroJSON(libro);
            }
        } else {
            printf("La línea no tiene el formato esperado: %s\n", linea);
        }

        // Lee la siguiente línea
        lineaActual++;
        linea = obtenerLinea(contenidoTxt, lineaActual);
    }
    

    printf("Se ha procesado el archivo .txt con éxito.\n");
}
/*
* Guarda los datos del usuario en un json
*E: Un struct usuario
*S: Gurda los datos del usuario recibido en el json
*R: debe existir el archivo datosUsusarios.json
*/
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
/*
* BUsca un usuario por su id
*E: El id del usuario
*S: Retorna el usuario con ese id
*R: EL argumento debe ser un char 
*/
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
* Guarda los datos del usuario en un json
*E: LOs datos de los usuarios
*S: Gurda los datos del usuario recibido en el json
*R: debe existir el archivo datosUsusarios.json
*/
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

/*
 *Mostrar todos los prestamos realizados en un rango de fechas inicio-fin
 *Entradas: El usuario ingresa las fechas de inicio y fin en formato dd-mm-aa
 *Salidas: Muestra todos los prestamos realizado en el rango de fechas ingresado
 *Restricciones: Las fechas indicadas deben tener el formato dd-mm-aa, de lo contrario resulta en error
 */
void historialPrestamos() {
    char fechaInicio[11];
    char fechaFinal[11];

    // Solicita al usuario la fecha de inicio y la valida
    do {
        printf("Ingrese la fecha de inicio (dd/mm/aaaa): ");
        scanf("%s", fechaInicio);
    } while (!esFormatoFechaValido(fechaInicio));

    // Solicita al usuario la fecha final y la valida
    do {
        printf("Ingrese la fecha final (dd/mm/aaaa): ");
        scanf("%s", fechaFinal);
    } while (!esFormatoFechaValido(fechaFinal));

    // Obtén todos los préstamos
    Prestamo **prestamos = obtenerTodosLosPrestamos();

    // Itera a través de los préstamos y muestra los que están en el rango de fechas
    printf("Préstamos en el rango de fechas %s - %s:\n", fechaInicio, fechaFinal);
    for (int i = 0; prestamos[i] != NULL; i++) {
        if (strcmp(prestamos[i]->fecha_prestamo, fechaInicio) >= 0 && strcmp(prestamos[i]->fecha_de_entrega, fechaFinal) <= 0) {
            printf("ID Préstamo: %d\n", prestamos[i]->id);
            printf("ID Ejemplar: %d\n", prestamos[i]->idEjemplar);
            printf("ID Usuario: %d\n", prestamos[i]->identificadorUsuario);
            printf("Estado: %d\n", prestamos[i]->estado);
            printf("Tardía: %d\n", prestamos[i]->tardia);
            printf("\n");
        }
    }

    // Libera la memoria
    for (int i = 0; prestamos[i] != NULL; i++) {
        free(prestamos[i]);
    }
    free(prestamos);
}
/*
*Retorna un arreglo con todos los prestamos
*Entradas: ninguna
*Salidas: Un arreglo con struct de todos los prestamos
*Restricciones: Debe existir un archivo json con las estructuras de prestamos generados
*/
Prestamo **obtenerTodosLosPrestamos() {
    // Leer el contenido del archivo JSON
    char *contenido = leerArchivo("./prestamos.json");

    if (contenido == NULL) {
        fprintf(stderr, "Error al leer el archivo de préstamos.\n");
        exit(EXIT_FAILURE);
    }

    // Crear un objeto JSON para el archivo completo
    struct json_object *prestamos = json_tokener_parse(contenido);
    free(contenido); // Liberar la memoria después de su uso

    int numPrestamos = json_object_array_length(prestamos);
    Prestamo **arregloPrestamos = malloc((numPrestamos + 1) * sizeof(Prestamo*));

    for (int i = 0; i < numPrestamos; i++) {
        struct json_object *prestamo = json_object_array_get_idx(prestamos, i);

        Prestamo *prestamoStruct = malloc(sizeof(Prestamo));

        prestamoStruct->id = json_object_get_int(json_object_object_get(prestamo, "id"));
        prestamoStruct->identificadorUsuario = json_object_get_int(json_object_object_get(prestamo, "identificadorUsuario"));
        prestamoStruct->estado = json_object_get_int(json_object_object_get(prestamo, "estado"));
        prestamoStruct->idEjemplar = json_object_get_int(json_object_object_get(prestamo, "idEjemplar"));
        strcpy(prestamoStruct->fecha_prestamo, json_object_get_string(json_object_object_get(prestamo, "fecha_prestamo")));
        strcpy(prestamoStruct->fecha_devolucion, json_object_get_string(json_object_object_get(prestamo, "fecha_devolucion")));
        strcpy(prestamoStruct->fecha_de_entrega, json_object_get_string(json_object_object_get(prestamo, "fecha_de_entrega")));
        prestamoStruct->tardia = json_object_get_int(json_object_object_get(prestamo, "tardia"));

        arregloPrestamos[i] = prestamoStruct;
    }
    // Agregar un elemento NULL al final del arreglo para indicar su fin
    arregloPrestamos[numPrestamos] = NULL;

    return arregloPrestamos;
}
/*
* MUestra los rprestamos vencidos
*E: ninguno
*S: Muestra todos los restamos vencidos o proxios a vencer
*R: NInguna
*/
void vencimientoPrestamos() {
    // Obtener la lista de todos los préstamos
    Prestamo **prestamos = obtenerTodosLosPrestamos();

    // Obtener la fecha actual del sistema
    time_t t;
    struct tm *tm_info;
    time(&t);
    tm_info = localtime(&t);

    // Calcular la fecha actual más 3 días
    tm_info->tm_mday += 3;
    mktime(tm_info);

    // Imprimir los préstamos con fecha de entrega en los próximos 3 días o menos
    printf("Préstamos con fecha de entrega en los próximos 3 días o menos:\n");
    for (int i = 0; prestamos[i] != NULL; i++) {
        // Convertir la fecha de entrega del préstamo al formato dd/mm/aaaa
        struct tm fechaEntrega;
        char fechaEntregaStr[11]; // "dd/mm/aaaa" + '\0'

        strcpy(fechaEntregaStr, prestamos[i]->fecha_de_entrega);

        // Extraer el día, el mes y el año
        int dia, mes, anio;
        if (sscanf(fechaEntregaStr, "%d/%d/%d", &dia, &mes, &anio) == 3) {
            fechaEntrega.tm_mday = dia;
            fechaEntrega.tm_mon = mes - 1; // Ajustar el mes a partir de 0
            fechaEntrega.tm_year = anio - 1900; // Restar 1900 al año
            fechaEntrega.tm_hour = 0;
            fechaEntrega.tm_min = 0;
            fechaEntrega.tm_sec = 0;
        } else {
            fprintf(stderr, "Error en el formato de fecha: %s\n", prestamos[i]->fecha_de_entrega);
            continue;
        }

        // Calcular la diferencia de días entre la fecha de entrega y la fecha actual
        time_t tiempoEntrega = mktime(&fechaEntrega);
        double diferenciaDias = difftime(tiempoEntrega, t) / (60 * 60 * 24);

        if (diferenciaDias <= 3) {
            printf("ID Préstamo: %d\n", prestamos[i]->id);
            printf("ID Usuario: %d\n", prestamos[i]->identificadorUsuario);
            printf("Fecha de Entrega: %s\n", prestamos[i]->fecha_de_entrega);
            printf("ID Ejemplar: %d\n", prestamos[i]->idEjemplar);
            printf("\n");
        }
    }

    // Liberar la memoria
    for (int i = 0; prestamos[i] != NULL; i++) {
        free(prestamos[i]);
    }
    free(prestamos);
}

void estadisticas() {
    printf("Generando Estadísticas...\n");
    // Lógica de generación de estadísticas
}
/*
* Retona una lista de los libros
*E: ninguna
*S: Una lista de struct libro
*R: debe existir el archivo datosLibros.json
*/
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
/*
* Cuenta cuantos libros hay en el sistema
*E: ninguna
*S: un entero
*R: ninguna
*/
int obtenerTotalLibros() {
    char* contenido = leerArchivo("./datosLibros.json");
    // Crear un objeto JSON para el libro
    struct json_object *libros = json_tokener_parse(contenido);
    int cantidadLibros = json_object_array_length(libros);
    return cantidadLibros;
}
/*
* Busca un libro segun un criterio
*E: Un texto a buscar
*S: Los libros cuyos atributos contengan el texto indicado
*R: ninguna
*/
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
/*
* Permite realizar la busqueda avanzada And
*E: Los datos a buscar y los criterios de busqueda
*S: Muestra los datos buscados segun el criterio de busqueda
*R: Los datos deben coincidir con los atributos de los struct
*/
void busquedaAvanzadaAuxiliarAnd(char textoBuscarNombre[250],char textoBuscarAutor[250],char textoBuscarGenero[250],char textoBuscarResumen[250],char tecnica[10]){
    int modoBusqueda; // 0 para contenido, 1 para exacta
     if(tecnica == "1" || tecnica == '1' || tecnica == 'e' || tecnica == "e" || tecnica == 'E' || tecnica == "E"){
        modoBusqueda=1;
     }else{
        modoBusqueda=0;
     }
    Libro *libros = obtenerLibros();
    int totalLibros = obtenerTotalLibros();

    for (int i = 0; i < totalLibros; i++) {
        int coincidencia = 0; // Variable para verificar si hubo coincidencia en algún campo

        // Búsqueda por nombre
        if (modoBusqueda == 0 && ((strstr(libros[i].nombre, textoBuscarNombre) != NULL) && (strstr(libros[i].autor, textoBuscarAutor) != NULL))&& (strstr(libros[i].genero, textoBuscarGenero) != NULL)&& (strstr(libros[i].resumen, textoBuscarResumen) != NULL)) {
            coincidencia = 1;
        } else if (modoBusqueda == 1 &&  ((strstr(libros[i].nombre, textoBuscarNombre) == 0) && (strstr(libros[i].autor, textoBuscarAutor) == 0))&& (strstr(libros[i].genero, textoBuscarGenero) == 0)&& (strstr(libros[i].resumen, textoBuscarResumen) == 0)) {
            coincidencia = 1;
        }
        // Si hubo coincidencia en al menos un campo, mostrar la información del libro
        if (coincidencia) {
            printf("\n\n_______________\n");
            printf("Identificador: %d\n", libros[i].identificador);
            printf("Nombre: %s\n", libros[i].nombre);
            printf("Resumen: %s\n", libros[i].resumen);
            printf("Estado: %s\n", libros[i].cantidad > 0 ? "Disponible" : "No disponible");
        }
    }
}
/*
* Permite realizar la busqueda avanzada Or
*E: Los datos a buscar y los criterios de busqueda
*S: Muestra los datos buscados segun el criterio de busqueda
*R: Los datos deben coincidir con los atributos de los struct
*/
void busquedaAvanzadaAuxiliarOr(char textoBuscarNombre[250],char textoBuscarAutor[250],char textoBuscarGenero[250],char textoBuscarResumen[250],char tecnica[10]){
    int modoBusqueda; // 0 para contenido, 1 para exacta
     if(tecnica == "1" || tecnica == '1' || tecnica == 'e' || tecnica == "e" || tecnica == 'E' || tecnica == "E"){
        modoBusqueda=1;
     }else{
        modoBusqueda=0;
     }
    Libro *libros = obtenerLibros();
    int totalLibros = obtenerTotalLibros();

    for (int i = 0; i < totalLibros; i++) {
        int coincidencia = 0; // Variable para verificar si hubo coincidencia en algún campo

        // Búsqueda por nombre
        if (modoBusqueda == 0 && strstr(libros[i].nombre, textoBuscarNombre) != NULL) {
            coincidencia = 1;
        } else if (modoBusqueda == 1 && strcmp(libros[i].nombre, textoBuscarNombre) == 0) {
            coincidencia = 1;
        }

        // Búsqueda por autor
        if (modoBusqueda == 0 && strstr(libros[i].autor, textoBuscarAutor) != NULL) {
            coincidencia = 1;
        } else if (modoBusqueda == 1 && strcmp(libros[i].autor, textoBuscarAutor) == 0) {
            coincidencia = 1;
        }

        // Búsqueda por género
        if (modoBusqueda == 0 && strstr(libros[i].genero, textoBuscarGenero) != NULL) {
            coincidencia = 1;
        } else if (modoBusqueda == 1 && strcmp(libros[i].genero, textoBuscarGenero) == 0) {
            coincidencia = 1;
        }

        // Búsqueda por resumen
        if (modoBusqueda == 0 && strstr(libros[i].resumen, textoBuscarResumen) != NULL) {
            coincidencia = 1;
        } else if (modoBusqueda == 1 && strcmp(libros[i].resumen, textoBuscarResumen) == 0) {
            coincidencia = 1;
        }

        // Si hubo coincidencia en al menos un campo, mostrar la información del libro
        if (coincidencia) {
            printf("\n\n_______________\n");
            printf("Identificador: %d\n", libros[i].identificador);
            printf("Nombre: %s\n", libros[i].nombre);
            printf("Resumen: %s\n", libros[i].resumen);
            printf("Estado: %s\n", libros[i].cantidad > 0 ? "Disponible" : "No disponible");
        }
    }
}
/*
* Permite realizar la busqueda avanzada
*E: Los datos a buscar y los criterios de busqueda
*S: Muestra los datos buscados segun el criterio de busqueda
*R: Los datos deben coincidir con los atributos de los struct
*/
void busquedaAvanzada() {
    printf("Realizando Búsqueda Avanzada...\n");
    // Lógica de búsqueda avanzada

    char textoBuscarNombre[250];
    char textoBuscarAutor[250];
    char textoBuscarGenero[250];
    char textoBuscarResumen[250];

    char tecnica[10];
    char andOr[10];

    printf("Ingrese el texto del nombre del libro a buscar....Un espacio en blanco significa que no buscara con este criterio: ");
    scanf(" %[^\n]s", textoBuscarNombre);

    printf("Ingrese el texto del autor del libro a buscar....Un espacio en blanco significa que no buscara con este criterio: ");
    scanf(" %[^\n]s", textoBuscarAutor);

    printf("Ingrese el texto del genero del libro a buscar...Un espacio en blanco significa que no buscara con este criterio: ");
    scanf(" %[^\n]s", textoBuscarGenero);

    printf("Ingrese el texto del resumen del libro a buscar....Un espacio en blanco significa que no buscara con este criterio: ");
    scanf(" %[^\n]s", textoBuscarResumen);

    printf("¿Desea usar el metodo de coincidencia contenida o exacta?....Introduzca 1 o E para exacta, cualquier otro argumento sera para contenido: ");
    scanf(" %[^\n]s", tecnica);

    printf("¿Desea usar el metodo de busqueda (Y) u (O)?....Introduzca Y para Y (and), cualquier otro argumento sera para o (or): ");
    scanf(" %[^\n]s", andOr);

    if (andOr == "Y" || andOr == "y" || andOr == 'y' || andOr == 'Y'){
        printf("Realizando busqueda And");
        busquedaAvanzadaAuxiliarAnd(textoBuscarNombre,textoBuscarAutor,textoBuscarGenero,textoBuscarResumen,tecnica);
    }else{
        printf("Realizando busqueda Or");
        busquedaAvanzadaAuxiliarOr(textoBuscarNombre,textoBuscarAutor,textoBuscarGenero,textoBuscarResumen,tecnica);
    }    
}
/*
* Presmite realizar prestamos de ejemplares
*E: Los datos del ejemplar y usuario, las fechas
*S: Se realiza el prestamo
*R: Las fechas deben ser validas
*/
void prestamoEjemplar() {
    printf("Realizando Préstamo de Ejemplar...\n");

    struct Prestamo *prestamo;
    struct Libro *libros;
    struct Usuario *usuarios;

    // Valida la identificación del usuario
    char* identificacion = malloc(20*sizeof(char));
    printf("Ingrese la identificación del usuario: ");
    scanf("%s", identificacion);

    // Valida el identificador del libro
    char id_Ejemplar[50];
    printf("Ingrese el identificador ejemplar: ");
    scanf("%s", id_Ejemplar);

    // Valida la fecha de préstamo
    char* fecha_prestamo = malloc(50*sizeof(char));
    printf("Ingrese la fecha de préstamo (dd/mm/aaaa): ");
    scanf("%s", fecha_prestamo);

    // Valida la fecha de devolución
    char *fecha_devolucion = malloc(50*sizeof(char));
    printf("Ingrese la fecha de devolución (dd/mm/aaaa): ");
    scanf("%s", fecha_devolucion);

    // Busca el usuario
    Usuario *usuario = buscarUsuario(identificacion);

    // Si el usuario no se encontró
    if (usuario == NULL) {
        printf("No se encontró el usuario.\n");
        return;
    }

    // Busca el libro
    Libro *libro = buscarLibro(id_Ejemplar);

    // Si el libro no se encontró
    if (libro == NULL) {
        printf("No se encontró el libro.\n");
        return;
    }

    //realizar el prestamo
    
    //leer prestamos y guardar en lista de prestamos
    FILE *archivo;
    char buffer[5000];

    struct json_object *parsed_json;
    struct json_object *obj;

    struct json_object *id;
    struct json_object *identificadorUsuario;
    struct json_object *estado;
    struct json_object *idEjemplar;
    struct json_object *fechaPrestamo;
    struct json_object *fechaDevolucion;
    struct json_object *fecha_entrega;
    struct json_object *tardia;
    size_t n,i;
    archivo = fopen("./prestamos.json", "r"); 
    fread(buffer, 5000, 1, archivo);
    fclose(archivo);

    parsed_json = json_tokener_parse(buffer);
    n = json_object_array_length(parsed_json);

    Prestamo* prestamos[n+1];
    for(int x = 0; x<(n+1); x++){
        prestamos[x] = malloc(sizeof(Prestamo));
    }
    for(i = 0; i < n+1; i++){
        if(i == n){
            Prestamo* el_prestamo = malloc(sizeof(Prestamo));
            el_prestamo->id = n+1;
            el_prestamo->identificadorUsuario = atoi(identificacion);
            el_prestamo->estado = 1;
            el_prestamo->idEjemplar = atoi(id_Ejemplar);
            strcpy(el_prestamo->fecha_prestamo, fecha_prestamo);
            strcpy(el_prestamo->fecha_devolucion, fecha_devolucion);
            strcpy(el_prestamo->fecha_de_entrega, " ");
            el_prestamo->tardia = 0;
            prestamos[i] = el_prestamo;
            printf("guardado %d\n", prestamos[i]->id);
            break;
        }

        obj = json_object_array_get_idx(parsed_json, i);
        json_object_object_get_ex(obj, "id", &id); 
        
        json_object_object_get_ex(obj, "identificadorUsuario", &identificadorUsuario); 
        json_object_object_get_ex(obj, "estado", &estado); 
        json_object_object_get_ex(obj, "idEjemplar", &idEjemplar); 
        json_object_object_get_ex(obj, "fecha_prestamo", &fechaPrestamo); 
        json_object_object_get_ex(obj, "fecha_devolucion", &fechaDevolucion); 
        json_object_object_get_ex(obj, "fecha_entrega", &fecha_entrega); 
        json_object_object_get_ex(obj, "tardia", &tardia); 
        
        prestamos[i]->id = json_object_get_int(id);
        prestamos[i]->identificadorUsuario = json_object_get_int(identificadorUsuario);
        prestamos[i]->estado = json_object_get_int(estado);
        prestamos[i]->idEjemplar = json_object_get_int(idEjemplar);
        strcpy(prestamos[i]->fecha_prestamo, json_object_get_string(fechaPrestamo));
        strcpy(prestamos[i]->fecha_devolucion, json_object_get_string(fechaDevolucion));
        strcpy(prestamos[i]->fecha_de_entrega, json_object_get_string(fecha_entrega));
        prestamos[i]->tardia = json_object_get_int(tardia);
        
    }
    
    //volver a guardar los prestamos en el json
    struct json_object *json_array = json_object_new_array();

    for(int i = 0; i < n+1; i++){
        struct json_object *obj = json_object_new_object();
        json_object_object_add(obj, "id", json_object_new_int(prestamos[i]->id)); 
        json_object_object_add(obj, "identificadorUsuario", json_object_new_int(prestamos[i]->identificadorUsuario));
        json_object_object_add(obj, "estado", json_object_new_int(prestamos[i]->estado));
        json_object_object_add(obj, "idEjemplar", json_object_new_int(prestamos[i]->idEjemplar));
        json_object_object_add(obj, "fecha_prestamo", json_object_new_string(prestamos[i]->fecha_prestamo));
        json_object_object_add(obj, "fecha_devolucion", json_object_new_string(prestamos[i]->fecha_devolucion));
        json_object_object_add(obj, "fecha_entrega", json_object_new_string(prestamos[i]->fecha_de_entrega));
        json_object_object_add(obj, "tardia", json_object_new_int(prestamos[i]->tardia));

        json_object_array_add(json_array, obj);
   
    }
    
    const char *json_str = json_object_to_json_string(json_array);

    FILE *file = fopen("./prestamos.json", "w");
    if (file) {
        fprintf(file, "%s", json_str);
        fclose(file);
    }

    // Imprime un mensaje de confirmación
    printf("El ejemplar se ha prestado correctamente.\n");
    printf("El usuario es %s.\n", identificacion);
    printf("La fecha de préstamo es %s.\n", fecha_prestamo);
    printf("La fecha de devolución es %s.\n", fecha_devolucion);
    printf("El id del ejemplar es %s.\n", id_Ejemplar);
}
