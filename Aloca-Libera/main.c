#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "list.h"

void main()
{

    int *x = Aloca(sizeof(int));
    *x = 15;
    PrintMap();
    Libera(*x);
    PrintMap();

    int *list = Aloca(sizeof(List));
    initList(list);
    InsertEmptyList(list, 1);
    InsertEndList(list,3);
    InsertBefore(list, 2, 2);
    ShowList(list);
    Remove(list, 2);
    ShowList(list);

}