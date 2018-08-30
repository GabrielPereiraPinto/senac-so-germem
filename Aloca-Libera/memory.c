#include "memory.h"

void initMap()
{
    for (int i = 0; i < SIZE; i++)
    {
        map[i] = '0';
    }
}

void *Aloca(unsigned int nbytes)
{

    nbytes++;
    bool open = false;
    int *posicao = malloc(sizeof(int));
    *posicao = 0;

    for (int i = 0; i < SIZE; i++)
    {

        if (map[i] == '|')
        {
            open = false;
            continue;
        }
        else if (map[i] == 'F')
        {
            open = true;
            *posicao = i + 1;
            continue;
        }

        if (open && (i - (unsigned int)*posicao == nbytes))
        {
            map[i] = 'F';
            map[*posicao] = '|';

            *posicao = *posicao + 1;
            return (void *)posicao;
        }
    }

    printf("Sem espaço suficiente para alocar");
    return NULL;
}

void Libera(void *ponteiro)
{

    int *temp = (int *)ponteiro - 1;

    for (int i = *temp; i < SIZE; i++)
    {

        if (map[i] == 'F')
        {
            map[i] = '0';
            map[*temp] = '0';
            break;
        }
    }
}

void PrintMap()
{
    for (int i = 0; i < SIZE; i++)
    {
        printf("%c", map[i]);
    }
    printf("\n");
}