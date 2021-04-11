#include <stdio.h>
#include <omp.h>
#include <time.h>


void multiThread(int tam1,int tam2, int matriz_1[tam2][tam1], int matriz_2[tam1][tam2], int matriz_result[tam1][tam2]){
        clock_t time;
        time = clock();
        int i, j;
        #pragma omp parallel for  private( i, j) num_threads(2)
        for (i =0; i<tam2 ; i++){
            for(j =0; j< tam1; j++){
                matriz_result[i][j] = matriz_1[i][j] * matriz_2[j][i];
            }
        }
        
        
    
    time = clock() -time;
    printf("Tempo de execucao multi: %lf\n", ((double)time)/((CLOCKS_PER_SEC/1000)));
}

void singleThread(int tam1,int tam2, int matriz_1[tam2][tam1], int matriz_2[tam1][tam2], int matriz_result[tam1][tam2]){
    clock_t time;
    time = clock();
        int i, j;
        #pragma omp parallel for num_threads(1)
        for (i =0; i<tam2 ; i++){
            for(j =0; j< tam1; j++){
                matriz_result[i][j] = matriz_1[i][j] * matriz_2[j][i];
            }
        }
        
    
    time = clock() -time;
    printf("Tempo de execucao single: %lf\n", ((double)time)/((CLOCKS_PER_SEC/1000)));
    
}

void scheduling(int tam1,int tam2, int matriz_1[tam2][tam1], int matriz_2[tam1][tam2], int matriz_result[tam1][tam2]){
    clock_t time;
    time = clock();
        omp_set_num_threads(2);
        int i, j;
        //#pragma omp parallel shared(tam1, tam2, matriz_1, matriz_2, matriz_result) private(i,j)    
        #pragma omp parallel for schedule(dynamic, 100)  num_threads(2)
        for (i =0; i<tam2 ; i++){
            for(j =0; j< tam1; j++){
                matriz_result[i][j] = matriz_1[i][j] * matriz_2[j][i];
            }
        }

    
        
    
    time = clock() -time;
    printf("Tempo de execucao schedule: %lf\n", ((double)time)/((CLOCKS_PER_SEC/1000)));
}

void encadeado(int tam1,int tam2, int matriz_1[tam2][tam1], int matriz_2[tam1][tam2], int matriz_result[tam1][tam2]){
    clock_t time;
    time = clock();
        int i, j;
        #pragma omp parallel  num_threads(2) private(i,j) shared(tam2,tam1, matriz_1, matriz_2, matriz_result) 
        for (i =0; i<tam2 ; i++){
            #pragma omp for 
            for(j =0; j< tam1; j++){
                matriz_result[i][j] = matriz_1[i][j] * matriz_2[j][i];
            }
        }
        
    time = clock() -time;
    printf("Tempo de execucao encadeado: %lf\n", ((double)time)/((CLOCKS_PER_SEC/1000)));
}

int main(int argc, char** argv){
    
    int tam1 = 600, tam2 = 800;
    int matriz_1[tam2][tam1] ;
    int matriz_2[tam1][tam2]  ;
    int matriz_result[tam2][tam1] ;
    
        
        //#pragma omp parallel for num_threads(2)
        for (int i =0; i<tam2 ; i++){
            for(int j =0; j< tam1; j++){
                matriz_1[i][j] = i+j+1;
                matriz_2[j][i] = j+2;
                matriz_result[i][j] = 0;
            }
        }

        multiThread(tam1,tam2,matriz_1,matriz_2,matriz_result);
        singleThread(tam1,tam2,matriz_1,matriz_2,matriz_result); ;
        scheduling(tam1,tam2,matriz_1,matriz_2,matriz_result); 
        encadeado(tam1,tam2,matriz_1,matriz_2,matriz_result); 
       return 0;
}
