#include <stdio.h>
#include <corecrt_wstdio.h>
#include <stdbool.h>

#define SHELL_READ_BUFFER_TAMANHO 1024;
#define SHELL_COMANDO_BUFFER_TAMANHO 64
#define SHELL_COMANDO_DELIMITACAO " \t\r\n\a"

int main (int argc, char **argv){

    shell_loop();

    return 0;
}

void shell_loop (void){

    char *linha;
    char **args;
    int loop_status;

    do{
        printf("> ");

        linha = shell_read();
        args = shell_split();
        loop_status = shell_exec(args);

        free(linha);
        free(args);
    }
    while (loop_status);
}

char *shell_read(void){

    int buffer_tamanho = SHELL_READ_BUFFER_TAMANHO;
    char *buffer = malloc(sizeof(char) * buffer_tamanho);

    if(!buffer){
        fprintf(stderr, "Shell: Erro de alocação - buffer");
        exit (1);
    }
    
    int caracter;    
    int posicao = 0;
    while (true){
        caracter = getchar();

        if (caracter == EOF || caracter == "\n"){

            buffer[posicao] == "\0";
            return buffer;
        }

        else
            buffer[posicao] = caracter;

        posicao++;

        if(posicao >= buffer_tamanho){
            buffer_tamanho += SHELL_READ_BUFFER_TAMANHO;

            buffer = realloc(buffer, buffer_tamanho);

            if(!buffer){
                fprintf(stderr, "Shell: Erro de Alocação - buffer realloc");
                exit(1);
            }
        }
    }
}

char ** shell_split(char *linha){

    int buffer_tamanho = SHELL_COMANDO_BUFFER_TAMANHO;
    char **comandos = malloc(sizeof(char) * buffer_tamanho);
    char *comando;

    if(!comandos){
        fprintf(stderr, "Shell: Erro de Alocação - comandos alloc");
        exit(1);
    }

    comando = strtok(linha, SHELL_COMANDO_DELIMITACAO);
    int posicao = 0;
    while (comando != NULL){
        comandos[posicao] = comando;

        posicao++;

        if(posicao >= buffer_tamanho){
            buffer_tamanho += SHELL_COMANDO_BUFFER_TAMANHO;
            comandos = realloc(comandos, sizeof(char) * buffer_tamanho);

            if (!comandos){
                fprintf(stderr, "Shell: Erro de Alocação - comandos realloc");
                exit(1);
            }
        }
        
        comando = strtok(NULL, SHELL_COMANDO_DELIMITACAO);
    }

    comandos[posicao] = NULL;
    return comandos;
}