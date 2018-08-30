typedef struct block
{
    void *data;
    struct block *before;
    struct block *after;
} listBlock;

typedef struct ListControl
{
    listBlock *start;
    listBlock *end;
    int size;
} List;

void initList(List *lista);
listBlock *aloc(listBlock *novo_elemento, void *data);

int InsertEmptyList(List *lista, void *data);

int InsertStartList(List *lista, void *data);

int InsertEndList(List *lista, void *data);

int InsertAfter(List *lista, void *data, int pos);

int InsertBefore(List *lista, void *data, int pos);

int Remove(List *lista, int pos);

void ShowList(List *lista);

void ShowListReverse(List *lista);

void ShatterList(List *lista);