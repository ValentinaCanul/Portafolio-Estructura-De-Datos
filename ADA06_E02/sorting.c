#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// Código fuente base para el ordenamiento de arreglos dinámicos de números flotantes

// Estructura del arreglo
typedef struct {
    float *data;      // Apuntador al arreglo dinámico
    int size;         // Tamaño del arreglo
} Array;

// Funcion para crear un arreglo con números flotantes aleatorios
Array* createRandomArray(int n) {
    if (n <= 0) {
        printf("Error: Array size must be positive.\n");
        return NULL;
    }

    // Asignar memoria para la estructura del arreglo
    Array *arr = (Array*)malloc(sizeof(Array));
    if (arr == NULL) {
        printf("Error: Memory allocation failed for Array structure.\n");
        return NULL;
    }

    // Asignar memoria para el arreglo de datos
    arr->data = (float*)malloc(n * sizeof(float));
    if (arr->data == NULL) {
        printf("Error: Memory allocation failed for array data.\n");
        free(arr);
        return NULL;
    }
    
    arr->size = n;

    // Inicializar la semilla aleatoria
    srand(time(NULL));

    // Llenar el arreglo con números flotantes aleatorios (0.0 a 1000.0)
    for (int i = 0; i < n; i++) {
        arr->data[i] = ((float)rand() / RAND_MAX) * 1000.0f;
    }
    
    return arr;
}

// Funcion para crear un arreglo con valores definidos por el usuario
Array* createArray(int n) {
    if (n <= 0) {
        printf("Error: Array size must be positive.\n");
        return NULL;
    }
    
    Array *arr = (Array*)malloc(sizeof(Array));
    if (arr == NULL) {
        printf("Error: Memory allocation failed for Array structure.\n");
        return NULL;
    }
    
    arr->data = (float*)malloc(n * sizeof(float));
    if (arr->data == NULL) {
        printf("Error: Memory allocation failed for array data.\n");
        free(arr);
        return NULL;
    }
    
    arr->size = n;
    
    printf("Enter %d floating-point numbers:\n", n);
    for (int i = 0; i < n; i++) {
        printf("Element [%d]: ", i);
        scanf("%f", &arr->data[i]);
    }
    
    return arr;
}

// Funcion para desplegar el arreglo
void displayArray(Array *arr) {
    if (arr == NULL || arr->data == NULL) {
        printf("Error: Array is empty or not initialized.\n");
        return;
    }
    
    printf("\nArray (size = %d):\n", arr->size);
    printf("[");
    for (int i = 0; i < arr->size; i++) {
        printf("%.2f", arr->data[i]);
        if (i < arr->size - 1) {
            printf(", ");
        }
    }
    printf("]\n\n");
}

// Funcion para copiar un arreglo (útil para probar múltiples algoritmos de ordenamiento)
Array* copyArray(Array *source) {
    if (source == NULL || source->data == NULL) {
        return NULL;
    }
    
    Array *copy = (Array*)malloc(sizeof(Array));
    if (copy == NULL) {
        return NULL;
    }
    
    copy->data = (float*)malloc(source->size * sizeof(float));
    if (copy->data == NULL) {
        free(copy);
        return NULL;
    }
    
    copy->size = source->size;
    for (int i = 0; i < source->size; i++) {
        copy->data[i] = source->data[i];
    }
    
    return copy;
}

// Funcion para liberar la memoria asignada para el arreglo
void freeArray(Array *arr) {
    if (arr != NULL) {
        if (arr->data != NULL) {
            free(arr->data);
        }
        free(arr);
    }
}

