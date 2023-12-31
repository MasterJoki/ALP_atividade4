#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int num;
    char descricao[60];
    int num_lab;
    int num_maqna;
    char situacao;
    int contManutencoes;
} Equipamento;

int localizarEquipamento(int codigo){
    FILE *file;
    Equipamento e;
    int posicao = -1, i = 0;

    file = fopen("equipamentos.dat", "rb");

    if(file == NULL)
        printf("\nNao foi possivel abrir 'equipamentos.dat' em localizarEquipamento.\n");
    else {
        fread(&e, sizeof(Equipamento), 1, file);

        // no while eu verifico se nao chegou ao fim do arquivo
        // e tambem nao encontrou
        while(!feof(file) && posicao == -1) {
            if (e.num == codigo)
                posicao = i;
            else {
                i++;
                fread(&e, sizeof(Equipamento), 1, file);
            }
        }
    }
    return posicao;
}

Equipamento lerEquipamento(){
    int num = 0, verificarCodigo = 10;
    Equipamento equipamento;

    printf("\nA seguir, insira as informacoes para cadastrar um equipamento: \n");
    
    while (verificarCodigo != -1) {
        do {
            printf("\n Numero: ");
            scanf("%d", &num);

            if(num <= 0)
            printf("\n Numero invalido! Digite um numero maior que 0. \n");
        } while (num <= 0);

        if (sizeof(Equipamento) == 0)
            verificarCodigo = localizarEquipamento(num);
        else
            verificarCodigo = -1;

        if (verificarCodigo != -1)
            printf("Este numero ja existe, tente novamente. ");
    }

    equipamento.num = num;

    printf(" Descricao: ");
    fflush(stdin);
    fgets(equipamento.descricao, 60, stdin);
    equipamento.descricao[strcspn(equipamento.descricao, "\n")] = '\0';

    do {
        printf(" Numero do Laboratorio: ");
        scanf("%d", &equipamento.num_lab);

        if(equipamento.num_lab <= 0)
        printf("\n Laboratorio invalido! Digite um numero maior que 0. \n");

    } while (equipamento.num_lab <= 0);

    do {
        printf(" Numero da Maquina: ");
        scanf("%d", &equipamento.num_maqna);

        if(equipamento.num_maqna <= 0)
        printf("\n Maquina invalida! Digite um numero maior que 0.\n");

    } while(equipamento.num_maqna <= 0);

    equipamento.situacao = 'F'; // funcionando
    equipamento.contManutencoes = 0;

    return equipamento;
}

void incluirEquipamento(Equipamento equipamento){
    FILE *file;

    file = fopen("equipamentos.dat", "ab");//ab incluir arquivo binario
    if (file == NULL)
       file = fopen("equipamentos.dat", "wb"); // whrite binary archive

    if (file == NULL)
        printf("\nNao foi possivel abrir 'equipamentos.dat' em incluirEquipamento.\n");
    else {
        fwrite(&equipamento, sizeof(Equipamento), 1, file);//fwrite grava o eqpamento no final do arquivo
        fclose(file);
    }
}

void imprimirEquipamentos(Equipamento e){
    printf("\n=============== Dados dos Equipamentos ===============");

    printf("\n Numero: %d", e.num);
    printf("\n Descricao: %s", e.descricao);
    printf("\n Num. Laboratorio: %d", e.num_lab);
    printf("\n Num. Maquina: %d", e.num_maqna);
    if (e.situacao == 'F')
        printf("\n Situacao: Funcionando");
    else
        printf("\n Situacao: Em manutencao");

    printf("\n======================================================\n");
}

void todosEquipamentos(){
    FILE *file;
    Equipamento e;

    file = fopen("equipamentos.dat", "rb");// ler arquivo binario
    if(file == NULL)
        printf("\nNao foi possivel abrir 'equipamentos.dat' em todosEquipamentos.\n");
    else {
        fread(&e, sizeof(Equipamento), 1, file);
        while(!feof(file)) {
            imprimirEquipamentos(e);
            fread(&e, sizeof(Equipamento), 1, file);
        }

        fclose(file);
    }
}

