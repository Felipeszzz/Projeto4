#include <stdio.h>
#include <string.h>

#define MAX_CLIENTES 1000
#define TAM_NOME 50
#define TAM_SOBRENOME 50
#define TAM_CPF 15
#define TAM_SENHA 50
#define TAM_TIPO_CONTA 10

typedef struct {
    char nome[TAM_NOME];
    char sobrenome[TAM_SOBRENOME];
    char cpf[TAM_CPF];
    char senha[TAM_SENHA];
    char tipoConta[TAM_TIPO_CONTA];
    float saldo;
    float debito;
    float deposito;
    float transferencia;
    float taxaDesconto;
} Cliente;

// Declaração da função clearBuffer
void clearBuffer();

// Função para limpar o buffer do teclado
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void carregarClientes(Cliente lista[], int *numClientes) {
    FILE *arquivo = fopen("clientes.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

    // Lê o número de clientes do arquivo
    fread(numClientes, sizeof(int), 1, arquivo);

    // Verifica se o número de clientes lido é válido
    if (*numClientes < 0 || *numClientes > MAX_CLIENTES) {
        printf("Erro: número de clientes inválido.\n");
        fclose(arquivo);
        return;
    }

    // Lê os clientes do arquivo
    fread(lista, sizeof(Cliente), *numClientes, arquivo);

    fclose(arquivo);
}

void salvarClientes(Cliente lista[], int numClientes) {
    FILE *arquivo = fopen("clientes.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de clientes para escrita.\n");
        return;
    }

    // Escreve o número de clientes no arquivo
    fwrite(&numClientes, sizeof(int), 1, arquivo);

    // Escreve os clientes no arquivo
    fwrite(lista, sizeof(Cliente), numClientes, arquivo);

    fclose(arquivo);
}

void novoCliente(Cliente lista[], int *numClientes) {
    if (*numClientes >= MAX_CLIENTES) {
        printf("A lista de clientes está cheia.\n");
        return;
    }

    printf("Digite o seu nome: ");
    scanf("%49s", lista[*numClientes].nome);
    clearBuffer();

    printf("Digite o seu sobrenome: ");
    scanf("%49s", lista[*numClientes].sobrenome);
    clearBuffer();

    printf("Tipo de conta (comum ou plus): ");
    scanf("%9s", lista[*numClientes].tipoConta);
    clearBuffer();

    printf("Digite o seu CPF: ");
    scanf("%14s", lista[*numClientes].cpf);
    clearBuffer();

    printf("Digite o seu saldo: ");
    scanf("%f", &lista[*numClientes].saldo);
    clearBuffer();

    printf("Digite a sua senha: ");
    scanf("%49s", lista[*numClientes].senha);
    clearBuffer();

    (*numClientes)++;
}