// Funcion para intercambiar dos elementos (funcion auxiliar para algoritmos de ordenamiento)
void swap(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

// ============================================
// ALGORITMOS DE ORDENAMIENTO PARA IMPLEMENTAR
// ============================================

// Bubble Sort 

void bubbleSort(Array *arr) {
    if (arr == NULL || arr->data == NULL) {
        return;
    }

    printf("Aplicando Bubble Sort...\n");

    int i, j;
    int n = arr->size;
    float *data = arr->data;

    // Se añadió un contador para el número de iteraciones (comparaciones)
    int iteraciones = 0;

    // Se añadió un contador para el número de swaps (intercambios)
    int swaps = 0;

    // Se modificó el bucle externo para recorrer todas las pasadas del algoritmo
    for (i = 0; i < n - 1; i++) {

        // Se agregó una optimización opcional: bandera para detectar si ya está ordenado
        int ordenado = 1;

        // Se recorre el arreglo comparando elementos adyacentes
        for (j = 0; j < n - i - 1; j++) {
            iteraciones++;  // Se incrementa el contador de comparaciones

            if (data[j] > data[j + 1]) {
                swap(&data[j], &data[j + 1]);
                swaps++;       // Se incrementa el contador de swaps
                ordenado = 0;  //  Indica que se realizó un intercambio
            }
        }

        // Si en una pasada no hubo swaps, el arreglo ya está ordenado
        if (ordenado) {
            break;
        }
    }

    //  Se agregó la impresión final de estadísticas
    printf("\nEstadísticas del Bubble Sort:\n");
    printf("Iteraciones (comparaciones): %d\n", iteraciones);
    printf("Intercambios (swaps): %d\n", swaps);
}

// Selection Sort
void selectionSort(Array *arr) {
    if (arr == NULL || arr->data == NULL) {
        return;
    }

    int i, j, min_idx;
    int n = arr->size;

    unsigned long long iterations = 0ULL; 

    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            iterations++; 
            if (arr->data[j] < arr->data[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(&arr->data[min_idx], &arr->data[i]);
        }
    }

    printf("\nNumero de iteraciones: %llu\n", iterations);
}
// Insertion Sort
void insertionSort(Array *arr) {
    if (arr == NULL || arr->data == NULL) {
        return;
    }
    
    printf("Insertion Sort - To be implemented\n");
    // TODO: Implement insertion sort algorithm
}

// Función recursiva de QuickSort
void quickSortRecursive(float *data, int low, int high) {
    if (low >= high) {
        return;
    }

    int index = partition(data, low, high);

    if (low < index - 1) {
        quickSortRecursive(data, low, index - 1);
    }

    if (index < high) {
        quickSortRecursive(data, index, high);
    }
}
// Función principal de QuickSort usada en el menú
void quickSort(Array *arr) {
    if (arr == NULL || arr->data == NULL) {
        return;
    }

    int n = arr->size;
    if (n <= 1) {
        return;
    }

    quickSortRecursive(arr->data, 0, n - 1);
}

// Merge Sort
void mergeSort(Array *arr) {
    if (arr == NULL || arr->data == NULL) {
        return;
    }
    
    printf("Merge Sort - To be implemented\n");
    // TODO: Implement merge sort algorithm
}


// Cocktail Sort 
void cocktailSort(Array *arr) {
    printf("Aplicando Cocktail Sort...\n");
     if (arr == NULL || arr->data == NULL) return;
    
    float *a = arr->data; 
    int n = arr->size;

    int s=1, ini=0, fin=n-1, i, sw=0, it=0;
    float t; 

    while(s) {
        s=0; it++;
        // Ida ->
        for(i=ini; i<fin; ++i)
            if(a[i] > a[i+1]) { 
                printf("It %d(Ida): %.2f <-> %.2f\n", it, a[i], a[i+1]); 
                t=a[i]; a[i]=a[i+1]; a[i+1]=t; // Swap inline de floats
                s=1; sw++; 
            }
        
        if(!s) break;
        
        // Vuelta <-
        fin--;
        for(i=fin-1; i>=ini; --i)
            if(a[i] > a[i+1]) { 
                printf("It %d(Vuelta): %.2f <-> %.2f\n", it, a[i], a[i+1]); 
                t=a[i]; a[i]=a[i+1]; a[i+1]=t; // Swap inline de floats
                s=1; sw++; 
            }
        ini++;
    }
    printf("\nTotal Iteraciones: %d\nTotal Swaps: %d\n----------------\n", it, sw);
}

// Funciones auxiliares para Heap Sort
void heapify(float *data, int n, int i) {
    int largest = i;       // Inicializa el nodo raíz como el más grande
    int left = 2 * i + 1;  // Hijo izquierdo
    int right = 2 * i + 2; // Hijo derecho

    // Si el hijo izquierdo es más grande que la raíz
    if (left < n && data[left] > data[largest])
        largest = left;

    // Si el hijo derecho es más grande que el más grande hasta ahora
    if (right < n && data[right] > data[largest])
        largest = right;

    // Si el más grande no es la raíz
    if (largest != i) {
        swap(&data[i], &data[largest]);
        // Recursivamente aplica heapify en el subárbol afectado
        heapify(data, n, largest);
    }
}

// Heap Sort
void heapSort(Array *arr) {
    if (arr == NULL || arr->data == NULL) {
        return;
    }

    printf("Aplicando Heap Sort...\n");

    int n = arr->size;
    float *data = arr->data;

    // Construir el heap (reorganizar el arreglo)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(data, n, i);
    }

    // Extraer elementos uno por uno del heap
    for (int i = n - 1; i > 0; i--) {
        // Mover la raíz actual al final
        swap(&data[0], &data[i]);

        // Llamar heapify en el heap reducido
        heapify(data, i, 0);
    }
}
// Definir estructuras y funciones auxiliares para Tree Sort