Equipamento getEquipamento(int posicao){
    FILE *file;
    Equipamento e;

    file = fopen("equipamentos.dat", "rb");
    if(file == NULL)
        printf("\nNao foi possivel abrir 'equipamentos.dat' em getEquipamento.\n");
    else {
        // parametros
        // 1 - identificador do arquivo
        // 2 - quantas posicoes vou deslocar
        // 3 - a partir de onde comeca o deslocamento
        //          SEEK_SET - inicio do arquivo
        //          SEEK_CUR - posicao atual
        //          SEEK_END - final do arquivo
        fseek(file, posicao*sizeof(Equipamento), SEEK_SET);
        fread(&e, sizeof(Equipamento), 1, file);
        fclose(file);
    }
    return e;
}

int getCodigoEquipamento(int codigo){
    FILE *file;
    Equipamento e;
    int posicao = 0;

    file = fopen("equipamentos.dat", "rb");
    if (file == NULL)
        printf("\nNao foi possivel abrir 'equipamentos.dat' em getCodigoEquipamento.\n");
    else {
        posicao = localizarEquipamento(codigo);
        if (posicao == -1) {
            printf("\nNao ha equipamento com esse codigo. \n");
            return posicao;
        }
        else {
            e = getEquipamento(posicao);

            // posiciono no registro correto
            fseek(file, posicao * sizeof(Equipamento), SEEK_SET);
            fread(&e, sizeof(Equipamento), 1, file);
            codigo = e.num_maqna;

            fclose(file);
            return codigo;
        }
    }
    return -1;
}

void sobreescreverSituacaoEquipamento(Equipamento e, int posicao){
    FILE *file;

    file = fopen("equipamentos.dat", "rb+");
    if(file == NULL)
        printf("\nNao foi possivel abrir 'equipamentos.dat' em sobreescreverSituacaoEquipamento.\n");
    else {
        fseek(file, posicao*sizeof(Equipamento) , SEEK_SET);
        fwrite(&e, sizeof(Equipamento), 1, file);
        fclose(file);
    }
}

void alterarSituacaoEquipamento(int posicao, int codigo){
    FILE *file;
    Equipamento e;

    file = fopen("equipamentos.dat", "rb+");
    if(file == NULL)
        printf("\nNao foi possivel abrir 'equipamentos.dat' em alterarSituacaoEquipamento.\n");
    else {
        // posiciono no registro correto
        fseek(file, posicao*sizeof(Equipamento) , SEEK_SET);

        // leio o registro
        fread(&e, sizeof(Equipamento), 1, file);

        // mudando situacao do equipamento
        if(codigo == 0) {
            e.situacao = 'M';
            e.contManutencoes++;
            printf("\nA situacao do equipamento foi modificada para 'MANUTENCAO'");
        }
        else {
            e.situacao = 'F';
            printf("\nA situacao do equipamento foi modificada para 'FUNCIONANDO'");
        }

        // reposiciono no registro correto
        fseek(file, posicao*sizeof(Equipamento), SEEK_SET);

        // regravo o registro
        fwrite(&e, sizeof(Equipamento), 1, file);

        fclose(file);
    }
}

int verificarLab(int lab[], int *cont, int k){
    int verify = 0;
    
    for (int i = 0; i < *cont; i++) {
        if(lab[i] == k){
            verify = 1; 
            i = *cont;
        }
    }
    
    if (verify == 0) {
        lab[*cont] = k;
        (*cont)++;
    }

    return verify;
}

void imprimirLaboratorios(){
    FILE *file;
    Equipamento e;
    int lab[100], cont = 0;

    file = fopen("equipamentos.dat", "rb");
    if(file == NULL)
        printf("\nNao foi possivel abrir 'equipamentos.dat' em imprimirLaboratorios.\n");
    else {
        fread(&e, sizeof(Equipamento), 1, file);
        while(!feof(file)) {
            int verify = verificarLab(lab, &cont, e.num_lab);
            if(verify == 0) {
                printf("\n Num. Laboratorio: %d", e.num_lab);
            }       
            fread(&e, sizeof(Equipamento), 1, file);
        }
        fclose(file);
    }
}

