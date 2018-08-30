#include <stdint.h>

#define SIZE 4096

void *Aloca(unsigned int nbytes);
void Libera(void *ponteiro);
void InitMap();

uint8_t map[SIZE];