void listarClientes(Cliente lista[], int numClientes) {
    if (numClientes == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    printf("Lista de Clientes:\n");
    printf("\n");
    for (int i = 0; i < numClientes; i++) {
        printf("NOME: %s\n", lista[i].nome);
        printf("SOBRENOME: %s\n", lista[i].sobrenome);
        printf("CONTA: %s\n", lista[i].tipoConta);
        printf("SENHA: %s\n", lista[i].senha);
        printf("CPF: %s\n", lista[i].cpf);
        printf("SALDO: %.2f\n", lista[i].saldo);
        printf("\n");
    }
}

void deletarCliente(Cliente lista[], int *numClientes) {
    if (*numClientes == 0) {
        printf("Não há clientes cadastrados para serem deletados.\n");
        return;
    }

    char cpf[TAM_CPF];
    printf("Digite o CPF do cliente que deseja deletar: ");
    scanf("%14s", cpf);
    clearBuffer();

    int achouCliente = 0;
    for (int i = 0; i < *numClientes; i++) {
        if (strcmp(lista[i].cpf, cpf) == 0) {
            achouCliente = 1;

            for (int j = i; j < *numClientes - 1; j++) {
                lista[j] = lista[j + 1];
            }

            (*numClientes)--;
            printf("O cliente foi deletado com sucesso!\n");
            break;
        }
    }

    if (!achouCliente) {
        printf("Esse cliente não foi cadastrado para ser deletado.\n");
    }
}

float debitar(Cliente lista[], int numClientes) {
    if (numClientes == 0) {
        printf("Não há clientes para debitar.\n");
        return 0.0; 
    }

    char cpf[TAM_CPF];
    char senha[TAM_SENHA];
    float valor;
    printf("Digite o CPF do cliente: ");
    scanf("%14s", cpf);
    clearBuffer();

    int clienteEncontrado = 0;
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(lista[i].cpf, cpf) == 0) {
            clienteEncontrado = 1;
            printf("Digite a senha da conta: ");
            scanf("%49s", senha);
            clearBuffer();

            if (strcmp(lista[i].senha, senha) != 0) {
                printf("Senha incorreta.\n");
                return 0.0;
            }

            printf("Digite o valor a ser debitado: ");
            scanf("%f", &valor);
            clearBuffer();

            if (valor <= 0) {
                printf("Valor inválido para débito.\n");
                return 0.0;
            }

            float taxaDesconto;
            if (strcmp(lista[i].tipoConta, "comum") == 0) {
                taxaDesconto = 0.05;
                if (lista[i].saldo - valor - (valor * taxaDesconto) >= -1000.0) {
                    lista[i].saldo -= valor + (valor * taxaDesconto);
                    lista[i].debito += valor; 
                    lista[i].taxaDesconto = taxaDesconto; 
                    printf("Débito de %.2f realizado com sucesso.\n", valor);
                    return valor; 
                } else {
                    printf("Transação não autorizada. Limite de saldo excedido.\n");
                    return 0.0;
                }
            } else if (strcmp(lista[i].tipoConta, "plus") == 0) {
                taxaDesconto = 0.03;
                if (lista[i].saldo - valor - (valor * taxaDesconto) >= -5000.0) {
                    lista[i].saldo -= valor + (valor * taxaDesconto);
                    lista[i].debito += valor; 
                    lista[i].taxaDesconto = taxaDesconto; 
                    printf("Débito de %.2f realizado com sucesso.\n", valor);
                    return valor; 
                } else {
                    printf("Transação não autorizada. Limite de saldo excedido.\n");
                    return 0.0;
                }
            } else {
                printf("Tipo de conta inválido.\n");
                return 0.0;
            }
        }
    }

    if (!clienteEncontrado) {
        printf("Cliente não encontrado(o CPF pode estar incorreto).\n");
    }

    return 0.0; 
}


float depositoCliente(Cliente lista[], int numClientes) {
    if (numClientes == 0) {
        printf("Não há clientes para depositar.\n");
        return 0.0; 
    }

    char cpf[TAM_CPF];
    float valor;
    printf("Digite o CPF do cliente: ");
    scanf("%14s", cpf);
    clearBuffer();

    int clienteEncontrado = 0;
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(lista[i].cpf, cpf) == 0) {
            clienteEncontrado = 1;
            printf("Digite o valor a ser depositado: ");
            scanf("%f", &valor);
            clearBuffer();

            if (valor <= 0) {
                printf("Valor inválido para depósito.\n");
                return 0.0; 
            }

            lista[i].saldo += valor;
            lista[i].deposito += valor; 
            printf("Depósito de %.2f realizado com sucesso.\n", valor);
            return valor; 
        }
    }

    if (!clienteEncontrado) {
        printf("Cliente não encontrado.\n");
    }

    return 0.0; 
}

void extrato(Cliente lista[], int numClientes) {
    char cpf[TAM_CPF];
    char senha[TAM_SENHA];
    printf("Digite o CPF do cliente para ver o extrato: ");
    scanf("%14s", cpf);
    clearBuffer();

    printf("Digite a senha do cliente: ");
    scanf("%49s", senha);
    clearBuffer();

    int clienteEncontrado = 0;
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(lista[i].cpf, cpf) == 0 && strcmp(lista[i].senha, senha) == 0) {
            clienteEncontrado = 1;
            FILE *arquivo = fopen("extrato.txt", "w");
            if (arquivo == NULL) {
                printf("Erro ao abrir o arquivo de extrato.\n");
                return;
            }

            fprintf(arquivo, "Extrato para o cliente %s:\n", lista[i].cpf);
            fprintf(arquivo, "Nome: %s\n", lista[i].nome);
            fprintf(arquivo, "Sobrenome: %s \n", lista[i].sobrenome);
            fprintf(arquivo, "Tipo de conta: %s\n", lista[i].tipoConta);  
            fprintf(arquivo, "Débitos que foram realizados: %.2f\n", lista[i].debito);
            fprintf(arquivo, "Depósitos que foram realizados: %.2f\n", lista[i].deposito);
            fprintf(arquivo, "Transferências que foram realizadas: %.2f\n", lista[i].transferencia);
            fprintf(arquivo, "Taxa: %.2f\n", lista[i].taxaDesconto);
            fprintf(arquivo, "Saldo atual: %.2f\n", lista[i].saldo);

            fclose(arquivo);
            printf("Extrato salvo com sucesso no arquivo extrato.txt.\n");
            return;
        }
    }

    if (!clienteEncontrado) {
        printf("CPF ou senha incorretos. Não é possível fazer o extrato.\n");
    }
}

