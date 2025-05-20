#include <stdio.h>
#include <string.h>
#include "funciones.h"

int menu() {
    int opc;
    printf("\nSeleccione una opción\n");
    printf("1. Ingreso de componentes/material\n");
    printf("2. Ingreso de productos\n");
    printf("3. Realizar pedidos\n");
    printf("4. Editar/Borrar Producto\n");
    printf("5. Editar/Borrar componentes\n");
    printf ("6. Reabastecer componente\n");
    printf ("7. Salir\n");
    printf(">> ");
    opc = validar_cantidad(opc);
    while (getchar() != '\n'); // limpiar el buffer
    return opc;
}

void ing_recursos(int *contcomp, char component[8][50], int *confirm, int cant[5][8], int cantidad[8]) {
    if (*contcomp >= 8) {
        printf("Límite de componentes alcanzado.\n");
        return;
    }

    char nombre[50];
    int repetido;
    do {
        repetido = 0;
        printf("Ingrese el nombre del nuevo componente: ");
        fgets(nombre, 50, stdin);
        nombre[strcspn(nombre, "\n")] = '\0';

        for (int i = 0; i < *contcomp; i++) {
            if (strcmp(nombre, component[i]) == 0) {
                printf("Ese nombre de componente ya existe. Ingrese uno diferente.\n");
                repetido = 1;
                break;
            }
        }
    } while (repetido);

    strcpy(component[*contcomp], nombre);

    printf("Ingrese la cantidad de %s: ", component[*contcomp]);
    do {
        cantidad[*contcomp] = validar_cantidad(cantidad[*contcomp]);
        while (getchar() != '\n');
        if (cantidad[*contcomp] <= 0) {
            printf("La cantidad no puede ser negativa o cero. Por favor, ingrese una cantidad válida.\n");
        }
    } while (cantidad[*contcomp] <= 0);

    (*contcomp)++;
    (confirm)++;
}

