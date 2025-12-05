
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Define the Movie struct to hold all relevant movie data
typedef struct Movie {
    int id;                             // Movie ID
    char title[256];                    // Movie title
    float vote_average;                 // Average vote
    int vote_count;                     // Number of votes
    char status[32];                    // Status (e.g., Released)
    char release_date[16];              // Release date
    long revenue;                       // Revenue
    int runtime;                        // Runtime in minutes
    char adult[8];                      // Adult flag
    char backdrop_path[256];            // Backdrop image path
    long budget;                        // Budget
    char homepage[256];                 // Homepage URL
    char imdb_id[32];                   // IMDB ID
    char original_language[16];         // Original language
    char original_title[256];           // Original title
    char overview[1024];                // Overview/description
    float popularity;                   // Popularity
    char poster_path[256];              // Poster image path
    char tagline[256];                  // Tagline
    char genres[256];                   // Genres
    char production_companies[256];     // Production companies
    char production_countries[128];     // Production countries
    char spoken_languages[128];         // Spoken languages
    char keywords[512];                 // Keywords
    // If you want to add cast/director, add them here
} Movie;

// Define the double linked list node
typedef struct MovieNode {
    Movie data;                     // The movie data
    struct MovieNode* prev;         // Pointer to previous node
    struct MovieNode* next;         // Pointer to next node
} MovieNode;

// Function to create a new MovieNode with given Movie data
MovieNode* createMovieNode(Movie m) {
    MovieNode* node = (MovieNode*)malloc(sizeof(MovieNode)); // Allocate memory for node
    node->data = m;      // Copy movie data
    node->prev = NULL;   // Set previous pointer to NULL
    node->next = NULL;   // Set next pointer to NULL
    return node;         // Return the new node
}

// Function to trim newline and carriage return from a string
void trim_newline(char* str) {
    int len = strlen(str); // Get string length
    while (len > 0 && (str[len-1] == '\n' || str[len-1] == '\r')) {
        str[len-1] = '\0'; // Replace newline or carriage return with null terminator
        len--;
    }
}

// Function to load movies from a CSV file into a double linked list
MovieNode* loadMoviesFromCSV(const char* filename) {
    FILE* file = fopen(filename, "r"); // Open the CSV file for reading
    if (!file) { // If file could not be opened
        printf("Error: Could not open file %s\n", filename); // Print error message
        return NULL; // Return NULL
    }
    char line[4096]; // Buffer to hold each line
    fgets(line, sizeof(line), file); // Read and skip the header line
    MovieNode* head = NULL; // Head of the linked list
    MovieNode* tail = NULL; // Tail of the linked list
    while (fgets(line, sizeof(line), file)) { // Read each line from the file
        trim_newline(line); // Remove newline characters
        Movie m; // Create a Movie struct
        // Parse the CSV line using strtok
        char* token = strtok(line, ",");
        if (!token) continue; // Skip if line is empty
        m.id = atoi(token); // Parse id
        token = strtok(NULL, ","); strncpy(m.title, token ? token : "", sizeof(m.title));
        token = strtok(NULL, ","); m.vote_average = token ? atof(token) : 0.0f;
        token = strtok(NULL, ","); m.vote_count = token ? atoi(token) : 0;
        token = strtok(NULL, ","); strncpy(m.status, token ? token : "", sizeof(m.status));
        token = strtok(NULL, ","); strncpy(m.release_date, token ? token : "", sizeof(m.release_date));
        token = strtok(NULL, ","); m.revenue = token ? atol(token) : 0;
        token = strtok(NULL, ","); m.runtime = token ? atoi(token) : 0;
        token = strtok(NULL, ","); strncpy(m.adult, token ? token : "", sizeof(m.adult));
        token = strtok(NULL, ","); strncpy(m.backdrop_path, token ? token : "", sizeof(m.backdrop_path));
        token = strtok(NULL, ","); m.budget = token ? atol(token) : 0;
        token = strtok(NULL, ","); strncpy(m.homepage, token ? token : "", sizeof(m.homepage));
        token = strtok(NULL, ","); strncpy(m.imdb_id, token ? token : "", sizeof(m.imdb_id));
        token = strtok(NULL, ","); strncpy(m.original_language, token ? token : "", sizeof(m.original_language));
        token = strtok(NULL, ","); strncpy(m.original_title, token ? token : "", sizeof(m.original_title));
        token = strtok(NULL, ","); strncpy(m.overview, token ? token : "", sizeof(m.overview));
        token = strtok(NULL, ","); m.popularity = token ? atof(token) : 0.0f;
        token = strtok(NULL, ","); strncpy(m.poster_path, token ? token : "", sizeof(m.poster_path));
        token = strtok(NULL, ","); strncpy(m.tagline, token ? token : "", sizeof(m.tagline));
        token = strtok(NULL, ","); strncpy(m.genres, token ? token : "", sizeof(m.genres));
        token = strtok(NULL, ","); strncpy(m.production_companies, token ? token : "", sizeof(m.production_companies));
        token = strtok(NULL, ","); strncpy(m.production_countries, token ? token : "", sizeof(m.production_countries));
        token = strtok(NULL, ","); strncpy(m.spoken_languages, token ? token : "", sizeof(m.spoken_languages));
        token = strtok(NULL, ","); strncpy(m.keywords, token ? token : "", sizeof(m.keywords));
        // Create a new node for this movie
        MovieNode* node = createMovieNode(m); // Create node
        if (!head) { // If list is empty
            head = tail = node; // Set head and tail to this node
        } else {
            tail->next = node; // Link new node after tail
            node->prev = tail; // Link back to previous tail
            tail = node; // Update tail
        }
    }
    fclose(file); // Close the file
    return head; // Return the head of the list
}