// Tree Sort

void treeSort(Array *arr) {
    printf("Aplicando Tree Sort...\n"); 
}


// Funciones auxiliares para Tournament Sort
typedef struct TournamentNode {
    float value;
    int index;
} TournamentNode;

// Definir funciones auxiliares para Tournament Sort

// Tournament Sort
void tournamentSort(Array *arr) {

    printf("Aplicando Tournament Sort...\n");
    
}

// Shell Sort with Shell's increment sequence
void shellSort(Array *arr) {
    if (arr == NULL || arr->data == NULL) {
        return;
    }

    printf("Aplicando Shell Sort...\n");

    long comparisons = 0;
    long swaps = 0; // Use long to handle large counts and avoid overflow
    
    int iterations_gap = 0;
    int n = arr->size; // Size of the array
    int gap = n / 2;   // Initial gap between elements

    while (gap > 0) { /*
        * Continue while the gap is at least 1.
        * - Shifting elements by zero positions doesn’t make sense.
        * - Shell Sort keeps the array sorted even if gap were negative.
        */
        iterations_gap++; // Count the number of gap iterations
        for (int i = gap; i < n; i++) { // Iterate through the elements from 'gap' to the end of the array
            int j = i;
            comparisons++; // First comparison for while condition
            while (j >= gap && arr->data[j - gap] > arr->data[j]) { // Compare the current element with the ones 'gap' positions before it
                swap(&arr->data[j], &arr->data[j - gap]); // // Move inserted element back to its correct position
                swaps++; // Count the swap
                j -= gap;
                comparisons++; //next comparison for while condition
            }
        }
        gap /= 2; // Reduce gap for next iteration
    }

    printf("Métricas\n");
    printf("Comparaciones totales: %ld\n", comparisons);
    printf("Intercambios totales: %ld\n", swaps);
    printf("Gaps usados { ");
    for (int aux_gaps = n / 2; aux_gaps > 0; aux_gaps /= 2) {
        printf("%d ", aux_gaps);
    }
    printf("}\n");
}

// ============================================
// MAIN FUNCTION
// ============================================

int main() {
    int choice, size;
    Array *arr = NULL;
    
    printf("===========================================\n");
    printf("  Algoritmos de Ordenamiento - Implementación Base\n");
    printf("===========================================\n\n");
    
    while (1) {
        printf("\nMenu:\n");
        printf("1. Crear arreglo con números aleatorios\n");
        printf("2. Crear arreglo con números personalizados\n");
        printf("3. Desplegar arreglo\n");
        printf("4. Aplicar Bubble Sort\n");
        printf("5. Aplicar Selection Sort\n");
        printf("6. Aplicar Insertion Sort\n");
        printf("7. Aplicar Quick Sort\n");
        printf("8. Aplicar Merge Sort\n");
        printf("9. Aplicar Cocktail Sort\n");
        printf("10. Aplicar Heap Sort\n");
        printf("11. Aplicar Tree Sort\n");
        printf("12. Aplicar Tournament Sort\n");
        printf("13. Salir\n");
        printf("\nIngrese su opción: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                if (arr != NULL) {
                    freeArray(arr);
                }
                printf("Ingrese el tamaño del arreglo: ");
                scanf("%d", &size);
                arr = createRandomArray(size);
                if (arr != NULL) {
                    printf("Arreglo creado exitosamente!\n");
                    displayArray(arr);
                }
                break;
                
            case 2:
                if (arr != NULL) {
                    freeArray(arr);
                }
                printf("Ingrese el tamaño del arreglo: ");
                scanf("%d", &size);
                arr = createArray(size);
                if (arr != NULL) {
                    printf("Arreglo creado exitosamente!\n");
                    displayArray(arr);
                }
                break;
                
            case 3:
                displayArray(arr);
                break;
                
            case 4:
                bubbleSort(arr);
                displayArray(arr);
                break;
                
            case 5:
                selectionSort(arr);
                displayArray(arr);
                break;
                
            case 6:
                insertionSort(arr);
                displayArray(arr);
                break;
                
            case 7:
                quickSort(arr);
                displayArray(arr);
                break;
                
            case 8:
                mergeSort(arr);
                displayArray(arr);
                break;
                
            case 9:
                cocktailSort(arr);
                displayArray(arr);
                break;
                
            case 10:
                heapSort(arr);
                displayArray(arr);
                break;
                
            case 11:
                treeSort(arr);
                displayArray(arr);
                break;
                
            case 12:
                tournamentSort(arr);
                displayArray(arr);
                break;
                
            case 13:
                printf("\nFreeing memory and exiting...\n");
                freeArray(arr);
                printf("Goodbye!\n");
                return 0;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}