void ing_Productos(int *contprod, int contcomp, char producto[5][50], char component[8][50], int cant[5][8], float tiempo[5], int confirm2) {
    if (*contprod >= 5) {
        printf("Límite de productos alcanzado (5).\n");
        return;
    }
    if (contcomp == 0) {
        printf("No hay productos registrados para agregar un producto.\n");
        return;
    }
    char nombre[50];
    int repetido;
    do {
        repetido = 0;
        printf("Ingrese el nombre del producto %d: ", *contprod + 1);
        fgets(nombre, 50, stdin);
        nombre[strcspn(nombre, "\n")] = '\0';

        for (int i = 0; i < *contprod; i++) {
            if (strcmp(nombre, producto[i]) == 0) {
                printf("Ese nombre de producto ya existe. Ingrese uno diferente.\n");
                repetido = 1;
                break;
            }
        }
    } while (repetido);

    strcpy(producto[*contprod], nombre);

    printf("Ingrese la cantidad necesaria de cada componente para producir un solo %s:\n", producto[*contprod]);
    for (int i = 0; i < contcomp; i++) {
        do {
            printf("%s: ", component[i]);
            cant[*contprod][i] = validar_cantidad(cant[*contprod][i]);
            while (getchar() != '\n');
            if (cant[*contprod][i] < 0) {
                printf("La cantidad no puede ser negativa. Por favor, ingrese una cantidad válida.\n");
            }
        } while (cant[*contprod][i] < 0);
    }

    do {
        printf("Ingrese el tiempo necesario para producir un %s (en minutos): ", producto[*contprod]);
        tiempo[*contprod] = validar_cantidad(tiempo[*contprod]);
        while (getchar() != '\n');
        if (tiempo[*contprod] <= 0) {
            printf("El tiempo no puede ser negativo o cero. Por favor, ingrese un tiempo válido.\n");
        }
    } while (tiempo[*contprod] <= 0);

    (*contprod)++;
    (confirm2)++;
}
void pedido(int *contprod, char producto[5][50], int cant[5][8], float tiempo[5], char component[8][50], int cantidad[8], int *contcomp) {
    int opclist;
    float tiempo_total;
    int cantidades=0;
    if (*contprod == 0) {
        printf("No hay productos registrados para realizar un pedido.\n");
        return;
    }

    printf("\nSeleccione el producto que desea pedir:\n\n");
    printf("%-5s | %-25s\n", "N°", "Producto");
    printf("------------------------------------------\n");

    for (int i = 0; i < *contprod; i++) {
        printf("%-5d | %-25s\n", i, producto[i]);
    }

    do {
        printf("\nIngrese el número del producto: ");
        opclist = validar_cantidad(opclist);

        if (opclist < 0 || opclist >= *contprod) {
            printf("Opcion invalida, por favor seleccione un numero valido de la lista.\n");
        }
    } while (opclist < 0 || opclist >= *contprod);

    printf("¿Cuantos %s desea pedir? ", producto[opclist]);
    int cantidad_pedida;
    do{
cantidad_pedida = validar_cantidad(cantidad_pedida);
    while (getchar() != '\n'); // limpiar buffer
    if (cantidad_pedida <= 0) {
        printf("La cantidad no puede ser negativa o cero. Por favor, ingrese una cantidad válida.\n");
    }
    }while (cantidad_pedida <= 0);
    

    // Verificación de recursos
    for (int i = 0; i < *contcomp; i++) {
        int necesario = cant[opclist][i] * cantidad_pedida;
        if (necesario > cantidad[i]) {
            printf(" - Recurso insuficiente: '%s' | Necesario: %d | Disponible: %d\n", component[i], necesario, cantidad[i]);
            cantidades++;
        }   
    }
    if (cantidades > 0) {
        printf("No se puede cumplir el pedido por falta de recursos.\n");
        return;
    }

    float tiempo_pedido;
    do{
        printf("¿En qué tiempo desea recibir el pedido el cliente? (en minutos): ");
        tiempo_pedido = validar_cantidad_float(tiempo_pedido);
        if (tiempo_pedido <= 0) {
            printf("El tiempo no puede ser negativo o cero. Por favor, ingrese un tiempo válido.\n");
        }
    } while (tiempo_pedido <= 0);
    tiempo_total = tiempo[opclist] * cantidad_pedida;
    float falta;
    falta= tiempo_total - tiempo_pedido;

    if (tiempo_total > tiempo_pedido) {
        printf("No se puede cumplir el pedido en el tiempo solicitado, faltan %.2f minutos.\n", falta);
        return;
    }
    // Descontar recursos usados
    for (int i = 0; i < 8; i++) {
        cantidad[i] -= cant[opclist][i] * cantidad_pedida;
    }

    printf("Pedido de %d %s confirmado. Tiempo estimado de entrega: %.2f minutos.\n", cantidad_pedida, producto[opclist], tiempo_total);
}
void edit_prod(int *contprod, char producto[5][50], int cantstck[5], char component[8][50], int cant[5][8], float tiempo[5], int contcomp, int confirm2) {
    int pos, opc;
    if (*contprod == 0) {
        printf("No hay productos registrados.\n");
        return;
    }
    printf("Productos:\n");
    for (int i = 0; i < *contprod; i++) {
        printf("%d. %s\n", i + 1, producto[i]);
    }

    printf("Seleccione un producto: ");
    pos = validar_cantidad(pos);
    pos--;

    if (pos < 0 || pos >= *contprod) {
        printf("Opción inválida.\n");
        return;
    }

    printf("1. Editar producto\n2. Eliminar producto\n>> ");
    opc = validar_cantidad(opc);
    while (getchar() != '\n');

    if (opc == 1) {
        char nuevo_nombre[50];
        int repetido;
        do {
            repetido = 0;
            printf("Nuevo nombre: ");
            fgets(nuevo_nombre, 50, stdin);
            nuevo_nombre[strcspn(nuevo_nombre, "\n")] = '\0';
            for (int i = 0; i < *contprod; i++) {
                if (i != pos && strcmp(nuevo_nombre, producto[i]) == 0) {
                    printf("Ese nombre ya está registrado. Ingrese otro distinto.\n");
                    repetido = 1;
                    break;
                }
            }
        } while (repetido);
        strcpy(producto[pos], nuevo_nombre);

        printf("Nueva cantidad de cada componente:\n");
        for (int i = 0; i < contcomp; i++) {
            printf("%s: ", component[i]);
            do {
                cant[pos][i] = validar_cantidad(cant[pos][i]);
                if (cant[pos][i] < 0) {
                    printf("La cantidad no puede ser negativa. Por favor, ingrese una cantidad válida.\n");
                }
            } while (cant[pos][i] < 0);
        }

        do {
            printf("Nuevo tiempo de producción (en minutos): ");
            tiempo[pos] = validar_cantidad(tiempo[pos]);
            while (getchar() != '\n');
            if (tiempo[pos] <= 0) {
                printf("El tiempo no puede ser negativo o cero. Por favor, ingrese un tiempo válido.\n");
            }
        } while (tiempo[pos] <= 0);

        printf("Producto editado con éxito.\n");
    } else if (opc == 2) {
        for (int i = pos; i < *contprod - 1; i++) {
            strcpy(producto[i], producto[i + 1]);
            for (int j = 0; j < 8; j++) {
                cant[i][j] = cant[i + 1][j];
            }
            tiempo[i] = tiempo[i + 1];
        }
        (*contprod)--;
        (confirm2)--;
        printf("Producto eliminado.\n");
    } else {
        printf("Opción inválida.\n");
    }
}


