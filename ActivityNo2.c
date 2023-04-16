#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PARTICIONES 100
#define MAX_PROCESOS 100

#define FIRST_FIT 1
#define BEST_FIT 2
#define WORST_FIT 3

int particiones[MAX_PARTICIONES]; // arreglo que almacena el tamano de cada particion
int procesos[MAX_PROCESOS]; // arreglo que almacena el tamano de cada proceso

int num_particiones; // numero de particiones disponibles
int num_procesos; // numero de procesos a asignar

/*
 La función "asignar" busca una particion adecuada para el proceso y la asigna.
 Recibe como parametros el tamano del proceso a asignar y el metodo de asignación a utilizar.
 Devuelve el indice de la partición asignada, o -1 si no hay espacio disponible.
 */
int asignar(int tam_proceso, int metodo){
    int i, j;
    int tam_particion;
    int indice_particion = -1;

    switch (metodo){
        case FIRST_FIT:
            // Buscar la primera particion que tenga suficiente espacio para el proceso
            for (i = 0; i < num_particiones; i++){
                if (tam_proceso <= particiones[i]){
                    indice_particion = i;
                    break;
                }
            
            break;

        case BEST_FIT:
            tam_particion = -1;
            // Buscar la particion mas pequena que tenga suficiente espacio para el proceso
            for (i = 0; i < num_particiones; i++){
                if (particiones[i] >= tam_proceso){
                    if (tam_particion == -1 || particiones[i] < tam_particion){
                        indice_particion = i;
                        tam_particion = particiones[i];
                    }
                }
            }
            break;

        case WORST_FIT:
            tam_particion = -1;
            // Buscar la particion mas grande que tenga suficiente espacio para el proceso
            for (i = 0; i < num_particiones; i++){
                if (particiones[i] >= tam_proceso){
                    if (tam_particion == -1 || particiones[i] > tam_particion){
                        indice_particion = i;
                        tam_particion = particiones[i];
                    }
                }
            }
            break;
    }

    if (indice_particion == -1){
        return -1; // no hay espacio disponible
    }

    // Asignar el proceso a la particion
    particiones[indice_particion] -= tam_proceso;
    return indice_particion;
}

/*

  La funcion "imprimir_particiones" imprime en pantalla el tamano de cada particion.

*/
void imprimir_particiones() {
    int i;
    for (i = 0; i < num_particiones; i++){
        printf("tamano de particion %d, es %d\n", i, particiones[i]);
    }
    printf("\n");
}

int main(){
    int i, j;
    int metodo;
    int tam_proceso;
    int total_particiones = 0;
    scanf("%d", &metodo);

    // Seleccionar el método de asignación de memoria
    int metodo_asignacion;
    switch (metodo){
        case 1:
            metodo_asignacion = FIRST_FIT;
            break;
        case 2:
            metodo_asignacion = BEST_FIT;
            break;
        case 3:
            metodo_asignacion = WORST_FIT;
            break;
        default:
            printf("Metodo invalido\n");
            return 1;
    }

    // Leer el número de particiones disponibles
    scanf("%d", &num_particiones);

    // Leer las particiones disponibles
    for (i = 0; i < num_particiones; i++){
        scanf("%d", &particiones[i]);
        total_particiones += particiones[i];
    }

    // Leer el número de procesos a asignar
    scanf("%d", &num_procesos);

    // Leer los procesos a asignar
    for (i = 0; i < num_procesos; i++){
        scanf("%d", &procesos[i]);
    }

    // Verificar si hay suficiente espacio disponible para asignar todos los procesos
    int total_procesos = 0;
    for (i = 0; i < num_procesos; i++){
        total_procesos += procesos[i];
    }

    if (total_procesos > total_particiones){
        printf("No hay suficiente espacio disponible para asignar todos los procesos\n");
        return 1;
    }

    // Asignar los procesos a las particiones utilizando el método seleccionado
    int particiones_asignadas[MAX_PROCESOS];
    memset(particiones_asignadas, -1, sizeof(particiones_asignadas));

    for (i = 0; i < num_procesos; i++) {
        particiones_asignadas[i] = asignar(procesos[i], metodo_asignacion);
        if (particiones_asignadas[i] == -1) {
            printf("No hay suficiente espacio disponible para asignar el proceso %d\n", i+1);
        }
    }

    // Imprimir el resultado
    for (i = 0; i < num_procesos; i++) {
        if (particiones_asignadas[i] != -1) {
            printf("El proceso %d fue asignado a la particion %d\n", i+1, particiones_asignadas[i]+1);
        }
    }

    // Imprimir el estado final de las particiones
    imprimir_particiones();

    return 0;
}