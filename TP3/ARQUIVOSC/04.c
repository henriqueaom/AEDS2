#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// #define MAXTAM 200
// #define bool short
// #define true 1
// #define false 0

// struct SHOW
typedef struct
{
    char *show_id;
    char *type;
    char *title;
    char *director;
    char **cast;
    char *country;
    char *date_added;
    int release_year;
    char *rating;
    char *duration;
    char **listed_in;
    char *description;
} SHOW;

// struct para armazenar os shows
typedef struct
{
    SHOW **shows;
    int size;
    int capacity;
} ShowArray;

char *extractQuotedField(char **ptr)
{
    if (**ptr != '"')
        return NULL;

    (*ptr)++;
    char *start = *ptr;
    char *end = start;
    int escaped = 0;

    while (*end != '\0')
    {
        if (!escaped && *end == '"')
        {
            if (*(end + 1) == '"')
            {
                escaped = 1;
                end += 2;
                continue;
            }
            else
            {
                break;
            }
        }
        escaped = 0;
        end++;
    }

    if (*end == '\0')
        return NULL;

    size_t len = end - start;
    char *field = (char *)malloc(len + 1);
    char *dst = field;

    while (start < end)
    {
        if (*start == '"' && *(start + 1) == '"')
        {
            *dst++ = '"';
            start += 2;
        }
        else
        {
            *dst++ = *start++;
        }
    }
    *dst = '\0';

    *ptr = end + 1;
    return field;
}

// Função para dividir strings em arrays
char **splitString(const char *str, const char *delimiter, int *count)
{
    if (!str || strlen(str) == 0)
    {
        *count = 0;
        return NULL;
    }

    char *str_copy = strdup(str);
    if (!str_copy)
        return NULL;

    *count = 1;
    char *ptr = str_copy;
    while ((ptr = strstr(ptr, delimiter)))
    {
        *count += 1;
        ptr += strlen(delimiter);
    }

    char **result = (char **)malloc((*count + 1) * sizeof(char *));
    if (!result)
    {
        free(str_copy);
        return NULL;
    }

    int i = 0;
    char *token = strtok(str_copy, delimiter);
    while (token != NULL)
    {
        // Trim whitespace
        while (*token == ' ')
            token++;
        int len = strlen(token);
        while (len > 0 && token[len - 1] == ' ')
        {
            token[--len] = '\0';
        }

        result[i++] = strdup(token);
        token = strtok(NULL, delimiter);
    }
    result[i] = NULL;

    free(str_copy);
    return result;
}

// Função para criar um SHOW padrão
SHOW *showPadrao()
{
    SHOW *show = (SHOW *)malloc(sizeof(SHOW));
    if (!show)
        return NULL;

    show->show_id = strdup("NaN");
    show->type = strdup("NaN");
    show->title = strdup("NaN");
    show->director = strdup("NaN");

    show->cast = (char **)malloc(2 * sizeof(char *));
    show->cast[0] = strdup("NaN");
    show->cast[1] = NULL;

    show->country = strdup("NaN");
    show->date_added = strdup("NaN");
    show->release_year = 0;
    show->rating = strdup("NaN");
    show->duration = strdup("NaN");

    show->listed_in = (char **)malloc(2 * sizeof(char *));
    show->listed_in[0] = strdup("NaN");
    show->listed_in[1] = NULL;

    show->description = strdup("NaN");

    return show;
}

// Função para liberar memória de um SHOW
void freeShow(SHOW *show)
{
    if (!show)
        return;

    free(show->show_id);
    free(show->type);
    free(show->title);
    free(show->director);
    free(show->country);
    free(show->date_added);
    free(show->rating);
    free(show->duration);
    free(show->description);

    if (show->cast)
    {
        for (int i = 0; show->cast[i] != NULL; i++)
        {
            free(show->cast[i]);
        }
        free(show->cast);
    }

    if (show->listed_in)
    {
        for (int i = 0; show->listed_in[i] != NULL; i++)
        {
            free(show->listed_in[i]);
        }
        free(show->listed_in);
    }

    free(show);
}

