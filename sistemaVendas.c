#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define DIF_TABULACAO 40

/*
*	crie o arquivo produtos.txt na pasta
*	depois so pegar e rodar
*/

//Novidades da versão
/*

	* Simplificação da funcao de cadastro de vendas, correçao de bugs e mais
	* implementada nova forma, simplificada, de gravar o grupo do produto
	* adicionada a frase "SEM MOVIMENTO" nos relatorios de produto, caso o mesmo nao contenha informacao
	* modificado os campos de variáveis auxiliares nas funcoes de produto, passamos a utilizar a prorpia
estrutura como variaveis auxiliares

*/
// Rel exibir produtos cadastrados


/*Ponteiros para arquivos do sistema*/
FILE *pArq_Venda;
FILE *pArq_Produto;
FILE *pArq_Cliente;

typedef struct cliente{
	char cpf[12];
	char nome[50];
	int DataNascimento;
	float pontuacao;
}infoCliente;

typedef struct produto{
	int codigoBarras;
	char nome[50];
	char undMedida[3];
	int grupo;
	float pontuacao;
	float valor;
}infoProduto;

typedef struct RegistroVenda{		//Rodape de uma venda cadastrada, contem o valor total dos itens comprados, identificador do cliente, data e desconto aplicado;
	char verificador;
	long int codigoVenda;
	unsigned int data;
	char codigoCliente[12];
	float descAplicado;
	double valorTotal;
}RegistroVenda;

typedef struct ExtratoVenda{		//Extratos de uma venda cadastrada, contem o cÃ³digo, a quantidade, o valor unitario e total, e a pontuacao do produto;
	char verificador;
	int codigoProduto;
	unsigned int qtdProduto;
	float valorProduto;
	float ptsProduto;
	double valorTotal;
	}ExtratoVenda;

typedef struct item{				//Estrutura necessaria para encadeamento de todos os produtos do extrato da venda;
	ExtratoVenda extrato;
	struct item *proxItem;
	char nomeProduto[50];
}Item;
typedef struct listaClientes{
    char nome[50];
    double valorCompra;             //Estrutura necessaria para relatrio de cliente
    unsigned int nVisitas;			//Nome e Tipo Alterados
    struct listaClientes *proxCliente;
    struct listaClientes *predCliente;		//Nome Alterado
}listaClientes;



void cadastrarProduto();
void menuGrupo_Produto();
void menu_cadastrarProduto();

void cadastrarCliente(void);
short checkCliente(char codigoCliente[]);
void atualizarPontuacao(char cod_Cliente[], float pontuacao);

void normalizarString(char strNormal[]);
void retirarUnderlineStr(char strUnderline[]);
void espacamentoColunas(char strBase[]);

void alterarValor_Produto(int codInformado);
void menu_RelProdutosPorValor();
void rel_produtoPorGrupo(int grupoInformado);
void rel_produtosPorValor();
void rel_produtosCadastrados();
void adicionarCliente_rel(listaClientes **primeiroCliente);
void encadearClientes(listaClientes **primeiroCliente, char nomeCliente[], double valorTotal);
void relClienteOrdenado(listaClientes **primeiroCliente, char opcao);
void rel_Clientes();
void faturamentoPeriodo();

void cadastrarVenda(void);												//FunÃ§Ã£o de gerenciamento de vendas;
short checkCliente(char codCliente[]);									//Verificar existencia do cliente com base em seu cÃ³digo;
short checkProduto(int codProduto);										//Verificar existencia do produto com base em seu cÃ³digo;
unsigned int gerarData(unsigned int dataRegistrada);					//Gerar data da nova venda;
void adicionarItem(Item **primeiroItem, ExtratoVenda extrato, char nomeProduto[]);			//EnlaÃ§ar novos itens selecionados pelo cliente na lista;
float gravarItens(RegistroVenda *novaVenda, Item **primeiroItem);
void listarItens(Item *itens);								//passar primeiro item por valor
void removerItens(Item **primeiroItem);						//Escrever no arquivo a lista de itens comprados;
float aplicarDesconto(RegistroVenda *novaVenda);						//Sistema de pontuaÃ§Ã£o e descontos
short loginCliente(char codCliente[], char opcao);									//Sistema de identificacao do comprador
void selecaoProdutos(Item **primeiroItem);	//Sistema de gerenciamento de produtos a serem vendidos
unsigned int dataInvertida(unsigned int data);


int main(){

	pArq_Venda = fopen("Equipe4_venda.txt", "a+");
	char checkEOF = fgetc(pArq_Venda);
	if(fgetc(pArq_Venda) == EOF)
		fprintf(pArq_Venda, "  %ld %ld %s %.2f %2.lf\n",1000000,20190823,"000000000",0.00,0.00);
	fclose(pArq_Venda);

	for(short opcaoInf = -1;opcaoInf != 0;){ //inicio laço infinito
		system("cls");
		printf("===========================================\n\n");
		printf(" 01-> ===== GERENCIAMENTO DE PRODUTOS ====\n\n");
		printf(" 02-> ===== GERENCIAMENTO DE CLIENTES ====\n\n");
		printf(" 03-> ========= CADASTRAR VENDA ==========\n\n");
		printf(" 04-> ====== FATURAMENTO P/ PERIODO ======\n\n");
		printf(" 05-> ====== RELATORIOS DE CLIENTES ======\n\n");
		printf(" 00-> ============== SAIR ================\n\n");
		printf("===========================================\n\n");
		printf("Informe uma opcao: ");
		scanf(" %d", &opcaoInf);

		if(opcaoInf == 1){
				system("cls");
				printf("1-> CADASTRAR PRODUTO\n");
				printf("2-> ALTERAR O VALOR DE UM PRODUTO\n");
				printf("3-> RELATORIOS\n");
				int opcaoInf_Produto = 0;
				printf("\nInforme uma opcao: ");
				scanf(" %d", &opcaoInf_Produto);
				if(opcaoInf_Produto == 1){
					system("cls");
					fflush(stdin);
					cadastrarProduto();
				}else if(opcaoInf_Produto == 2){
					system("cls");
					fflush(stdin);
					int codInformado;
					do{
        				rel_produtosCadastrados();
       					printf("\nInforme o codigo do produto que deseja alterar o valor: ");
        				scanf(" %d", &codInformado);
    					}while(!checkProduto(codInformado));
						alterarValor_Produto(codInformado);
				}else if(opcaoInf_Produto == 3){
					system("cls");
					printf("1-> PRODUTOS POR VALOR\n");
					printf("2-> PRODUTOS POR GRUPO\n");
					printf("3-> TODOS OS PRODUTOS CADASTRADOS\n");
					int opcaoInf_Rel_Produto = 0;
					printf("\nInforme uma opcao: ");
					scanf(" %d", &opcaoInf_Rel_Produto);
					if(opcaoInf_Rel_Produto == 1){
						system("cls");
						rel_produtosPorValor();
					}else if(opcaoInf_Rel_Produto == 2){
						system("cls");
						printf("=============== MENU PRODUTOS POR SESSAO ===============\n\n");
						int grupoInformado;
						menuGrupo_Produto();
						printf("\nInforme um grupo: ");
						scanf(" %d", &grupoInformado);
						rel_produtoPorGrupo(grupoInformado);
					}else if(opcaoInf_Rel_Produto == 3){
						rel_produtosCadastrados();
					}
				}
			}//FIM opcao1
		else if(opcaoInf == 2){
			cadastrarCliente();
		}//FIM opcao2
		else if(opcaoInf == 3){
			cadastrarVenda();
		}
		else if(opcaoInf == 4)
            faturamentoPeriodo();
        else if(opcaoInf == 5)
            rel_Clientes();

    	system("pause");
	}//FIM laço infinito

	return 0;
}