// Utility: Case-insensitive substring search (returns 1 if found, 0 if not)
int strcasestr_custom(const char* haystack, const char* needle) {
    if (!*needle) return 1; // Empty needle always matches
    size_t nlen = strlen(needle);
    for (; *haystack; haystack++) {
        if (strncasecmp(haystack, needle, nlen) == 0) return 1;
    }
    return 0;
}

// Function to display all info of a movie (simple, no acciones)
void displayMovie(const Movie* m) {
    printf("\n--- Movie Info ---\n");
    printf("ID: %d\n", m->id);
    printf("Title: %s\n", m->title);
    printf("Vote Average: %.3f\n", m->vote_average);
    printf("Vote Count: %d\n", m->vote_count);
    printf("Status: %s\n", m->status);
    printf("Release Date: %s\n", m->release_date);
    printf("Revenue: %ld\n", m->revenue);
    printf("Runtime: %d\n", m->runtime);
    printf("Adult: %s\n", m->adult);
    printf("Backdrop Path: %s\n", m->backdrop_path);
    printf("Budget: %ld\n", m->budget);
    printf("Homepage: %s\n", m->homepage);
    printf("IMDB ID: %s\n", m->imdb_id);
    printf("Original Language: %s\n", m->original_language);
    printf("Original Title: %s\n", m->original_title);
    printf("Overview: %s\n", m->overview);
    printf("Popularity: %.3f\n", m->popularity);
    printf("Poster Path: %s\n", m->poster_path);
    printf("Tagline: %s\n", m->tagline);
    printf("Genres: %s\n", m->genres);
    printf("Production Companies: %s\n", m->production_companies);
    printf("Production Countries: %s\n", m->production_countries);
    printf("Spoken Languages: %s\n", m->spoken_languages);
    printf("Keywords: %s\n", m->keywords);
}


// Search for a movie by title (substring search)
void searchMovieByTitle(MovieNode* head, const char* title) {
    int found = 0; // Flag to check if any movie is found
    for (MovieNode* curr = head; curr; curr = curr->next) {
        if (strcasestr_custom(curr->data.title, title)) { // Substring search
            displayMovie(&curr->data); // Display movie info
            found = 1;
        }
    }
    if (!found) printf("No movie found with title '%s'\n", title);
}

// Search for a movie by ID
void searchMovieById(MovieNode* head, int id) {
    int found = 0; // Flag to check if any movie is found
    for (MovieNode* curr = head; curr; curr = curr->next) {
        if (curr->data.id == id) { // If the ID matches
            displayMovie(&curr->data); // Display movie info
            found = 1;
        }
    }
    if (!found) printf("No movie found with ID %d\n", id);
}


