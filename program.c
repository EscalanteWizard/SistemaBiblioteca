#include <stdio.h>

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

int main() {
    int opcion;
    
    do {
	printf("\n*************************************\n");
        printf("\n*** Menú Principal ***\n");
        printf("1. Opciones Operativas\n");
        printf("2. Opciones Generales\n");
        printf("3. Salir\n");
        printf("Ingrese su opción: ");
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
        printf("6. Volver\n");
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
    printf("Realizando Gestión de Catálogo...\n");
    // Lógica de gestión de catálogo
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