void cadastrarProduto(void){
	infoProduto cadastroProduto, auxProduto;

	char v_EhVazio;

	//pedir o nome do produto
	printf("Informe o nome do produto: ");
	scanf("%[^\n]s", cadastroProduto.nome);
	fflush(stdin);
	normalizarString(cadastroProduto.nome);

	//gerar um novo cod. e validar a entrada de produtos pelo nome
	pArq_Produto = fopen("Equipe4_produto.txt", "r");
	if(pArq_Produto == NULL) exit(EXIT_FAILURE);
	if((v_EhVazio = fgetc(pArq_Produto)) == EOF) auxProduto.codigoBarras = 9999;
	else{
		while((fscanf(pArq_Produto, "%d %s %d %f %f\n", &auxProduto.codigoBarras, auxProduto.nome, &auxProduto.grupo,
			&auxProduto.pontuacao, &auxProduto.valor)) != EOF){
				if(strcmp(auxProduto.nome, cadastroProduto.nome) == 0){
				printf("ERRO 0000x2: Jah existe um produto cadastrado com esse nome!\n");
				return;
			}
		}//FIM while
	}
	fclose(pArq_Produto);
	auxProduto.codigoBarras++;

	//agora ja pode pedir as informações
	//daqui p baixo

	//grupo do produto
	system("cls");
	int opInf_GpProduto = 0;
	menuGrupo_Produto();
	printf("Informe o grupo do produto: ");
	scanf(" %d", &opInf_GpProduto);

	//pontuacao do produto
	system("cls");
	printf("Informe a pontuacao do produto: ");
	scanf("%f%*c", &cadastroProduto.pontuacao);

	//valor do produto
	printf("\nInforme o valor do produto: ");
	scanf("%f%*c", &cadastroProduto.valor);

	//escrever os dados no arquivo
	pArq_Produto = fopen("Equipe4_produto.txt", "a");
	if(pArq_Produto == NULL) exit(EXIT_FAILURE);
	//codigo produto
	if(auxProduto.codigoBarras == 10000) //se for o primeiro produto. há um bug ao inserir no arquivo a primeira linha sem o espaço no inicio
		fprintf(pArq_Produto, " %d ", auxProduto.codigoBarras);
	else
		fprintf(pArq_Produto, "%d ", auxProduto.codigoBarras);
	//nome produto
	fprintf(pArq_Produto, "%s ", cadastroProduto.nome);
	//grupo produto
	fprintf(pArq_Produto, "%d ", opInf_GpProduto * 100);
	//pontuacao
	fprintf(pArq_Produto, "%.2f ", cadastroProduto.pontuacao);
	//valor
	fprintf(pArq_Produto, "%.2f\n", cadastroProduto.valor);

	fclose(pArq_Produto);
	printf("Produto cadastrado com sucesso!\n"); //VERIFICACAO INCORRETA!! O CORRETO EH RETORNAR UM COD DE RETORNO P VER SE CADSTROU OU N
}//FIM function

void normalizarString(char strNormal[]){
	short tamStr = strlen(strNormal);
	for(size_t iNome = 0; iNome < tamStr; iNome++){
		if(strNormal[iNome] >= 97 && strNormal[iNome] <= 122)
			strNormal[iNome] = strNormal[iNome] - 32;

		if(strNormal[iNome] == 32)
			strNormal[iNome] =  '_';
	}
}//FIM function

void retirarUnderlineStr(char strUnderline[]){
	unsigned short tamStr = strlen(strUnderline);
	for(size_t iStr = 0; iStr < tamStr; iStr++)
		if(strUnderline[iStr] == '_')
			strUnderline[iStr] = ' ';
}//FIM function

void espacamentoColunas(char strBase[]){
	short tamStr = strlen(strBase);
	short difTab = DIF_TABULACAO - tamStr;
	char strEspaco[difTab + 1] = {'\0'};
	for(size_t iStr = 0; iStr < difTab; iStr++)
		strEspaco[iStr] = ' ';
	strcat(strBase, strEspaco);
}

void menuGrupo_Produto(){
	printf("01-> Acondicionamento e Embalagem\n");
	printf("02-> Bebida\n");
	printf("03-> Alimento\n");
	printf("04-> Alimento Animal\n");
	printf("05-> Ferramenta de Manutencao em Geral\n");
	printf("06-> Medicamento\n");
	printf("07-> Material de Cozinha\n");
	printf("08-> Higienizacao Pessoal\n");
	printf("09-> Semente, Muda de Plantas\n");
	printf("10-> Equipamento de Informatica\n");
	printf("11-> Material de Escritorio\n");
	printf("12-> Material de Limpeza\n");
	printf("13-> Hotalicas\n");
	printf("14-> Equipamento de Audio e Video\n");
	printf("15-> Arte, Papelaria e Armarinho\n");
	printf("16-> Bebidas Alcoolicas\n");
	printf("17-> Musica, Filmes e Seriados\n");
	printf("18-> Joias e Relogios\n");
	printf("19-> Games\n");
	printf("20-> Esporte e Fitness\n");
	printf("21-> Eletrodomesticos\n");
	printf("22-> Celulares e Telefones\n");
	printf("23-> Calcados, Roupas e Bolsas\n");
	printf("24-> Livros, Revistas e Comics\n");
}//FIM function