// Helper function to get a valid integer from user
int getInt(const char* prompt) {
    int value;
    char buf[64];
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin)) {
            char* endptr;
            value = (int)strtol(buf, &endptr, 10);
            if (endptr != buf && *endptr == '\0' || *endptr == '\n') return value;
        }
        printf("Invalid input. Please enter an integer.\n");
    }
}

// Helper function to get a valid long from user
long getLong(const char* prompt) {
    long value;
    char buf[64];
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin)) {
            char* endptr;
            value = strtol(buf, &endptr, 10);
            if (endptr != buf && (*endptr == '\0' || *endptr == '\n')) return value;
        }
        printf("Invalid input. Please enter a long integer.\n");
    }
}

// Helper function to get a valid float from user
float getFloat(const char* prompt) {
    float value;
    char buf[64];
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin)) {
            char* endptr;
            value = strtof(buf, &endptr);
            if (endptr != buf && (*endptr == '\0' || *endptr == '\n')) return value;
        }
        printf("Invalid input. Please enter a number.\n");
    }
}

// Insert a new movie at the beginning, end, or a specific position
void insertMovie(MovieNode** headRef, MovieNode** tailRef) {
    Movie m;
    printf("\n--- Insert New Movie ---\n");
    m.id = getInt("Enter ID: ");
    printf("Enter Title: "); fgets(m.title, sizeof(m.title), stdin); trim_newline(m.title);
    m.vote_average = getFloat("Enter Vote Average: ");
    m.vote_count = getInt("Enter Vote Count: ");
    printf("Enter Status: "); fgets(m.status, sizeof(m.status), stdin); trim_newline(m.status);
    printf("Enter Release Date: "); fgets(m.release_date, sizeof(m.release_date), stdin); trim_newline(m.release_date);
    m.revenue = getLong("Enter Revenue: ");
    m.runtime = getInt("Enter Runtime: ");
    printf("Enter Adult (True/False): "); fgets(m.adult, sizeof(m.adult), stdin); trim_newline(m.adult);
    printf("Enter Backdrop Path: "); fgets(m.backdrop_path, sizeof(m.backdrop_path), stdin); trim_newline(m.backdrop_path);
    m.budget = getLong("Enter Budget: ");
    printf("Enter Homepage: "); fgets(m.homepage, sizeof(m.homepage), stdin); trim_newline(m.homepage);
    printf("Enter IMDB ID: "); fgets(m.imdb_id, sizeof(m.imdb_id), stdin); trim_newline(m.imdb_id);
    printf("Enter Original Language: "); fgets(m.original_language, sizeof(m.original_language), stdin); trim_newline(m.original_language);
    printf("Enter Original Title: "); fgets(m.original_title, sizeof(m.original_title), stdin); trim_newline(m.original_title);
    printf("Enter Overview: "); fgets(m.overview, sizeof(m.overview), stdin); trim_newline(m.overview);
    m.popularity = getFloat("Enter Popularity: ");
    printf("Enter Poster Path: "); fgets(m.poster_path, sizeof(m.poster_path), stdin); trim_newline(m.poster_path);
    printf("Enter Tagline: "); fgets(m.tagline, sizeof(m.tagline), stdin); trim_newline(m.tagline);
    printf("Enter Genres: "); fgets(m.genres, sizeof(m.genres), stdin); trim_newline(m.genres);
    printf("Enter Production Companies: "); fgets(m.production_companies, sizeof(m.production_companies), stdin); trim_newline(m.production_companies);
    printf("Enter Production Countries: "); fgets(m.production_countries, sizeof(m.production_countries), stdin); trim_newline(m.production_countries);
    printf("Enter Spoken Languages: "); fgets(m.spoken_languages, sizeof(m.spoken_languages), stdin); trim_newline(m.spoken_languages);
    printf("Enter Keywords: "); fgets(m.keywords, sizeof(m.keywords), stdin); trim_newline(m.keywords);

    MovieNode* newNode = createMovieNode(m);
    int option, pos, i;
    option = getInt("\nWhere to insert? 1=Beginning, 2=End, 3=Specific position: ");
    if (option == 1) {
        // Insert at beginning
        newNode->next = *headRef;
        if (*headRef) (*headRef)->prev = newNode;
        *headRef = newNode;
        if (!*tailRef) *tailRef = newNode;
    } else if (option == 2) {
        // Insert at end
        newNode->prev = *tailRef;
        if (*tailRef) (*tailRef)->next = newNode;
        *tailRef = newNode;
        if (!*headRef) *headRef = newNode;
    } else if (option == 3) {
        pos = getInt("Enter position (1 = first): ");
        if (pos <= 1 || !*headRef) {
            // Insert at beginning
            newNode->next = *headRef;
            if (*headRef) (*headRef)->prev = newNode;
            *headRef = newNode;
            if (!*tailRef) *tailRef = newNode;
        } else {
            MovieNode* curr = *headRef;
            for (i = 1; i < pos-1 && curr->next; i++) curr = curr->next;
            newNode->next = curr->next;
            newNode->prev = curr;
            if (curr->next) curr->next->prev = newNode;
            curr->next = newNode;
            if (!newNode->next) *tailRef = newNode;
        }
    } else {
        printf("Invalid option. Inserting at end.\n");
        newNode->prev = *tailRef;
        if (*tailRef) (*tailRef)->next = newNode;
        *tailRef = newNode;
        if (!*headRef) *headRef = newNode;
    }
    printf("Movie inserted successfully!\n");
}

