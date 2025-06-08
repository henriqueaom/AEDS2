#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 12
#define MAX_CAST 100

typedef struct {
    char showId[100];
    char type[100];
    char title[100];
    char director[100];
    char cast[MAX_CAST][100];
    int castSize;
    char country[100];
    struct tm dateAdded;
    int releaseYear;  // Campo principal para ordenação
    char rating[100];
    char duration[100];
    char listedIn[100];
    char description[1000];
} Show;

void parseLinhaCSV(const char *linha, char campos[][1000]) {
    bool emAspas = false;
    int campoAtual = 0;
    int pos = 0;
    
    for (int i = 0; i < strlen(linha); i++) {
        char c = linha[i];
        if (c == '\"') {
            emAspas = !emAspas;
        } else if (c == ',' && !emAspas) {
            campos[campoAtual][pos] = '\0';
            campoAtual++;
            pos = 0;
        } else {
            campos[campoAtual][pos++] = c;
        }
    }
    campos[campoAtual][pos] = '\0';
}

const char* getOrNaN(char campos[][1000], int index) {
    if (index >= MAX_FIELDS || strlen(campos[index]) == 0) {
        return "NaN";
    }
    return campos[index];
}

void parseDate(const char* dateStr, struct tm* date) {
    memset(date, 0, sizeof(struct tm));
    if (strcmp(dateStr, "NaN") == 0) {
        return;
    }
    
    char month[20];
    int day, year;
    sscanf(dateStr, "%s %d, %d", month, &day, &year);
    
    const char* months[] = {"January", "February", "March", "April", "May", "June", 
                           "July", "August", "September", "October", "November", "December"};
    
    for (int i = 0; i < 12; i++) {
        if (strcmp(month, months[i]) == 0) {
            date->tm_mon = i;
            break;
        }
    }
    date->tm_mday = day;
    date->tm_year = year - 1900;
}

void parseCast(const char* castStr, Show* show) {
    show->castSize = 0;
    if (strcmp(castStr, "NaN") == 0) {
        return;
    }
    
    char temp[1000];
    strcpy(temp, castStr);
    char* token = strtok(temp, ",");
    
    while (token != NULL && show->castSize < MAX_CAST) {
        // Remover espaços em branco no início e fim
        while (isspace(*token)) token++;
        int len = strlen(token);
        while (len > 0 && isspace(token[len-1])) {
            token[--len] = '\0';
        }
        
        strcpy(show->cast[show->castSize++], token);
        token = strtok(NULL, ",");
    }
}

void ler(Show* show, const char* linha) {
    char campos[MAX_FIELDS][1000];
    parseLinhaCSV(linha, campos);
    
    strcpy(show->showId, getOrNaN(campos, 0));
    strcpy(show->type, getOrNaN(campos, 1));
    strcpy(show->title, getOrNaN(campos, 2));
    strcpy(show->director, getOrNaN(campos, 3));
    
    parseCast(getOrNaN(campos, 4), show);
    
    strcpy(show->country, getOrNaN(campos, 5));
    
    parseDate(getOrNaN(campos, 6), &show->dateAdded);
    
    // Campo releaseYear (campo 7 no CSV)
    const char* yearStr = getOrNaN(campos, 7);
    show->releaseYear = strcmp(yearStr, "NaN") == 0 ? 0 : atoi(yearStr);
    
    strcpy(show->rating, getOrNaN(campos, 8));
    strcpy(show->duration, getOrNaN(campos, 9));
    strcpy(show->listedIn, getOrNaN(campos, 10));
    strcpy(show->description, getOrNaN(campos, 11));
}

