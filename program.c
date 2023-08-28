#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <json-c/json.h> INSTALAR LA LIBRERÍA EN EL PROYECTO PARA QUE FUNCIONE


// Declaración de funciones
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
void devolucionesEjemplar();
void IngresarLibro();

// Estructura de Libro 
struct  {
    char nombre[50];
    char autor[50];
    int anioPublicacion;
    char genero[50];
    char resumen[50];
    int cantidad;
} Libro;

// Estructura de Usuario
struct  {
    char identificacion[50];
    char nombre[50];
    char direccion[50];
} Usuario;

//Estructura de Prestamo
struct  {
    char identificador[50];
    char usuario[50]; //identificación del usuario
    char estado[50];
    char nombre[50];
    char idEjemplar[50]; //identificador del libro
    int tardia; //true para tardía
} Prestamo;



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
    printf("Gestionando Catálogo...\n"); 
    // Lógica de gestión de catálogo
    int opcion;
    do {
        printf("\n*************************************\n");
        printf("\n*** Gestión de Catálogo ***\n");
        printf("1. Ingresar Libro\n");
        printf("2. Modificar Libro\n");
        printf("3. Eliminar Libro\n");
        printf("4. Volver\n");
        printf("Ingrese su opción: ");
        scanf("%d", &opcion);
        
        switch (opcion) {
            case 1:
                IngresarLibro("datosLibros.json");
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

void IngresarLibro(char* rutaArchivo){
    FILE *archivo; //Puntero de tipo FILE
    struct json_object *obj;

    archivo = fopen(rutaArchivo, "a");
    if(archivo == NULL){
        printf("Error al abrir el archivo");
    }
    else{
        obj = json_object_new_object();

        //Ingresar datos
        printf("Ingrese el nombre del libro: ");
        scanf("%s", Libro.nombre);
        printf("Ingrese el nombre del autor: ");
        scanf("%s", Libro.autor);
        printf("Ingrese el año de publicación: ");
        scanf("%d", &Libro.anioPublicacion);
        printf("Ingrese el género del libro: ");
        scanf("%s", Libro.genero);
        printf("Ingrese el resumen del libro: ");
        scanf("%s", Libro.resumen);
        printf("Ingrese la cantidad de libros: ");
        scanf("%d", &Libro.cantidad);

        //Guardar datos en el archivo
        json_object_put_string(obj, "nombre", Libro.nombre);
        json_object_put_string(obj, "autor", Libro.autor);
        json_object_put_int(obj, "anioPublicacion", Libro.anioPublicacion);
        json_object_put_string(obj, "genero", Libro.genero);
        json_object_put_string(obj, "resumen", Libro.resumen);
        json_object_put_int(obj, "cantidad", Libro.cantidad);

        //limpiar datos
        fprintf(archivo, "%s\n", json_object_to_json_string(obj)); //Escribir en el archivo
        json_object_put_string(obj, "nombre", NULL); //Limpiar el objeto
        json_object_put_string(obj, "autor", NULL); //Limpiar el objeto
        json_object_put_int(obj, "anioPublicacion", 0); //Limpiar el objeto
        json_object_put_string(obj, "genero", NULL); //Limpiar el objeto
        json_object_put_string(obj, "resumen", NULL);   //Limpiar el objeto 
        json_object_put_int(obj, "cantidad", 0); //Limpiar el objeto

        json_object_put(archivo, "libros", obj); //Limpiar el objeto
        printf("Datos ingresados correctamente"); //Limpiar el objeto
    }
    fclose(archivo);
}

void IngresarUsuario(char* rutaArchivo){
    FILE *archivo; //Puntero de tipo FILE
    struct json_object *obj;

    archivo = fopen(rutaArchivo, "a");
    if(archivo == NULL){
        printf("Error al abrir el archivo");
    }
    else{
        obj = json_object_new_object();

        //Ingresar datos
        printf("Ingrese la identificación del usuario: ");
        scanf("%s", Usuario.identificacion);
        printf("Ingrese el nombre del usuario: ");
        scanf("%s", Usuario.nombre);
        printf("Ingrese la dirección del usuario: ");
        scanf("%s", Usuario.direccion);
        
        //guardar datos en el archivo
        json_object_put_string(obj, "identificacion", Usuario.identificacion);
        json_object_put_string(obj, "nombre", Usuario.nombre);
        json_object_put_string(obj, "direccion", Usuario.direccion);

        //limpiar datos
        fprintf(archivo, "%s\n", json_object_to_json_string(obj)); //Escribir en el archivo
        json_object_put_string(obj, "identificacion", NULL); //Limpiar el objeto
        json_object_put_string(obj, "nombre", NULL); //Limpiar el objeto
        json_object_put_string(obj, "direccion", NULL); //Limpiar el objeto
        
        json_object_put(archivo, "usuarios", obj); //Limpiar el objeto
        printf("Datos ingresados correctamente"); //Limpiar el objeto
    }
    fclose(archivo);
}

void showHistorial(){
    FILE *archivo; //Puntero de tipo FILE
    struct json_object *obj;
    char inicio[50];
    char fin[50];
    char fecha[50];
    int i = 0;

    archivo = fopen("datosPrestamos.json", "r"); //Abrir el archivo en modo lectura 
    if(archivo == NULL){
        printf("Error al abrir el archivo");
    }
    else{
        obj = json_object_new_object();

        //Ingresar datos
        printf("Ingrese la fecha de inicio: ");
        scanf("%s", inicio);
        printf("Ingrese la fecha de fin: ");
        scanf("%s", fin);

        //leer datos del archivo e imprimirlos en pantalla y si no hay decir que no hay
        while(fgets(fecha, 50, archivo) != NULL){
            if(strcmp(fecha, inicio) >= 0 && strcmp(fecha, fin) <= 0){ //comparar fechas
                printf("%s", fecha);
                i++;
            }
        }
        if(i == 0){
            printf("No hay datos en el rango de fechas");
        }
    }
    fclose(archivo);
}



void gestionUsuarios() {
    printf("Realizando Gestión de Usuarios...\n");
    // Lógica de gestión de usuarios
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

void busquedaSimple() {
    printf("Realizando Búsqueda Simple...\n");
    // Lógica de búsqueda simple
}

void busquedaAvanzada() {
    printf("Realizando Búsqueda Avanzada...\n");
    // Lógica de búsqueda avanzada
}

void prestamoEjemplar() {
    printf("Realizando Préstamo de Ejemplar...\n");
    // Lógica de préstamo de ejemplar
}

void devolucionesEjemplar() {
    printf("Realizando Devolución de Ejemplar...\n");
    // Lógica de devolución de ejemplar
}
