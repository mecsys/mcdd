#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <locale.h>
#include <time.h>
#include <string.h>

struct msg {
        char *m;
        struct msg *ptrprox;
        };

struct msg *ptrprim, *ptrnovo, *ptratual;

void novo();
void lista();

int main()
{
        char c;
        ptrprim = (struct msg *) NULL;
        
        while(1) {
                printf("\nPesquisa Com Lista Ligada\n\n");
                printf("n - Novo No\nl - Lista Nos\ne - Exit\n: ");
                __fpurge(stdin);
                scanf("%c",&c);

                switch(c) {
        
                case 'n': 
                        novo();
                        break;
                case 'l': 
                        lista();
                        break;
                case 'e':
                        exit(EXIT_SUCCESS);
                default: printf("\nOpcao incorreta\n");
                }        
        }
        exit(EXIT_SUCCESS);
}

void novo()
{
        system("clear");
        if(!(ptrnovo = (struct msg *) malloc(sizeof(struct msg *)))) {
                perror("ptrnovo = [MALLOC]");
                exit(EXIT_FAILURE);
        }

        if(!(ptrnovo->m = (char *) malloc(81 * sizeof(char)))) {
                perror("ptrnovo->m = [MALLOC]");
                exit(EXIT_FAILURE);
        }                

        if(ptrprim == (struct msg *) NULL)
                ptrprim = ptratual = ptrnovo;
        else {
                ptratual = ptrprim;
                while(ptratual -> ptrprox != (struct msg *) NULL)
                        ptratual = ptratual -> ptrprox;
                ptratual -> ptrprox = ptrnovo;
                ptratual = ptratual -> ptrprox;
        }

        __fpurge(stdin);
        printf("\nEscreva no No: ");
        fgets(ptratual -> m, 80, stdin);

        ptratual -> ptrprox = (struct msg *) NULL;
}

void lista()
{       
        int count = 0;
        if(ptrprim == (struct msg *) NULL) {
                printf("\nLista vazia!\n\n");
                return;
        }

        ptratual = ptrprim;
        do {
                printf("==============================");
                printf("\nNo nr: %d",++count);
                printf("\nstrlen(ptratual -> m) = %d",strlen(ptratual->m));
                printf("\nConteudo: %s",ptratual -> m);
                printf("==============================\n\n");
                ptratual = ptratual -> ptrprox;
        } while(ptratual != (struct msg *) NULL);             
}
