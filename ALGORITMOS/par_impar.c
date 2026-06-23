#include <stdio.h>
int ehPar(int numero){
    if(numero % 2 == 0){
        return 1;
    } else {
        return 0;
    }
}
int main(){
    int resultado;

    resultado = ehPar(5);
    
    if(resultado == 1){
        printf("Número Par");
    } else {
        printf("Número Ímpar");
    }
    return 0;
}