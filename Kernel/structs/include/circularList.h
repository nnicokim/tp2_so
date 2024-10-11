// Lista circular del Round-robin

typedef struct CircularListNode
{
    struct CircularList *next;
    struct CircularList *prev;
    int pid;
} CircularListNode;

typedef struct CircularList
{
    CircularListNode *head;
    int size;
} CircularList;

// VER SI HAY QUE CREAR UNA STRUCT "ITERATOR" QUE CORRE SOLO COMO SI FUESE EL SCHEDULER !!!!!

void initializeCircularList(CircularList *list);
void addCircularList(CircularList *list, int pid);
void removeCircularList(CircularList *list, int pid);
