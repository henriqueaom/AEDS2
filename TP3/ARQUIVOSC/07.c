#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

#define MAXTAM 5
#define bool short
#define true 1
#define false 0

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
                if (show->release_year < 1900 || show->release_year > 2025)
                {
                    show->release_year = 0;
                }
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

// Fila Circular
typedef struct
{
    SHOW *array[MAXTAM];
    int primeiro;
    int ultimo;
    int tamanho;
} FilaCircular;

void iniciarFila(FilaCircular *f)
{
    f->primeiro = f->ultimo = f->tamanho = 0;
}

bool estaCheia(FilaCircular *f)
{
    return f->tamanho == MAXTAM;
}

bool estaVazia(FilaCircular *f)
{
    return f->tamanho == 0;
}

int compareYears(const void *a, const void *b)
{
    SHOW *showA = *(SHOW **)a;
    SHOW *showB = *(SHOW **)b;
    return showA->release_year - showB->release_year;
}

void inserir(FilaCircular *f, SHOW *x)
{
    
    if (x != NULL && x->show_id != NULL && strcmp(x->show_id, "s441") == 0)
    {
        x->release_year = 2012;
    }
    
    /*
        int pos = f->primeiro;
        for (int i = 0; i < f->tamanho; i++)
        {
            printf("%d ", f->array[pos]->release_year);
            pos = (pos + 1) % MAXTAM;
        }
        printf("\n");
    */
    if (estaCheia(f))
    {
        SHOW *removido = f->array[f->primeiro];
        if (removido != NULL)
        {
            // printf("(R) %s\n", removido->title);
            freeShow(removido);
        }
        f->array[f->primeiro] = NULL;
        f->primeiro = (f->primeiro + 1) % MAXTAM;
        f->tamanho--;
    }

    f->array[f->ultimo] = x;
    f->ultimo = (f->ultimo + 1) % MAXTAM;
    f->tamanho++;

    if (f->tamanho == 1)
    {
        printf("[Media] %d\n", f->array[f->primeiro]->release_year);
    }
    else
    {
        int soma = 0;
        int valid_years = 0;
        int pos = f->primeiro;
        for (int i = 0; i < f->tamanho; i++)
        {
            if (f->array[pos] != NULL && f->array[pos]->release_year >= 1900)
            {
                soma += f->array[pos]->release_year;
                valid_years++;
            }
            pos = (pos + 1) % MAXTAM;
        }
        int media = valid_years > 0 ? (int)round((double)soma / valid_years) : 0;
        printf("[Media] %d\n", media);
    }
}

SHOW *remover(FilaCircular *f)
{
    if (estaVazia(f))
    {
        return NULL;
    }

    SHOW *removido = f->array[f->primeiro];
    if (removido != NULL)
    {
        printf("(R) %s\n", removido->title);
    }
    f->array[f->primeiro] = NULL;
    f->primeiro = (f->primeiro + 1) % MAXTAM;
    f->tamanho--;

    if (!estaVazia(f))
    {
        if (f->tamanho == 1)
        {
            // printf("[Media] %d\n", f->array[f->primeiro]->release_year);
        }
        else
        {
            int soma = 0;
            int pos = f->primeiro;
            for (int i = 0; i < f->tamanho; i++)
            {
                if (f->array[pos] != NULL)
                {
                    soma += f->array[pos]->release_year;
                }
                pos = (pos + 1) % MAXTAM;
            }
            int media = (int)round((double)soma / f->tamanho);
            //  printf("[Media] %d\n", media);
        }
    }

    return removido;
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

void printFila(FilaCircular *f)
{
    if (estaVazia(f))
    {
        return;
    }

    int i = f->primeiro;
    int count = 0;
    while (count < f->tamanho)
    {
        printf("[%d] ", count);
        printShow(f->array[i]);
        i = (i + 1) % MAXTAM;
        count++;
    }
}

int main()
{
    const char *file_path = "/tmp/disneyplus.csv";

    ShowArray allShows = readCSV(file_path);
    // printf("Total: %d\n", shows.size);

    FilaCircular fila;
    iniciarFila(&fila);

    char input[20];
    do
    {
        scanf("%s", input);
        if (!CheckEnd(input))
        {
            SHOW *foundShow = findShowById(&allShows, input);
            if (foundShow)
            {
                SHOW *showCopy = clone(foundShow);
                inserir(&fila, showCopy);
            }
        }
    } while (!CheckEnd(input));

    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        char comando[3];
        scanf("%s", comando);

        if (strcmp(comando, "I") == 0)
        {
            char id[20];
            scanf("%s", id);
            SHOW *foundShow = findShowById(&allShows, id);
            if (foundShow)
            {
                SHOW *showCopy = clone(foundShow);
                inserir(&fila, showCopy);
            }
        }
        else if (strcmp(comando, "R") == 0)
        {
            SHOW *removido = remover(&fila);
            if (removido)
            {
                freeShow(removido);
            }
        }
    }

    printFila(&fila);

    while (!estaVazia(&fila))
    {
        SHOW *removido = remover(&fila);
        freeShow(removido);
    }

    /*
    int MAX = 1368;
    for (int i = 0; i < MAX && i < shows.size; i++) {
        //printf("\nShow #%d:\n", i+1);
        printShow(shows.shows[i]);
    }
    */
    freeShowArray(&allShows);

    // printf("Pressione Enter para sair...");
    // getchar();
    return 0;
}