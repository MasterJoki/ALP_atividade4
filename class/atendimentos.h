#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int dia;
    int mes;
    int cod_equip;
    int pront_tec;
    char problema[60];
    char solucao[60];
    char situ;

} Atendimento;

Atendimento lerAtendimento(){
    Atendimento atendimento;
    int dia2, mes2, posicao=0, pront, verificarCodigo, verificarProntuario, nr_equipamento;

    printf("\nA seguir, insira as informacoes para cadastrar um atendimento: \n");
    do {
        printf(" Dia: ");
        scanf("%d", &dia2);

        if(dia2 < 1 || dia2 > 31)
            printf("\nDia invalido! Tente novamente.\n");
    } while(dia2 < 1 || dia2 > 31);
    atendimento.dia = dia2;

    do {
        printf(" Mes: ");
        scanf("%d", &mes2);

        if(mes2 < 1 || mes2 > 12)
            printf("\nMes invalido! Tente novamente.\n");
    } while(mes2 < 1 || mes2 > 12);
    atendimento.mes = mes2;

    do {
        do {
            apresentarEquipamentoFuncionando();

            printf("\n Equipamento: ");
            scanf("%d", &nr_equipamento);

            if (nr_equipamento < 0)
                printf("\n Codigo invalido! tente novamente.\n");
        } while (nr_equipamento < 0);

        posicao = localizarEquipamento(nr_equipamento);

        if (posicao != -1) {
            verificarCodigo = verificarSituacaoEquipamento(posicao);
            if(verificarCodigo == 0)
                atendimento.cod_equip = nr_equipamento;
            else
                printf("\n O equipamento ja esta em manutencao! Escolha outro.\n");
        }
        else {
            verificarCodigo = 1;
            printf("\n Codigo invalido! Escolha um equipamento existente.\n");
        }
    } while (verificarCodigo == 1);

    printf("\n================ Tecnicos Disponiveis ================");
        imprimirProntuario();
    printf("\n======================================================\n");

    do {
        do {
            printf("\n Insira o protuario de um tecnico disponivel: ");
            scanf("%d", &pront);

            if (pront < 0)
                printf("\n Prontuario invalido! digite um numero positivo.\n");
        } while (pront < 0);

        verificarProntuario = getProntuarioTecnico(pront);

        if (verificarProntuario != -1)
            atendimento.pront_tec = verificarProntuario;

    } while (verificarProntuario == -1);

    printf(" Problema: ");
    fflush(stdin);
    fgets(atendimento.problema, 60, stdin);
    atendimento.problema[strcspn(atendimento.problema, "\n")] = '\0';

    printf(" Solucao: ");
    fflush(stdin);
    fgets(atendimento.solucao, 60, stdin);
    atendimento.solucao[strcspn(atendimento.solucao, "\n")] = '\0';

    atendimento.situ = 'P';

    alterarSituacaoEquipamento(posicao, 0);
    
    return atendimento;
}

void incluirAtendimento(Atendimento at){
    FILE *file;

    file = fopen("atendimentos.dat", "ab");//ab incluir no arquivo binario
    if (file == NULL)
       file = fopen("atendimentos.dat", "wb"); // write binary archive 

    if (file == NULL)
        printf("\nNao foi possivel abrir 'atendimentos.dat' em incluirAtendimento.\n");
    else{
        fwrite(&at, sizeof(Atendimento), 1, file);//fwrite grava o tecnico no final do arquivo
        printf("\nAtendimendo cadastrado com sucesso!\n");


        fclose(file);
    }
}

void imprimirAtendimento(Atendimento a) {
    printf("\n================ Dados do Atendimento ================");

    printf("\n Data: %d/%d", a.dia, a.mes);
    printf("\n Numero do equipamento: %d", a.cod_equip);
    printf("\n Prontuario do Tecnico: %d", a.pront_tec);
    printf("\n Problema: %s", a.problema);
    printf("\n Solucao: %s", a.solucao);
    if (a.situ == 'P')
        printf("\n Situacao: Pendente");
    else
        printf("\n Situacao: Finalizado");

    printf("\n======================================================\n");
}

