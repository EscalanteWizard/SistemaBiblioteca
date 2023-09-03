#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h> //INSTALAR LA LIBRERÍA EN EL PROYECTO PARA QUE FUNCIONE


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
    FILE *archivo;
    archivo = fopen(rutaArchivo,"a");
    if(archivo == NULL){
        printf("Error al abrir el archivo");
    }
    else{
        printf("Ingrese el nombre del libro: ");
        scanf("%s", Libro.nombre);
        printf("Ingrese el autor del libro: ");
        scanf("%s", Libro.autor);
        printf("Ingrese el año de publicación del libro: ");
        scanf("%d", &Libro.anioPublicacion);
        printf("Ingrese el género del libro: ");
        scanf("%s", Libro.genero);
        printf("Ingrese el resumen del libro: ");
        scanf("%s", Libro.resumen);
        printf("Ingrese la cantidad de ejemplares del libro: ");
        scanf("%d", &Libro.cantidad);
        fprintf(archivo, "%s %s %d %s %s %d\n", Libro.nombre, Libro.autor, Libro.anioPublicacion, Libro.genero, Libro.resumen, Libro.cantidad);
        fclose(archivo);
    }


}

void IngresarUsuario(char* rutaArchivo){
    FILE *archivo;
    archivo = fopen(rutaArchivo,"a");
    if(archivo == NULL){
        printf("Error al abrir el archivo");
    }
    else{
        printf("Ingrese el nombre del usuario: ");
        scanf("%s", Usuario.nombre);
        printf("Ingrese la identificación del usuario: ");
        scanf("%s", Usuario.identificacion);
        printf("Ingrese la dirección del usuario: ");
        scanf("%s", Usuario.direccion);
        fprintf(archivo, "%s %s %s\n", Usuario.nombre, Usuario.identificacion, Usuario.direccion);
        fclose(archivo);
    }
 
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
    struct Libro *libros;
    int n = 0;
    char *texto;

    printf("Ingrese el texto de búsqueda: ");
    scanf("%s", texto);

    

    // Valida el texto de búsqueda
    if (texto == NULL || strlen(texto) == 0) {
      printf("Debe ingresar un texto de búsqueda válido.\n");
      return;
    }

    // Busca el libro
    int i = buscar_libro(libros, n, texto);

    // Si el libro no se encontró
    if (i == -1) {
      printf("No se encontró el libro.\n");
      return;
    }

    // Imprime el libro
    imprimir_libro(&libros[i]);
}

// Busca un préstamo por su ID
int buscar_prestamo(struct Prestamo *prestamos, int n, int id_prestamo) {
  int i;

  // Recorre los préstamos
  for (i = 0; i < n; i++) {
    // Si el ID del préstamo coincide
    if (prestamos[i].id == id_prestamo) {
      // Retorna el índice del préstamo
      return i;
    }
  }

  // Si no se encontró el préstamo
  return -1;
}

void devolucionesEjemplar(int id_prestamo, char *fecha_devolucion) {

  struct Prestamo *prestamos;
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
  prestamos[i].fecha_devolucion = fecha_devolucion;

  // Calcula el monto asociado al préstamo
  int dias = (difftime(fecha_devolucion, prestamos[i].fecha_prestamo) / 86400);
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
  prestamos[i].estado = "Devuelto";

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

void devolucionesEjemplar() {
    printf("Realizando Devolución de Ejemplar...\n");
    // Lógica de devolución de ejemplar
}
