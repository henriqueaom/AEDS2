#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h> // For date parsing

// --- Show Structure ---
typedef struct {
    char showId[20];
    char type[20];
    char title[200];
    char director[200];
    char** cast; // Array of strings for cast
    int numCast;
    char country[100];
    time_t dateAdded; // epoch time for date
    int releaseYear;
    char rating[20];
    char duration[50];
    char listedIn[200];
    char description[500];
} Show;

// --- Global CSV Data Storage (HashMap equivalent) ---
// Using a simple array for demonstration, a hash map would be more efficient for large datasets
Show* allShows = NULL;
int numAllShows = 0;
int capacityAllShows = 10000; // Initial capacity for allShows

// Function to initialize a Show object
void initShow(Show* s) {
    s->cast = NULL;
    s->numCast = 0;
    s->dateAdded = 0;
    s->releaseYear = 0;
    // Initialize string fields to empty or "NaN" as appropriate during parsing
    strcpy(s->showId, "NaN");
    strcpy(s->type, "NaN");
    strcpy(s->title, "NaN");
    strcpy(s->director, "NaN");
    strcpy(s->country, "NaN");
    strcpy(s->rating, "NaN");
    strcpy(s->duration, "NaN");
    strcpy(s->listedIn, "NaN");
    strcpy(s->description, "NaN");
}

// Function to free memory allocated for a Show object
void freeShow(Show* s) {
    if (s->cast != NULL) {
        for (int i = 0; i < s->numCast; i++) {
            free(s->cast[i]);
        }
        free(s->cast);
        s->cast = NULL;
    }
}

// Helper to get field or "NaN"
char* getOrNaN(char* field) {
    if (field == NULL || strlen(field) == 0 || (strlen(field) == 1 && field[0] == '\r')) {
        return "NaN";
    }
    // Remove leading/trailing quotes if present (for parsed fields)
    char* cleanedField = strdup(field);
    if (cleanedField[0] == '\"' && cleanedField[strlen(cleanedField) - 1] == '\"') {
        cleanedField[strlen(cleanedField) - 1] = '\0'; // Remove last quote
        memmove(cleanedField, cleanedField + 1, strlen(cleanedField)); // Remove first quote
    }
    return cleanedField;
}

// Custom CSV parsing function
void parseLinhaCSV(char* linha, char** campos, int* numCampos) {
    bool inQuotes = false;
    int campoIdx = 0;
    int charIdx = 0;
    char temp[1000]; // Temporary buffer for each field

    for (int i = 0; linha[i] != '\0'; i++) {
        if (linha[i] == '\"') {
            inQuotes = !inQuotes;
        } else if (linha[i] == ',' && !inQuotes) {
            temp[charIdx] = '\0';
            campos[campoIdx] = strdup(temp);
            campoIdx++;
            charIdx = 0;
            if (campoIdx >= 12) break; // Prevent buffer overflow for campos array
        } else {
            temp[charIdx++] = linha[i];
        }
    }
    temp[charIdx] = '\0';
    campos[campoIdx] = strdup(temp);
    *numCampos = campoIdx + 1;
}

// Function to parse a date string (e.g., "September 24, 2021")
time_t parseDate(const char* dateStr) {
    struct tm tm_obj = {0};
    char monthStr[20];
    int day, year;

    // Use sscanf to parse month, day, and year
    if (sscanf(dateStr, "%s %d, %d", monthStr, &day, &year) != 3) {
        return 0; // Invalid date format
    }

    // Convert month string to month number
    if (strcmp(monthStr, "January") == 0) tm_obj.tm_mon = 0;
    else if (strcmp(monthStr, "February") == 0) tm_obj.tm_mon = 1;
    else if (strcmp(monthStr, "March") == 0) tm_obj.tm_mon = 2;
    else if (strcmp(monthStr, "April") == 0) tm_obj.tm_mon = 3;
    else if (strcmp(monthStr, "May") == 0) tm_obj.tm_mon = 4;
    else if (strcmp(monthStr, "June") == 0) tm_obj.tm_mon = 5;
    else if (strcmp(monthStr, "July") == 0) tm_obj.tm_mon = 6;
    else if (strcmp(monthStr, "August") == 0) tm_obj.tm_mon = 7;
    else if (strcmp(monthStr, "September") == 0) tm_obj.tm_mon = 8;
    else if (strcmp(monthStr, "October") == 0) tm_obj.tm_mon = 9;
    else if (strcmp(monthStr, "November") == 0) tm_obj.tm_mon = 10;
    else if (strcmp(monthStr, "December") == 0) tm_obj.tm_mon = 11;
    else return 0; // Invalid month

    tm_obj.tm_mday = day;
    tm_obj.tm_year = year - 1900; // tm_year is years since 1900

    return mktime(&tm_obj);
}


