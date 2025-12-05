w#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CAJAS 3

//structs 
typedef struct {
    int id;
    int transacciones; //el tiempo que tarda en la caja
    int tiempo_llegada;
    int tiempo_inicio; //tiempo de inicio en la cola
    int tiempo_Salida; 
} Cliente; 

typedef struct Nodo {
    Cliente cliente;
    struct Nodo *siguiente;
} Nodo;

typedef struct { 
    Nodo* frente, *final;
    int clientes_Atendidos;
    int tiempo_totalEspera;
} Cola; 

// cola

void inicializarCola(Cola* cola){
    cola -> frente = cola -> final = NULL;
    cola -> clientes_Atendidos = 0;
    cola -> tiempo_totalEspera = 0;
}

int colaVacia(Cola* cola){
    return cola -> frente == NULL;
}

void enqueue (Cola* cola, Cliente* cl){
    Nodo *nuevo = (Nodo*) malloc(sizeof(Nodo));
    nuevo ->cliente = *cl;
    nuevo ->siguiente = NULL;
    if(colaVacia(cola)){
        cola -> frente = nuevo;
    } else {
        cola -> final -> siguiente = nuevo;
    }
    cola -> final = nuevo;

    
}

void dequeue(Cola* cola){
    if(colaVacia(cola)) return;
    Nodo* temp = cola -> frente;
    cola -> frente = cola -> frente -> siguiente;
    if(cola -> frente == NULL)
        cola -> final = NULL;
    free(temp);
}

// Simulacion 
Cliente generarCliente(int id, int tiempo_llegada){
    Cliente cl;
    cl.id = id;
    cl.transacciones = rand() % 10 + 1; // entre 1 y 10
    cl.tiempo_llegada = tiempo_llegada;
    cl.tiempo_inicio = -1; // no ha empezado
    cl.tiempo_Salida = -1; // no ha salido
    return cl;
}

//asignacion de caja
void asignarCaja (Cola cajas [],Cliente cl){
    int n= rand() % NUM_CAJAS; //caja aleatoria 
    enqueue(&cajas[n], &cl);
    printf("Cliente %d asignado a caja %d\n", cl.id, n+1);
}

void atenderCliente(Cola cajas[], int tiempo){
    for(int i=0; i<NUM_CAJAS; i++){
        if(!colaVacia(&cajas[i])){
            Nodo *nodo = cajas[i].frente;
            Cliente *cl = &nodo->cliente; 

            if(cl -> tiempo_inicio == -1)
                cl->tiempo_inicio = tiempo;
            cl->transacciones--;

            if (cl->transacciones == 0){
                cl->tiempo_Salida = tiempo;
                int espera = cl->tiempo_inicio - cl -> tiempo_llegada;
                cajas[i].tiempo_totalEspera += espera;
                cajas[i].clientes_Atendidos++;
                
                printf("Cliente %d atendido en caja %d. Tiempo de espera: %d\n", cl->id, i+1, espera);

                dequeue(&cajas[i]);

            }
        }
    }
}

void mostrarEstadisticas(Cola cajas[]){
    printf("\nEstadisticas finales:\n");
    for(int i=0; i<NUM_CAJAS; i++){
        if(cajas[i].clientes_Atendidos > 0){
            float espera_promedio = (float)cajas[i].tiempo_totalEspera / cajas[i].clientes_Atendidos;
            printf("Caja %d: Clientes atendidos: %d, Tiempo de espera promedio: %.2f\n", i+1, cajas[i].clientes_Atendidos, espera_promedio);
        } else {
            printf("Caja %d: No se atendieron clientes.\n", i+1);
        }
    }
}

// main
int main(){
    int MAX_CLIENTES;
    srand (time(NULL));
    Cola cajas[NUM_CAJAS];
    for(int i=0; i<NUM_CAJAS; i++)
        inicializarCola(&cajas[i]);

    int tiempo =0; int id =1; 

    printf("Por favor indique el numero de clientes a atender: ");
    scanf("%d", &MAX_CLIENTES);

    //llegada de clientes
    while(id <= MAX_CLIENTES || !colaVacia(&cajas[0]) || !colaVacia(&cajas[1]) || !colaVacia(&cajas[2])){
        if(id <= MAX_CLIENTES && (rand() % 3 == 0)){ //probabilidad de llegada
            Cliente cl = generarCliente(id++, tiempo);
            asignarCaja(cajas, cl);
        }
        atenderCliente(cajas, tiempo);
        tiempo++;
    }
    mostrarEstadisticas(cajas);

    return 0;

}