// Delete a movie by title or ID
void deleteMovie(MovieNode** headRef, MovieNode** tailRef) {
    int mode;
    mode = getInt("\nDelete by: 1=Title, 2=ID: ");
    MovieNode* curr = *headRef;
    int found = 0;
    if (mode == 1) {
        char title[256];
        printf("Enter title to delete: ");
        fgets(title, sizeof(title), stdin); trim_newline(title);
        while (curr) {
            if (strcasestr_custom(curr->data.title, title)) {
                displayMovie(&curr->data);
                printf("Delete this movie? (y/n): ");
                char confirm = getchar(); getchar();
                if (confirm == 'y' || confirm == 'Y') {
                    found = 1;
                    break;
                }
            }
            curr = curr->next;
        }
    } else if (mode == 2) {
        int id = getInt("Enter ID to delete: ");
        while (curr) {
            if (curr->data.id == id) {
                displayMovie(&curr->data);
                printf("Delete this movie? (y/n): ");
                char confirm = getchar(); getchar();
                if (confirm == 'y' || confirm == 'Y') {
                    found = 1;
                    break;
                }
            }
            curr = curr->next;
        }
    } else {
        printf("Invalid option.\n");
        return;
    }
    if (!found) {
        printf("No movie found or not confirmed for deletion.\n");
        return;
    }
    // Remove node from list
    if (curr->prev) curr->prev->next = curr->next;
    else *headRef = curr->next;
    if (curr->next) curr->next->prev = curr->prev;
    else *tailRef = curr->prev;
    free(curr);
    printf("Movie deleted successfully!\n");
}