void todosAtendimentos(){
    FILE *file;
    Atendimento a;

    file = fopen("atendimentos.dat", "rb");// ler arquivo binario
    if(file == NULL)
        printf("\nNao foi possivel abrir 'atendimentos.dat' em todosAtendimentos.\n");
    else {
        fread(&a, sizeof(Atendimento), 1, file);
        while(!feof(file)) {
            imprimirAtendimento(a);
            fread(&a, sizeof(Atendimento), 1, file);
        }

        fclose(file);
    }
}

int pegarPront(){
    int pront = 0, verificarProntuario = -1;

    printf("\n====================== Tecnicos ======================");
    imprimirProntuario();
    printf("\n======================================================\n");

    do {
        do {
            printf("\n Insira o protuario do tecnico que deseja procurar: ");
            scanf("%d", &pront);

            if (pront < 0)
                printf("\n Prontuario invalido! digite um numero positivo.\n");
        } while (pront < 0);

        verificarProntuario = getProntuarioTecnico(pront);

        if (verificarProntuario == -1)
            printf("\n Prontuario invalido! Tente novamente.\n");
    } while (verificarProntuario == -1);

    return pront;
}

void atendimentoTecnico(){
    FILE *file;
    Atendimento a;
    int pront = 0;

    file = fopen("atendimentos.dat", "rb");
    if(file == NULL)
        printf("\nNao foi possivel abrir 'atendimentos.dat' em atendimentoTecnico.\n");
    else {
        pront = pegarPront();

        fread(&a, sizeof(Atendimento), 1, file);
        while(!feof(file)){
            if(a.pront_tec == pront)
                imprimirAtendimento(a);

            fread(&a, sizeof(Atendimento), 1, file);
        }

        fclose(file);
    }
}

void atendimentoLab(){
    FILE *file;
    Atendimento a;
    int 
        lab = 0, 
        cont = 0,
        verificar = 0;

    file = fopen("atendimentos.dat", "rb");
    if(file == NULL)
        printf("\nNao foi possivel abrir 'atendimentos.dat' em atendimentoLab.\n");
    else {

        printf("\n====================== Laboratorios ==================");
        imprimirLaboratorios();
        printf("\n======================================================\n");

        do {
            do {
                printf("\n Insira o laboratorio que deseja procurar: ");
                scanf("%d", &lab);

                if (lab < 0)
                    printf("\n Laboratorio invalido! Digite um numero positivo.\n");
            } while (lab < 0);

            verificar = verificarLaboratorios(lab);

            if (verificar == 0)
                printf("\n Laboratorio invalido! Digite um laboratorio existente.\n");
        } while (verificar == 0);

        fread(&a, sizeof(Atendimento), 1, file);
        while(!feof(file)) {
            int posicao = localizarEquipamento(a.cod_equip);
            Equipamento e = getEquipamento(posicao);
            
            if(e.num_lab == lab){
                imprimirAtendimento(a);
                cont++;
            }

            fread(&a, sizeof(Atendimento), 1, file);
        }
    
        if (cont == 0)
            printf("\nNenhum chamado realizado no laboratorio especificado. \n");
        else
            printf("\nQuantidade de atendimentos no laboratorio: %d.\n", cont);

        fclose(file);
    }
}

int verificarCodigoEquipamento(){
    int
        codigo = 0,
        verificar = 0;

    printf("\n====================== Equipamentos ==================");
    imprimirCodigoEquipamento();
    printf("\n======================================================\n");

    do {
        do {
            printf("\n Insira o Equipamento que deseja: ");
            scanf("%d", &codigo);

            if (codigo < 0)
                printf("\n Equipamento invalido! Digite um numero positivo.\n");
        } while (codigo < 0);

        verificar = getCodigoEquipamento(codigo);

        if (verificar == -1)
            printf("\n Equipamento invalido! Digite um equipamento existente.\n");
    } while (verificar == -1);

    return codigo;
}