// Função para criar array de shows
ShowArray buildShowArray(int size)
{
    ShowArray array;
    array.size = 0;
    array.capacity = size;
    array.shows = (SHOW **)malloc(size * sizeof(SHOW *));
    if (!array.shows)
    {
        fprintf(stderr, "Falha ao alocar memória!\n");
        exit(EXIT_FAILURE);
    }
    return array;
}

// Função para adicionar show ao array
void addToArray(ShowArray *array, SHOW *show)
{
    if (array->size >= array->capacity)
    {
        array->capacity *= 2;
        array->shows = (SHOW **)realloc(array->shows, array->capacity * sizeof(SHOW *));
        if (!array->shows)
        {
            fprintf(stderr, "Falha ao realocar memória!\n");
            exit(EXIT_FAILURE);
        }
    }
    array->shows[array->size++] = show;
}

// metodo para adicionar um show ao array streamingShows
void addToStreamingShows(ShowArray *streamingShows, SHOW *show)
{
    if (streamingShows->size >= streamingShows->capacity)
    {
        streamingShows->capacity *= 2;
        streamingShows->shows = realloc(streamingShows->shows, streamingShows->capacity * sizeof(SHOW *));
    }
    streamingShows->shows[streamingShows->size++] = show;
}

// Função para liberar array de shows
void freeShowArray(ShowArray *array)
{
    for (int i = 0; i < array->size; i++)
    {
        if (array->shows[i])
        {
            freeShow(array->shows[i]);
        }
    }
    free(array->shows);
    array->size = 0;
    array->capacity = 0;
}