// Actualiza directamente el nodo recibido (sin buscar)
void updateMovieDirect(MovieNode* curr) {
    if (!curr) return;
    printf("\nWhich field do you want to update?\n");
    printf("0. Cancel\n1. Title\n2. Vote Average\n3. Vote Count\n4. Status\n5. Release Date\n6. Revenue\n7. Runtime\n8. Adult\n9. Backdrop Path\n10. Budget\n11. Homepage\n12. IMDB ID\n13. Original Language\n14. Original Title\n15. Overview\n16. Popularity\n17. Poster Path\n18. Tagline\n19. Genres\n20. Production Companies\n21. Production Countries\n22. Spoken Languages\n23. Keywords\n");
    int field = getInt("Enter field number to update: ");
    switch (field) {
        case 0:
            printf("Update cancelled.\n");
            return;
        case 1: printf("New Title: "); fgets(curr->data.title, sizeof(curr->data.title), stdin); trim_newline(curr->data.title); break;
        case 2: curr->data.vote_average = getFloat("New Vote Average: "); break;
        case 3: curr->data.vote_count = getInt("New Vote Count: "); break;
        case 4: printf("New Status: "); fgets(curr->data.status, sizeof(curr->data.status), stdin); trim_newline(curr->data.status); break;
        case 5: printf("New Release Date: "); fgets(curr->data.release_date, sizeof(curr->data.release_date), stdin); trim_newline(curr->data.release_date); break;
        case 6: curr->data.revenue = getLong("New Revenue: "); break;
        case 7: curr->data.runtime = getInt("New Runtime: "); break;
        case 8: printf("New Adult (True/False): "); fgets(curr->data.adult, sizeof(curr->data.adult), stdin); trim_newline(curr->data.adult); break;
        case 9: printf("New Backdrop Path: "); fgets(curr->data.backdrop_path, sizeof(curr->data.backdrop_path), stdin); trim_newline(curr->data.backdrop_path); break;
        case 10: curr->data.budget = getLong("New Budget: "); break;
        case 11: printf("New Homepage: "); fgets(curr->data.homepage, sizeof(curr->data.homepage), stdin); trim_newline(curr->data.homepage); break;
        case 12: printf("New IMDB ID: "); fgets(curr->data.imdb_id, sizeof(curr->data.imdb_id), stdin); trim_newline(curr->data.imdb_id); break;
        case 13: printf("New Original Language: "); fgets(curr->data.original_language, sizeof(curr->data.original_language), stdin); trim_newline(curr->data.original_language); break;
        case 14: printf("New Original Title: "); fgets(curr->data.original_title, sizeof(curr->data.original_title), stdin); trim_newline(curr->data.original_title); break;
        case 15: printf("New Overview: "); fgets(curr->data.overview, sizeof(curr->data.overview), stdin); trim_newline(curr->data.overview); break;
        case 16: curr->data.popularity = getFloat("New Popularity: "); break;
        case 17: printf("New Poster Path: "); fgets(curr->data.poster_path, sizeof(curr->data.poster_path), stdin); trim_newline(curr->data.poster_path); break;
        case 18: printf("New Tagline: "); fgets(curr->data.tagline, sizeof(curr->data.tagline), stdin); trim_newline(curr->data.tagline); break;
        case 19: printf("New Genres: "); fgets(curr->data.genres, sizeof(curr->data.genres), stdin); trim_newline(curr->data.genres); break;
        case 20: printf("New Production Companies: "); fgets(curr->data.production_companies, sizeof(curr->data.production_companies), stdin); trim_newline(curr->data.production_companies); break;
        case 21: printf("New Production Countries: "); fgets(curr->data.production_countries, sizeof(curr->data.production_countries), stdin); trim_newline(curr->data.production_countries); break;
        case 22: printf("New Spoken Languages: "); fgets(curr->data.spoken_languages, sizeof(curr->data.spoken_languages), stdin); trim_newline(curr->data.spoken_languages); break;
        case 23: printf("New Keywords: "); fgets(curr->data.keywords, sizeof(curr->data.keywords), stdin); trim_newline(curr->data.keywords); break;
        default: printf("Invalid field number.\n"); return;
    }
    printf("Movie updated successfully!\n");
}

// Update a movie's data after searching by title or ID (mantener para menú clásico)
void updateMovie(MovieNode* head) {
    int mode = getInt("\nUpdate by: 1=Title, 2=ID: ");
    MovieNode* curr = head;
    int found = 0;
    if (mode == 1) {
        char title[256];
        printf("Enter title to update: ");
        fgets(title, sizeof(title), stdin); trim_newline(title);
        while (curr) {
            if (strcasestr_custom(curr->data.title, title)) {
                displayMovie(&curr->data);
                found = 1;
                break;
            }
            curr = curr->next;
        }
    } else if (mode == 2) {
        int id = getInt("Enter ID to update: ");
        while (curr) {
            if (curr->data.id == id) {
                displayMovie(&curr->data);
                found = 1;
                break;
            }
            curr = curr->next;
        }
    } else {
        printf("Invalid option.\n");
        return;
    }
    if (!found) {
        printf("No movie found to update.\n");
        return;
    }
    updateMovieDirect(curr);
}