//Apresentar um histórico de atendimento para um determinado equipamento;
void atendimentoEquipamento(){
    FILE *file;
    Atendimento a;
    int codigo, 
        cont = 0;

    file = fopen("atendimentos.dat", "rb");
    if(file == NULL)
        printf("\nNao foi possivel abrir 'atendimentos.dat' em atendimentoEquipamento.\n");
    else {
        codigo = verificarCodigoEquipamento();

        fread(&a, sizeof(Atendimento), 1, file);
        while(!feof(file)) {
            int posicao = localizarEquipamento(a.cod_equip);
            Equipamento e = getEquipamento(posicao);
            
            if(e.num == codigo){
                imprimirAtendimento(a);
                cont++;
            }

            fread(&a, sizeof(Atendimento), 1, file);
        }
    
        if (cont == 0)
            printf("\nNenhum chamado realizado no equipamento especificado. \n");
        else
            printf("\nQuantidade de atendimentos deste equipamento: %d.\n", cont);

        fclose(file);
    }
}

void verificarMes(int mes){
    FILE *file;
    Atendimento a;
    int cont = 0;

    file = fopen("atendimentos.dat", "rb");
    if(file == NULL)
        printf("\nNao foi possivel abrir 'atendimentos.dat' em verificarMes.\n");
    else {
        fread(&a, sizeof(Atendimento), 1, file);
        while(!feof(file)) {
            if(a.mes == mes){
                imprimirAtendimento(a);
                cont++;
            }
            
            fread(&a, sizeof(Atendimento), 1, file);
        }
        if(cont == 0)
            printf("\n Nenhum chamado realizado no mes especificado. \n");
        else
            printf("\n Total de chamados realizados no mes especificado: %d \n", cont);
            
        fclose(file);
    }
}

//Apresentar as manutenções realizadas em um determinado mês;
void manutencaoMes(){
    int mes = 0;

    do {
        printf("\n Insira o mes que deseja procurar: ");
        scanf("%d", &mes);

        if (mes < 1 || mes > 12)
            printf("\nMes invalido! Tente novamente.\n");
    } while (mes < 1 || mes > 12);

    verificarMes(mes);
}

void imprimirEquipamentosManutencao(Equipamento e, Atendimento a){
    printf("\n=============== Dados dos Equipamentos ===============");

    printf("\n Numero: %d", e.num);
    printf("\n Descricao: %s", e.descricao);
    printf("\n Num. Laboratorio: %d", e.num_lab);
    printf("\n Num. Maquina: %d", e.num_maqna);
    if (e.situacao == 'M')
        printf("\n Situacao: Em manutencao");
    printf("\n Problema: %s", a.problema);
    printf("\n Solucao: %s", a.solucao);

    printf("\n======================================================\n");
}

int localizarAtendimentoPendente(int codigo){
    FILE *file;
    Atendimento a;
    int posicao = -1, i = 0;

    file = fopen("atendimentos.dat", "rb");
    if(file == NULL)
        printf("\nNao foi possivel abrir 'atendimentos.dat' em localizarAtendimentoPendente.\n");
    else {
        fread(&a, sizeof(Atendimento), 1, file);
        while(!feof(file) && posicao == -1) {
            if (codigo == a.cod_equip && a.situ == 'P')
                posicao = i;
            else
                i++;
            fread(&a, sizeof(Atendimento), 1, file);
        }
    }
    return posicao;
}

Atendimento getAtendimento(int posicao){
    FILE *file;
    Atendimento a;

    file = fopen("atendimentos.dat", "rb");
    if(file == NULL)
        printf("\nNao foi possivel abrir 'atendimentos.dat' em getAtendimento.\n");
    else {
        fseek(file, posicao*sizeof(Atendimento), SEEK_SET);
        fread(&a, sizeof(Atendimento), 1, file);
        fclose(file);
    }
    return a;
}

void apresentarEquipamentoManutencao(){
    FILE *file;
    Equipamento e;
    int verifica;

    file = fopen("equipamentos.dat", "rb");
    if(file == NULL)
        printf("\nNao foi possivel abrir 'equipamentos.dat' em apresentarEquipamentoManutencao.\n");
    else {
        fread(&e, sizeof(Equipamento), 1, file);
        while(!feof(file)) {
            if(e.situacao == 'M'){
                int posicao = localizarAtendimentoPendente(e.num);
                Atendimento a = getAtendimento(posicao);
                imprimirEquipamentosManutencao(e, a);
                verifica = 1;
            }
            
            if (verifica == 0)
                printf("\n Nenhum chamado realizado no mes especificado. \n");
            fread(&e, sizeof(Equipamento), 1, file);
        }

        fclose(file);
    }
}