void transferencia(Cliente lista[], int numClientes) {
    char cpfOrigem[TAM_CPF];
    char cpfDestino[TAM_CPF];
    char senhaOrigem[TAM_SENHA];
    float valor;

    printf("Digite o CPF do cliente de origem: ");
    scanf("%14s", cpfOrigem);
    clearBuffer();

    printf("Digite a senha da conta de origem: ");
    scanf("%49s", senhaOrigem);
    clearBuffer();

    printf("Digite o CPF do cliente de destino: ");
    scanf("%14s", cpfDestino);
    clearBuffer();

    int clienteOrigemEncontrado = 0;
    int clienteDestinoEncontrado = 0;
    int indexClienteOrigem = -1;
    int indexClienteDestino = -1;

    // Verifica se o cliente de origem existe e se a senha está correta
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(lista[i].cpf, cpfOrigem) == 0 && strcmp(lista[i].senha, senhaOrigem) == 0) {
            clienteOrigemEncontrado = 1;
            indexClienteOrigem = i;
            break;
        }
    }

    // Verifica se o cliente de destino existe
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(lista[i].cpf, cpfDestino) == 0) {
            clienteDestinoEncontrado = 1;
            indexClienteDestino = i;
            break;
        }
    }

    if (!clienteOrigemEncontrado) {
        printf("CPF ou senha incorretos para o cliente de origem.\n");
        return;
    }

    if (!clienteDestinoEncontrado) {
        printf("Cliente de destino não encontrado.\n");
        return;
    }

    // Solicita o valor a ser transferido
    printf("Digite o valor a ser transferido: ");
    scanf("%f", &valor);
    clearBuffer();

    // Verifica se o saldo do cliente de origem é suficiente
    if (valor > lista[indexClienteOrigem].saldo) {
        printf("Saldo insuficiente para realizar a transferência.\n");
        return;
    }

    // Realiza a transferência
    lista[indexClienteOrigem].saldo -= valor;
    lista[indexClienteOrigem].transferencia -= valor;
    lista[indexClienteDestino].saldo += valor;
    lista[indexClienteDestino].transferencia += valor;

    printf("Transferência de %.2f realizada com sucesso de %s para %s.\n", valor,
            lista[indexClienteOrigem].cpf, lista[indexClienteDestino].cpf);
}

int main() {
    Cliente listaclientes[MAX_CLIENTES];
    int numClientes = 0;
    char opcao;

    // Carrega os clientes do arquivo binário
    carregarClientes(listaclientes, &numClientes);

    do {
        printf("\nMenu Principal:\n");
        printf("1- Novo Cliente\n");
        printf("2- Apagar Cliente\n");
        printf("3- Listar Clientes\n");
        printf("4- Débito\n");
        printf("5- Depósito\n");
        printf("6- Transferência entre contas\n");
        printf("7- Extrato\n");
        printf("0- Sair\n");
        printf("Escolha uma opção: ");
        scanf(" %c", &opcao);

        switch (opcao) {
            case '1':
                novoCliente(listaclientes, &numClientes);
                break;

            case '2':
                deletarCliente(listaclientes, &numClientes);
                break;

            case '3':
                listarClientes(listaclientes, numClientes);
                break;

            case '4':
                debitar(listaclientes, numClientes);
                break;

            case '5':
                depositoCliente(listaclientes, numClientes);
                break;

            case '6':
                transferencia(listaclientes, numClientes);
                break;

            case '7':
                extrato(listaclientes, numClientes);
                break;

            case '0':
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida.\n");
                break;
        }

    } while (opcao != '0');

    // Salva os clientes de volta no arquivo binário antes de sair
    salvarClientes(listaclientes, numClientes);

    return 0;
}
