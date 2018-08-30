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

void initList(List *list);

listBlock *aloc(listBlock *new, void *data);

int InsertEmptyList(List *list, void *data);

int InsertStartList(List *list, void *data);

int InsertEndList(List *list, void *data);

int InsertAfter(List *list, void *data, int position);

int InsertBefore(List *list, void *data, int position);

int Remove(List *list, int position);

void ShowList(List *list);

void ShowListReverse(List *list);

void ShatterList(List *list);