// Função de partição para o quicksort
int partition(char *array[], int esq, int dir)
{
    char *pivo = array[dir];
    int i = esq - 1;

    for (int j = esq; j < dir; j++)
    {
        if (strcasecmp(array[j], pivo) < 0)
        {
            i++;
            char *temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }

    char *temp = array[i + 1];
    array[i + 1] = array[dir];
    array[dir] = temp;
    return i + 1;
}

// Função quicksort para ordenação
void quickSort(char *array[], int esq, int dir)
{
    if (esq < dir)
    {
        int pivo = partition(array, esq, dir);
        quickSort(array, esq, pivo - 1);
        quickSort(array, pivo + 1, dir);
    }
}

void sort_cast(SHOW *show)
{
    if (!show->cast || !show->cast[0])
        return;

    int count = 0;
    while (show->cast[count] != NULL)
        count++;

    quickSort(show->cast, 0, count - 1);
}

char *clean_field(const char *str)
{
    if (!str || strlen(str) == 0 || strcmp(str, "NaN") == 0)
    {
        return strdup("NaN");
    }

    char *result = strdup(str);
    if (!result)
        return NULL;

    char *dst = result;
    int in_quotes = 0;

    for (char *src = result; *src; src++)
    {
        if (*src == '"')
        {
            in_quotes = !in_quotes;
            continue;
        }
        *dst++ = *src;
    }
    *dst = '\0';

    char *start = result;
    while (*start == ' ')
        start++;

    char *end = start + strlen(start) - 1;
    while (end > start && *end == ' ')
        end--;
    *(end + 1) = '\0';

    if (start != result)
    {
        memmove(result, start, strlen(start) + 1);
    }

    return result;
}

void printList(char **list)
{
    printf("[");
    if (list == NULL || list[0] == NULL)
    {
        printf("NaN");
    }
    else
    {
        for (int i = 0; list[i] != NULL; i++)
        {
            printf("%s", list[i]);
            if (list[i + 1] != NULL)
                printf(", ");
        }
    }
    printf("]");
}

void printShow(SHOW *show)
{
    if (!show)
        return;

    printf("=> %s ## ", show->show_id ? show->show_id : "NaN");
    printf("%s ## ", show->title ? show->title : "NaN");
    printf("%s ## ", show->type ? show->type : "NaN");
    printf("%s ## ", show->director ? show->director : "NaN");

    sort_cast(show);
    printList(show->cast);

    printf(" ## %s ## ", show->country ? show->country : "NaN");
    printf("%s ## ", show->date_added ? show->date_added : "NaN");

    if (show->release_year != 0)
        printf("%d ## ", show->release_year);
    else
        printf("NaN ## ");

    printf("%s ## ", show->rating ? show->rating : "NaN");
    printf("%s ## ", show->duration ? show->duration : "NaN");

    printList(show->listed_in);
    printf(" ##\n");
}

// Função para ler o arquivo CSV
ShowArray readCSV(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (!file)
    {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", file_path);
        exit(EXIT_FAILURE);
    }

    ShowArray show_array = buildShowArray(1368);

    char line[4096];
    fgets(line, sizeof(line), file); // Ignora cabeçalho

    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0';
        SHOW *show = showPadrao();
        if (!show)
            continue;

        char *ptr = line;
        int field = 0;

        while (*ptr != '\0' && field < 12)
        {
            char *field_value = NULL;
            char *end = ptr;

            if (*ptr == '"')
            {
                field_value = extractQuotedField(&ptr);
                while (*ptr && *ptr != ',')
                    ptr++;
            }
            else
            {
                end = ptr;
                while (*end && *end != ',')
                    end++;

                size_t len = end - ptr;
                field_value = malloc(len + 1);
                strncpy(field_value, ptr, len);
                field_value[len] = '\0';
                ptr = end;
            }

            char *cleaned = clean_field(field_value);
            free(field_value);
            field_value = cleaned;

            switch (field)
            {
            case 0:
                show->show_id = field_value;
                break;
            case 1:
                show->type = field_value;
                break;
            case 2:
            { // Título
                char *cleaned = clean_field(field_value);
                show->title = cleaned;
                break;
            }
            case 3:
            { // Diretor
                char *cleaned = clean_field(field_value);
                show->director = cleaned;
                break;
            }
            case 4:
            {
                int cast_count = 0;
                char **cast = splitString(field_value, ",", &cast_count);
                if (cast_count > 0)
                {
                    cast = (char **)realloc(cast, (cast_count + 1) * sizeof(char *));
                    cast[cast_count] = NULL;

                    for (int i = 0; show->cast[i] != NULL; i++)
                    {
                        free(show->cast[i]);
                    }
                    free(show->cast);
                    show->cast = cast;
                }
                free(field_value);
                break;
            }
            case 5:
            { // País
                char *cleaned = clean_field(field_value);
                show->country = cleaned;
                break;
            }
            case 6:
            { // Data de adição
                char *cleaned = clean_field(field_value);
                show->date_added = cleaned;
                break;
            }
            case 7:
                show->release_year = atoi(field_value);
                free(field_value);
                break;
            case 8:
            { // Classificação
                char *cleaned = clean_field(field_value);
                show->rating = cleaned;
                break;
            }
            case 9:
            { // Duração
                char *cleaned = clean_field(field_value);
                if (strcmp(show->type, "Movie") == 0)
                {
                    if (strstr(cleaned, "min") == NULL && strstr(cleaned, "Season") == NULL)
                    {
                        char *new_duration = malloc(strlen(cleaned) + 5);
                        sprintf(new_duration, "%s min", cleaned);
                        free(cleaned);
                        show->duration = new_duration;
                    }
                    else
                    {
                        show->duration = cleaned;
                    }
                }
                else
                {
                    show->duration = cleaned;
                }
                break;
            }
            case 10:
            { // listed_in
                int genres_count = 0;
                char **genres = splitString(field_value, ",", &genres_count);
                if (genres_count > 0)
                {
                    genres = realloc(genres, (genres_count + 1) * sizeof(char *));
                    genres[genres_count] = NULL;

                    quickSort(genres, 0, genres_count - 1);

                    free(show->listed_in);
                    show->listed_in = genres;
                }
                free(field_value);
                break;
            }
            case 11:
                show->description = field_value;
                break;
            default:
                free(field_value);
                break;
            }

            if (*ptr == ',')
                ptr++;
            field++;
        }

        addToArray(&show_array, show);
    }

    fclose(file);
    return show_array;
}

