#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <locale.h>
#include <time.h>
#include <stdio_ext.h>

struct msg {
        char *m;
        struct msg *prtprox;
        };

struct msg *ptrprim, *ptrnovo, *ptratual;

void novo();
void lista();

int main()
{
        char c;
        ptrprim = (struct msg *) NULL;
        
        while(1) {
                puts("Pesquisa Com Lista Ligada");
                puts("");
                printf("n - Novo No\nl - Lista Nos:");
                __fpurge(stdin);
                scanf("%c",&c);

                switch(c) {
        
                case 'n': 
                        novo();
                        break;
                case 'l': 
                        lista();
                        break;
                default: printf("\nOpcao incorreta\n");
                }        
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
