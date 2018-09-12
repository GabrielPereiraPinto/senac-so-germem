#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <corecrt_wstdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define SHELL_READ_BUFFER_TAMANHO 1024;
#define SHELL_COMANDO_BUFFER_TAMANHO 64
#define SHELL_COMANDO_DELIMITACAO " \t\r\n\a"


// ------------------------COMANDOS--------------------
#define HELP    "hlp"
#define CHANGE  "cgd"
#define LIST    "lst"
#define MAKE    "mkd"
#define REMOVE  "rmv"
#define EXIT    "ext"

void shell_loop (void);
char *shell_read(void);
char ** shell_split(char *linha);

int shell_hlp(char **args);
int shell_cgd(char **args);
int shell_ext(char **args);

char *builtin_comandos[]{
    HELP,
    CHANGE,
    EXIT
}

int (*buitin_funcoes[]) (char **) ={
    &shell_hlp,
    &shell_cgd,
    &shell_ext
}


int main (int argc, char **argv){

    shell_loop();

    return 0;
}

// ------------------------METODOS BASE--------------------
void shell_loop (void){

    char *linha;
    char **args;
    int loop_status;

    printf ("Para ajuda digite hlp ... \n");
    do{
        printf("> ");

        linha = shell_read();
        args = shell_split(linha);
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
        exit (-1);
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
                exit(-1);
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
                exit(-1);
            }
        }
        
        comando = strtok(NULL, SHELL_COMANDO_DELIMITACAO);
    }

    comandos[posicao] = NULL;
    return comandos;
}

int shell_launch(char **args){
    
    pid_t process_id, wait_id;
    int status;

    process_id = fork();

    if (process_id == 0){ //processo filho
        if(execvp(args[0], args) == -1){
            perror("shell");
        }

        exit(-1);
    }

    else if (process_id < 0){ //erro no fork
        perror("shell");
    }

    else { //processo pai
        do{
            wait_id = wait_process_id(process_id, &status, WUNTRACED);
        }

        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}


// ------------------------METODOS DOS COMANDOS--------------------

int shell_num_builtins(){
    return sizeof(builtin_comandos) / sizeof(char *);
}

int shell_cgd(char **args){

    if (args[1] == NULL)
        fprintf(stderr, "Shell: Esperado argumento para \"cgd\" \n ");

    else{
        if (chdir(args[1]) != 0)
        perror("shell");
    }

    return 1;
}

int shell_hlp(char **args){

    printf("Bem vindo ao Shell \n");
    printf("Criado por Gabriel Pereira Pinto \n");
    printf("Digite o comando desejado e seus argumentos, depois precionse enter \n");
    printf("Existem os seguintes comandos: \n");

    printf("hlp - HELP - ajuda \n");
    printf("cgd - CHANGE DIRECTORY - mudar o diretorio\n");
    printf("lst - LIST - listar arquivos no diretorio");
    printf("mkd - MAKE DIRECTORY - criar um diretorio");
    printf("rmv - REMOVE - remover arquivo");
    printf("ext - EXIT - sair do shell");

}

int shell_ext(char **args){
    return 0;
}