void rel_produtoPorGrupo(int grupoInformado){
    //escreva daqui pra baixo
    infoProduto auxProduto;
	short encontrouProduto = 0;

	grupoInformado = grupoInformado * 100;

	pArq_Produto = fopen("Equipe4_produto.txt", "r"); if(pArq_Produto == NULL) exit(EXIT_FAILURE);
	system("cls");
	printf("============ PRODUTOS POR SESSAO ============\n\n");
	printf("\nCOD   = PRODUTO ================================= VALOR\n\n");
	while((fscanf(pArq_Produto, "%d %s %d %f %f\n", &auxProduto.codigoBarras, auxProduto.nome, &auxProduto.grupo,
		 &auxProduto.pontuacao, &auxProduto.valor)) != EOF){
		if(grupoInformado == auxProduto.grupo){
			encontrouProduto = 1; //verificar se tem produto vinculado ao grupo que foi passado
			retirarUnderlineStr(auxProduto.nome);
			espacamentoColunas(auxProduto.nome);
			printf("%d - %s R$%.2f\n", auxProduto.codigoBarras, auxProduto.nome, auxProduto.valor);
		}
	}
	if(encontrouProduto == 0)
		printf("   ================= SEM MOVIMENTO =================\n");
	printf("\n========================================================\n");
	fclose(pArq_Produto);
}//FIM function

void alterarValor_Produto(int codInformado){
	FILE *ptrAuxArquivo;
	infoProduto produtoArq;
	float novoValor;
    char strPrint[50];


	system("cls");
	ptrAuxArquivo = fopen("auxEquipe4_produto.txt", "a");
	if(ptrAuxArquivo == NULL)
		exit(EXIT_FAILURE);
	ptrArquivo = fopen("Equipe4_produto.txt", "r");
	if(ptrArquivo == NULL)
		exit(EXIT_FAILURE);

	while((fscanf(ptrArquivo, "%d %s %d %f %f\n", &produtoArq.codigoBarras, produtoArq.nome, &produtoArq.grupo,
		&produtoArq.pontuacao, &produtoArq.valor)) != EOF){
		if(codInformado == produtoArq.codigoBarras){
			printf("===================== PRODUTO ==================\n\n");
			strcpy(strPrint, produtoArq.nome);
			retirarUnderlineStr(strPrint);
			printf("%d - %s R$%.2f\n", produtoArq.codigoBarras, strPrint, produtoArq.valor); //prit do produto para confirmar se é o desejado
			printf("\n================================================\n");
			printf("\nInforme o novo valor: ");
			scanf(" %f", &novoValor);
			fprintf(ptrAuxArquivo, "%d %s %d %.2f %.2f\n", produtoArq.codigoBarras, produtoArq.nome, produtoArq.grupo, produtoArq.pontuacao, novoValor);
		}else fprintf(ptrAuxArquivo, "%d %s %d %.2f %.2f\n", produtoArq.codigoBarras, produtoArq.nome, produtoArq.grupo, produtoArq.pontuacao, produtoArq.valor);
	}//FIM while
	fclose(ptrAuxArquivo); fclose(ptrArquivo);
	remove("Equipe4_produto.txt");
	rename("auxEquipe4_produto.txt", "Equipe4_produto.txt");
	system("cls");
	printf("======== VALOR ALTERADO COM SUCESSO ============\n\n");					//OBERSERVAR QUE ESTA VALIDANDO SE O PRODUTO ALTEROU O VALOR
	printf("%d - %s R$%.2f\n", produtoArq.codigoBarras, strPrint, novoValor);		//PELA FUNCAO!!!
	printf("\n================================================\n\n");				//-> DAR UM RETORNO A FUNCAO; 1-> OK, 0-> FALHOU
}//FIM function

void cadastrarCliente(void){
    /*Essa funcão do tipo (void) não recebe nada como parâmetro, pois será
    simplimente para entrada de dados, não irá retornar nada./
    Após os valores serem colocados nas variavéis da struct, será simplesmente inserido no arquivo Txt.

    */
	system("cls");

	infoCliente cliente;

	cliente.pontuacao = 0.0;

	printf("Digite o nome do cliente:\n");
	scanf(" %[^\n]s", cliente.nome);
	normalizarString(cliente.nome);

	printf("Digite o CPF do cliente:\n");
	scanf(" %s", cliente.cpf);
	fflush(stdin);

	//Verificação do cliente por código.
	short check_ClienteCadastrado = checkCliente(cliente.cpf);
	if(check_ClienteCadastrado == 1){
		 printf("ERROR Cliente ja cadastrado\n");
		 system("pause");
		return;
	}

	printf("Digite o data de nascimento do cliente: ");
	scanf(" %d", &cliente.DataNascimento);

	pArq_Cliente = fopen("Equipe4_cliente.txt", "a");
		if(pArq_Cliente == NULL) exit(1);
	system("cls");
	//coloquei espaço no lugar dos ;
	//pontuação do cliente irá para o reltório com duas casas decimais. %.2f
	fprintf(pArq_Cliente,"%s %s %d %.2f\n", cliente.cpf, cliente.nome, cliente.DataNascimento, cliente.pontuacao);
	fclose(pArq_Cliente);
	printf("\t\t\tDADOS INFORMADOS:\n\n");
	printf("%s\n", cliente.nome);
	printf("%s\n", cliente.cpf);
	printf("%d\n",cliente.DataNascimento);
	system("pause");

}

void atualizarPontuacao(char cod_Cliente[], float pontuacaoAtt){
    /*Essa função recebe como parametro uma string e um float , string que representa o cod do cliente
    e float que representa a nova pontuação.
    Um ponteiro temporario para manipular o arquivo também temporario;
    */
    FILE *PtrTempArq;

	pArq_Cliente = fopen("Equipe4_cliente.txt", "r");
	if(pArq_Cliente == NULL) exit(EXIT_FAILURE);
	PtrTempArq = fopen("Equipe4_clienteTemp.txt", "a");
	if(PtrTempArq == NULL) exit(EXIT_FAILURE);

    infoCliente Busca;
    /* Buscando os valores no arquivo de cliente usando a proprio struct */
	while(fscanf(ptrArquivo, "%s %s %d %f", Busca.cpf, Busca.nome, &Busca.DataNascimento, &Busca.pontuacao) != EOF){

		if(!strcmp(Busca.cpf, cod_Cliente))
			fprintf(PtrTempArq, "%s %s %d %.2f\n", Busca.cpf, Busca.nome, Busca.DataNascimento, Busca.pontuacao);
		else
			fprintf(PtrTempArq, "%s %s %d %.2f\n", Busca.cpf, Busca.nome, Busca.DataNascimento, Busca.pontuacao);
	}
	fclose(ptrArquivo); fclose(PtrTempArq);
	remove("Equipe4_cliente.txt");
	rename("Equipe4_clienteTemp.txt", "Equipe4_cliente.txt");
}//FIM function

void menu_RelProdutosPorValor(){
	system("cls");
	printf("========== RELATORIO DE PRODUTOS POR VALOR ==========\n\n");
	printf("1-> MAIOR OU IGUAL AO VALOR INFORMADO\n");
	printf("2-> MENOR OU IGUAL AO VALOR INFORMADO\n");
	printf("3-> IGUAL AO VALOR INFORMADO\n");
	printf("4-> ENTRE UM INTERVALO INFORMADO\n");
	printf("\n========================================================\n");
}//FIM funtion

