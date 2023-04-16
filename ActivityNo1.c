#include <stdio.h>
#include <stdlib.h>

#define MAX_PARTICIONES 100
#define MAX_PROCESOS 100

#define MFT 1
#define MVT 2

int particiones[MAX_PARTICIONES]; // arreglo que almacena el tamano de cada particion
int procesos[MAX_PROCESOS]; // arreglo que almacena el tamano de cada proceso

int num_particiones; // numero de particiones disponibles
int num_procesos; // numero de procesos a asignar
int total_memoria; // tamano total de la memoria

/*
  La funcion "asignar_mft" busca una particion adecuada para el proceso y la asigna utilizando el algoritmo de particionamiento fijo (MFT).
  Recibe como parametros el tamano del proceso a asignar y devuelve el indice de la partición asignada, o -1 si no hay suficiente espacio disponible.
*/
int asignar_mft(int tam_proceso){
    int i;
    int tam_particion;
    int indice_particion = -1;

    // Buscar la primera particion que tenga suficiente espacio para el proceso
    for (i = 0; i < num_particiones && indice_particion == -1; i++){
        tam_particion = particiones[i];
        if (tam_proceso <= tam_particion){
            indice_particion = i;
        }
    }

    if (indice_particion != -1){
        // Asignar la particion al proceso
        particiones[indice_particion] -= tam_proceso;
    }

    return indice_particion;
}

/*
  La funcion "asignar_mvt" busca un bloque adecuado para el proceso y lo asigna utilizando el algoritmo de particionamiento variable (MVT).
  Recibe como parámetros el tamano del proceso a asignar y devuelve el tamano del bloque asignado, o -1 si no hay suficiente espacio disponible.
*/
int asignar_mvt(int tam_proceso){
    int i;
    int tam_bloque;
    int indice_bloque = -1;

    // Buscar el primer bloque que tenga suficiente espacio para el proceso
    for (i = 0; i < num_particiones && indice_bloque == -1; i++){
        tam_bloque = particiones[i];
        if (tam_proceso <= tam_bloque){
            indice_bloque = i;
        }
    }

    if (indice_bloque != -1){
        // Asignar el bloque al proceso
        particiones[indice_bloque] -= tam_proceso;

        // Si hay espacio libre después del bloque asignado, crear una nueva partición con el espacio sobrante
        if (particiones[indice_bloque] > 0){
            num_particiones++;
            for (i = num_particiones - 1; i > indice_bloque + 1; i--){
                particiones[i] = particiones[i - 1];
            }
            particiones[indice_bloque + 1] = particiones[indice_bloque] - tam_proceso;
            particiones[indice_bloque] = tam_proceso;
        }

        tam_bloque = particiones[indice_bloque];
    }

    return tam_bloque;
}

int main(){
    int i;
    int metodo;
    int tam_particion = -1;
    int memoria_asignada = 0;
    int fragmentacion_interna = 0;
    int fragmentacion_externa = 0;

    //Pedir al usuario que ingrese los parametros del problema
    scanf("%d", &metodo);
    scanf("%d", &total_memoria);
    if (metodo == MFT) {
        scanf("%d", &tam_particion);
    }
    scanf("%d", &num_procesos);
    for (i = 0; i < num_procesos; i++){
        scanf("%d", &procesos[i]);
    }

    // Inicializar las particiones con el tamano especificado
    if (metodo == MFT){
        num_particiones = total_memoria / tam_particion;
        for (i = 0; i < num_particiones; i++){
            particiones[i] = tam_particion;
        }
    } else{
        num_particiones = 1;
        particiones[0] = total_memoria;
    }

    // Asignar los procesos utilizando el metodo seleccionado
    if (metodo == MFT){
        for (i = 0; i < num_procesos; i++){
            printf("P%d -> ", i);
            int indice_particion = asignar_mft(procesos[i]);
            if (indice_particion != -1){
                printf("%d asignado, %d de fragmentacion.\n", indice_particion, particiones[indice_particion]);
                memoria_asignada += procesos[i];
                fragmentacion_interna += particiones[indice_particion];
            } else{
                printf("No asignado.\n");
                fragmentacion_externa += procesos[i];
            }
        }
    } else {
        for (i = 0; i < num_procesos; i++){
            printf("P%d -> ", i);
            int tam_bloque = asignar_mvt(procesos[i]);
            if (tam_bloque != -1){
                printf("%d asignado.\n", tam_bloque);
                memoria_asignada += procesos[i];
                fragmentacion_interna += tam_bloque - procesos[i];
            } else{
                printf("No asignado.\n");
                fragmentacion_externa += procesos[i];
            }
        }
    }

    // Calcular la fragmentacion externa si se utiliza MFT
    if (metodo == MFT){
        for (i = 0; i < num_particiones; i++) {
            fragmentacion_externa += particiones[i];
        }
    }

    // Imprimir los resultados
    printf("\nMemoria Asignada: %d\n", memoria_asignada);
    printf("Fragmentacion Interna: %d\n", fragmentacion_interna);
    printf("Fragmentacion Externa: %d\n", fragmentacion_externa);

    return 0;
}