// Programa de teste do algor�timo Binary Search
// FATEC - MC - CCAJ - 13/10/2016 - Vers�o 0.0
#include "BinarySearch.h"
//
// entry point do programa
//
void main(void)
{
	PRODUTO stProduto,						// struct para conter um produto
		vetProdutos[QTDE_REGISTROS];		// vetor de produtos em mem�ria
	SYSTEMTIME stTime;						// para buscar a data e hora do sistema
	FILE *fdCadastro;						// file descriptor do cadastro
	char cWork[200],						// para uso do sprintf
		cOpcao;								// op��o de escolha do operador
	int i, j,								// �ndices gen�ricos
		nCodigo;							// para receber o c�digo
	bool flgRepetido,						// indica nCodigo repetido
		flgHouveTroca;						// para auxiliar no Bubble Sort
	ACENTUACAO_BRASILEIRA;
	srand(SEMENTE_RANDOM);					// semente rand�mica
	// loop para gerar o arquivo
	for (i = 0; i < sizeof(vetProdutos) / sizeof(PRODUTO); i++)
	{
		vetProdutos[i].nCodigo = rand();		// c�digo aleat�rio
		if (vetProdutos[i].nCodigo == 0)				// c�digo zero?
		{
			vetProdutos[i].nCodigo == 1;				// torna c�digo um
		}
		//verificar se temos um c�digo repetido
		for (j = 0, flgRepetido = false; j < i; j++)
		{
			if (vetProdutos[j].nCodigo == vetProdutos[i].nCodigo)	// repetido?
			{
				i--;									// para manter a quantidade desejada
				flgRepetido = true;						// indica repeti��o
				break;									// cai fora do for j
			}
		} // for j
		// um c�digo repetido ou n�o
		if (flgRepetido)									// c�digo repetido?
		{
			continue;										// continua no for i
		}
		// n�o repetido
		sprintf_s(vetProdutos[i].cDescricao, sizeof(stProduto.cDescricao) - 1,
			"Produto de c�digo: %10d", vetProdutos[i].nCodigo);
		vetProdutos[i].cAtivo = 'A';						// produto ativo
		vetProdutos[i].dPrecoUnit = rand() * 1.3476;		// define pre�o unit�rio
	} // for i
	// bubble sort do vetor
	do
	{
		flgHouveTroca = false;												// come�a n�o trocando
		for (i = 0; i < QTDE_REGISTROS - 1; i++)
		{
			if (vetProdutos[i].nCodigo > vetProdutos[i + 1].nCodigo)		// fora de ordem?
			{ // fazer a troca entre elem[i] com elem[i + 1]
				memcpy_s(&stProduto, sizeof(PRODUTO), &vetProdutos[i], sizeof(PRODUTO)); // salva o elemento i
				memcpy_s(&vetProdutos[i], sizeof(PRODUTO), &vetProdutos[i + 1], sizeof(PRODUTO));	// troca o elemento i+1
				memcpy_s(&vetProdutos[i + 1], sizeof(PRODUTO), &stProduto, sizeof(PRODUTO));		// troca o elemento i
				flgHouveTroca = true;										// indica que houve troca
			}
		}
	} while (flgHouveTroca);								// loop enquanto houver troca
	// gravar o vetor em um arquivo no disco
	if (fopen_s(&fdCadastro, CAMINHO_CADASTRO, "w+b") != 0)		// erro de abertura?
	{ // erro irrecuper�vel
		cout << "Erro irrecuper�vel." << endl;
		PAUSA;
		return;												// volta ao sistema operacional
	}
	// loop de grava��o de cada produto
	for (i = 0; i < QTDE_REGISTROS; i++)
	{
		if (fwrite(&vetProdutos[i], sizeof(PRODUTO), 1, fdCadastro) == 0) // erro de grava��o?
		{
			fclose(fdCadastro);
			cout << "Erro irrecuper�vel de grava��o." << endl;
			PAUSA;
			return;											// volta ao sistema operacional
		}
	} // for i
	// instanciar a classe binary search e criar um objeto da classe
	clBinarySearch objBinary(QTDE_REGISTROS);
	// loop infinito com um menu de op��es para o programador
	while (true)												// outro loop infinito � o for(;;)
	{
		LIMPAR_TELA;
		GetLocalTime(&stTime);								// busca a data e hora do sistema
		sprintf_s(cWork, sizeof(cWork),
			"\n\tTeste do algoritmo Binary Search em %02d/%02d/%02d �s %02d:%02d",
			stTime.wDay, stTime.wMonth, stTime.wYear, stTime.wHour, stTime.wMinute);
		cout << cWork << endl;
		cout << MOSTRAR_PRODUTO << " - Mostrar um produto" << endl;
		cout << SAIR_DO_PROGRAMA << " - Sair do programa" << endl;
		cout << "\tSelecione: ";
		cin >> cOpcao;										// op��o do operador
		cOpcao = toupper(cOpcao);							// converte pra mai�scula
		switch (cOpcao)										// avaliar a op��o escolhida
		{
			case MOSTRAR_PRODUTO:
				cout << "C�digo: ";
				cin >> nCodigo;
				if (objBinary.BinarySearch(nCodigo, &stProduto, fdCadastro))		// encontrou?
				{	// mostrar produto
					sprintf_s(cWork, sizeof(cWork),
						"C�digo: %6d Descri��o: %-40s\nPre�o: %10.2f", stProduto.nCodigo, stProduto.cDescricao,
						stProduto.dPrecoUnit);
					cout << cWork << endl;
					PAUSA;
				}
				else
				{
					cout << "C�digo: " << nCodigo << " n�o existe." << endl;
					PAUSA;
				}
				break;
			case SAIR_DO_PROGRAMA:
				cout << "Sair realmente? (S ou N): ";
				cin >> cOpcao;
				if (cOpcao == 'S' || cOpcao == 's')			// sair realmente?
				{
					return;									// sair do sistema operacional
				}
				break;										// voltar ao menu
			default:
				cout << "Op��o inv�lida." << endl;
				PAUSA;
				break;										// voltar ao menu
		} // switch
	} // while
} // main