#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#define NLA  600
#define NLB  400
#define NCA  400
#define NCB  600

typedef struct
{
    int id;
    int tamIni;
    int tamMax;
    int tamColB;
    int tamColA;
} thread_arg;

int matriz_1[NLA][NCA], matriz_2[NLB][NCB], matriz_result[NLA][NLB];

void *thread(void *vargp);

void carregamat (int nl, int nc, char mat){
    int l,c;
    for (int l =0; l<nl ; l++){
            for(int c =0; c< nc; c++){
                if(mat == 'A'){
                    matriz_1[l][c] = l+c+1;
                }else if(mat=='B'){
                    matriz_2[l][c] = l+c+1;
                }else{
                    matriz_result[l][c] =1;
                }             
                
            }
        }

}
void imprimeMatriz (int nl, int nc,int matriz_imp[nl][nc]) {
 int l, c;
 for (l=0;l<nl;l++){
     for (c=0;c<nc;c++) {
         printf("\t %d", matriz_imp[l][c]);
     }
     printf ("\n");
 }    
}

int main() 
{
    int nla = NLA,nca = NCA,nlb = NLB ,ncb = NCB;
    int matA[nla][nca],matB[nlb][ncb];
    pthread_t tid[2];
    thread_arg a[2];
    int i = 0;
    int n_threads = 2;
    carregamat(nla,nca, 'A');
    carregamat(nlb,ncb, 'B');
    carregamat(nla,nlb, 'R');
    /*printf("Matriz A\n");
    imprimeMatriz (nla, nlb, matriz_1);
    printf("Matriz B\n");
    imprimeMatriz (nlb, ncb, matriz_2);*/
    int aux1 = (nla/n_threads);
    int aux2 = 0;
    clock_t time;
    time = clock();
    //Cria as threads
    for(i=0; i<n_threads; i++)
    {
        //preenche os argumentos
        if(i == n_threads -1 && (aux2+aux1) < nla){
            aux1 = aux1 + 1;
        }
        a[i].id = i;
        a[i].tamIni = aux2;
        aux2 += aux1;
        a[i].tamColA = nca;
        a[i].tamColB = ncb;
        a[i].tamMax = aux2;
        //chama a função que cria e manda a função que a thread irá executar
        pthread_create(&(tid[i]), NULL, thread, (void *)&(a[i]));
    }
    
    // Espera que as threads terminem
    for(i=0; i<n_threads; i++)
    {
        pthread_join(tid[i], NULL);
    }
    time = clock() -time;
    printf("Tempo de execucao single: %lf\n", ((double)time)/((CLOCKS_PER_SEC/1000)));
    /*printf("Matriz Result\n");
    imprimeMatriz (nla, nlb, matriz_result);*/
    pthread_exit((void *)NULL);
    
}

void *thread(void *vargp)
{
    int i = 0;
    thread_arg *a = (thread_arg *) vargp;

        for (int l=a->tamIni;l<a->tamMax;l++){
            for (int c=0;c<a->tamColA;c++){
                matriz_result[l][c] = matriz_1[l][c] * matriz_2[c][l];
                //printf("multiplicando linha:%d coluna:%d = %d thread:%d\n",l , c,matriz_result[l][c] , a->id);
                     //}
            }
    }     

    pthread_exit((void *)NULL);
}

