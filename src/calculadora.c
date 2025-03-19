#include <stdio.h>
#include "calculadora.h"

float soma(float primeira_parcela, float segunda_parcela) {
    return primeira_parcela + segunda_parcela;
}

float subtrai(float adtivo, float subtrativo) {
    return adtivo - subtrativo;
}

float multiplica(float primeiro_fator, float segundo_fator) {
    return primeiro_fator * segundo_fator;
}

float divide(float dividendo, float divisor) {
    if(divisor == 0) {
        printf("Erro: Divisão por zero!\n");
        return 0;
    }
    return dividendo / divisor;
}

int main() {
    int opcao;
    float primeiro_numero;
    float segundo_numero;
    float resultado;
    
    do {
        printf("\nCalculadora Simples\n");
        printf("1. Soma\n");
        printf("2. Subtração\n");
        printf("3. Multiplicação\n");
        printf("4. Divisão\n");
        printf("0. Sair\n");
        printf("Escolha uma operação: ");
        scanf("%d", &opcao);
        
        if(opcao == 0) {
            printf("Saindo...\n");
            break;
        }
        
        if(opcao < 1 || opcao > 4) {
            printf("Opção inválida! Tente novamente.\n");
            continue;
        }
        
        printf("Digite o primeiro número: ");
        scanf("%f", &primeiro_numero);
        printf("Digite o segundo número: ");
        scanf("%f", &segundo_numero);
        
        switch(opcao) {
            case 1:
                resultado = soma(primeiro_numero, segundo_numero);
                printf("Resultado: %.2f\n", resultado);
                break;
            case 2:
                resultado = subtrai(primeiro_numero, segundo_numero);
                printf("Resultado: %.2f\n", resultado);
                break;
            case 3:
                resultado = multiplica(primeiro_numero, segundo_numero);
                printf("Resultado: %.2f\n", resultado);
                break;
            case 4:
                resultado = divide(primeiro_numero, segundo_numero);
                printf("Resultado: %.2f\n", resultado);
                break;
            default:
                printf("Operação inválida! Tente novamente.\n");
                break;
        }
    } while(1);
    
    return 0;
}
