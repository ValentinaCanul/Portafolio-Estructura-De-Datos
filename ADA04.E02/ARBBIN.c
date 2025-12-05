#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;   //matricula
    char name[50];
    char apellido[50];
    double *promedio;
    int n_cal; 
} Student;

typedef struct Node{
    Student *s;
    struct Node *left;
    struct Node *right;
} Node;

Student *crear_student(int id, const char *nombre, const char *apellido) {
    Student *s = (Student *)malloc(sizeof(Student));
    if (!s) return NULL;
    s->id = id;
    /* struct field is 'name' not 'nombre' */
    strcpy(s->name, nombre);
    strcpy(s->apellido, apellido);
    s->promedio = NULL;
    s->n_cal = 0;
    return s;
}

int agregar_calificacion(Student *s, double cal) {
    if (!s) return 0;
    double *tmp = (double *)realloc(s->promedio, sizeof(double) * (s->n_cal + 1));
    if (!tmp) return 0; /* allocation failed, keep original buffer */
    s->promedio = tmp;
    s->promedio[s->n_cal++] = cal;
    return 1;
}

double promedio(Student *s) {
    if (!s || s->n_cal <= 0 || s->promedio == NULL) return 0;
    double suma = 0;
    for (int i = 0; i < s->n_cal; i++) suma += s->promedio[i];
    return suma / s->n_cal;
}

//Arbol Binario

Node *insertar(Node *r, Student *s){
    if (r == NULL) {
    Node *n = (Node *)malloc(sizeof(Node));
        n->s = s;
        n->left = n->right = NULL;
        return n;
    }
    if (s->id < r->s->id) {
        r->left = insertar(r->left, s);
    } else if (s->id > r->s->id) {
        r->right = insertar(r->right, s);
    }
    return r;
}

Node *buscar(Node *r, int id){
    if (r == NULL || r->s->id == id) {
        return r;
    }
    if (id < r->s->id) {
        return buscar(r->left, id);
    } else {
        return buscar(r->right, id);
    }
}

Node *minimo(Node *r){
    while (r->left != NULL) {
        r = r->left;
    }
    return r;
}

Node *eliminar(Node *r, int id) {
    if (r == NULL) return r;
    if (id < r->s->id) {
        r->left = eliminar(r->left, id);
    } else if (id > r->s->id) {
        r->right = eliminar(r->right, id);
    } else {
        // Nodo encontrado
        if (r->left == NULL) {
            Node *temp = r->right;
            if (r->s) {
                free(r->s->promedio);
                free(r->s);
            }
            free(r);
            return temp;
        } 
        else if (r->right == NULL) {
            Node *temp = r->left;
            if (r->s) {
                free(r->s->promedio);
                free(r->s);
            }
            free(r);
            return temp;
        }
        // Nodo con dos hijos
        Node *temp = minimo(r->right);
        /* swap student pointers between current node and successor
           so that we can delete the original student from the right subtree
           without causing a dangling pointer or double-free. */
        Student *tmpStudent = r->s;
        r->s = temp->s;
        temp->s = tmpStudent;
        /* delete the node that now contains the original student (same id) */
        r->right = eliminar(r->right, id);
    }
    return r;
}

/* Free entire tree and all students/arrays */
void free_tree(Node *r) {
    if (!r) return;
    free_tree(r->left);
    free_tree(r->right);
    if (r->s) {
        free(r->s->promedio);
        free(r->s);
    }
    free(r);
}

/* ----------------- Interactive helpers ----------------- */

/* safe line read */
static void read_line(char *buf, size_t sz) {
    if (!fgets(buf, (int)sz, stdin)) {
        buf[0] = '\0';
        return;
    }
    size_t n = strcspn(buf, "\r\n");
    buf[n] = '\0';
}

int count_nodes(Node *r) {
    if (!r) return 0;
    return 1 + count_nodes(r->left) + count_nodes(r->right);
}

/* collect students into array (inorder) */
void collect_students(Node *r, Student **arr, int *idx) {
    if (!r) return;
    collect_students(r->left, arr, idx);
    arr[(*idx)++] = r->s;
    collect_students(r->right, arr, idx);
}

/* print a student's info */
void print_student(Student *s) {
    if (!s) return;
    printf("ID %d: %s %s | avg=%.2f | grades:", s->id, s->name, s->apellido, promedio(s));
    for (int i = 0; i < s->n_cal; ++i) printf(" %g", s->promedio[i]);
    printf("\n");
}