// Submenú de acciones para una película específica (actualizar, borrar, regresar)
// Devuelve 1 si la película fue borrada, 0 si no
int movieDetailMenu(MovieNode** headRef, MovieNode** tailRef, MovieNode* node) {
    char opt[16];
    while (1) {
        displayMovie(&node->data);
        printf("\nOpciones: [a]ctualizar, [b]orrar, [r]egresar: ");
        if (!fgets(opt, sizeof(opt), stdin)) return 0;
        size_t len = strlen(opt);
        if (len > 0 && opt[len-1] == '\n') opt[len-1] = '\0';
        if (strcmp(opt, "a") == 0 || strcmp(opt, "A") == 0) {
            // Actualizar directamente este nodo
            updateMovieDirect(node);
            printf("\n(Película actualizada)\n");
        } else if (strcmp(opt, "b") == 0 || strcmp(opt, "B") == 0) {
            // Confirmar borrado
            printf("¿Seguro que deseas borrar esta película? (y/n): ");
            char c = getchar();
            while (getchar() != '\n'); // limpiar buffer
            if (c == 'y' || c == 'Y') {
                // Borrar el nodo
                MovieNode* curr = node;
                if (curr->prev) curr->prev->next = curr->next;
                else *headRef = curr->next;
                if (curr->next) curr->next->prev = curr->prev;
                else *tailRef = curr->prev;
                free(curr);
                printf("Película borrada.\n");
                return 1; // borrada
            } else {
                printf("Borrado cancelado.\n");
            }
        } else if (strcmp(opt, "r") == 0 || strcmp(opt, "R") == 0 || strlen(opt) == 0) {
            return 0; // regresar
        } else {
            printf("Opción inválida.\n");
        }
    }
}


// Menu display function
void showMainMenu() {
    printf("\n==== Movie Database Menu ====\n");
    printf("1. Modify List\n");
    printf("2. Search Movie\n");
    printf("3. Display All Movies\n");
    printf("0. Exit\n");
}
// Submenu for modifying the list (insert, delete, update)
void modifyMenu(MovieNode** headRef, MovieNode** tailRef) {
    while (1) {
        printf("\n--- Modify List Menu ---\n");
        printf("1. Insert a new movie\n");
        printf("2. Delete a movie\n");
        printf("3. Update a movie\n");
        printf("0. Return to main menu\n");
        int opt = getInt("Choose an option: ");
        if (opt == 1) {
            insertMovie(headRef, tailRef);
        } else if (opt == 2) {
            deleteMovie(headRef, tailRef);
        } else if (opt == 3) {
            updateMovie(*headRef);
        } else if (opt == 0) {
            break;
        } else {
            printf("Invalid option.\n");
        }
    }
}

void insertMenu(MovieNode** headRef, MovieNode** tailRef) {
    while (1) {
        printf("\n--- Insert Menu ---\n");
        printf("1. Insert a new movie\n");
        printf("0. Return to main menu\n");
        int opt = getInt("Choose an option: ");
        if (opt == 1) {
            insertMovie(headRef, tailRef);
        } else if (opt == 0) {
            break;
        } else {
            printf("Invalid option.\n");
        }
    }
}

void deleteMenu(MovieNode** headRef, MovieNode** tailRef) {
    while (1) {
        printf("\n--- Delete Menu ---\n");
        printf("1. Delete a movie\n");
        printf("0. Return to main menu\n");
        int opt = getInt("Choose an option: ");
        if (opt == 1) {
            deleteMovie(headRef, tailRef);
        } else if (opt == 0) {
            break;
        } else {
            printf("Invalid option.\n");
        }
    }
}

void updateMenu(MovieNode* head) {
    while (1) {
        printf("\n--- Update Menu ---\n");
        printf("1. Update a movie\n");
        printf("0. Return to main menu\n");
        int opt = getInt("Choose an option: ");
        if (opt == 1) {
            updateMovie(head);
        } else if (opt == 0) {
            break;
        } else {
            printf("Invalid option.\n");
        }
    }
}

void searchMenu(MovieNode* head) {
    while (1) {
        printf("\n--- Search Menu ---\n");
        printf("1. Search by title\n");
        printf("2. Search by ID\n");
        printf("0. Return to main menu\n");
        int opt = getInt("Choose an option: ");
        if (opt == 1) {
            char searchTitle[256];
            printf("Enter movie title to search: ");
            fgets(searchTitle, sizeof(searchTitle), stdin);
            trim_newline(searchTitle);
            searchMovieByTitle(head, searchTitle);
        } else if (opt == 2) {
            int searchId = getInt("Enter movie ID to search: ");
            searchMovieById(head, searchId);
        } else if (opt == 0) {
            break;
        } else {
            printf("Invalid option.\n");
        }
    }
}

