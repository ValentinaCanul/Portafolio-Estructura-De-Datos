#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define table_size 120  //tamaño de table de hash 
#define vacio -1  // Celda vacio 
#define borrado -2  // Celda borrada
 
// estuctura de estudiate 
typedef struct {
    int matricula;  
    char nombre[50];
    char carrera[50];
} Estudiante;

//tabla hash direccionamiento abrierto 
typedef struct {
    Estudiante tabla[table_size];
    int estado[table_size]; // 0: vacio, 1: ocupado, 2: borrado
}Tablahash; 

//funcion para tablahash 
int hash(int matricula){
    return matricula % table_size;
}

//crear tabla 
 void inicializarTabla(Tablahash *t) {
    for (int i = 0; i < table_size; i++) {
        t->estado[i] = vacio; // todas las posiciones empiezan vacías
    }
}

//insertar estudiante (sondeo lineal)
int insertar(Tablahash *t, Estudiante e){
    int indice = hash(e.matricula);
    //int inicio=indice;

    for(int i=0; i< table_size; i++){
        int pos = (indice +i) %table_size;  //sondeo lineal
        if (t->estado[pos]==vacio || t-> estado[pos]==borrado){
            t->tabla[pos]=e;
            t->estado[pos]=1; // espacio ocupado 
            return 1; 
        }
            //revisar 
    if (t->estado[pos] == 1 && t->tabla[pos].matricula == e.matricula) {
         t->tabla[pos] = e;  // actualiza solo si es el mismo estudiante
    return 1;
        }
    }
    return 0; //tabla llena 
}

//buscar estudiante
Estudiante* buscar(Tablahash *t, int matricula){
    int indice = hash(matricula);
    
    for(int i=0; i< table_size; i++){
        int pos = (indice +i) %table_size; 
        
        if (t->estado[pos]==vacio){
            return NULL; // no encontrado 
        }
        if (t->estado[pos]==1 && t->tabla[pos].matricula==matricula){
            return &t->tabla[pos]; // encontrado 
        }
    }
    return NULL; 
}

//Eliminar Estudiante
int eliminar(Tablahash *t, int matricula){
    int indice = hash(matricula);
    int inicio=indice;

    for (int i=0; i< table_size; i++){
        int pos = (indice +i) %table_size; 
        
        if (t->estado[pos]==vacio){
            return 0; // no encontrado 
        }
        if (t->estado[pos]==1 && t->tabla[pos].matricula==matricula){
            t->estado[pos]=borrado; 
            return 1; // eliminado 
        }
    }

    return 0; 
}

//main 
int main(){
    Tablahash tabla;
    inicializarTabla(&tabla);

    Estudiante e1 ={18979, "Juan Perez", "Ingenieria Software"};
    Estudiante e2 ={19071, "Miguel Gomez", "Medicina"};
    Estudiante e3 ={19175, "Ana Lopez", "Matematicas"};

    insertar(&tabla, e1);
    insertar(&tabla, e2);
    insertar(&tabla, e3);

    //Busca
    int matricula_buscar = 19070;
    Estudiante *resultado = buscar(&tabla, matricula_buscar);

    if (resultado!=NULL){
        printf("Estudiante encontrado: \n ");
        printf("Matricula: %d\n", resultado->matricula);
        printf("Nombre: %s\n", resultado->nombre);
        printf("Carrera: %s\n", resultado->carrera); 
    } else {
        printf("Estudiante %d NO ENCONTRADO. \n", matricula_buscar);
    }

    //Eliminar
    eliminar(&tabla, 19070);
    printf("Despues de eliminar 19079: %s\n",
           buscar(&tabla, 19070) ? "ENCONTRADO" : "NO ENCONTRADO");

    return 0;
}
