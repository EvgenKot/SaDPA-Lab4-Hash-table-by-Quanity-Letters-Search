#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

/*
Ращупкин Евгений КЭ-203
28 вариант

Алгоритмы хеширования данных
1 задание
Составьте хеш-таблицу, содержащую буквы и количество их вхождений во введенной строке.
Вывести таблицу на экран. Осуществить поиск введенной буквы в хеш-таблице.
*/

typedef struct Node
{
    char value;
    struct Node *next; //На случай колизии
} Node;

typedef struct Queue
{
    Node *head;
    Node *tail;

    size_t size;
} Queue;

typedef struct Table
{
    Queue **table;
    size_t size;
} Table;

typedef struct CharsQuanity
{
    char str[100];
    int quanity[100];
} CharsQuanity;


//Выводит элементы очереди
void QueuePrint(Queue *q)
{
    Node *current = q->head;
    while (current)
    {
        printf("%5c ", current->value);
        current = current->next;
    }
    printf(" <");
    printf("\n");
}


//Ищет узел со значением
Node *QueueSearch(Queue *q, char data)
{
    Node *current = q->head;
    while (current)
    {
        if (current->value == data)
            return current;
        current = current->next;
    }
    return NULL;
}

//Ищет узел со значением и возвращает номер
Node *QueueSearchNumber(Queue *q, char data, int *n)
{
    Node *current = q->head;
    *n = 0;
    while (current)
    {
        *n = *n + 1;
        if (current->value == data)
            return current;
        current = current->next;
    }
    return NULL;
}

//Добавляет элемент в конец очереди
void QueuePushBack(Queue *q, char data)
{
    Node *search = QueueSearch(q, data);
    if (!search)
    {
        q->size++;
        Node *newNode = (Node *)malloc(sizeof(Node));

        newNode->value = data;
        newNode->next = NULL;

        if (q->head == NULL)
            q->head = newNode;
        else
            q->tail->next = newNode;
        q->tail = newNode;
    }
}

//Удаление элемента очереди
int QueuePop(Queue *q)
{
    q->size--;
    Node *prev = NULL;
    char val;
    if (q->head == NULL)
    {
        exit(-1);
    }
    prev = q->head;
    val = prev->value;
    q->head = q->head->next;
    free(prev);
    return val;
}

//Отчистка памяти очереди
int QueueFree(Queue *q)
{
    while (q->size != 0)
    {
        QueuePop(q);
    }
}

//Поиск по символу.
int CharSearch(char data, char *str)
{
    for (int i = 0; i < 100; i++)
        if (str[i] == data)
            return i;
    return -1;
}

//Поиск максимального в целочисленном массиве
int ArrayMax(int *arr)
{
    int size = 100;
    int max = arr[0];
    for (int i = 0; i < size; i++)
        if (arr[i] > max)
            max = arr[i];
    return max;
}

CharsQuanity* CharsQuanityCreate()
{
    CharsQuanity *ch = malloc(sizeof(CharsQuanity));
    int size = 100;
    for (size_t i = 0; i < size; i++)
    {
        ch->quanity[i] = 0;
        ch->str[i] = '0';
    }
    return ch;
}

//Заполнение структуры символов и количества символов
void CharsQuanityFill(CharsQuanity *cq, char *str)
{
    int j = 0;
    for (char *c = str; *c != '\0'; c++)
	{
        int i = CharSearch(*c, cq->str);
        if (i == -1)
        {
            cq->str[j] = *c;
            cq->quanity[j] = 1;
            j++;
        }
        else
            cq->quanity[i] = cq->quanity[i] + 1;
    }
    return;
}

//Создание хеш-таблицы
Table *HashTableCreate(int size)
{

    Table *t = malloc(sizeof(Table));
    t->table = malloc(size * sizeof(*(t->table)));

    t->size = size;

    for (int i = 0; i < size; i++)
    {
        t->table[i] = malloc(sizeof(*(t->table[i])));
        t->table[i]->head = NULL;
        t->table[i]->tail = t->table[i]->head;
        t->table[i]->size = 0;
    }

    return t;
}

//Создание хеша по количество 
int HashGenerate(char data, CharsQuanity *qua)
{   
    int i = CharSearch(data, qua->str);
    if (i != -1)
        return qua->quanity[i];
    return 0;
}

//Вывод хещ-таблицы
void HashTablePrint(Table *t)
{
    printf("Hash-table\n");
    printf("Size = %d\n", t->size);
    printf(" hash  qua\n");
    for (int i = 0; i < t->size; i++)
    {
        printf("|%4d|%4d|", i, t->table[i]->size);
        QueuePrint(t->table[i]);
    }
}

//Добавление элемента в хеш-таблицу
void HashTableAdd(Table *t, char data, CharsQuanity *qua)
{
    int index = HashGenerate(data, qua);
    QueuePushBack(t->table[index], data);
}