void rel_produtosPorValor(){

	infoProduto produtoArq;
	int tipoConsulta = 0;
	float valorInf1, valorInf2;

	menu_RelProdutosPorValor();
	printf("\nInforme o tipo de consulta: ");
	scanf(" %d", &tipoConsulta);

	short encontrouProduto = 0;

	ptrArquivo = fopen("Equipe4_produto.txt", "r");
	if(ptrArquivo == NULL) exit(EXIT_FAILURE);

	if(tipoConsulta == 1){
		printf("\nInforme um valor: ");
		scanf(" %f", &valorInf1);
		system("cls");
		printf("================== PRODUTOS POR VALOR ==================\n\n");
		printf("\nCOD   = PRODUTO ================================= VALOR\n\n");
		while((fscanf(ptrArquivo, "%d %s %d %f %f\n", &produtoArq.codigoBarras, produtoArq.nome, &produtoArq.grupo,
			&produtoArq.pontuacao, &produtoArq.valor)) != EOF){
			if(produtoArq.valor >= valorInf1){
				encontrouProduto = 1;
				retirarUnderlineStr(produtoArq.nome);
				espacamentoColunas(produtoArq.nome);
				printf("%d - %s R$%.2f\n", produtoArq.codigoBarras, produtoArq.nome, produtoArq.valor);
			}
		}
	}//FIM consulta1

		if(tipoConsulta == 2){
		printf("\nInforme um valor: ");
		scanf(" %f", &valorInf1);
		system("cls");
		printf("================== PRODUTOS POR VALOR ==================\n\n");
		printf("\nCOD   = PRODUTO ================================= VALOR\n\n");
		while((fscanf(ptrArquivo, "%d %s %d %f %f\n", &produtoArq.codigoBarras, produtoArq.nome, &produtoArq.grupo,
			&produtoArq.pontuacao, &produtoArq.valor)) != EOF){
			if(produtoArq.valor <= valorInf1){
				encontrouProduto = 1;
				retirarUnderlineStr(produtoArq.nome);
				espacamentoColunas(produtoArq.nome);
				printf("%d - %s R$%.2f\n", produtoArq.codigoBarras, produtoArq.nome, produtoArq.valor);
			}
		}
	}//FIM consulta2

		if(tipoConsulta == 3){
		printf("\nInforme um valor: ");
		scanf(" %f", &valorInf1);
		system("cls");
		printf("================== PRODUTOS POR VALOR ==================\n\n");
		printf("\nCOD   = PRODUTO ================================= VALOR\n\n");
		while((fscanf(ptrArquivo, "%d %s %d %f %f\n", &produtoArq.codigoBarras, produtoArq.nome, &produtoArq.grupo,
			&produtoArq.pontuacao, &produtoArq.valor)) != EOF){
			if(produtoArq.valor == valorInf1){
				encontrouProduto = 1;
				retirarUnderlineStr(produtoArq.nome);
				espacamentoColunas(produtoArq.nome);
				printf("%d - %s R$%.2f\n", produtoArq.codigoBarras, produtoArq.nome, produtoArq.valor);
			}
		}
	}//FIM consulta3

		if(tipoConsulta == 4){
		printf("\nMaior que: ");
		scanf(" %f", &valorInf1);
		printf("\nMenor que: ");
		scanf(" %f", &valorInf2);
		system("cls");
		printf("================== PRODUTOS POR VALOR ==================\n\n");
		printf("\nCOD   = PRODUTO ================================= VALOR\n\n");
		while((fscanf(ptrArquivo, "%d %s %d %f %f\n", &produtoArq.codigoBarras, produtoArq.nome, &produtoArq.grupo,
			&produtoArq.pontuacao, &produtoArq.valor)) != EOF){
			if(produtoArq.valor >= valorInf1 && produtoArq.valor <= valorInf2){
				encontrouProduto = 1;
				retirarUnderlineStr(produtoArq.nome);
				espacamentoColunas(produtoArq.nome);
				printf("%d - %s R$%.2f\n", produtoArq.codigoBarras, produtoArq.nome, produtoArq.valor);
			}
		}
	}//FIM consulta4
	if(encontrouProduto == 0)
		printf("   ================= SEM MOVIMENTO =================\n");
	printf("\n========================================================\n");
}//FIM function

void rel_produtosCadastrados(){
	infoProduto produtoArq;
	short encontrouProduto = 0;

	ptrArquivo = fopen("Equipe4_produto.txt", "r");
	if(ptrArquivo == NULL) exit(EXIT_FAILURE);
	system("cls");
	printf("============ LISTAGEM DE PRODUTOS CADASTRADOS ===========\n\n");
	printf("\nCOD   = PRODUTO ================================= VALOR\n\n");
	while((fscanf(ptrArquivo, "%d %s %d %f %f\n", &produtoArq.codigoBarras, produtoArq.nome, &produtoArq.grupo,
		&produtoArq.pontuacao, &produtoArq.valor)) != EOF){
		encontrouProduto = 1;
		retirarUnderlineStr(produtoArq.nome);
		espacamentoColunas(produtoArq.nome);
		printf("%d - %s R$%.2f\n", produtoArq.codigoBarras, produtoArq.nome, produtoArq.valor);
	}
	if(encontrouProduto == 0)
		printf("   ================= SEM MOVIMENTO =================\n");
	printf("\n========================================================\n");
	fclose(ptrArquivo);
}//FIM function