// Function to read a line and populate a Show object
void lerShow(Show* s, char* linha) {
    initShow(s); // Initialize show with default values

    char* campos[12]; // Max 12 fields
    int numCampos;
    parseLinhaCSV(linha, campos, &numCampos);

    // Assign fields, handling "NaN" and parsing as needed
    strcpy(s->showId, getOrNaN(campos[0]));
    strcpy(s->type, getOrNaN(campos[1]));
    strcpy(s->title, getOrNaN(campos[2]));
    strcpy(s->director, getOrNaN(campos[3]));

    // Handle cast
    char* elenco = getOrNaN(campos[4]);
    if (strcmp(elenco, "NaN") != 0) {
        char* tempElenco = strdup(elenco);
        char* token = strtok(tempElenco, ",");
        s->numCast = 0;
        while (token != NULL) {
            s->numCast++;
            s->cast = (char**)realloc(s->cast, s->numCast * sizeof(char*));
            s->cast[s->numCast - 1] = strdup(token);
            // Trim whitespace from cast members
            char* trimmed = s->cast[s->numCast - 1];
            while (*trimmed == ' ') trimmed++; // Trim leading spaces
            s->cast[s->numCast - 1] = trimmed;
            token = strtok(NULL, ",");
        }
        free(tempElenco);
    }

    strcpy(s->country, getOrNaN(campos[5]));

    char* dateStr = getOrNaN(campos[6]);
    if (strcmp(dateStr, "NaN") != 0) {
        s->dateAdded = parseDate(dateStr);
    } else {
        s->dateAdded = 0; // Or some default invalid date value
    }

    char* releaseYearStr = getOrNaN(campos[7]);
    if (strcmp(releaseYearStr, "NaN") != 0) {
        s->releaseYear = atoi(releaseYearStr);
    } else {
        s->releaseYear = 0;
    }

    strcpy(s->rating, getOrNaN(campos[8]));
    strcpy(s->duration, getOrNaN(campos[9]));
    strcpy(s->listedIn, getOrNaN(campos[10]));
    strcpy(s->description, getOrNaN(campos[11]));

    // Free the temporary fields allocated by parseLinhaCSV
    for (int i = 0; i < numCampos; i++) {
        free(campos[i]);
    }
}

