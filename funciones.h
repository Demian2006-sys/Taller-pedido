void ing_recursos(int *contcomp, char component[8][50], int *confirm, int cant[5][8], int cantidad[8]);
void ing_Productos(int *contprod, int contcomp, char producto[5][50], char component[8][50], int cant[5][8], float tiempo[5], int confirm2);
void pedido(int *contprod, char producto[5][50], int cant[5][8], float tiempo[5], char component[8][50], int cantidad[8], int *contcomp);
void edit_prod(int *contprod, char producto[5][50], int cantstck[5], char component[8][50], int cant[5][8], float tiempo[5], int contcomp, int confirm2);
void edit_recursos(int *contcomp, char component[8][50], int cant[5][8], int cantidad[8], int confirm);
int menu();
int validar_cantidad(int num);
float validar_cantidad_float(float num);
void reabastecer_componentes(int *contcomp, char component[8][50], int cantidad[8]);