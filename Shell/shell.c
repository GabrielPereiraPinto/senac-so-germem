#define _GNU_SOURCE
#define _POSIX_SOURCE
#define SHELL_READ_BUFFER_TAMANHO 1024;
#define SHELL_COMANDO_BUFFER_TAMANHO 64
#define SHELL_COMANDO_DELIMITACAO " \t\r\n\a"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <libgen.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

#define GREEN "\x1b[92m"
#define BLUE "\x1b[94m"
#define DEF "\x1B[0m"
#define CYAN "\x1b[96m"



// ------------------------COMANDOS--------------------


void shell_loop (void);
char *shell_read(void);
char ** shell_split(char *linha);

int shell_hlp(char **args);
int shell_cgd(char **args);
int shell_ext(char **args);
int shell_cls(char **args);
int shell_lsd(char **args);
int shell_mkd(char **args);

char *builtin_comandos[] = {
    "hlp",
    "cgd",
    "ext",
    "cls",
    "lsd",
    "mkd"

};

int (*buitin_funcoes[]) (char **) ={
    &shell_hlp,
    &shell_cgd,
    &shell_ext,
    &shell_cls,
    &shell_lsd,
    &shell_mkd
};


int shell_num_builtins(){
    return sizeof(builtin_comandos) / sizeof(char *);
}

// ------------------------METODOS DOS COMANDOS--------------------


int shell_cgd(char **args){

    if (args[1] == NULL)
        fprintf(stderr, "Shell: Esperado argumento para \"cgd\" \n ");

    else{
        if (chdir(args[1]) != 0)
        perror("Shell: Erro ao executar cgd");
    }

    return 1;
}

int shell_hlp(char **args){

    printf("Bem vindo ao Shell \n");
    printf("Criado por Gabriel Pereira Pinto \n");
    printf("Digite o comando desejado e seus argumentos, depois precionse enter \n");
    printf("Existem os seguintes comandos: \n");

    printf("hlp - HELP - ajuda \n");
    printf("cgd - CHANGE DIRECTORY - mudar o diretorio \n");
    printf("lsd - LIST - listar arquivos no diretorio \n");
    printf("mkd - MAKE DIRECTORY - criar um diretorio \n");
    printf("rmv - REMOVE - remover arquivo \n");
    printf("ext - EXIT - sair do shell \n");

}

int shell_ext(char **args){
    return 0;
}

int shell_cls(char **args){

    const char* branco = "\e[1;1H\e[2J";
    write(STDOUT_FILENO,branco,20);
}


void nameFile(struct dirent* name,char* followup)
{
    if(name->d_type == DT_REG)          // regular file
    {
        printf("%s%s%s",BLUE, name->d_name, followup);
    }
    else if(name->d_type == DT_DIR)    // a directory
    {
        printf("%s%s/%s",GREEN, name->d_name, followup);
    }
    else                              // unknown file types
    {
        printf("%s%s%s",CYAN, name->d_name, followup);
    }
}

int shell_lsd(char **args){

    struct dirent **diretorio_string;

    int list_diretorio = scandir(".", &diretorio_string, 0, alphasort);

    if (list_diretorio >= 0) { 
        for (int i = 0; i < list_diretorio; i++){
            if (strcmp(diretorio_string[i] -> d_name, ".") == 0 || strcmp(diretorio_string[i] -> d_name, "..") == 0)
                continue;

            else 
                nameFile(diretorio_string[i], "             ");

            if (i%8 == 0)
                printf("\n");
        }
        printf("\n");
    }

    else
        perror("Shell: Erro ao executar o lsd ");
}


int shell_mkd(char **args){

    if (args[1] == NULL){
        perror("Shell: Erro sem argumento para o mkd");
        exit(EXIT_FAILURE); 
    }

    int status = mkdir(args[1], 0777);

    if (status == -1)
        perror("Shell: Erro ao executar mkd");

        return 1;
}

// ------------------------METODOS BASE--------------------

char *shell_read(void){    

    char *linha = NULL;
    ssize_t buffer_tamanho =0;
    getline(&linha, &buffer_tamanho, stdin);

    return linha;
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
                exit(EXIT_FAILURE);
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
            perror("Shell: Erro ao executar launch");
        }

        exit(EXIT_FAILURE);
    }

    else if (process_id < 0){ //erro no fork
       perror("Shell: Erro ao executar launch");
    }

    else { //processo pai
        do{
            wait_id = waitpid(process_id, &status, WUNTRACED);
        }

        while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int shell_exec(char **args){

    if (args == NULL)
        return 1;

    for (int i = 0; i < shell_num_builtins(); i++){
        if (strcmp(args[0], builtin_comandos[i]) == 0)
            return (*buitin_funcoes[i])(args);
    }

    return shell_launch(args);
}

void shell_loop (void){

    char *linha;
    char **args;
    int loop_status;

    printf ("Para ajuda digite hlp ... \n");
    do{

        char diretorio[10247];
        getcwd( diretorio, sizeof(diretorio));

        printf("%s%s >",GREEN, diretorio);

        linha = shell_read();
        args = shell_split(linha);
        loop_status = shell_exec(args);

        free(linha);
        free(args);
    }
    while (loop_status);
}



//--------------------------MAIN----------------------------

int main (int argc, char **argv){

    shell_loop();

    return 0;
}