// Versión mejorada: permite actualizar/borrar desde detalles
void displayAllMovies(MovieNode** headRef, MovieNode** tailRef) {
    const int PAGE_SIZE = 10;
    int total = 0;
    for (MovieNode* curr = *headRef; curr; curr = curr->next) total++;
    if (total == 0) {
        printf("No movies in the list.\n");
        return;
    }
    int page = 0, pages = (total + PAGE_SIZE - 1) / PAGE_SIZE;
    MovieNode* curr;
    char cmd[32];
    while (1) {
        // Recalcular total y páginas por si se borró
        total = 0;
        for (MovieNode* c = *headRef; c; c = c->next) total++;
        if (total == 0) {
            printf("No movies in the list.\n");
            return;
        }
        pages = (total + PAGE_SIZE - 1) / PAGE_SIZE;
        if (page >= pages) page = 0;
        printf("\n--- All Movies (Page %d/%d) ---\n", page + 1, pages);
        printf("| %-6s | %-30s | %-12s | %-9s |\n", "ID", "Title", "Release Date", "Vote Avg");
        printf("|-%-6s-|-%-30s-|-%-12s-|-%-9s-|\n", "------", "------------------------------", "------------", "---------");
        int start = page * PAGE_SIZE, shown = 0, idx = 0;
        curr = *headRef;
        while (curr && idx < start) { curr = curr->next; idx++; }
        while (curr && shown < PAGE_SIZE && curr) {
            printf("| %-6d | %-30.30s | %-12s | %-9.2f |\n",
                curr->data.id,
                curr->data.title,
                curr->data.release_date,
                curr->data.vote_average);
            curr = curr->next;
            shown++;
        }
        printf("\nPage %d of %d. Enter movie ID for details, 'n'=next, 'p'=prev, 'q'=quit: ", page + 1, pages);
        if (!fgets(cmd, sizeof(cmd), stdin)) break;
        size_t len = strlen(cmd);
        if (len > 0 && cmd[len-1] == '\n') cmd[len-1] = '\0';
        if (strcmp(cmd, "n") == 0 || strcmp(cmd, "N") == 0) {
            if (page < pages - 1) {
                page++;
            } else {
                page = 0; // wrap to first page
            }
        } else if (strcmp(cmd, "p") == 0 || strcmp(cmd, "P") == 0) {
            if (page > 0) {
                page--;
            } else {
                page = pages - 1; // wrap to last page
            }
        } else if (strcmp(cmd, "q") == 0 || strcmp(cmd, "Q") == 0 || strlen(cmd) == 0) {
            break;
        } else {
            // Try to parse as ID
            int id = atoi(cmd);
            if (id > 0) {
                MovieNode* search = *headRef;
                while (search) {
                    if (search->data.id == id) {
                        // Submenú de detalles: si se borra, salir a menú modificar
                        int deleted = movieDetailMenu(headRef, tailRef, search);
                        if (deleted) return; // salir a menú modificar
                        break;
                    }
                    search = search->next;
                }
                if (!search) printf("No movie found with ID %d.\n", id);
                printf("\nPresiona Enter para regresar a la lista...");
                fgets(cmd, sizeof(cmd), stdin);
            } else {
                printf("Invalid input. Enter a valid movie ID, 'n', 'p', or 'q'.\n");
            }
        }
    }
}

// Example main to test search by title
int main() {
    // Load movies from CSV file
    MovieNode* movieList = loadMoviesFromCSV("Movies.csv"); // Load data
    MovieNode* tail = movieList;
    if (!movieList) {
        printf("No movies loaded.\n");
        return 1;
    }
    // Find tail
    while (tail && tail->next) tail = tail->next;

    int running = 1;
    while (running) {
        showMainMenu();
        int choice = getInt("Choose an option: ");
        switch (choice) {
            case 1:
                modifyMenu(&movieList, &tail);
                break;
            case 2:
                searchMenu(movieList);
                break;
            case 3:
                displayAllMovies(&movieList, &tail);
                break;
            case 0:
                running = 0;
                break;
            default:
                printf("Invalid option.\n");
        }
    }

    // Free the list after use
    MovieNode* curr = movieList;
    while (curr) {
        MovieNode* next = curr->next;
        free(curr);
        curr = next;
    }

    printf("Exiting program.\n");
    return 0;
}


