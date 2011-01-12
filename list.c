#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <locale.h>
#include <time.h>

struct msg {
        char *m;
        };

struct msg *ptrprim, *ptrnovo, *ptratual;

void novo();
void lista();

int main()
{
        ptrprim = (struct msg *) NULL;
        
        if(ptrnovo = (struct msg *) malloc(sizeof(struct msg *)) == NULL) {
                perror("ptrnovo [MALLOC]");
                exit(EXIT_FAILURE);
                }

        if(ptrprim == NULL)
                ptrprim = ptratual = ptrnovo;
 
        while(1) {
                puts("Pesquisa Com Lista Ligada");
                puts("");
                printf("n - Novo No\nl - Lista Nos:");

                switch(getchar()) {
        
                case 'n': 
                        novo();
                        break;
                case 'l': 
                        lista();
                        break;
                default: printf("\nOpcao incorreta\n");
                }        
                sleep(5);
        }
        exit(EXIT_SUCCESS);
}

void novo()
{
        printf("\nNOVO\n");       
}

void lista()
{
        printf("\nLISTA\n");       
}