// Comparison function for qsort (sorting cast members)
int compareStrings(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

// Function to format and print a Show object
void formatarShow(const Show* s) {
    printf("=> %s ## %s ## %s ## %s ## [", s->showId, s->title, s->type, s->director);

    // Sort and print cast
    if (s->numCast > 0) {
        char** sortedCast = (char**)malloc(s->numCast * sizeof(char*));
        for (int i = 0; i < s->numCast; i++) {
            sortedCast[i] = s->cast[i];
        }
        qsort(sortedCast, s->numCast, sizeof(char*), compareStrings);

        for (int i = 0; i < s->numCast; i++) {
            printf("%s%s", sortedCast[i], (i == s->numCast - 1) ? "" : ", ");
        }
        free(sortedCast);
    } else {
        printf("NaN");
    }
    printf("] ## %s ## ", s->country);

    // Format dateAdded
    if (s->dateAdded != 0) {
        char buffer[80];
        struct tm* tm_info = localtime(&s->dateAdded);
        strftime(buffer, sizeof(buffer), "%B %d, %Y", tm_info);
        printf("%s", buffer);
    } else {
        printf("NaN"); // Or some default date format for 0
    }

    printf(" ## %d ## %s ## %s ## [%s] ##\n",
           s->releaseYear, s->rating, s->duration, s->listedIn);
}

// --- ListaShow Structure (Dynamic Array Implementation) ---
typedef struct {
    Show* array;
    int n; // Current number of elements
    int capacity; // Maximum capacity
} ListaShow;

// Function to initialize ListaShow
void initListaShow(ListaShow* lista, int tamanho) {
    lista->array = (Show*)malloc(tamanho * sizeof(Show));
    if (lista->array == NULL) {
        fprintf(stderr, "Memory allocation failed for ListaShow\n");
        exit(EXIT_FAILURE);
    }
    lista->n = 0;
    lista->capacity = tamanho;
}

// Function to free memory used by ListaShow
void freeListaShow(ListaShow* lista) {
    for (int i = 0; i < lista->n; i++) {
        freeShow(&lista->array[i]);
    }
    free(lista->array);
    lista->array = NULL;
    lista->n = 0;
    lista->capacity = 0;
}

// InserirInicio: Inserts a show at the beginning
void inserirInicio(ListaShow* lista, Show show) {
    if (lista->n >= lista->capacity) {
        fprintf(stderr, "Error: Lista cheia (list full) - cannot insert at beginning.\n");
        return;
    }
    for (int i = lista->n; i > 0; i--) {
        lista->array[i] = lista->array[i-1];
    }
    lista->array[0] = show;
    lista->n++;
}

// InserirFim: Inserts a show at the end
void inserirFim(ListaShow* lista, Show show) {
    if (lista->n >= lista->capacity) {
        fprintf(stderr, "Error: Lista cheia (list full) - cannot insert at end.\n");
        return;
    }
    lista->array[lista->n++] = show;
}

// Inserir: Inserts a show at a specific position
void inserir(ListaShow* lista, Show show, int pos) {
    if (lista->n >= lista->capacity || pos < 0 || pos > lista->n) {
        fprintf(stderr, "Error: Posicao invalida ou lista cheia (invalid position or list full) - cannot insert at %d.\n", pos);
        return;
    }
    for (int i = lista->n; i > pos; i--) {
        lista->array[i] = lista->array[i-1];
    }
    lista->array[pos] = show;
    lista->n++;
}

// RemoverInicio: Removes and returns the first show
Show removerInicio(ListaShow* lista) {
    if (lista->n == 0) {
        fprintf(stderr, "Error: Lista vazia (list empty) - cannot remove from beginning.\n");
        // Return an empty/invalid Show to indicate error
        Show emptyShow;
        initShow(&emptyShow);
        return emptyShow;
    }
    Show resp = lista->array[0];
    for (int i = 0; i < lista->n - 1; i++) {
        lista->array[i] = lista->array[i+1];
    }
    lista->n--;
    return resp;
}

// RemoverFim: Removes and returns the last show
Show removerFim(ListaShow* lista) {
    if (lista->n == 0) {
        fprintf(stderr, "Error: Lista vazia (list empty) - cannot remove from end.\n");
        Show emptyShow;
        initShow(&emptyShow);
        return emptyShow;
    }
    lista->n--;
    return lista->array[lista->n];
}

// Remover: Removes and returns the show at a specific position
Show remover(ListaShow* lista, int pos) {
    if (lista->n == 0 || pos < 0 || pos >= lista->n) {
        fprintf(stderr, "Error: Posicao invalida ou lista vazia (invalid position or list empty) - cannot remove from %d.\n", pos);
        Show emptyShow;
        initShow(&emptyShow);
        return emptyShow;
    }
    Show resp = lista->array[pos];
    for (int i = pos; i < lista->n - 1; i++) {
        lista->array[i] = lista->array[i+1];
    }
    lista->n--;
    return resp;
}

// Mostrar: Displays all shows in the list
void mostrarLista(const ListaShow* lista) {
    for (int i = 0; i < lista->n; i++) {
        formatarShow(&lista->array[i]);
    }
}

// --- CSV Reading (Global Map Equivalent) ---
// In C, a simple array and linear search will be used as a HashMap equivalent for small data.
// For large datasets, a hash table implementation would be necessary.

void lerCSV(const char* caminho) {
    FILE* file = fopen(caminho, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char linha[1024];
    fgets(linha, sizeof(linha), file); // Skip header

    allShows = (Show*)malloc(capacityAllShows * sizeof(Show));
    if (allShows == NULL) {
        fprintf(stderr, "Memory allocation failed for allShows\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(linha, sizeof(linha), file) != NULL) {
        if (numAllShows >= capacityAllShows) {
            capacityAllShows *= 2;
            allShows = (Show*)realloc(allShows, capacityAllShows * sizeof(Show));
            if (allShows == NULL) {
                fprintf(stderr, "Memory reallocation failed for allShows\n");
                exit(EXIT_FAILURE);
            }
        }
        lerShow(&allShows[numAllShows], linha);
        numAllShows++;
    }
    fclose(file);
}

// Helper function to find a Show by showId (mimics map.get(id))
Show* findShowById(const char* showId) {
    for (int i = 0; i < numAllShows; i++) {
        if (strcmp(allShows[i].showId, showId) == 0) {
            return &allShows[i];
        }
    }
    return NULL; // Not found
}

// Function to read a specific line based on showId (similar to lerLinhaPorArquivo in Java)
// This function is still needed to parse individual lines from CSV for insertion commands
// and create *new* Show objects for them, rather than directly using the global `allShows`.
// This is to simulate the Java behavior where `new Show().ler(lerLinhaPorArquivo(arquivo))`
// creates a distinct object for each insertion.
char* lerLinhaPorArquivo(const char* showId) {
    FILE* file = fopen("/tmp/disneyplus.csv", "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL; // Indicate error
    }

    char linha[1024];
    fgets(linha, sizeof(linha), file); // Skip header

    char* foundLine = NULL;
    while (fgets(linha, sizeof(linha), file) != NULL) {
        char tempShowId[20];
        sscanf(linha, "%19[^,]", tempShowId); // Read up to the first comma

        if (strcmp(tempShowId, showId) == 0) {
            foundLine = strdup(linha); // Duplicate the line to return
            break;
        }
    }
    fclose(file);
    return foundLine;
}


int main() {
    // Set locale for date formatting if needed, though not strictly required for this problem.
    // setlocale(LC_ALL, "en_US.UTF-8");

    // Load all shows from CSV once
    lerCSV("/tmp/disneyplus.csv");

    ListaShow lista;
    initListaShow(&lista, 1000); // Initialize list with capacity

    char id[20];
    // First part: read IDs and add to the list
    while (scanf("%19s", id) == 1 && strcmp(id, "FIM") != 0) {
        Show* s = findShowById(id);
        if (s != NULL) {
            inserirFim(&lista, *s); // Insert a copy of the show
        }
    }

    int n_commands;
    scanf("%d", &n_commands);
    getchar(); // Consume the newline character after reading n_commands

    for (int i = 0; i < n_commands; i++) {
        char commandLine[256];
        fgets(commandLine, sizeof(commandLine), stdin);
        commandLine[strcspn(commandLine, "\n")] = 0; // Remove newline

        char cmd[5];
        char arg1[20]; // For showId or position
        char arg2[20]; // For showId (in I* case)

        sscanf(commandLine, "%s", cmd);

        if (strcmp(cmd, "II") == 0) {
            sscanf(commandLine, "%*s %s", arg1);
            char* lineFromFile = lerLinhaPorArquivo(arg1);
            if (lineFromFile != NULL) {
                Show newShow;
                lerShow(&newShow, lineFromFile);
                inserirInicio(&lista, newShow);
                free(lineFromFile);
            } else {
                fprintf(stderr, "Error: Show '%s' not found for insertion.\n", arg1);
            }
        } else if (strcmp(cmd, "IF") == 0) {
            sscanf(commandLine, "%*s %s", arg1);
            char* lineFromFile = lerLinhaPorArquivo(arg1);
            if (lineFromFile != NULL) {
                Show newShow;
                lerShow(&newShow, lineFromFile);
                inserirFim(&lista, newShow);
                free(lineFromFile);
            } else {
                fprintf(stderr, "Error: Show '%s' not found for insertion.\n", arg1);
            }
        } else if (strcmp(cmd, "I*") == 0) {
            int pos;
            sscanf(commandLine, "%*s %d %s", &pos, arg2);
            char* lineFromFile = lerLinhaPorArquivo(arg2);
            if (lineFromFile != NULL) {
                Show newShow;
                lerShow(&newShow, lineFromFile);
                inserir(&lista, newShow, pos);
                free(lineFromFile);
            } else {
                fprintf(stderr, "Error: Show '%s' not found for insertion.\n", arg2);
            }
        } else if (strcmp(cmd, "RI") == 0) {
            Show removed = removerInicio(&lista);
            printf("(R) %s\n", removed.title);
            freeShow(&removed); // Free memory associated with the removed show
        } else if (strcmp(cmd, "RF") == 0) {
            Show removed = removerFim(&lista);
            printf("(R) %s\n", removed.title);
            freeShow(&removed); // Free memory associated with the removed show
        } else if (strcmp(cmd, "R*") == 0) {
            int pos;
            sscanf(commandLine, "%*s %d", &pos);
            Show removed = remover(&lista, pos);
            printf("(R) %s\n", removed.title);
            freeShow(&removed); // Free memory associated with the removed show
        }
    }

    mostrarLista(&lista);

    // Clean up
    freeListaShow(&lista);
    if (allShows != NULL) {
        for (int i = 0; i < numAllShows; i++) {
            freeShow(&allShows[i]);
        }
        free(allShows);
    }

    return 0;
}