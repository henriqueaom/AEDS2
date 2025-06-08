#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX_SHOWS 5000
#define MAX_CAST_MEMBERS 50
#define MAX_CAST_NAME 100

typedef struct {
    char show_id[20];
    char type[50];
    char title[200];
    char director[200];
    char cast[600];
    char country[100];
    char date_added[50];
    int release_year;
    char rating[10];
    char duration[50];
    char listed_in[200];
    char description[500];
} Show;

Show shows[MAX_SHOWS];
int total_shows = 0;

void remover_aspas(char *str) {
    int len = strlen(str);
    if (len > 1 && str[0] == '"' && str[len - 1] == '"') {
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}

void substituir_vazio(char *str) {
    if (strlen(str) == 0) {
        strcpy(str, "NaN");
    }
}

void limpar_string(char *str) {
    remover_aspas(str);
    
    // Trim leading spaces
    while (isspace(*str)) str++;
    
    // Trim trailing spaces
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    *(end + 1) = '\0';
    
    if (strlen(str) == 0) {
        strcpy(str, "NaN");
    }
}

int comparar_strings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void ordenar_e_limpar_cast(Show *s) {
    if (strcmp(s->cast, "NaN") == 0) {
        strcpy(s->cast, "NaN");
        return;
    }

    char *members[MAX_CAST_MEMBERS];
    int count = 0;
    char temp[600];
    strcpy(temp, s->cast);

    // Separar e limpar os membros do elenco
    char *token = strtok(temp, ",");
    while (token != NULL && count < MAX_CAST_MEMBERS) {
        // Limpar cada membro individualmente
        while (isspace(*token)) token++;
        char *end = token + strlen(token) - 1;
        while (end > token && isspace(*end)) end--;
        *(end + 1) = '\0';
        remover_aspas(token);

        members[count] = malloc(strlen(token) + 1);
        strcpy(members[count], token);
        count++;
        token = strtok(NULL, ",");
    }

    // Ordenar os membros
    qsort(members, count, sizeof(char *), comparar_strings);

    // Reconstruir a string do elenco
    strcpy(s->cast, "");
    for (int i = 0; i < count; i++) {
        if (i > 0) strcat(s->cast, ", ");
        strcat(s->cast, members[i]);
        free(members[i]);
    }
}

void limpar_listed_in(Show *s) {
    if (strcmp(s->listed_in, "NaN") == 0) return;

    char temp[200] = "";
    char *token = strtok(s->listed_in, ",");
    while (token != NULL) {
        // Limpar cada categoria individualmente
        while (isspace(*token)) token++;
        char *end = token + strlen(token) - 1;
        while (end > token && isspace(*end)) end--;
        *(end + 1) = '\0';
        remover_aspas(token);

        if (strlen(temp) > 0) strcat(temp, ", ");
        strcat(temp, token);
        token = strtok(NULL, ",");
    }
    strncpy(s->listed_in, temp, sizeof(s->listed_in) - 1);
}

void parse_line(char *linha, Show *s) {
    char *fields[12];
    int field_count = 0;
    char *start = linha;
    int in_quotes = 0;
    
    // Initialize all fields
    memset(s, 0, sizeof(Show));
    
    // Split line into fields
    for (char *c = linha; *c; c++) {
        if (*c == '"') {
            in_quotes = !in_quotes;
        } else if (*c == ',' && !in_quotes) {
            *c = '\0';
            fields[field_count++] = start;
            start = c + 1;
            if (field_count >= 11) break;
        }
    }
    fields[field_count++] = start;
    
    // Assign fields to struct
    if (field_count > 0) strncpy(s->show_id, fields[0], sizeof(s->show_id) - 1);
    if (field_count > 1) strncpy(s->type, fields[1], sizeof(s->type) - 1);
    if (field_count > 2) strncpy(s->title, fields[2], sizeof(s->title) - 1);
    if (field_count > 3) strncpy(s->director, fields[3], sizeof(s->director) - 1);
    if (field_count > 4) strncpy(s->cast, fields[4], sizeof(s->cast) - 1);
    if (field_count > 5) strncpy(s->country, fields[5], sizeof(s->country) - 1);
    if (field_count > 6) strncpy(s->date_added, fields[6], sizeof(s->date_added) - 1);
    if (field_count > 7) s->release_year = atoi(fields[7]);
    if (field_count > 8) strncpy(s->rating, fields[8], sizeof(s->rating) - 1);
    if (field_count > 9) strncpy(s->duration, fields[9], sizeof(s->duration) - 1);
    if (field_count > 10) strncpy(s->listed_in, fields[10], sizeof(s->listed_in) - 1);
    if (field_count > 11) strncpy(s->description, fields[11], sizeof(s->description) - 1);
    
    // Clean all fields
    limpar_string(s->show_id);
    limpar_string(s->type);
    limpar_string(s->title);
    limpar_string(s->director);
    limpar_string(s->country);
    limpar_string(s->date_added);
    limpar_string(s->rating);
    limpar_string(s->duration);
    limpar_string(s->description);
    
    // Process cast - ordenar e limpar membros
    ordenar_e_limpar_cast(s);
    
    // Process listed_in
    limpar_listed_in(s);
}

int comparar_showid(const void *a, const void *b) {
    Show *s1 = (Show *)a;
    Show *s2 = (Show *)b;
    return strcmp(s1->show_id, s2->show_id);
}

int busca_binaria(char *id) {
    int esq = 0, dir = total_shows - 1;
    while (esq <= dir) {
        int meio = (esq + dir) / 2;
        int cmp = strcmp(shows[meio].show_id, id);
        if (cmp == 0) return meio;
        else if (cmp < 0) esq = meio + 1;
        else dir = meio - 1;
    }
    return -1;
}

void imprimir_show(Show *s) {
    printf("=> %s ## %s ## %s ## %s ## [%s] ## %s ## %s ## %d ## %s ## %s ## [%s] ##\n",
        s->show_id,
        s->title,
        s->type,
        s->director,
        strcmp(s->cast, "NaN") == 0 ? "NaN" : s->cast,
        s->country,
        strcmp(s->date_added, "NaN") == 0 ? "NaN" : s->date_added,
        s->release_year,
        s->rating,
        s->duration,
        strcmp(s->listed_in, "NaN") == 0 ? "NaN" : s->listed_in
    );
}

int main() {
    FILE *fp = fopen("/tmp/disneyplus.csv", "r");
    if (!fp) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    char linha[MAX_LINE];
    fgets(linha, MAX_LINE, fp); // pular cabeçalho

    while (fgets(linha, MAX_LINE, fp) && total_shows < MAX_SHOWS) {
        linha[strcspn(linha, "\n")] = '\0';
        parse_line(linha, &shows[total_shows]);
        total_shows++;
    }

    fclose(fp);

    qsort(shows, total_shows, sizeof(Show), comparar_showid);

    char entrada[30];
    while (1) {
        if (fgets(entrada, sizeof(entrada), stdin) == NULL) break;
        entrada[strcspn(entrada, "\n")] = '\0';
        if (strcmp(entrada, "FIM") == 0) break;

        int idx = busca_binaria(entrada);
        if (idx != -1) {
            imprimir_show(&shows[idx]);
        } else {
            printf("Não encontrado: %s\n", entrada);
        }
    }

    return 0;
}