// Função para verificar se a entrada é "FIM"
bool CheckEnd(const char *input)
{
    return strcasecmp(input, "FIM") == 0;
}

// Função para buscar um show pelo ID
SHOW *findShowById(ShowArray *allShows, const char *id)
{
    for (int i = 0; i < allShows->size; i++)
    {
        if (allShows->shows[i] && allShows->shows[i]->show_id &&
            strcmp(allShows->shows[i]->show_id, id) == 0)
        {
            return allShows->shows[i];
        }
    }
    return NULL;
}

SHOW *clone(SHOW *original)
{
    if (!original)
        return NULL;

    SHOW *copia = showPadrao();

    copia->release_year = original->release_year;

    copia->show_id = strdup(original->show_id);
    copia->type = strdup(original->type);
    copia->title = strdup(original->title);
    copia->director = strdup(original->director);
    copia->country = strdup(original->country);
    copia->date_added = strdup(original->date_added);
    copia->rating = strdup(original->rating);
    copia->duration = strdup(original->duration);
    copia->description = strdup(original->description);

    if (original->cast)
    {
        int cast_count = 0;
        while (original->cast[cast_count] != NULL)
            cast_count++;

        copia->cast = (char **)malloc((cast_count + 1) * sizeof(char *));
        for (int i = 0; i < cast_count; i++)
        {
            copia->cast[i] = strdup(original->cast[i]);
        }
        copia->cast[cast_count] = NULL;
    }

    if (original->listed_in)
    {
        int listed_count = 0;
        while (original->listed_in[listed_count] != NULL)
            listed_count++;

        copia->listed_in = (char **)malloc((listed_count + 1) * sizeof(char *));
        for (int i = 0; i < listed_count; i++)
        {
            copia->listed_in[i] = strdup(original->listed_in[i]);
        }
        copia->listed_in[listed_count] = NULL;
    }

    return copia;
}

// Estrutura para representar uma célula da lista
typedef struct Celula
{
    SHOW *elemento;
    struct Celula *prox;
} Celula;

// Estrutura da fila circular
typedef struct
{
    SHOW **elementos;
    int inicio;
    int fim;
    int tamanho;
    int capacidade;
} FilaCircular;

Celula *novaCelula(SHOW *elemento)
{
    Celula *nova = (Celula *)malloc(sizeof(Celula));
    if (nova == NULL)
    {
        fprintf(stderr, "Erro: falha ao alocar memoria para nova celula\n");
        exit(EXIT_FAILURE);
    }
    nova->elemento = elemento;
    nova->prox = NULL;
    return nova;
}

// Função para criar uma fila circular
FilaCircular *criarFila(int capacidade)
{
    FilaCircular *fila = (FilaCircular *)malloc(sizeof(FilaCircular));
    fila->elementos = (SHOW **)malloc(capacidade * sizeof(SHOW *));
    fila->inicio = 0;
    fila->fim = 0;
    fila->tamanho = 0;
    fila->capacidade = capacidade;
    return fila;
}

// Função para verificar se a fila está vazia
bool filaVazia(FilaCircular *fila)
{
    return fila->tamanho == 0;
}

// Função para verificar se a fila está cheia
bool filaCheia(FilaCircular *fila)
{
    return fila->tamanho == fila->capacidade;
}

// Função para remover do início da fila (desenfileirar)
SHOW *desenfileirar(FilaCircular *fila)
{
    if (filaVazia(fila))
    {
        printf("Erro: fila vazia!\n");
        return NULL;
    }

    SHOW *removido = fila->elementos[fila->inicio];
    fila->inicio = (fila->inicio + 1) % fila->capacidade;
    fila->tamanho--;
    // printf("removido!\n");
    return removido;
}