//Заполнение хеш-таблицы
void HashTableFill(Table *t, CharsQuanity *qua)
{
    int size = sizeof(qua->quanity) / sizeof(int);
    int count = 0;
    for (int i = 0; i < size; i++) //Количество элементов до 1 встреченного 0
        if (qua->quanity[i] == 0)
        {
            count = i;
            break;
        }
    for (int i = 0; i < count; i++)
    {
        //printf("To hash %d Adding %c\n", qua->quanity[i], qua->str[i]);
        //HashTableAdd(t, qua->str[i], qua);
        QueuePushBack(t->table[qua->quanity[i]], qua->str[i]);
    }
}


//Поиск элемента в хеш-таблице
void HashTableSearch(Table *t, char data, CharsQuanity *qua)
{
    int position;
    int index = HashGenerate(data, qua);
    Node *n = QueueSearchNumber(t->table[index], data, &position);
    if (n)
    {
        printf("|%4d|%4d|", index, t->table[index]->size);
        QueuePrint(t->table[index]);
        printf("               ");
        for (int i = position; i > 1; i--)
            printf("      ");
        printf("^\n");
        return;
    }
    else
        printf("No element.\n");
}

//Отчистка памяти
void HashTableFree(Table *t)
{
    printf("Hash table free...\n");
    for (int i = 0; i < t->size; i++)
    {
        if (t->table[i])
        {
            QueueFree(t->table[i]);
            free(t->table[i]);
        }
    }
    free(t->table);
    free(t);
    printf("Completed.\n");
}

//Ввод строки
char *StringGet(int *len)
{
   *len = 0;                               // изначально строка пуста
   int capacity = 8;                       // ёмкость контейнера динамической строки (1, так как точно будет '\0')
   char *s = (char *)malloc(capacity * sizeof(char)); // динамическая пустая строка

   char c = getchar(); // символ для чтения данных

   // читаем символы, пока не получим символ переноса строки (\n)
   while (c != '\n')
   {
      s[(*len)++] = c; // заносим в строку новый символ

      // если реальный размер больше размера контейнера, то увеличим его размер
      if (*len >= capacity)
      {
         capacity = capacity * 2;                                      // увеличиваем ёмкость строки на 1
         s = (char *)realloc(s, capacity * sizeof(char)); // создаём новую строку с увеличенной ёмкостью
      }
      c = getchar(); // считываем следующий символ
   }

   s[*len] = '\0'; // завершаем строку символом конца строки

   return s; // возвращаем указатель на считанную строку
}

//Ввод цифры
int IntGet()
{
    char *string;
    int len;
    string = StringGet(&len);

    int number = 0;
    for (size_t i = 0; i < len; i++)
    {
        if ((string[i] <= '9') && (string[i] >= '0'))
        {
            number *= 10;
            number += string[i] - '0';
        }
        else
            break;
    }
    free(string);
    return number;
}

int main()
{
    

    // char *text = "qwertyuiosdrfgdysuhfhvnituecormitneorvitwbcrhnuotewmcuionew";
    //Ввод текста
    int text_len;
    char *text;

    CharsQuanity *cq;
    cq = CharsQuanityCreate();

    printf("Write some string to Hash table\n>>");
    text = StringGet(&text_len);

    
    printf("You wrote: %s\n", text);

    printf("CharsQuanity filling...\n");
    CharsQuanityFill(cq, text);
    printf("Completed.\n");

    int table_size = ArrayMax(cq->quanity) + 1;
    printf("Creating table size %d...\n", table_size);

    
    Table *t = HashTableCreate(table_size);
    printf("Completed.\n");

    printf("Hash table filling...\n");
    HashTableFill(t, cq);
    printf("Completed.\n");

    HashTablePrint(t);
    
    /*
    for (size_t i = 0; i < 100; i++)
    {
        printf("%2d|%2d|%c\n", i, cq->quanity[i], cq->str[i]);
    }
    */
    
    
    /*

    //Создание хеш-таблицы
    int table_size;
    printf("Enter size of hash table:\n>>");
    table_size = IntGet();
    printf("Creating table size %d\n", table_size);
    Table *t = HashTableCreate(table_size);
    printf("Hash table created.\n");

    //Перевод символов в хеш-таблицу
    printf("Chars to hash table...\n");
    for (char *c = text; *c != '\0'; c++)
        HashTableAdd(t, *c);
    printf("Completed.\n");
    */
    
    //Поиск символов
    printf("Write string to search chars from\n>>");
    int text_search_len;
    char *text_search;
    text_search = StringGet(&text_search_len);
    for (int i = 0; i < text_search_len; i++)
    {
        printf("%c Searching\n", text_search[i]);
        HashTableSearch(t, text_search[i], cq);
    }
    printf("Completed.\n");

    

    free(text);
//    free(text_search);
    HashTableFree(t);

    return 0;
}