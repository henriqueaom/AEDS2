#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 20
#define MAX_SHOWS 1000
#define MAX_INPUTS 100

typedef struct {
    char* showId;
    char* title;
} Show;

char** parseLineCSV(const char* line, int* fieldCount) {
    char** fields = (char**)malloc(MAX_FIELDS * sizeof(char*));
    char buffer[MAX_LINE_LENGTH];
    int bufferIndex = 0;
    int fieldIndex = 0;
    bool inQuotes = false;

    for (int i = 0; line[i] != '\0' && fieldIndex < MAX_FIELDS; i++) {
        if (line[i] == '"') {
            inQuotes = !inQuotes;
        } else if (line[i] == ',' && !inQuotes) {
            buffer[bufferIndex] = '\0';
            fields[fieldIndex] = strdup(buffer);
            fieldIndex++;
            bufferIndex = 0;
        } else {
            buffer[bufferIndex++] = line[i];
        }
    }

    // Add the last field
    buffer[bufferIndex] = '\0';
    fields[fieldIndex] = strdup(buffer);
    fieldIndex++;

    *fieldCount = fieldIndex;
    return fields;
}

Show* readCSV(const char* path, int* showCount) {
    FILE* file = fopen(path, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    Show* shows = (Show*)malloc(MAX_SHOWS * sizeof(Show));
    char line[MAX_LINE_LENGTH];
    *showCount = 0;

    // Skip header
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && *showCount < MAX_SHOWS) {
        int fieldCount;
        char** fields = parseLineCSV(line, &fieldCount);

        if (fieldCount >= 3) {
            shows[*showCount].showId = strdup(fields[0]);
            shows[*showCount].title = strdup(fields[2]);
            (*showCount)++;
        }

        for (int i = 0; i < fieldCount; i++) {
            free(fields[i]);
        }
        free(fields);
    }

    fclose(file);
    return shows;
}

Show* findShowById(Show* shows, int showCount, const char* id) {
    for (int i = 0; i < showCount; i++) {
        if (shows[i].showId && strcmp(shows[i].showId, id) == 0) {
            return &shows[i];
        }
    }
    return NULL;
}

void freeShows(Show* shows, int showCount) {
    for (int i = 0; i < showCount; i++) {
        free(shows[i].showId);
        free(shows[i].title);
    }
    free(shows);
}

int main() {
    int showCount;
    Show* shows = readCSV("/tmp/disneyplus.csv", &showCount);
    if (!shows) {
        return 1;
    }

    // Ler todos os IDs de entrada primeiro
    char inputIds[MAX_INPUTS][50];
    int idCount = 0;

    while (1) {
        char input[50];
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "FIM") == 0) {
            break;
        }

        if (strlen(input) > 0 && idCount < MAX_INPUTS) {
            strcpy(inputIds[idCount], input);
            idCount++;
        }
    }

    // Agora ler os títulos para pesquisa
    char searchTitles[MAX_INPUTS][100];
    int titleCount = 0;

    while (1) {
        char input[100];
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "FIM") == 0) {
            break;
        }

        if (strlen(input) > 0 && titleCount < MAX_INPUTS) {
            strcpy(searchTitles[titleCount], input);
            titleCount++;
        }
    }

    // Verificar cada título nos shows correspondentes aos IDs de entrada
    for (int i = 0; i < titleCount; i++) {
        bool found = false;
        
        for (int j = 0; j < idCount; j++) {
            Show* show = findShowById(shows, showCount, inputIds[j]);
            if (show && show->title && strcasecmp(show->title, searchTitles[i]) == 0) {
                found = true;
                break;
            }
        }
        
        printf("%s\n", found ? "SIM" : "NAO");
    }

    freeShows(shows, showCount);
    return 0;
}