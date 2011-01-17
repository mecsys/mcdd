#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <locale.h>
#include <time.h>

struct msg {
        char m[80];
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
                puts("Pesquisa Com Lista Ligada");
                puts("");
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
        if(!(ptrnovo = (struct msg *) malloc(sizeof(struct msg *)))) {
                perror("ptrnovo = [MALLOC]");
                exit(EXIT_FAILURE);
        }

        if(ptrprim == (struct msg *) NULL)
                ptrprim = ptratual = ptrnovo;
        else {
                while(ptratual -> ptrprox != (struct msg *) NULL)
                        ptratual = ptratual -> ptrprox;
                ptratual -> ptrprox = ptrnovo;
                ptratual = ptratual -> ptrprox;
        }

        __fpurge(stdin);
        puts("\nEscreva no No: ");
        getchar();
        fgets(ptratual -> m, 80, stdin);

        ptratual -> ptrprox = (struct msg *) NULL;
}

void lista()
{       
        int count = 0;
        system("clear");
        if(ptrprim == (struct msg *) NULL) {
                printf("\nLista vazia!\n\n");
                return;
        }

        ptratual = ptrprim;
        do {
                puts("==============================");
                printf("\nNo nr %d",++count);
                printf("\nConteudo: %s\n",ptratual -> m);
                puts("==============================");
                ptratual = ptratual -> ptrprox;
        } while(ptratual);             
}
