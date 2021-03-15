/*
NOME: Mariana de Jesus Antunes
MATRÍCULA: CJ300452X
DATA: 06/12/2019
*/

#pragma warning(disable:4996)
#include <iostream>
#include <clocale>
#include "FuncionalidadesReg.h"
using namespace std;

int main(void)
{
	setlocale(LC_ALL, "Portuguese");
	int opcao = 0;
	do
	{
		menu();
		cin >> opcao;
		lbuffer();
		opcoesMenu(opcao);
	} while (opcao != 6);

	pause();
	return 0;
}