void cadastrarVenda(void){
	long int codVenda;
	char strInutil[99];
	char verificador;
	unsigned int dataRegistrada = 0;


	//Capturando informacoes da ultima venda cadastrada - ultima linha do arquivo de texto
	ptrVendatxt = fopen("Equipe4_venda.txt", "r");
	while(fgetc(ptrVendatxt) != EOF)
		fscanf(ptrVendatxt,"%c %ld %u %[^\n]s" , &verificador, &codVenda, &dataRegistrada, strInutil);
    fclose(ptrVendatxt);

	//Declaração e inicialização da estrutura de registro da nova venda
	RegistroVenda novaVenda;
	(novaVenda).data = gerarData(dataRegistrada);
	(novaVenda).codigoVenda = ++codVenda;
	(novaVenda).valorTotal = 0;
	(novaVenda).descAplicado = 0;

	Item *primeiroItem = NULL;
	//Recebe os pontos referente aos produtos selecionados adicionado aos pontos contidos no registro de cliente
	float pontosAtualizadosCliente = 0;
	short tipoCliente;
	char opcao;

	system("cls");
	printf("=================== SISTEMA DE VENDAS DA REDE DE LOJAS VALMARTE ===================\n\n");

	do{
		printf("====================== SECAO DE IDENTIFICACAO DO CLIENTE ======================\n\n");
		printf("\t\tEscolha uma das opcoes de login:\n       (1)Nao se identificar;       (2)Realizar Cadastro;       (3)Inserir Cadastro;       (0)Sair;\n");
		printf("Opcao -> ");
		scanf(" %c", &opcao);
		system("cls");
		if(opcao != '1' && opcao != '2' && opcao != '3')
			printf("A opcao informada eh invalida, tente outra vez\n");
		else
			tipoCliente = loginCliente(novaVenda.codigoCliente, opcao);
		system("cls");
	}while(opcao != '0' && opcao != '1' && opcao != '2' && opcao != '3');


	printf("============== NAVEGUE PELAS SECOES DE PRODUTOS E ESCOLHA SEUS ITENS ==============\n\n");
	do{
		printf("Deseja selecionar itens:\n(1)Sim, desejo;       (2)Nao, estou satisfeito;       (3)Ver Carrinho;\n");
		printf("Opcao -> ");
		scanf(" %c", &opcao);
		if(opcao != '1' && opcao != '2' && opcao != '3')
			printf("\nA opcao informada eh invalida, tente outra vez\n");
		else if(opcao == '1')
			selecaoProdutos(&primeiroItem);
		else if(opcao == '3')
			listarItens(primeiroItem);
		system("pause");
		system("cls");
	}while(opcao != '2');

	//Nenhum produto selecionado ou todos deletados
	if(!primeiroItem){
		printf("Ce ta de sacanagem. Nao vai comprar nada? Entao Adeus");
		return;
	}

	printf("================================ CARRINHO DE COMPRAS ================================\n");
	do{
		listarItens(primeiroItem);
		printf("\n\nDeseja modificar algum item?\n\n(s)Sim;	(n)Nao;\n");
		printf("Opcao -> ");
		scanf(" %c", &opcao);
		while(opcao == 's'){
			removerItens(&primeiroItem);
			printf(" Deseja remover mais produtos?\n\n(s)Sim, desejo;       (n)Nao, estou satisfeito;\n\n");
			printf("Opcao-> ");
			scanf(" %c", &opcao);
		}
		if(opcao != 'n' && opcao != 's')
			printf("A opcao informada eh invalida, tente outra vez\n");
		system("cls");
	}while(opcao != 'n');

	//Nenhum produto selecionado ou todos deletados
	if(!primeiroItem){
		printf("Ce ta de sacanagem. Nao vai comprar nada? Entao Adeus");
		return;
	}

	listarItens(primeiroItem);
	printf("\nDeseja confirmar a compra?\n\n(s)Sim;       (n)Nao;\n");
	printf("Opcao -> ");
	scanf(" %c", &opcao);
	printf("======================== OBRIGADO POR UTILIZAR NOSSO SISTEMA ========================\n");
	if(opcao != 'n'){
		pontosAtualizadosCliente = gravarItens(&novaVenda, &primeiroItem);
		if(tipoCliente){
			pontosAtualizadosCliente += aplicarDesconto(&novaVenda);
       			 atualizarPontuacao(novaVenda.codigoCliente, pontosAtualizadosCliente);
		}
		ptrVendatxt = fopen("Equipe4_venda.txt", "a");
		fprintf(ptrVendatxt, "~ %ld %d %s %.2f %.2lf\n",novaVenda.codigoVenda,novaVenda.data,novaVenda.codigoCliente,novaVenda.descAplicado,novaVenda.valorTotal);
		fclose(ptrVendatxt);
	}
}

short checkCliente(char codCliente[]){
    /*Recebe como parametro uma string contendo o codigo de cliente que se deseja validar;
      Realiza busca no arquivo de texto que contem os registros de clientes;
      Retorna 1 caso encontre o codigo de cliente ao qual se buscava e 0 caso contrario.*/
    infoCliente check;
    char resto[30];

	pArq_Cliente = fopen("Equipe4_cliente.txt", "r");
	while((fscanf(pArq_Cliente,"%s %[^\n]s", check.cpf, resto) != EOF)){
		if(strcmp(codCadastrado, codCliente) == 0){
            fclose(pArq_Cliente);
			return 1;
		}
	}
    fclose(pArq_Cliente);
	return 0;
}

short checkProduto(int codProduto){
	/*Recebe como parametro um numero inteiro contendo o codigo do produto que se deseja validar;
	  Realiza busca no arquivo de texto que contem os registros de produtos;
	  Retorna 1 caso encontre o codigo de produto ao qual se buscava e 0 caso contrario.*/
    int codCadastrado;
    char strResto[100];

    ptrProdutotxt = fopen("Equipe4_produto.txt", "r");
	while((fscanf(ptrProdutotxt,"%d %[^\n]s", &codCadastrado, strResto) != EOF)){
		if(codCadastrado == codProduto){
            fclose(ptrProdutotxt);
			return 1;
		}
	}
    fclose(ptrProdutotxt);
    printf("O codigo informado nao esta vinculado a nehum produto\n");
    getchar();
	return 0;
}

unsigned int gerarData(unsigned int dataRegistrada){
	/*Recebe a ultima data de vendas registrada;
	Gera um valor aleatorio para o campo dia entre 1 e 31;
	Valida a data gerada, segundo o calendario gregoriano e ultima data registrada;
	Formata a nova data, a tornando do tipo aaaammdd*/

	unsigned int dataGerada = 1 + rand() % 31;

	//Segmentando a data registrada no arquivo em: dia, mes, ano
	short diaRegistrado = dataRegistrada%100;
	short mesRegistrado = (dataRegistrada/100)%100;
	short anoRegistrado = dataRegistrada/10000;
	//Validacao e formantacao a nova data
	//****************Utilizar enum para comparacao de meses****************
	if((dataGerada <= diaRegistrado) || (dataGerada == 31 && (mesRegistrado == 4 || mesRegistrado == 6 || mesRegistrado == 9 || mesRegistrado == 11) || (mesRegistrado == 2 && dataGerada > 28))){
		if(mesRegistrado == 12)
			dataGerada = (anoRegistrado*10000)+10101;
		else
			dataGerada = (anoRegistrado*10000)+(mesRegistrado*100)+101;
	}
	else
		dataGerada = (anoRegistrado*10000)+(mesRegistrado*100)+dataGerada;
	return dataGerada;
}

void adicionarItem(Item **primeiroItem, ExtratoVenda extrato, char nomeProduto[]){
	Item *itemNovo;
	Item *itemAux;

	itemNovo = (Item*)malloc(sizeof(Item));
	if(itemNovo == NULL){
		printf("0xE005 - Falha em processo de alocacao de memoria");
		exit(1);
	}

	itemNovo->extrato = extrato;
	strcpy(itemNovo->nomeProduto, nomeProduto);
	itemNovo->proxItem = NULL;

	if(!(*primeiroItem)){
		*primeiroItem = itemNovo;
		return;
	}

  	itemAux = *primeiroItem;
	while(itemAux->proxItem && itemAux->extrato.codigoProduto != itemNovo->extrato.codigoProduto)
		itemAux = itemAux->proxItem;

    if(itemAux->extrato.codigoProduto == itemNovo->extrato.codigoProduto){
        itemAux->extrato.qtdProduto += itemNovo->extrato.qtdProduto;
        itemAux->extrato.valorTotal += itemNovo->extrato.valorTotal;
        free(itemNovo);
    }
    else
        itemAux->proxItem = itemNovo;
}

