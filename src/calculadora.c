#include <stdio.h>
#include "calculadora.h"

float soma(float a, float b) {
    return a + b;
}

float subtrai(float a, float b) {
    return a - b;
}

float multiplica(float a, float b) {
    return a * b;
}

float divide(float a, float b) {
    if(b == 0) {
        printf("Erro: Divisão por zero!\n");
        return 0;
    }
    return a / b;
}

int main() {
    int opcao;
    float a, b, resultado;
    
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
        scanf("%f", &a);
        printf("Digite o segundo número: ");
        scanf("%f", &b);
        
        switch(opcao) {
            case 1:
                resultado = soma(a, b);
                printf("Resultado: %.2f\n", resultado);
                break;
            case 2:
                resultado = subtrai(a, b);
                printf("Resultado: %.2f\n", resultado);
                break;
            case 3:
                resultado = multiplica(a, b);
                printf("Resultado: %.2f\n", resultado);
                break;
            case 4:
                resultado = divide(a, b);
                printf("Resultado: %.2f\n", resultado);
                break;
        }
    } while(1);
    
    return 0;
}