int compareCast(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

void formatar(const Show* show, char* output) {
    // Criar cópia ordenada do elenco
    char* castOrdenado[MAX_CAST];
    for (int i = 0; i < show->castSize; i++) {
        castOrdenado[i] = strdup(show->cast[i]);
    }
    qsort(castOrdenado, show->castSize, sizeof(char*), compareCast);
    
    // Construir string do elenco
    char castStr[1000] = "";
    for (int i = 0; i < show->castSize; i++) {
        if (i > 0) strcat(castStr, ", ");
        strcat(castStr, castOrdenado[i]);
        free(castOrdenado[i]);
    }
    if (show->castSize == 0) {
        strcpy(castStr, "NaN");
    }
    
    // Formatar data de adição
    char dataStr[100];
    if (show->dateAdded.tm_year == 0 && show->dateAdded.tm_mon == 0 && show->dateAdded.tm_mday == 0) {
        strcpy(dataStr, "NaN");
    } else {
        const char* months[] = {"January", "February", "March", "April", "May", "June", 
                               "July", "August", "September", "October", "November", "December"};
        sprintf(dataStr, "%s %d, %d", months[show->dateAdded.tm_mon], 
                show->dateAdded.tm_mday, show->dateAdded.tm_year + 1900);
    }
    
    sprintf(output, "=> %s ## %s ## %s ## %s ## [%s] ## %s ## %s ## %d ## %s ## %s ## [%s] ##",
            show->showId,
            show->title,
            show->type,
            show->director,
            castStr,
            show->country,
            dataStr,
            show->releaseYear,  // Ano de lançamento incluído na saída
            show->rating,
            show->duration,
            show->listedIn);
}

char** lerCSV(const char* caminho, int* numLinhas) {
    FILE* file = fopen(caminho, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }
    
    char** linhas = NULL;
    *numLinhas = 0;
    char buffer[MAX_LINE_LENGTH];
    
    // Pular cabeçalho
    fgets(buffer, sizeof(buffer), file);
    
    while (fgets(buffer, sizeof(buffer), file)) {
        // Remover nova linha
        buffer[strcspn(buffer, "\n")] = '\0';
        
        linhas = realloc(linhas, (*numLinhas + 1) * sizeof(char*));
        linhas[*numLinhas] = strdup(buffer);
        (*numLinhas)++;
    }
    
    fclose(file);
    return linhas;
}

int compareShowsByReleaseYearThenTitle(const void* a, const void* b) {
    const Show* showA = *(const Show**)a;
    const Show* showB = *(const Show**)b;
    
    // Primeiro compara por ano de lançamento
    if (showA->releaseYear != showB->releaseYear) {
        return showA->releaseYear - showB->releaseYear;
    }
    
    // Se anos forem iguais, compara por título
    return strcmp(showA->title, showB->title);
}

int main() {
    int numLinhas;
    char** linhas = lerCSV("/tmp/disneyplus.csv", &numLinhas);
    if (!linhas) {
        return 1;
    }
    
    Show** shows = malloc(numLinhas * sizeof(Show*));
    for (int i = 0; i < numLinhas; i++) {
        shows[i] = malloc(sizeof(Show));
        ler(shows[i], linhas[i]);
        free(linhas[i]);
    }
    free(linhas);
    
    // Criar mapa de shows por ID
    Show** mapaShows = malloc(numLinhas * sizeof(Show*));
    for (int i = 0; i < numLinhas; i++) {
        mapaShows[i] = shows[i];
    }
    
    // Ler entradas
    Show** showsSelecionados = NULL;
    int numSelecionados = 0;
    char entrada[100];
    
    while (1) {
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';
        
        if (strcmp(entrada, "FIM") == 0) {
            break;
        }
        
        // Buscar show pelo ID
        for (int i = 0; i < numLinhas; i++) {
            if (strcmp(mapaShows[i]->showId, entrada) == 0) {
                showsSelecionados = realloc(showsSelecionados, (numSelecionados + 1) * sizeof(Show*));
                showsSelecionados[numSelecionados++] = mapaShows[i];
                break;
            }
        }
    }
    
    // Ordenar shows por ano de lançamento e depois por título
    qsort(showsSelecionados, numSelecionados, sizeof(Show*), compareShowsByReleaseYearThenTitle);
    
    // Imprimir resultados
    char output[2000];
    for (int i = 0; i < numSelecionados; i++) {
        formatar(showsSelecionados[i], output);
        printf("%s\n", output);
    }
    
    // Liberar memória
    for (int i = 0; i < numLinhas; i++) {
        free(shows[i]);
    }
    free(shows);
    free(mapaShows);
    free(showsSelecionados);
    
    return 0;
}