/* count how many students have better/worse average than s (excluding s) */
void count_better_worse(Node *root, Student *s, int *better, int *worse) {
    *better = *worse = 0;
    if (!root || !s) return;
    int total = count_nodes(root);
    if (total <= 1) return;
    Student **arr = (Student **)malloc(sizeof(Student*) * total);
    if (!arr) return;
    int idx = 0;
    collect_students(root, arr, &idx);
    double ref = promedio(s);
    for (int i = 0; i < idx; ++i) {
        if (arr[i]->id == s->id) continue;
        double a = promedio(arr[i]);
        if (a > ref) (*better)++;
        else if (a < ref) (*worse)++;
    }
    free(arr);
}

/* add a student interactively */
void add_student_interactive(Node **root) {
    char buf[128];
    printf("Enter id: "); read_line(buf, sizeof(buf));
    int id = (int)strtol(buf, NULL, 10);
    if (buscar(*root, id)) {
        printf("Student with id %d already exists.\n", id);
        return;
    }
    char name[50], apellido[50];
    printf("Enter name: "); read_line(name, sizeof(name));
    printf("Enter last name: "); read_line(apellido, sizeof(apellido));
    printf("Enter first grade (or leave empty for 0): "); read_line(buf, sizeof(buf));
    double g = 0;
    if (buf[0] != '\0') g = strtod(buf, NULL);

    Student *s = crear_student(id, name, apellido);
    if (!s) { printf("Allocation failed\n"); return; }
    if (buf[0] != '\0') agregar_calificacion(s, g);
    *root = insertar(*root, s);
    printf("Student added. Total students: %d\n", count_nodes(*root));
}

/* submenu for a specific student */
void student_submenu(Node **root) {
    char buf[128];
    printf("Enter student id to search: "); read_line(buf, sizeof(buf));
    int id = (int)strtol(buf, NULL, 10);
    Node *n = buscar(*root, id);
    if (!n) { printf("Student %d not found.\n", id); return; }
    Student *s = n->s;
    while (1) {
        int better = 0, worse = 0;
        count_better_worse(*root, s, &better, &worse);
        printf("\nStudent menu for %d - %s %s\n", s->id, s->name, s->apellido);
        print_student(s);
        printf("(Compared to others: %d better, %d worse)\n", better, worse);
        printf("1) Add grade\n2) Update grade\n3) Delete student\n4) Back\nChoose: ");
        read_line(buf, sizeof(buf));
        int opt = (int)strtol(buf, NULL, 10);
        if (opt == 1) {
            printf("Enter grade to add: "); read_line(buf, sizeof(buf));
            double g = strtod(buf, NULL);
            if (agregar_calificacion(s, g)) printf("Grade added. New avg=%.2f\n", promedio(s));
            else printf("Failed to add grade.\n");
        } else if (opt == 2) {
            if (s->n_cal == 0) { printf("No grades to update.\n"); continue; }
            printf("Grades:\n");
            for (int i = 0; i < s->n_cal; ++i) printf("%d: %g\n", i, s->promedio[i]);
            printf("Enter index to update: "); read_line(buf, sizeof(buf));
            int idx = (int)strtol(buf, NULL, 10);
            if (idx < 0 || idx >= s->n_cal) { printf("Invalid index.\n"); continue; }
            printf("Enter new value: "); read_line(buf, sizeof(buf));
            double nv = strtod(buf, NULL);
            s->promedio[idx] = nv;
            printf("Updated. New avg=%.2f\n", promedio(s));
        } else if (opt == 3) {
            /* confirm deletion */
            printf("Are you sure you want to delete student %d (%s %s)? (y/n): ", s->id, s->name, s->apellido);
            read_line(buf, sizeof(buf));
            if (buf[0] == 'y' || buf[0] == 'Y') {
                *root = eliminar(*root, s->id);
                printf("Student deleted.\n");
                return;
            } else {
                printf("Deletion cancelled.\n");
                continue;
            }
        } else {
            return;
        }
    }
}

/* report submenu */
int cmp_avg_desc(const void *a, const void *b) {
    Student *const *sa = (Student *const *)a;
    Student *const *sb = (Student *const *)b;
    double da = promedio(*sa);
    double db = promedio(*sb);
    if (da < db) return 1;
    if (da > db) return -1;
    return 0;
}