float gravarItens(RegistroVenda *novaVenda, Item **primeiroItem){
	Item *limparMemoria;
    float pontuacaoGerada = 0;
//Funcao alterada 10/09/2019 - Atualização de valor total da venda feita ao se gravar os produtos
//Arquivo aberto antes do while e fechado apos o while nao mais aberto e fechado no laço
//Retorna pontuacao gerada pelos produtos selecionados

ptrVendatxt = fopen("Equipe4_venda.txt", "a");
	while(*primeiroItem){
		limparMemoria = *primeiroItem;
		fprintf(ptrVendatxt,"* %d %.2f %d %.2f %.2lf\n" , (*primeiroItem)->extrato.codigoProduto, (*primeiroItem)->extrato.valorProduto, (*primeiroItem)->extrato.qtdProduto, (*primeiroItem)->extrato.ptsProduto, (*primeiroItem)->extrato.valorTotal);
		(*novaVenda).valorTotal +=(*primeiroItem)->extrato.valorTotal;
		pontuacaoGerada += (*primeiroItem)->extrato.qtdProduto * (*primeiroItem)->extrato.ptsProduto;
		*primeiroItem = (*primeiroItem)->proxItem;
		free(limparMemoria);
	}
	fclose(ptrVendatxt);
	primeiroItem = NULL;
	return pontuacaoGerada;
}

void removerItens(Item **primeiroItem){
	char opcao;
	int codItem;
	int qtdRemovida;
	Item *itemRemovido;
	Item *itemAnterior;

    itemRemovido = *primeiroItem;
	printf("\n\nDigite o codigo do produto que se deseja alterar:\n");
	printf("Opcao -> ");
	scanf("%d", &codItem);

	//Validacao do codigo - verificando existencia na lista ("carrinho")
	while(itemRemovido && (itemRemovido->extrato.codigoProduto != codItem)){
		itemAnterior = itemRemovido;
		itemRemovido = itemRemovido->proxItem;
	}
	if(!itemRemovido){
		printf("0xE004 O codigo digitado nao esta associado a nenhum item do carrinho\n");
		return;
	}

	printf("\nDigite a quantidade do item a ser removida:\n");
	printf("Opcao -> ");
	scanf("%d", &qtdRemovida);

	//Caso seja necessario eliminar o item
	if(qtdRemovida == itemRemovido->extrato.qtdProduto){
		if(itemRemovido == *primeiroItem)
			*primeiroItem = (*primeiroItem)->proxItem;
		else if(!itemRemovido->proxItem)
			itemAnterior->proxItem = NULL;
		else
			itemAnterior->proxItem = itemRemovido->proxItem;

		printf("====================================== ITEM REMOVIDO ======================================\n");
		printf("%d\t%s\t\t%.2f\t%d\t%.2lf\n", itemRemovido->extrato.codigoProduto, itemRemovido->nomeProduto, itemRemovido->extrato.valorProduto, itemRemovido->extrato.qtdProduto,  itemRemovido->extrato.valorTotal);
		free(itemRemovido);
	}
	else if(qtdRemovida < itemRemovido->extrato.qtdProduto){
		itemRemovido->extrato.qtdProduto -= qtdRemovida;
		itemRemovido->extrato.valorTotal = (itemRemovido->extrato.valorProduto)*(itemRemovido->extrato.qtdProduto);
	}
	else
        printf("A quantidade digitada eh invalida, o item nao sofreu alteracoes!");
	system("pause");
	system("cls");
}

void listarItens(Item *itens){
	system("cls");
	double valorTotal = 0;
	printf("============================================== ITENS NO CARRINHO ==============================================\n");
	printf("CODIGO:\tNOME:\t\tVALOR UNITARIO:\tQUANTIDADE:\tVALOR TOTAL:\n");
	while(itens){
		printf("%d\t%s\t%.2f\t%d\t%.2lf\n", itens->extrato.codigoProduto, itens->nomeProduto, itens->extrato.valorProduto, itens->extrato.qtdProduto, itens->extrato.valorTotal);
		valorTotal += (itens->extrato.qtdProduto * itens->extrato.valorProduto);
		itens = itens->proxItem;
	}
	printf("TOTAL DO CARRINHO:								   [%.2lf];\n", valorTotal);
}

float aplicarDesconto(RegistroVenda *novaVenda){
	/*A rotina recebe uma estrutura contendo as informacoes, sobre a venda e pontuaÃ§Ã£o referente aos produtos, da venda vigente
	Eh feito calculo de desconto sobre o valor total da venda e retornada o resto da pontucao registrada do cliente*/
	char codProcurado[12];
	char nomeCliente[99];
	int dataNasc;
	float restoPontuacaoCliente = 0;
	char opcao;
	//printf("\n\nO valor total da sua compra eh: [%lf]", (*novaVenda).valorTotal);

	/*
	//O cliente anonimo nao participa do sistema de pontuacao e desconto
	if(!strcmp((*novaVenda).codigoCliente, clienteAnonimo))
		(*novaVenda).descAplicado = 0;
	else{*/

		ptrClientetxt = fopen("Equipe4_cliente.txt", "r");
		while(strcmp(codProcurado, (*novaVenda).codigoCliente))
			fscanf(ptrClientetxt,"%s %s %d %f" , codProcurado, nomeCliente, &dataNasc, &restoPontuacaoCliente);
		fclose(ptrClientetxt);

		printf(" A PONTUACAO DO SR(A) %s EH:\t\t%.2f", nomeCliente, restoPontuacaoCliente);
		printf("\n\nDESEJA APLICAR A PONTUACAO DO SISTEMA DE DESCONTOS E FIDELIDADE?\n\n(s)Sim       (n)Nao\n");
		printf("Opcao-> ");
		scanf(" %c", &opcao);

		if(opcao == 's'){
			if(restoPontuacaoCliente <= (*novaVenda).valorTotal){
				(*novaVenda).valorTotal -= restoPontuacaoCliente;
				(*novaVenda).descAplicado = restoPontuacaoCliente;
				restoPontuacaoCliente = 0;
                }
			else{
				restoPontuacaoCliente -= (*novaVenda).valorTotal;
				(*novaVenda).descAplicado = (*novaVenda).valorTotal;
				(*novaVenda).valorTotal = 0;
			}

			printf("\n\nO valor total da sua compra agora eh: R$[%.2lf]\n", (*novaVenda).valorTotal);
		}

	return restoPontuacaoCliente;
}