int verificarLaboratorios(int lab){
    FILE *file;
    Equipamento e;
    int verificar = 0;

    file = fopen("equipamentos.dat", "rb");
    if(file == NULL)
        printf("\nNao foi possivel abrir 'equipamentos.dat' em verificarLaboratorios.\n");
    else {
        fread(&e, sizeof(Equipamento), 1, file);

        while(!feof(file) && verificar == 0) {
            if (e.num_lab == lab)
                verificar = 1;
            else
                fread(&e, sizeof(Equipamento), 1, file);
        }
    }
    return verificar;
}

void imprimirCodigoEquipamento(){
    FILE *file;
    Equipamento e;

    file = fopen("equipamentos.dat", "rb");
    if(file == NULL)
        printf("\nNao foi possivel abrir 'equipamentos.dat' em imprimirCodigoEquipamento.\n");
    else {
        fread(&e, sizeof(Equipamento), 1, file);
        while(!feof(file)) {
            printf("\n Numero do Equipamento: %d  ", e.num);
            fread(&e, sizeof(Equipamento), 1, file);
        }
        fclose(file);
    }
}

void imprimirCodigoEquipamentoManutencao(){
    FILE *file;
    Equipamento e;

    file = fopen("equipamentos.dat", "rb");
    if(file == NULL)
        printf("\nNao foi possivel abrir 'equipamentos.dat' em imprimirCodigoEquipamentoManutencao.\n");
    else {
        printf("\n============= Equipamentos em manutencao =============");

        fread(&e, sizeof(Equipamento), 1, file);
        while(!feof(file)) {
            if (e.situacao == 'M')
                printf("\n Numero do Equipamento: %d  ", e.num);
            
            fread(&e, sizeof(Equipamento), 1, file);
        }

        printf("\n======================================================\n");

        fclose(file);
    }
}

int verificarSituacaoEquipamento(int posicao){
    Equipamento e = getEquipamento(posicao);

    if (e.situacao == 'F')
        return 0;
    else
        return 1;
}

void apresentarEquipamentoFuncionando(){
    FILE *file;
    Equipamento e;

    file = fopen("equipamentos.dat", "rb");
    if(file == NULL)
        printf("\nNao foi possivel abrir 'equipamentos.dat' em apresentarEquipamentoFuncionando.\n");
    else {
        printf("\n============== Equipamentos Disponiveis ==============");
        
        fread(&e, sizeof(Equipamento), 1, file);
        while(!feof(file)) {
            if(e.situacao == 'F')
                printf("\n Numero do Equipamento: %d ", e.num);
            
            fread(&e, sizeof(Equipamento), 1, file);
        }

        printf("\n======================================================\n");
        
        fclose(file);
    }
}

int verificarTemEquip(char situ){
    FILE *file;
    Equipamento e;
    int verifica = 0;

    file = fopen("equipamentos.dat", "rb");
    if(file == NULL)
        printf("\nNao foi possivel abrir 'equipamentos.dat' em verificarTemEquip.\n");
    else {
        fread(&e, sizeof(Equipamento), 1, file);
        while(!feof(file) && verifica == 0) {
            if(e.situacao == situ)
                verifica = 1;

            fread(&e, sizeof(Equipamento), 1, file);
        }

        if (verifica == 0){
            printf("\n Nenhum equipamento em ");
            if (situ == 'M')
                printf("manutencao no momento. \n");
            else
                printf("funcionamento no momento. \n");
        }
        else
            return 1;
        
        fclose(file);
    }
    return 0;
}

//(Desafio) Apresentar o total de manutenções para cada um dos equipamentos
void totalManutencaoEquipamentos(){
    FILE *file;
    Equipamento e;

    file = fopen("equipamentos.dat", "rb");// ler arquivo binario
    if(file == NULL)
        printf("\nNao foi possivel abrir 'equipamentos.dat' em totalManutencaoEquipamentos.\n");
    else {
        fread(&e, sizeof(Equipamento), 1, file);
        while(!feof(file)) {
            printf("\n Numero: %d", e.num);
            printf("\n Numero de manutencoes: %d\n", e.contManutencoes);
            fread(&e, sizeof(Equipamento), 1, file);
        }

        fclose(file);
    }
}
