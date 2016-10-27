// Declara��es do Algoritmo de Binary Search ou Busca Bin�ria
#include <iostream>
#include <Windows.h>
using namespace std;
// defini��es do programa
#define PAUSA							system("pause")
#define LIMPAR_TELA						system("cls")
#define ACENTUACAO_BRASILEIRA			setlocale(LC_ALL, "portuguese_brazil")
// menu do programa
#define MOSTRAR_PRODUTO					'M'
#define SAIR_DO_PROGRAMA				'S'
//
#define SEMENTE_RANDOM					250				// semente para gerar o arquivo de produto
#define CAMINHO_CADASTRO				"..\\Produtos.arq"
#define QTDE_REGISTROS					5000
//
// STRUCT de Produto
typedef struct tagPRODUTO
{
	int nCodigo;						// c�digo do produto e chave prim�ria
	char cDescricao[41],				// descri��o do produto
		cAtivo;							// 'A' - produto ativo e 'I' - produto inativo ou exclu�do
	double dPrecoUnit;					// pre�o unit�rio do produto
} PRODUTO;
//
// classe Binary Search
//
class clBinarySearch
{
	// atributos e m�todos private da classe
private:
	PRODUTO stProduto;					// para conter os dados de um produto
	unsigned int nQtdeAcessos,			// para conter a quantidade de acessos 2^n + 1
		regInf,							// posicional inferior
		regSup,							// posicional superior
		nQtRegistros;					// para salvar a quantidade total de registros
public:
	// atributos e m�todos public da classe
	clBinarySearch(int nQtRegistros)
	{
		unsigned int nQtdeTrab = nQtRegistros;		// quantidade para calcular a pot�ncia de 2
		this->nQtRegistros = nQtRegistros;			// salva a quantidade de registros
		// divis�o sucessiva por 2 para calcular o n de 2^n
		for (nQtdeAcessos = 1; nQtdeTrab > 1; nQtdeAcessos++)	// inicia com um a mais
		{
			nQtdeTrab >>= 1;									// divis�o por 2
		}
	} // construtor
	// M�todo p�blico para buscar os dados de um produto dado a sua chave de busca
	// Par�metros:
	//		Entrada: int nKey - c�digo do produto (chave de busca)
	//				PRODUTO *ptrStPRoduto - ponteiro de uma struct que conter� os dados
	//										do produto se ele existir
	//				FILE *fdCadastro - file descriptor do arquivo de produtos
	//		Retorno: bool - true - indica que o produt existe e seus dados est�o
	//								na struct que foi apontada pelo invocador do m�todo
	//						false - indica que o produto n�o existe
	bool BinarySearch(int nKey, PRODUTO *ptrStProduto, FILE *fdCadastro)
	{
		unsigned int nQtdeAcessos = this->nQtdeAcessos; // quantidade calculada no construtor
		int regAcessar;									// posicional a acessar
		// inicializar os extremos inferior e superior
		regInf = 0;										// limite inferior
		regSup = nQtRegistros;							// limite superior
		// loop de acessos
		while (nQtdeAcessos)								// while true
		{
			nQtdeAcessos--;									// um a menos
			// vamos calcular o caminho metade
			regAcessar = ((regSup - regInf) / 2) + regInf;	// calculo do posicional a acessar
			if (fseek(fdCadastro, regAcessar * sizeof(PRODUTO), SEEK_SET) != 0) // seek error?
			{
				return false;													// indica que n�o encontrou
			}
			if (fread_s(&stProduto, sizeof(PRODUTO), sizeof(PRODUTO), 1, fdCadastro) == 0)	// erro de leitura?
			{
				return false;				// erro
			}
			// fazer os testes com o registro lido
			if (nKey == stProduto.nCodigo)		// encontrou o produto desejado?
			{	// a chave de busca foi encontrada
				memcpy_s(ptrStProduto, sizeof(PRODUTO), &stProduto, sizeof(PRODUTO));	// copia o produto na struct do invocador
				return true;				// indica que encontrou
			}
			else if (nKey > stProduto.nCodigo) // o produto deve estar na metade superior
			{
				regInf = regAcessar;			// busca na metade superior
			}
			else
			{
				regSup = regAcessar;			// busca na metade inferior
			}
		} // while
		// n�o encontrou o produto desejado
		return false;							// indica que n�o encontrou
	} // BinarySearch
};