short loginCliente(char codCliente[], char opcao){
	/*Identifica o comprador, verificando se seu cadastro eh valido, se necessario
	  Recebe a string onde sera gravado o codigo do cliente
	  Retorna 1 caso o cliente tenha se identificado e 0 caso o contrario*/
	short tipoCliente;

	if(opcao == '1'){
        strcpy(codCliente, "00000000000");
		tipoCliente = 0;
	}
	if(opcao == '3' || opcao == '2'){
		if(opcao == '2')
			cadastrarCliente();
		do{
			printf("\n========================== DIGITE O CODIGO DE CLIENTE CADASTRADO ==========================\n\n");
			printf("Codigo -> ");
			scanf("%s", codCliente);
			fflush(stdin);

			if(!(checkCliente(codCliente))){
				printf("O codigo digitado nao corresponde a nenhum cliente cadastrado.\n       (0)Nao fazer login;       (1)Inserir Novamente o codigo;\n");
				scanf(" %c", &opcao);
			}
		//Verificacao de existencia do cliente
		system("cls");
		}while(!(checkCliente(codCliente)) && opcao != '0');
			tipoCliente = 1;
	}
		//teste correçao bug menu
	return tipoCliente;
}

void selecaoProdutos(Item **primeiroItem){
	/*
	  Encadeia em uma lista as informacoes dos produtos e quantidades destes, selecionados pelos clientes
	  Retorna pontuacao gerada para o cliente, com base nos produtos e quantidades selecionadas*/

	ExtratoVenda extrato;
	int selectProduto = 0;
	char nomeProduto[99];
	char nomeGrupo[99];
	char opcao;
	int opcaoGrupo;

    system("cls");
    printf("=============== Escolha a secao que contem o(s) produto(s) desejado(s) =============== \n\n");
    menuGrupo_Produto();
    printf("\nINFORME O GRUPO DESEJADO\n");
    printf("Grupo -> ");
    scanf("%d", &opcaoGrupo);
		do{
			rel_produtoPorGrupo(opcaoGrupo);
			printf("\nO produto desejado esta nesta secao?\n\n(n)Nao, sair da secao      (s)Sim, selecionar produto\n\n");
			printf("Opcao -> ");
			scanf(" %c", &opcao);

			if(opcao == 's'){
                rel_produtoPorGrupo(opcaoGrupo);
				printf("\n\nDigite o codigo do produto desejado\n");
				printf("Codigo-> ");
				scanf("%d", &(extrato).codigoProduto);
				system("cls");

				if((checkProduto((extrato).codigoProduto))){
					ptrProdutotxt = fopen("Equipe4_produto.txt", "r");
					while(selectProduto != (extrato).codigoProduto)
						fscanf(ptrProdutotxt,"%d %s %s %f %f" , &selectProduto, nomeProduto, nomeGrupo, &(extrato).ptsProduto, &(extrato).valorProduto);
					fclose(ptrProdutotxt);

					printf("\nProduto Selecionado: [%s];\nValor Unitario: [%.2f];\nPontuacao do Produto: [%.2f];\n\n", nomeProduto, (extrato).valorProduto, (extrato).ptsProduto);

					printf("\nDigite a quantidade do produto desejada\n");
					printf("Quantidade-> ");
					scanf("%ud", &extrato.qtdProduto);
					extrato.valorTotal = extrato.qtdProduto * extrato.valorProduto;

					system("cls");
					printf("\nO VALOR TOTAL DESTE ITEM EH: [%.2lf]\n\n", extrato.valorTotal);

					printf("\nDESEJA CONFIRMAR A INSERCAO DO ITEM NO CARRINHO?\n\n(s)Sim, desejo       (n)Nao, selecionar outro item\n");
					printf("Opcao-> ");
					scanf(" %c", &opcao);
					if(opcao == 's')
						adicionarItem(primeiroItem, extrato, nomeProduto);
				}
				else
					printf("0 codigo digitado nao corresponde a nenhum produto cadastrado neste grupo\n\n");
			system("cls");
			printf("Ainda deseja comprar algum produto desta secao?\n\n(s)Sim, desejo;       (n)Nao, sair da secao;\n\n");
			printf("Opcao-> ");
			scanf(" %c", &opcao);
			system("cls");
			}
		}while(opcao != 'n');
}

unsigned int dataInvertida(unsigned int data){
	unsigned int dia, ano, mes, dataInvertida;

	dia = data / 1000000;
	ano = data % 10000;
	mes = (data / 10000) % 100;
	dataInvertida = (ano * 10000) + (mes * 100) + dia;

	return dataInvertida;
}

void faturamentoPeriodo(){
	unsigned int dataInicial, dataFinal;
	char verificador;
	double valorTotal;
	double faturamento = 0;
	float desconto;
	unsigned int dataRegistrada;
	char strLixo[50];

	FILE *ptrArquivoTemp;

	system("cls");
	printf("============== FATURAMENTO POR PERIODO =============\n\n");
	printf("Digite a data inicial que voce quer o faturamento: ");
	scanf("%u",&dataInicial);
	printf("Digite a data final que voce quer o faturamento: ");
	scanf("%u",&dataFinal);
	dataInicial = dataInvertida(dataInicial);
	dataFinal = dataInvertida(dataFinal);

	ptrArquivo = fopen("Equipe4_venda.txt", "r");
	if (ptrArquivo == NULL) exit(EXIT_FAILURE);

	ptrArquivoTemp = fopen("Equipe4_faturamento.txt", "w");
	if (ptrArquivoTemp == NULL) exit(EXIT_FAILURE);

	system("cls");
	printf("[DATA INICIAL: %u] ===== [DATA FINAL: %u]\n\n", dataInicial, dataFinal);
	while(fscanf(ptrArquivo,"%c %s %u %s %f %lf", &verificador, strLixo, &dataRegistrada, strLixo, &desconto, &valorTotal) != EOF){
			fflush(stdin);
		  	if ((verificador == '~') && (dataRegistrada >= dataInicial && dataRegistrada <= dataFinal)){
				fprintf(ptrArquivoTemp,"%.2lf %u \n", valorTotal, dataRegistrada);
				faturamento += valorTotal;
				printf("Em [%u/%u/%u] o faturamento foi [%.2lf]\n\n", ((dataRegistrada%10000)%100), ((dataRegistrada%10000)/100), (dataRegistrada/10000), valorTotal);
			}

		}
    printf("\nFaturamento total do periodo [%.2lf]\n", faturamento);
    fprintf(ptrArquivoTemp, "%s%.2lf\n", "TOTAL: R$", faturamento);
    system("pause");
	fclose(ptrArquivo);
    fclose(ptrArquivoTemp);
}//FIM function

