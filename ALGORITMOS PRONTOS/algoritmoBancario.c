#include <stdio.h>
#define MAX_TRANSACOES 50

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void mostrarTelaInicial() {
    printf("\x1b[34m");
    printf("┌─────────────────────┐\n");
    printf("│    BEM VINDO AO     │\n");
    printf("│    BANCO COMANDO    │\n");
    printf("│       CENTRAL       │\n");
    printf("└─────────────────────┘\n");
    printf("\x1b[0m");
}

int autenticar() {
    int senhaCorreta = 1234, tentativa, tentativasErradas = 0;
    do {
        mostrarTelaInicial();
        printf("Digite a senha: ");
        if(scanf("%i", &tentativa) != 1) {
            limparBuffer();
            tentativa = -1;
        } else {
            limparBuffer();
        }
        
        if (tentativa == senhaCorreta) return 1;
        
        tentativasErradas++; 
        printf("\x1b[31m Senha incorreta. Tente novamente. \x1b[0m\n\n");
        
        if(tentativasErradas < 3) {
            printf("Você tem mais %i tentativa(s).\x1b[0m\n", 3 - tentativasErradas);
        } else {
            printf("SISTEMA BLOQUEADO POR SEGURANÇA!\x1b[0m \n");
            return 0;
        }
    } while (tentativasErradas < 3);
    return 0;
}

int mostrarMenu() {
    int opcao;
    printf("\n \x1b[33m====\x1b[0m\x1b[34m CONTA BANCÁRIA \x1b[0m\x1b[33m====\x1b[0m\n");
    printf("\x1b[32m1 - SALDO \n"); 
    printf("2 - SACAR \n");
    printf("3 - DEPOSITAR \n");
    printf("4 - EXTRATO \n");
    printf("0 - SAIR DO SISTEMA \x1b[0m\n");
    printf("Opção: ");
    
    if(scanf("%i", &opcao) != 1) {
        printf("\n \x1b[31mDigite apenas números!\x1b[0m\n");
        limparBuffer();
        return -1;
    }
    limparBuffer();
    return opcao;
}

void processoSaque(float *saldo, char historico[MAX_TRANSACOES][100], int *qtdTransacoes) {
    float valorSolicitado;
    int n100, n50, n20, n10, n5, n2, resto, valor_int;

    printf("\x1b[36m---- SACAR ----\x1b[0m\nValor para saque: R$ ");
    if(scanf("%f", &valorSolicitado) != 1) {
        printf("\x1b[31mValor inválido.\x1b[0m\n");
        limparBuffer();
        return;
    }
    limparBuffer();

    if(valorSolicitado <= 0) {
        printf("\x1b[31mValor inválido. \x1b[0m\n");
        return;
    }
    if(valorSolicitado > *saldo) {
        printf("\x1b[31mSaldo insuficiente! \x1b[0m\n");
        return;
    }

    valor_int = (int)valorSolicitado;
    if (valorSolicitado != valor_int || valor_int == 1 || valor_int == 3) {
        printf("\n\x1b[31m O caixa não possui essas notas ou centavos. \x1b[0m\n");
        return;
    }
    
    resto = valor_int;
    if (resto % 2 != 0) {
        n5 = 1;
        resto -= 5;
    } else {
        n5 = 0;
    }
    
    n100 = resto / 100; resto %= 100;
    n50 = resto / 50;   resto %= 50;
    n20 = resto / 20;   resto %= 20;
    n10 = resto / 10;   resto %= 10;
    n2 = resto / 2;     resto %= 2;
    
    *saldo -= valorSolicitado; 
    
    printf("\n| Saque processado! \n| Solicitado: R$ %.2f \n| Sacado: R$ %.2f \n", valorSolicitado, valorSolicitado);

    if(n100 + n50 + n20 + n10 + n5 + n2 > 0) {
        printf("\nNotas a receber: \n");
        if (n100 > 0) printf("%i nota(s) de R$ 100,00 \n", n100);
        if (n50 > 0) printf("%i nota(s) de R$ 50,00 \n", n50);
        if (n20 > 0) printf("%i nota(s) de R$ 20,00 \n", n20);
        if (n10 > 0) printf("%i nota(s) de R$ 10,00 \n", n10);
        if (n5 > 0) printf("%i nota(s) de R$ 5,00 \n", n5);
        if (n2 > 0) printf("%i nota(s) de R$ 2,00 \n", n2);
    }
    
    if(*qtdTransacoes < MAX_TRANSACOES) {
        snprintf(historico[*qtdTransacoes], 100, "Saque: -R$ %.2f", valorSolicitado);
        (*qtdTransacoes)++;
    }
    
    printf("\n Saldo atualizado: R$ %.2f \n", *saldo);
}

void processoDeposito(float *saldo, char historico[MAX_TRANSACOES][100], int *qtdTransacoes) {
    float valorSolicitado;
    printf("\x1b[36m---- DEPOSITO ----\x1b[0m\nValor a ser depositado: R$ ");
    
    if(scanf("%f", &valorSolicitado) != 1) {
        printf("\x1b[31mValor inválido.\x1b[0m\n");
        limparBuffer();
        return;
    }
    limparBuffer();
    
    if(valorSolicitado > 0) {
        *saldo += valorSolicitado;
        printf("\x1b[32mDepósito realizado!\nNovo saldo disponivel: R$ %.2f \x1b[0m\n", *saldo);
        
        if(*qtdTransacoes < MAX_TRANSACOES) {
            snprintf(historico[*qtdTransacoes], 100, "Depósito: +R$ %.2f", valorSolicitado);
            (*qtdTransacoes)++;
        }
    } else {
        printf("\x1b[31mValor inválido para depósito. \x1b[0m\n");
    }
}

void exibirExtrato(float saldo, char historico[MAX_TRANSACOES][100], int qtdTransacoes) {
    printf("\n\x1b[36m---- EXTRATO ----\x1b[0m\n");
    if(qtdTransacoes == 0) {
        printf("Nenhuma transação realizada\n");
    } else {
        for(int i = 0; i < qtdTransacoes; i++) {
            printf(" [%i] %s\n", i + 1, historico[i]);
        }
    }
    printf("\x1b[32m Saldo atual: R$ %.2f \x1b[0m\n", saldo);
}

void processarOpcao(int opcao, float *saldo, char historico[MAX_TRANSACOES][100], int *qtdTransacoes) {
    switch(opcao) {
        case 1:
            printf("\n\x1b[32m O saldo é de: R$ \x1b[0m%.2f\n", *saldo);
            break;
        case 2:
            processoSaque(saldo, historico, qtdTransacoes);
            break;
        case 3:
            processoDeposito(saldo, historico, qtdTransacoes);
            break;
        case 4:
            exibirExtrato(*saldo, historico, *qtdTransacoes);
            break;
        case 0:
            printf("\n \x1b[32mObrigado por usar o banco!\n Volte sempre! \x1b[0m\n");
            break;
        case -1:
            break; 
        default:
            printf("\n \x1b[31mOpção inválida! Escolha 0, 1, 2, 3 ou 4. \x1b[0m\n");
    }
}

int main() {
    float saldo = 0;
    char hist[MAX_TRANSACOES][100];
    int qtd = 0, opcao;
    
    if (!autenticar()) return 0;
    
    do {
        opcao = mostrarMenu();
        processarOpcao(opcao, &saldo, hist, &qtd);
    } while (opcao != 0);
    return 0;
}