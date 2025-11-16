#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FUNCIONARIOS 100

typedef enum { PENDENTE, APROVADO, REJEITADO } StatusHoras;

typedef struct {
    int id;
    char nome[100];
    float horas_extras;
    float valor_hora;
    StatusHoras status;
} Funcionario;

Funcionario funcionarios[MAX_FUNCIONARIOS];
int total_funcionarios = 0;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausa() {
    printf("\nPressione Enter para continuar...");
    getchar();
}

void cadastrarFuncionario() {
    if (total_funcionarios >= MAX_FUNCIONARIOS) {
        printf("\nLimite máximo de funcionários atingido.\n");
        return;
    }

    Funcionario f;
    f.id = total_funcionarios + 1;

    printf("\nDigite o nome do funcionário: ");
    fgets(f.nome, sizeof(f.nome), stdin);
    f.nome[strcspn(f.nome, "\n")] = '\0';

    printf("Digite o valor da hora extra (R$): ");
    scanf("%f", &f.valor_hora);
    limparBuffer();

    f.horas_extras = 0.0;
    f.status = PENDENTE;
    funcionarios[total_funcionarios] = f;
    total_funcionarios++;

    printf("\nFuncionário cadastrado com sucesso! ID: %d\n", f.id);
}

void registrarHorasExtras() {
    if (total_funcionarios == 0) {
        printf("\nNenhum funcionário cadastrado.\n");
        return;
    }

    printf("\n=== FUNCIONÁRIOS CADASTRADOS ===\n");
    for (int i = 0; i < total_funcionarios; i++) {
        printf("ID: %d | Nome: %s\n", funcionarios[i].id, funcionarios[i].nome);
    }

    int id;
    float horas;
    printf("\nDigite o ID do funcionário: ");
    scanf("%d", &id);
    limparBuffer();

    if (id < 1 || id > total_funcionarios) {
        printf("\nFuncionário não encontrado.\n");
        return;
    }

    printf("Digite o número de horas extras trabalhadas: ");
    scanf("%f", &horas);
    limparBuffer();

    funcionarios[id - 1].horas_extras += horas;
    funcionarios[id - 1].status = PENDENTE;

    printf("\n%.2f horas registradas para %s.\n", horas, funcionarios[id - 1].nome);
}

void aprovarHoras() {
    if (total_funcionarios == 0) {
        printf("\nNenhum funcionário cadastrado.\n");
        return;
    }

    printf("\n=== FUNCIONÁRIOS COM HORAS PENDENTES ===\n");
    int encontrou = 0;
    for (int i = 0; i < total_funcionarios; i++) {
        if (funcionarios[i].horas_extras > 0 && funcionarios[i].status == PENDENTE) {
            printf("ID: %d | Nome: %s | Horas: %.2f\n", funcionarios[i].id, funcionarios[i].nome, funcionarios[i].horas_extras);
            encontrou = 1;
        }
    }
    if (!encontrou) {
        printf("Nenhum registro pendente.\n");
        return;
    }

    int id, opcao;
    printf("\nDigite o ID do funcionário que deseja aprovar/rejeitar: ");
    scanf("%d", &id);
    limparBuffer();

    if (id < 1 || id > total_funcionarios) {
        printf("\nFuncionário não encontrado.\n");
        return;
    }

    printf("1 - Aprovar horas\n2 - Rejeitar horas\nEscolha uma opção: ");
    scanf("%d", &opcao);
    limparBuffer();

    if (opcao == 1) {
        funcionarios[id - 1].status = APROVADO;
        printf("\nHoras aprovadas para %s.\n", funcionarios[id - 1].nome);
    } else if (opcao == 2) {
        funcionarios[id - 1].status = REJEITADO;
        printf("\nHoras rejeitadas para %s.\n", funcionarios[id - 1].nome);
    } else {
        printf("\nOpção inválida.\n");
    }
}

void exibirRelatorio() {
    printf("\n===== RELATÓRIO DE HORAS EXTRAS =====\n");

    if (total_funcionarios == 0) {
        printf("Nenhum funcionário cadastrado.\n");
        pausa();
        return;
    }

    for (int i = 0; i < total_funcionarios; i++) {
        float total = funcionarios[i].horas_extras * funcionarios[i].valor_hora;
        char *statusStr;
        switch(funcionarios[i].status) {
            case PENDENTE: statusStr = "Pendente"; break;
            case APROVADO: statusStr = "Aprovado"; break;
            case REJEITADO: statusStr = "Rejeitado"; break;
        }

        printf("\n----------------------------------------");
        printf("\nID: %d", funcionarios[i].id);
        printf("\nNome: %s", funcionarios[i].nome);
        printf("\nHoras extras: %.2f", funcionarios[i].horas_extras);
        printf("\nValor por hora: R$ %.2f", funcionarios[i].valor_hora);
        printf("\nTotal a pagar: R$ %.2f", total);
        printf("\nStatus: %s", statusStr);
        printf("\n----------------------------------------\n");
    }
    pausa();
}

void salvarEmArquivo() {
    FILE *arquivo = fopen("horas_extras.txt", "w");
    if (arquivo == NULL) {
        printf("\nErro ao salvar o arquivo.\n");
        return;
    }

    for (int i = 0; i < total_funcionarios; i++) {
        fprintf(arquivo, "%d;%s;%.2f;%.2f;%d\n",
                funcionarios[i].id,
                funcionarios[i].nome,
                funcionarios[i].horas_extras,
                funcionarios[i].valor_hora,
                funcionarios[i].status);
    }

    fclose(arquivo);
    printf("\nDados salvos no arquivo 'horas_extras.txt'.\n");
}

void carregarDoArquivo() {
    FILE *arquivo = fopen("horas_extras.txt", "r");
    if (arquivo == NULL) {
        printf("\nNenhum arquivo encontrado.\n");
        return;
    }

    total_funcionarios = 0;
    while (fscanf(arquivo, "%d;%99[^;];%f;%f;%d\n",
                  &funcionarios[total_funcionarios].id,
                  funcionarios[total_funcionarios].nome,
                  &funcionarios[total_funcionarios].horas_extras,
                  &funcionarios[total_funcionarios].valor_hora,
                  (int*)&funcionarios[total_funcionarios].status) == 5) {
        total_funcionarios++;
        if (total_funcionarios >= MAX_FUNCIONARIOS) break;
    }

    fclose(arquivo);
    printf("\nDados carregados com sucesso!\n");
    pausa();
}

int main() {
    int opcao;

    do {
        printf("\n=========================================\n");
        printf("   SISTEMA DIGITAL DE CONTROLE DE HORAS EXTRAS\n");
        printf("=========================================\n");
        printf("1. Cadastrar funcionário\n");
        printf("2. Registrar horas extras\n");
        printf("3. Exibir relatório\n");
        printf("4. Salvar em arquivo\n");
        printf("5. Carregar do arquivo\n");
        printf("6. Aprovar/rejeitar horas extras\n");
        printf("0. Sair\n");
        printf("=========================================\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: cadastrarFuncionario(); break;
            case 2: registrarHorasExtras(); break;
            case 3: exibirRelatorio(); break;
            case 4: salvarEmArquivo(); break;
            case 5: carregarDoArquivo(); break;
            case 6: aprovarHoras(); break;
            case 0: printf("\nEncerrando o sistema... Até mais!\n"); break;
            default: printf("\nOpção inválida!\n");
        }
    } while(opcao != 0);

    return 0;
}