void adicionarCliente_rel(listaClientes **primeiroCliente){
    //Variaveis para leitura de arquivo de vendas;
    char verificador;
    long int codigoVenda;
    int data;
    long long int codigoCliente;
    float descAplicado;
    double valorTotal;
    //Variaveis para leitura de arquivo de clientes;
    long long int codigoCadastradoCliente;
    char nomeCliente[50];
    char stringInutil[99];

    FILE *ptrCliente;
    FILE *ptrVenda = fopen("Equipe4_venda.txt", "r");
	if(ptrVenda == NULL) exit(EXIT_FAILURE);
	while(fscanf(ptrVenda, "%c %d %u %lld %f %lf",&verificador, &codigoVenda, &data, &codigoCliente, &descAplicado, &valorTotal) != EOF){
       if(verificador == '~' && codigoCliente){       //Verificando se eh a linha que contem o codigo do cliente e que o cdigo nao eh a do cliente anonimo
            ptrCliente = fopen("Equipe4_cliente.txt", "r");
            if(ptrVenda == NULL) exit(EXIT_FAILURE);
            while(fscanf(ptrCliente, "%lld %s %[^\n]s", &codigoCadastradoCliente, nomeCliente, stringInutil) != EOF  && (codigoCadastradoCliente != codigoCliente));
        system("pause");
        encadearClientes(primeiroCliente, nomeCliente, valorTotal);
        fclose(ptrCliente);
       }
	}
fclose(ptrVenda);

}//FIM function

void encadearClientes(listaClientes **primeiroCliente, char nomeCliente[], double valorTotal){
    listaClientes *novoCliente = (listaClientes*)malloc(sizeof(listaClientes));
    if(!novoCliente)
        exit(1);
    novoCliente->valorCompra = valorTotal;
    strcpy(novoCliente->nome, nomeCliente);
    novoCliente->nVisitas = 1;
    novoCliente->proxCliente = NULL;
	novoCliente->predCliente = NULL;

    //Encadeamento
    if(!(*primeiroCliente))
        *primeiroCliente = novoCliente;
    else{
        listaClientes *auxCliente = *primeiroCliente;
        while(auxCliente->proxCliente  && strcmp(auxCliente->nome, novoCliente->nome))
            auxCliente = auxCliente->proxCliente ;
        if(!(strcmp(auxCliente->nome,novoCliente->nome))){
             auxCliente->nVisitas++;
             auxCliente->valorCompra += novoCliente->valorCompra;
        }
        else
            auxCliente->proxCliente = novoCliente;
			novoCliente->predCliente = auxCliente;
    }

}

void relClienteOrdenado(listaClientes **primeiroCliente, char opcao){
	listaClientes *ordenador;
	if(opcao == '1'){	//Ordenação por valor das compras
		while((*primeiroCliente)->proxCliente){
			ordenador = *primeiroCliente;
			while(ordenador){
				if(ordenador->valorCompra > (*primeiroCliente)->valorCompra){
					if(!ordenador->proxCliente)
						ordenador->predCliente->proxCliente = NULL;
					else{
						ordenador->predCliente->proxCliente = ordenador->proxCliente;
						ordenador->proxCliente->predCliente = ordenador->predCliente;
					}
					ordenador->proxCliente = *primeiroCliente;
					if((*primeiroCliente)->predCliente){
						(*primeiroCliente)->predCliente->proxCliente = ordenador;
						ordenador->predCliente = (*primeiroCliente)->predCliente;
					}
					else
						ordenador->predCliente = NULL;
					(*primeiroCliente)->predCliente = ordenador;
					*primeiroCliente = ordenador;
				}
				ordenador = ordenador->proxCliente;
			}
			(*primeiroCliente) = (*primeiroCliente)->proxCliente;
		}
	}
    else{	//Ordenação por numero de visitas
		while((*primeiroCliente)->proxCliente){
			ordenador = *primeiroCliente;
			while(ordenador){
				if(ordenador->nVisitas > (*primeiroCliente)->nVisitas){
					if(!ordenador->proxCliente)
						ordenador->predCliente->proxCliente = NULL;
					else{
						ordenador->predCliente->proxCliente = ordenador->proxCliente;
						ordenador->proxCliente->predCliente = ordenador->predCliente;
					}
					ordenador->proxCliente = *primeiroCliente;
					if((*primeiroCliente)->predCliente){
						(*primeiroCliente)->predCliente->proxCliente = ordenador;
						ordenador->predCliente = (*primeiroCliente)->predCliente;
					}
					else
						ordenador->predCliente = NULL;
					(*primeiroCliente)->predCliente = ordenador;
					*primeiroCliente = ordenador;
				}
				ordenador = ordenador->proxCliente;
			}
			(*primeiroCliente) = (*primeiroCliente)->proxCliente;
		}
	}
	//Setando o primeiro cliente como inicio da listas
    while((*primeiroCliente)->predCliente)
		*primeiroCliente = (*primeiroCliente)->predCliente;
	return;
}

void rel_Clientes(){
	listaClientes *primeiroCliente = NULL;
	listaClientes *listaAux;
	char opcao;
	int indiceRank;

    pArq_Cliente = fopen("Equipe4_cliente.txt", "r");
    if(fgetc(pArq_Cliente) == EOF)
		printf("Nao foi possivel gerar o relatorio pois nao existe nenhum cliente cadastrado\n");
    else{
        adicionarCliente_rel(&primeiroCliente);
        fclose(pArq_Cliente);
        do{
        	system("pause");
			system("cls");
            printf("Qual o tipo de relatorio o sr(a) deseja consultar - clientes que:\n       (1)Que compraram mais;       (2)Que mais visitaram;       (0)Sair\n");
            printf("Opcao -> ");
            scanf(" %c", &opcao);
            system("cls");
            if(opcao == '1'){
                indiceRank = 1;
                relClienteOrdenado(&primeiroCliente, opcao);
                listaAux = primeiroCliente;
                printf("============= RANKING DE CLIENTES QUE GERARAM MAIOR RECEITA =============\n\n");
                while(listaAux){
                    printf("%d --- [%s]\t[%.2lf]\n", indiceRank++, listaAux->nome, listaAux->valorCompra);
                    listaAux = listaAux->proxCliente ;
                }
            }
            else if(opcao == '2'){
                indiceRank = 1;
                relClienteOrdenado(&primeiroCliente, opcao);
                listaAux = primeiroCliente;
                printf("============= RANKING DE CLIENTES QUE MAIS USARAM O SISTEMA =============\n\n");
                while(listaAux){
                    printf("%d --- [%s]\t[%u]\n", indiceRank++, listaAux->nome, listaAux->nVisitas);
                    listaAux = listaAux->proxCliente ;
                }
            }
        }while(opcao != '0');
    }

	//Livrar memoria alocada
	while(primeiroCliente){
		listaAux = primeiroCliente;
		primeiroCliente = primeiroCliente->proxCliente;
		free(listaAux);
	}
}

//fim do programa
