#include "memory.h"

void main (){

    int *x = Aloca(sizeof(int));
    *x = 15;
    PrintMap();
    Libera(*x);
    PrintMap();
}