void enfileirar(FilaCircular *fila, SHOW *show, const char* file_path) {
    if (fila->tamanho == fila->capacidade) {
        SHOW* removido = desenfileirar(fila);
        if (removido) {
           // printf("(R) %s\n", removido->title);
            freeShow(removido);
        }
    }

    FILE *file = fopen(file_path, "r");
    if (file) {
        char line[4096];
        fgets(line, sizeof(line), file);
        
        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = '\0';
            char* ptr = line;
            
            char* current_id = extractQuotedField(&ptr);
            if (current_id && strcmp(current_id, show->show_id) == 0) {
                for (int field = 0; field < 7; field++) {
                    char* value = extractQuotedField(&ptr);
                    if (!value) break;
                    free(value);
                    if (*ptr == ',') ptr++;
                }

                char* year_str = extractQuotedField(&ptr);
                if (year_str) {
                    show->release_year = atoi(year_str);
                    free(year_str);
                }
                free(current_id);
                break;
            }
            free(current_id);
        }
        fclose(file);
    }

    SHOW *copia = clone(show);
    int fim = (fila->inicio + fila->tamanho) % fila->capacidade;
    fila->elementos[fim] = copia;
    fila->tamanho++;

    if (fila->tamanho > 0) {
        float soma = 0;
        int count = 0;
        int pos = fila->inicio;

        for (int i = 0; i < fila->tamanho; i++) {
            if (fila->elementos[pos]->release_year != 0) {
                soma += fila->elementos[pos]->release_year;
                count++;
            }
            pos = (pos + 1) % fila->capacidade;
        }

        if (count > 0) {
            float media = soma / count;
            printf("[Media] %.0f\n", media);
        } else {
            printf("[Media] NaN\n");
        }
    }
}

// Função para mostrar a fila
void mostrarFila(FilaCircular *fila)
{
    int i, pos;
    for (i = 0, pos = fila->inicio; i < fila->tamanho; i++, pos = (pos + 1) % fila->capacidade)
    {
        printShow(fila->elementos[pos]);
    }
}

// Função para liberar a fila
void liberarFila(FilaCircular *fila)
{
    while (!filaVazia(fila))
    {
        SHOW *removido = desenfileirar(fila);
        freeShow(removido);
    }
    free(fila->elementos);
    free(fila);
}

int main()
{
    const char *file_path = "/tmp/disneyplus.csv";

    ShowArray allShows = readCSV(file_path);
    // printf("Acabou a leitura do csv");
    // printf("Total: %d\n", shows.size);

    /*
    int MAX = 1368;
    for (int i = 0; i < MAX && i < allShows.size; i++)
    {
        // printf("\nShow #%d:\n", i+1);
        printShow(allShows.shows[i]);
    }
    */

    // printf("Ate aqui tudo bem!\n");

    FilaCircular *fila = criarFila(5);

    // printf("DEBUG: Iniciando leitura de IDs...\n");
    char input[10];
    do
    {
        scanf("%s ", input);

        if (!CheckEnd(input))
        {
            SHOW *foundShow = findShowById(&allShows, input);
            if (foundShow)
            {
                enfileirar(fila, foundShow, file_path);
                // printf("DEBUG: Inseriu show ID %s\n", input);
            }
            else
            {
                printf("Show com ID %s não encontrado.\n", input);
            }
        }
    } while (!CheckEnd(input));

    /*
    for (int i = 0; i < MAX && i < allShows.size; i++) {
        //printf("\nShow #%d:\n", i+1);
        printShow(allShows.shows[i]);
    }
    */

    // mostrarFila(fila);

    int n;
    scanf("%d ", &n);
    for (int i = 0; i < n; i++)
    {
        char comando[3];
        scanf("%s ", comando);

        if (strcmp(comando, "I") == 0)
        {
            char id[10];
            scanf("%s ", id);
            SHOW *show = findShowById(&allShows, id);
            if (show)
            {
                enfileirar(fila, show, file_path);
            }
        }
        else if (strcmp(comando, "R") == 0)
        {
            SHOW *removido = desenfileirar(fila);
            if (removido)
            {
                printf("(R) %s\n", removido->title);
                freeShow(removido);
            }
        }
    }

    mostrarFila(fila);

    freeShowArray(&allShows);

    liberarFila(fila);

    // printf("Pressione Enter para sair...");
    // getchar();

    return 0;
}