void edit_recursos(int *contcomp, char component[8][50], int cant[5][8], int cantidad[8], int confirm) {
    int pos, opc;
    if (*contcomp == 0) {
        printf("No hay componentes registrados.\n");
        return;
    }

    printf("Componentes:\n");
    for (int i = 0; i < *contcomp; i++) {
        printf("%d. %s\n", i + 1, component[i]);
    }

    printf("Seleccione un componente: ");
    scanf("%d", &pos);
    pos--;

    if (pos < 0 || pos >= *contcomp) {
        printf("Opción inválida.\n");
        return;
    }

    printf("1. Editar componente\n2. Eliminar componente\n>> ");
    scanf("%d", &opc);
    while (getchar() != '\n');

    if (opc == 1) {
        char nuevo_nombre[50];
        int repetido;
        do {
            repetido = 0;
            printf("Nuevo nombre: ");
            fgets(nuevo_nombre, 50, stdin);
            nuevo_nombre[strcspn(nuevo_nombre, "\n")] = '\0';
            for (int i = 0; i < *contcomp; i++) {
                if (i != pos && strcmp(nuevo_nombre, component[i]) == 0) {
                    printf("Ese nombre ya está registrado. Ingrese otro distinto.\n");
                    repetido = 1;
                    break;
                }
            }
        } while (repetido);
        strcpy(component[pos], nuevo_nombre);

        do {
            printf("Nueva cantidad: ");
            cantidad[pos] = validar_cantidad(cantidad[pos]);
            if (cantidad[pos] <= 0) {
                printf("La cantidad no puede ser negativa o cero. Por favor, ingrese una cantidad válida.\n");
            }
        } while (cantidad[pos] <= 0);

        printf("Componente editado.\n");
    } else if (opc == 2) {
        for (int i = pos; i < *contcomp - 1; i++) {
            strcpy(component[i], component[i + 1]);
        }
        (*contcomp)--;
        (confirm)--;
        printf("Componente eliminado.\n");
    } else {
        printf("Opcion invalida.\n");
    }
}

int validar_cantidad(int num)
{
    int validacion;
    validacion = scanf("%d", &num);
    while (validacion != 1)
    {
        printf("Entrada invalida. Por favor, ingrese un numero entero positivo: ");
        while (getchar() != '\n');
        validacion = scanf("%d", &num);
    }
    return num;
}

float validar_cantidad_float(float num)
{
    int validacion;
    validacion = scanf("%f", &num);
    while (validacion != 1)
    {
        printf("Entrada invalida. Por favor, ingrese un numero: ");
        while (getchar() != '\n');
        validacion = scanf("%f", &num);
    }
    return num;
}

void reabastecer_componentes(int *contcomp, char component[8][50], int cantidad[8]) {
    if (*contcomp == 0) {
        printf("No hay componentes registrados para reabastecer.\n");
        return;
    }

    printf("Componentes disponibles:\n");
    for (int i = 0; i < *contcomp; i++) {
        printf("%d. %s (Cantidad actual: %d)\n", i + 1, component[i], cantidad[i]);
    }

    int seleccion;
    printf("Seleccione el número del componente que desea reabastecer: ");
    seleccion = validar_cantidad(seleccion);
    while (seleccion < 1 || seleccion > *contcomp) {
        printf("Selección inválida. Intente nuevamente: ");
        seleccion = validar_cantidad(seleccion);
    }
    seleccion--; // Convertimos a índice de array

    int adicional;
    printf("Ingrese la cantidad a agregar al componente '%s': ", component[seleccion]);
    do {
        adicional = validar_cantidad(adicional);
        if (adicional <= 0) {
            printf("La cantidad debe ser positiva. Intente nuevamente: ");
        }
    } while (adicional <= 0);

    cantidad[seleccion] += adicional;
    printf("Componente '%s' reabastecido. Nueva cantidad: %d\n", component[seleccion], cantidad[seleccion]);
}