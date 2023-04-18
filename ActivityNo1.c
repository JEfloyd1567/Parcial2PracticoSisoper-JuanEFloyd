#include <stdio.h>
#include <stdlib.h>

#define MAX_PARTICIONES 100
#define MAX_PROCESOS 100

#define MFT 1
#define MVT 2

int particiones[MAX_PARTICIONES]; // arreglo que guarda el tamano de las particiones
int procesos[MAX_PROCESOS]; // arreglo que guarda el tamano de los procesos

int num_particiones; // numero de particiones
int num_procesos; // numero de procesos
int total_memoria; // tamano total de la memoria

/* Funcion que asigna una particion a un proceso usando el algoritmo MFT */
int asignar_mft(int tam_proceso){
    int i;
    int tam_particion;
    int indice_particion = -1;
     // Recorre el arreglo de particiones hasta encontrar una particion que tenga suficiente espacio para el proceso
    for (i = 0; i < num_particiones && indice_particion == -1; i++){
        tam_particion = particiones[i];
        if (tam_proceso <= tam_particion){
            indice_particion = i;
        }
    }
    // Si se encontro una partición disponible, se asigna el proceso a esa particion y se actualiza el tamano de la particion
    if (indice_particion != -1){
        particiones[indice_particion] -= tam_proceso;
    }
    // Si no hay particiones disponibles, se retorna -1 como senal de error
    return indice_particion;
}

/* Función que asigna una particion a un proceso usando el algoritmo MVT */
int asignar_mvt(int tam_proceso){
    int i;
    int tam_bloque;
    int indice_bloque = -1;
    int min_fragmentacion = total_memoria + 1;
    // Recorre el arreglo de particiones buscando la partición que tenga el tamano mas cercano al tamano del proceso
    for (i = 0; i < num_particiones; i++){
        tam_bloque = particiones[i];
        if (tam_proceso <= tam_bloque && (tam_bloque - tam_proceso) < min_fragmentacion){
            indice_bloque = i;
            min_fragmentacion = tam_bloque - tam_proceso;
        }
    }
    // Si se encontro una partición disponible, se asigna el proceso a esa particion y se actualiza el tamano de la particion
    if (indice_bloque != -1){
        particiones[indice_bloque] -= tam_proceso;
        if (particiones[indice_bloque] > 0){
            num_particiones++;
            for (i = num_particiones - 1; i > indice_bloque + 1; i--){
                particiones[i] = particiones[i - 1];
            }
            particiones[indice_bloque + 1] = particiones[indice_bloque];
            particiones[indice_bloque] = 0;
        }
    }

    return indice_bloque;
}

int main(){
    int i;
    int metodo; // variable que indica el algoritmo a usar (MFT o MVT)
    int tam_particion = -1; // tamano de las particiones (en el caso de MFT)
    int memoria_asignada = 0; // cantidad de memoria asignada a los procesos
    int fragmentacion_interna = 0; // cantidad de fragmentacion interna
    int fragmentacion_externa = 0; // cantidad de fragmentacion externa

    scanf("%d", &metodo);
    scanf("%d", &total_memoria);
    
    if (metodo == MFT) {
        scanf("%d", &tam_particion);
        num_particiones = total_memoria / tam_particion; // se calcula el numero de particiones
        for (i = 0; i < num_particiones; i++){
            particiones[i] = tam_particion; // se inicializan todas las particiones con el mismo tamano
        }
    } 
    else if (metodo == MVT) {
        num_particiones = 1;
        particiones[0] = total_memoria; // se tiene una sola particion que ocupa toda la memoria
    }

    scanf("%d", &num_procesos);
    
    for (i = 0; i < num_procesos; i++){
        scanf("%d", &procesos[i]); // se lee el tamano de cada proceso
    }

    if (metodo == MFT){
        for (i = 0; i < num_procesos; i++){
            printf("P%d -> ", i);
            int tam_particion;
            int indice_particion = asignar_mft(procesos[i]); // se asigna una particion al proceso
            if (indice_particion != -1){
                tam_particion = procesos[i] + particiones[indice_particion];
                printf("%d asignado en %d, %d de fragmentacion.\n", procesos[i], tam_particion, particiones[indice_particion]);
                memoria_asignada += tam_particion;
                fragmentacion_interna += particiones[indice_particion];
            } 
            else{
                printf("No asignado.\n");
                fragmentacion_externa += procesos[i];
            }
        }

    } 
    else if (metodo == MVT){
        for (i = 0; i < num_procesos; i++){
            printf("P%d -> ", i);
            int indice_bloque = asignar_mvt(procesos[i]); // se asigna una particion al proceso
            if (indice_bloque != -1){
                printf("%d asignado en Mem.\n", procesos[i]);
                memoria_asignada += procesos[i];
            } 
            else{
                printf("No asignado.\n");
                fragmentacion_externa += procesos[i];
            }
        }
    }

    else if (metodo == MVT){
        fragmentacion_interna = -1;
    }
    // Imprimimos los resultados
    fragmentacion_externa = total_memoria - memoria_asignada; // se calcula la fragmentacion externa
    printf("\nMemoria Asignada: %d\n", memoria_asignada);
    printf("Fragmentacion Interna: %d\n", fragmentacion_interna);
    printf("Fragmentacion Externa: %d\n", fragmentacion_externa);

    return 0;