int verificarCodigoEquipamentoManutencao(){
    int
        codigo = -1,
        verificar = 0;

    imprimirCodigoEquipamentoManutencao();

    do {
        do {
            printf("\n Insira o Equipamento que deseja: ");
            scanf("%d", &codigo);

            if (codigo < 0)
                printf("\n Equipamento invalido! Digite um numero positivo.\n");
        } while (codigo < 0);

        verificar = getCodigoEquipamento(codigo);

        if (verificar == -1)
            printf("\n Equipamento invalido! Digite um equipamento existente.\n");
    } while (verificar == -1);

    return codigo;
}

int verificarResposta(){
    int resp = 0;

    while (resp != 1 && resp != 2) {
        printf("\n Deseja finalizar a manutencao? ");
        printf("\n [1] - Sim");
        printf("\n [2] - Nao");
        printf("\n\n Sua resposta: ");
        scanf("%d", &resp);

        if (resp != 1 && resp != 2)
            printf("\nResposta invalida! Por Favor inserir apenas 1 ou 2. ");
    }

    return resp;
}

void alterarSituacaoAtendimento(int posicao){
    FILE *file;
    Atendimento a;

    file = fopen("atendimentos.dat", "rb+");
    if(file == NULL)
        printf("\nNao foi possivel abrir 'atendimentos.dat' em alterarSituacaoAtendimento.\n");
    else {
        fseek(file, posicao*sizeof(Atendimento) , SEEK_SET);

        fread(&a, sizeof(Atendimento), 1, file);
        a.situ = 'F';

        fseek(file, posicao*sizeof(Atendimento), SEEK_SET);
        fwrite(&a, sizeof(Atendimento), 1, file);

        fclose(file);
    }
}

void finalizarAtendimento() {
    FILE *file;
    Equipamento e;
    Atendimento a;
    int 
        codigo = 0,
        verifica = 0, 
        mes,
        dia,
        posicao;

    file = fopen("atendimentos.dat", "rb");
    if (file == NULL)
        printf("\nNao foi possivel abrir 'atendimentos.dat' em finalizarAtendimento.\n");
    else {
        codigo = verificarCodigoEquipamentoManutencao();

        if (codigo != -1)
        {
            posicao = localizarEquipamento(codigo);
            e = getEquipamento(posicao);

            posicao = localizarAtendimentoPendente(e.num);
            a = getAtendimento(posicao);
            
            imprimirEquipamentosManutencao(e, a);

            printf("\n A seguir, digite as informacoes do atendimento que deseja finalizar: ");
            do {
                printf("\n Dia: ");
                scanf("%d", &dia);

                if (dia < 1 || dia > 31)
                    printf("\nDia invalido! Tente novamente.\n");
            } while (dia < 1 || dia > 31);

            do {
                printf(" Mes: ");
                scanf("%d", &mes);

                if (mes < 1 || mes > 12)
                    printf("\nMes invalido! Tente novamente.\n");
            } while (mes < 1 || mes > 12);

            fread(&a, sizeof(Atendimento), 1, file);
            while(!feof(file) && verifica == 0) {
                if(a.dia == dia && a.mes == mes && codigo == a.cod_equip){
                    int resp = verificarResposta();

                    if (resp == 1) {
                        posicao = localizarEquipamento(codigo);
                        alterarSituacaoEquipamento(posicao, 1);
                        posicao = localizarAtendimentoPendente(codigo);
                        alterarSituacaoAtendimento(posicao);
                        printf("\nChamado Finalizado com sucesso. \n");
                    }

                    verifica = 1;
                }
                fread(&a, sizeof(Atendimento), 1, file);
            }

            if(verifica == 0)
                printf("\n Nenhum chamado realizado com os dados especificados. \n");
        }

        fclose(file);
    }
}
