#include <stdio.h>
#include <string.h>
#include "funciones.h"
int main(int argc, char *argv[]) {
    int contprod = 0;
    int contcomp = 0;
    int confirm = 0;
    int confirm2 = 0;
    char producto[5][50];
    char component[8][50];
    int cant[5][8];
    float tiempo[5];
    int cantstck[5];
    int cantidad[8];
    int opc;

    do {
        opc = menu();
        switch (opc) {
            case 1:
                ing_recursos(&contcomp, component, &confirm, cant, cantidad);
                break;
            case 2:
                ing_Productos(&contprod, contcomp, producto, component, cant, tiempo,confirm2);
                break;
            case 3:
                pedido(&contprod, producto, cant, tiempo, component, cantidad, &contcomp);
                break;
            case 4:
                edit_prod(&contprod, producto, cantstck, component, cant, tiempo, contcomp, confirm2);
                break;
            case 5:
                edit_recursos(&contcomp, component, cant, cantidad, confirm);
                break;
            case 6:
            reabastecer_componentes(&contcomp, component, cantidad);
            break;
            case 7:
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
                break;
        }
    } while (opc != 7);

    return 0;
}