#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "memory.h"

void *Aloca(unsigned int nbytes);
void Libera(void *ponteiro);

void initList(List *list)
{
    list->start = NULL;
    list->end = NULL;
    list->size = 0;
}

listBlock *aloc(listBlock *new, void *data)
{
    new->before = NULL;
    new->after = NULL;
    new->data = Aloca(sizeof(data));
    return new;
}

int InsertEmptyList(List *list, void *data)
{
    listBlock *new = Aloca(sizeof(listBlock));

    new = aloc(new, data);
    if (new == NULL)
        return -1;
    new->data = data;
    new->before = list->start;
    new->after = list->end;
    list->start = new;
    list->end = new;
    list->size++;
    return 0;
}

int InsertStartList(List *list, void *data)
{
    listBlock *new = Aloca(sizeof(listBlock));
    new = aloc(new, data);
    if (new == NULL)
        return -1;
    new->data = data;
    new->before = NULL;
    new->after = list->start;
    list->start->before = new;
    list->start = new;
    list->size++;
    return 0;
}

int InsertEndList(List *list, void *data)
{
    listBlock *new = Aloca(sizeof(listBlock));
    new = aloc(new, data);
    if (new == NULL)
        return -1;
    new->data = data;
    new->after = NULL;
    new->before = list->end;
    list->end->after = new;
    list->end = new;
    list->size++;
    return 0;
}

int InsertAfter(List *list, void *data, int position)
{
    int i;
    listBlock *new = Aloca(sizeof(listBlock)), *temp = Aloca(sizeof(listBlock));
    new = aloc(new, data);
    if (new == NULL)
        return -1;
    new->data = data;
    temp = list->start;
    for (i = 1; i < position; ++i)
        temp = temp->after;
    new->after = temp->after;
    new->before = temp;
    if (temp->after == NULL)
        list->end = new;
    else
        temp->after->before = new;
    temp->after = new;
    list->size++;
    return 0;
}

int InsertBefore(List *list, void *data, int position)
{
    int i;
    listBlock *new = Aloca(sizeof(listBlock)), *temp = Aloca(sizeof(listBlock));
    new = aloc(new, data);
    if (new == NULL)
        return -1;
    new->data = data;
    temp = list->start;
    for (i = 1; i < position; ++i)
        temp = temp->after;
    new->after = temp;
    new->before = temp->before;
    if (temp->before == NULL)
        list->start = new;
    else
        temp->before->after = new;
    temp->before = new;
    list->size++;
    return 0;
}

int Remove(List *list, int position)
{
    int i;
    listBlock *removeBlock, *temp;

    if (list->size == 0)
        return -1;

    if (position == 1)
    {
        removeBlock = list->start;
        list->start = list->start->after;
        if (list->start == NULL)
            list->end = NULL;
        else
            list->start->before = NULL;
    }
    else if (position == list->size)
    {
        removeBlock = list->end;
        list->end->before->after = NULL;
        list->end = list->end->before;
    }
    else
    {
        temp = list->start;
        for (i = 1; i < position; ++i)
            temp = temp->after;
        removeBlock = temp;
        temp->before->after = temp->after;
        temp->after->before = temp->before;
    }
    Libera(removeBlock->data);
    Libera(removeBlock);
    list->size--;
    return 0;
}

void ShowList(List *list)
{
    listBlock *temp;
    temp = list->start;
    printf("[ ");
    while (temp != NULL)
    {
        printf("%p ", temp->data);
        temp = temp->after;
    }
    printf("]\n");
}

void ShowListReverse(List *list)
{
    listBlock *temp;
    temp = list->end;
    printf("[ ");
    while (temp != NULL)
    {
        printf("%p : ", temp->data);
        temp = temp->before;
    }
    printf("]\n");
}

void ShatterList(List *list)
{
    while (list->size > 0)
        Remove(list, 1);
}