void report_submenu(Node *root) {
    char buf[128];
    int total = count_nodes(root);
    if (total == 0) { printf("No students to report.\n"); return; }
    printf("Report options:\n1) Ordered by ID\n2) Ordered by average (desc)\n3) Filter by threshold\nChoose: ");
    read_line(buf, sizeof(buf));
    int opt = (int)strtol(buf, NULL, 10);
    Student **arr = (Student **)malloc(sizeof(Student*) * total);
    int idx = 0;
    collect_students(root, arr, &idx);
    if (opt == 1) {
        for (int i = 0; i < idx; ++i) print_student(arr[i]);
    } else if (opt == 2) {
        qsort(arr, idx, sizeof(Student*), cmp_avg_desc);
        for (int i = 0; i < idx; ++i) print_student(arr[i]);
    } else if (opt == 3) {
        printf("Enter threshold grade: "); read_line(buf, sizeof(buf));
        double thr = strtod(buf, NULL);
        printf("1) >= threshold\n2) < threshold\nChoose: "); read_line(buf, sizeof(buf));
        int pick = (int)strtol(buf, NULL, 10);
        for (int i = 0; i < idx; ++i) {
            double a = promedio(arr[i]);
            if (pick == 1) { if (a >= thr) print_student(arr[i]); }
            else { if (a < thr) print_student(arr[i]); }
        }
    } else {
        printf("Unknown option.\n");
    }
    free(arr);
}

/* print nodes at given level (0 = root) */
void print_nodes_at_level(Node *r, int level) {
    if (!r) return;
    if (level == 0) {
        printf("%d (%s %s) ", r->s->id, r->s->name, r->s->apellido);
        return;
    }
    print_nodes_at_level(r->left, level-1);
    print_nodes_at_level(r->right, level-1);
}

/* ASCII tree (sideways) */
void print_tree_ascii(Node *r, int space) {
    if (!r) return;
    const int GAP = 6;
    space += GAP;
    print_tree_ascii(r->right, space);
    for (int i = GAP; i < space; ++i) putchar(' ');
    printf("%d\n", r->s->id);
    print_tree_ascii(r->left, space);
}

/* Pretty ASCII tree: assign inorder column positions and print rows by depth
   This produces columns where left-to-right order is the BST inorder, and
   nodes at the same depth appear on the same line spaced by a fixed cell width. */
void inorder_assign(Node *r, int depth, Node **cols, int *depths, int *col) {
    if (!r) return;
    inorder_assign(r->left, depth+1, cols, depths, col);
    cols[*col] = r;
    depths[*col] = depth;
    (*col)++;
    inorder_assign(r->right, depth+1, cols, depths, col);
}

void print_tree_pretty(Node *root) {
    int n = count_nodes(root);
    if (n == 0) return;
    Node **cols = (Node **)malloc(sizeof(Node*) * n);
    int *depths = (int *)malloc(sizeof(int) * n);
    if (!cols || !depths) {
        free(cols); free(depths); return;
    }
    int col = 0;
    inorder_assign(root, 0, cols, depths, &col);
    int maxdepth = 0;
    for (int i = 0; i < n; ++i) if (depths[i] > maxdepth) maxdepth = depths[i];

    const int CELL = 12; /* width per column */
    char cellbuf[64];
    for (int d = 0; d <= maxdepth; ++d) {
        for (int c = 0; c < n; ++c) {
            if (depths[c] == d) {
                int id = cols[c]->s->id;
                int len = snprintf(cellbuf, sizeof(cellbuf), "%d", id);
                if (len < 0) len = 0;
                int left = (CELL - len) / 2;
                int right = CELL - len - left;
                for (int k = 0; k < left; ++k) putchar(' ');
                fputs(cellbuf, stdout);
                for (int k = 0; k < right; ++k) putchar(' ');
            } else {
                for (int k = 0; k < CELL; ++k) putchar(' ');
            }
        }
        putchar('\n');
    }
    free(cols);
    free(depths);
}

/* ----------------- Interactive main ----------------- */
int main(void) {
    Node *root = NULL;
    char buf[128];
    while (1) {
        int total = count_nodes(root);
        printf("\nMain menu - %d students\n", total);
        printf("1) Add student\n2) Search student (submenu)\n3) Reports\n4) Show nodes at level\n5) Show ASCII tree\n6) Quit\nChoose: ");
        read_line(buf, sizeof(buf));
        int opt = (int)strtol(buf, NULL, 10);
        if (opt == 1) {
            add_student_interactive(&root);
        } else if (opt == 2) {
            student_submenu(&root);
        } else if (opt == 3) {
            report_submenu(root);
        } else if (opt == 4) {
            printf("Enter level (0 = root): "); read_line(buf, sizeof(buf));
            int level = (int)strtol(buf, NULL, 10);
            printf("Nodes at level %d: ", level);
            print_nodes_at_level(root, level);
            printf("\n");
        } else if (opt == 5) {
            printf("ASCII tree:\n");
            print_tree_pretty(root);
        } else if (opt == 6) {
            break;
        } else {
            printf("Unknown option.\n");
        }
    }
    free_tree(root);
    return 0;
}
