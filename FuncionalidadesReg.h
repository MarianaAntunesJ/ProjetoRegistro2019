#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <algorithm>
#include <windows.h>
#include "Registro.h"
#include "calculaData.h"
#include "calendar.h"

using namespace std;

Registro registro_base;
int i, j;

#if defined _WIN32 || defined _WIN64

// Limpar tela

void limpa(void)
{
	HANDLE TELA;
	DWORD ESCRITA = 0;
	COORD POS;
	TELA = GetStdHandle(STD_OUTPUT_HANDLE);
	POS.X = 0;
	POS.Y = 0;
	FillConsoleOutputCharacter(TELA, 32, 100 * 100, POS, &ESCRITA);
}


// Posicionar cursor nos limites 80 x 24

void position(int LINHA, int COLUNA)
{
	if (COLUNA >= 1 and COLUNA <= 80 and LINHA >= 1 and LINHA <= 24)
	{
		HANDLE TELA;
		COORD POS;
		TELA = GetStdHandle(STD_OUTPUT_HANDLE);
		POS.X = COLUNA - 1;
		POS.Y = LINHA - 1;
		SetConsoleCursorPosition(TELA, POS);
	}
}


// Limpar linha a partir da posicao do cursor
void clearline(void)
{
	HANDLE TELA;
	COORD POS;
	CONSOLE_SCREEN_BUFFER_INFO VIDEO;
	DWORD ESCRITA = 0;
	TELA = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(TELA, &VIDEO);
	POS.Y = VIDEO.dwCursorPosition.Y;
	POS.X = VIDEO.dwCursorPosition.X;
	FillConsoleOutputCharacter(TELA, 32, 80 - POS.X, POS, &ESCRITA);
}

#else

// ACOES EXCLUSIVAS PARA MODO: TERMINAL ANSI

// LIMPAR TELA

void limpa(void)
{
	cout << "\033[2J";
}

// LIMPAR LINHA A PARTIR DA POSICAO DO CURSOR

void position(int LINHA, int COLUNA)
{
	if (COLUNA >= 1 && COLUNA <= 80 && LINHA >= 1 && LINHA <= 24)
		cout << "\033[" << LINHA << ";" << COLUNA << "H";
}

// LIMPAR LINHA A PARTIR DA POSICAO DO CURSOR

void clearline(void)
{
	cout << "\033[K";
}

#endif

void lbuffer(void)
{
	cin.ignore(100, '\n');
	cout << "\n";
}

void pause(void)
{
	cin.get();
	cout << "PRESSIONE QUALQUER TECLA PARA CONTINUAR.  .  .";
	cout << endl;
}

void tracalinha(string tipo)
{
	limpa();
	position(1, 1);
	for (i = 0; i <= 79; i++)
		cout << ".";
	cout << endl;
	position(2, 26);
	cout << " PROGRAMA DE REGISTROS  " << endl;
	position(3, 29);
	cout << tipo;
	cout << endl;
	position(4, 1);
	for (i = 0; i <= 79; i++)
		cout << ".";
}

void menu()
{
	limpa();
	string tipo;
	tipo = "Menu Principal";
	tracalinha(tipo);
	position(8, 16);
	cout << "Cadastrar ............................: [1]" << "\n";
	position(10, 16);
	cout << "Pesquisar por nome ...................: [2]" << "\n";
	position(12, 16);
	cout << "Pesquisar por data de nascimento......: [3]" << "\n";
	position(14, 16);
	cout << "Remover ..............................: [4]" << "\n";
	position(16, 16);
	cout << "Alterar ..............................: [5]" << "\n";
	position(18, 16);
	cout << "Finalizar ............................: [6]" << "\n";
	position(22, 16);
	cout << "Entre com a opçao : ";
}

bool salvarRegistro(Registro registro)
{
	try //caso ocorro exception (erro) não estoura erro para o usuário.
	{
		fstream arquivo;
		arquivo.open("arquivo.dat", ios_base::out | ios_base::app | ios_base::binary);
		arquivo.seekp(0, ios::end); //mover posição arquivo.
		arquivo.write(reinterpret_cast<char*>(&registro), sizeof(class Registro));
		arquivo.flush(); //esvaziar o buffer
		arquivo.close();
		return true;
	}
	catch (const exception) //caso exception, apenas retorne falso.
	{
		return false;
	}
}

string maiuscula(string palavra)
{
	for_each(palavra.begin(), palavra.end(), [](char& c) //para cada elemento da lista
		{
			c = ::toupper(c);
		});
	return palavra;
}

void criaArquivo(void)
{
	fstream criaArquivo;
	criaArquivo.open("arquivo.dat", ios_base::out | ios_base::app | ios_base::binary);
	criaArquivo.close();
}

bool registroExistente(Registro registro)
{
	criaArquivo();
	fstream arquivo("arquivo.dat", ios_base::in | ios_base::binary);
	bool acha = false; //flag;
	while ((not arquivo.eof()) and acha != true)
	{
		arquivo.read(reinterpret_cast<char*>(&registro_base), sizeof(class Registro));
		string dataCadastro = registro.getData();
		string dataBanco = registro_base.getData();
		if (registro.getId() == registro_base.getId())
			acha = true;
		else if (dataCadastro == dataBanco && registro.getNome() == registro_base.getNome()) //se não se;
			acha = true;
	}
	arquivo.flush();
	arquivo.close();
	return acha;
}

void cadastrar(void)
{
	Registro registro;
	int id;
	do
	{
		limpa();
		string tipo = "Módulo de Cadastro";
		tracalinha(tipo);
		position(6, 2);
		cout << "Digite os dados abaixo:";
		position(22, 2);
		cout << "Digite [0] para voltar ao menu: ";
		position(8, 5);
		cout << "ID .........................: ";
		cin >> id;
		position(22, 2);
		clearline();
		registro.setId(id);
		if (id != 0) //fazer uso no do/while para voltar ao menu com 0 (zero).
		{
			position(10, 5);
			string nome;
			cin.get();  //limpa buffer
			cout << "Nome ....................: ";
			getline(cin, nome);
			registro.setNome(maiuscula(nome));
			position(12, 5);
			string data;
			cout << "Data de nascimento ......: ";
			cin >> data;
			lbuffer();
			char dataArray[11];
			strcpy(dataArray, data.c_str()); //parse, função que percorre string e manda para o array.
			registro.setData(dataArray);
			if (validate(dataArray)) //validar com o (calendar.h).
			{
				if (registroExistente(registro))
					cout << " Registro existente, tente novamente.";
				else
				{
					if (salvarRegistro(registro))
						cout << "\n Registro salvo!\n";
					else
						cout << " Ocorreu um problema, tente novamente.\n";
				}
			}
			else
			{
				cout << " Formato da data invalido.\n";
			}
			cout << "\n Deseja cadastrar novo registro? (1) Sim; (0) Não, voltar ao menu; " << endl;
			cin >> id;
		}
	} while (id != 0);
}

void pesquisarNome(void)
{
	criaArquivo();
	int opcao;
	fstream arquivo("arquivo.dat", ios_base::in | ios_base::binary);
	do
	{
		limpa();
		string tipo = "Módulo de Consulta";
		tracalinha(tipo);
		position(6, 2);
		cout << "Digite os dados abaixo:";
		position(8, 2);
		clearline();
		string nome;
		cout << "Nome ..................: ";
		getline(cin, nome);
		cout << endl;
		nome = maiuscula(nome);
		bool acha = false;
		string data;
		while (( not arquivo.eof()))
		{
			arquivo.read(reinterpret_cast<char*>(&registro_base), sizeof(class Registro));
			if (nome == registro_base.getNome())
			{
				if (data != registro_base.getData())
				{
					data = registro_base.getData();
					registro_base.mostraDados();
					cout << "\n";
					acha = true;
					
				}
			}
		}
		if (acha == false)
			cout << "Arquivo não encontrado!" << endl;
		cout << "\n Deseja pesquisar outro registro? (1) Sim; (0) Não, retornar ao menu;  " << endl;
		cin >> opcao;
	} while (opcao != 0);
	arquivo.flush();
	arquivo.close();
}

void pesquisarDataNascimento(void)
{
	int opcao;
	fstream arquivo("arquivo.dat", ios_base::in | ios_base::binary);
	do
	{
		limpa();
		string tipo = "Módulo de Consulta";
		tracalinha(tipo);
		position(6, 2);
		cout << "Digite os dados abaixo:";
		position(8, 2);
		clearline();
		string data;
		cout << "Data de nascimento ......: ";
		cin >> data;
		bool acha = false;
		string nome;
		while ((not arquivo.eof()))
		{
			arquivo.read(reinterpret_cast<char*>(&registro_base), sizeof(class Registro));
			if (data == registro_base.getData())
			{
				if (nome != registro_base.getNome()) //pausa da lista; se o nome for diferente, o deixa igual para não haver repetições.
				{
					nome = registro_base.getNome();
					registro_base.mostraDados();
					acha = true;
					cout << "\n";
				}
			}
		}
		if (acha == false)
			cout << " Registro não encontrado!" << endl;
		cout << "\n Deseja pesquisar outro registro? (1) Sim; (0) Não, retornar ao menu;  " << endl;
		cin >> opcao;
	} while (opcao != 0);
	arquivo.flush();
	arquivo.close();
}
int encontrarPosicaoId(int idEntrada)
{
	fstream arquivo("arquivo.dat", ios_base::in | ios_base::binary);
	unsigned int pos = -1, contador = 0;
	bool acha = false;
	while (not(arquivo.eof()) and acha != true)
	{
		arquivo.read(reinterpret_cast<char*>(&registro_base), sizeof(class Registro));
		if (idEntrada == registro_base.getId())
		{
			acha = true;
			pos = contador;
		}
		contador++;
	}
	arquivo.flush();
	arquivo.close();
	return pos;
}

int encontrarPosicaoNome(string nomeEntrada)
{
	fstream arquivo("arquivo.dat", ios_base::in | ios_base::binary);
	unsigned int pos = -1, contador = 0;
	bool acha = false;
	while (not(arquivo.eof()) and acha != true)
	{
		arquivo.read(reinterpret_cast<char*>(&registro_base), sizeof(class Registro));
		if (nomeEntrada == registro_base.getNome())
		{
			acha = true;
			pos = contador;
		}
		contador++;
	}
	arquivo.flush();
	arquivo.close();
	return pos;
}

int encontrarPosicaoData(string data)
{
	fstream arquivo("arquivo.dat", ios_base::in | ios_base::binary);
	unsigned int pos = -1, contador = 0;
	bool acha = false;
	while (not(arquivo.eof()) and acha != true)
	{
		arquivo.read(reinterpret_cast<char*>(&registro_base), sizeof(class Registro));
		if (data == registro_base.getData())
		{
			acha = true;
			pos = contador;
		}
		contador++;
	}
	arquivo.flush();
	arquivo.close();
	return pos;
}

void excluirRegistro(int pos)
{
	fstream arquivo("arquivo.dat", ios_base::in | ios_base::out | ios_base::binary);
	arquivo.seekp(pos * sizeof(registro_base));
	arquivo.write(reinterpret_cast<char*>(&registro_base), sizeof(class Registro));
	arquivo.flush();
	arquivo.close();
}

void removerRegistro(void)
{
	int id;
	int opcaoRemover;
	do
	{
		limpa();
		string tipo = "Módulo de Exclusão";
		tracalinha(tipo);
		position(6, 2);
		cout << "Digite os dados abaixo:";
		position(8, 2);
		clearline();
		position(22, 2);
		cout << "Digite [0] para voltar ao menu; ";
		clearline();
		position(22, 2);
		fstream arquivo("arquivo.dat", ios_base::in | ios_base::binary);
		position(10, 2);
		cout << "ID do registro que deseja excluir ...: ";
		cin >> id;
		position(22, 2);
		clearline();
		position(12, 2);
		if (id != 0)
		{
			int posicaoRegistro = encontrarPosicaoId(id);
			bool acha = false;
			while ((not arquivo.eof()) and acha != true)
			{
				arquivo.read(reinterpret_cast<char*>(&registro_base), sizeof(class Registro));
				if (id == registro_base.getId())
				{
					acha = true;
					registro_base.mostraDados();
					cout << " Deseja excluir este registro permanentemente? (1) Sim; (2) Não;\n ";
					cin >> opcaoRemover;
					lbuffer();
					if (opcaoRemover == 1)
					{
						excluirRegistro(posicaoRegistro);
						cout << " Registro excluído.\n";
					}
					else
						cout << " Registro não excluído.\n";
				}
			}
			if (acha == false)
				cout << " Registro não encontrado!" << endl;
			arquivo.flush();
			arquivo.close();
			cout << "\n Deseja pesquisar outro registro? (1) Sim; (0) Não, retornar ao menu;  " << endl;
			cin >> id;
		}
	} while (id != 0);
}

void alteracaoNome(int pos)
{
	string nome;
	cin.get();
	getline(cin, nome);
	nome = maiuscula(nome);
	registro_base.setNome(nome);
	fstream arquivo("arquivo.dat", ios_base::in | ios_base::out | ios_base::binary);
	arquivo.seekp(pos * sizeof(registro_base));
	arquivo.write(reinterpret_cast<char*>(&registro_base), sizeof(class Registro));
	arquivo.flush();
	arquivo.close();
}

void alterarNome(void)
{
	int opcaoAlterar;
	int id;
	do
	{
		limpa();
		string tipo = "Módulo de Alteração de Nome";
		tracalinha(tipo);
		position(6, 2);
		cout << "Digite os dados abaixo:";
		position(8, 2);
		clearline();
		position(22, 2);
		cout << "Digite [0] para voltar ao menu: ";
		fstream arquivo("arquivo.dat", ios_base::in | ios_base::binary);
		position(10, 2);
		cout << "ID do registro que deseja alterar o nome ...: ";
		cin >> id;
		position(22, 2);
		clearline();
		position(12, 2);
		if (id != 0)
		{
			int posicaoRegistro = encontrarPosicaoId(id);
			bool acha = false;
			while ((not arquivo.eof()) and acha != true)
			{
				arquivo.read(reinterpret_cast<char*>(&registro_base), sizeof(class Registro));
				if (id == registro_base.getId())
				{
					acha = true;
					registro_base.mostraDados();
					cout << " Deseja alterar este registro? (1) Sim; (2) Não;\n ";
					cin >> opcaoAlterar;
					if (opcaoAlterar == 1)
					{
						cout << " Digite o novo nome: ";
						alteracaoNome(posicaoRegistro);
						cout << " Registro alterado.\n";
					}
					else
						cout << " Registro não alterado.\n";
				}
			}
			if (acha == false)
				cout << " Registro não encontrado!" << endl;
			arquivo.flush();
			arquivo.close();
			cout << "\n Deseja pesquisar outro registro? (1) Sim; (0) Não, retornar ao menu;  " << endl;
			cin >> id;
		}
	} while (id != 0);
}

bool registroExistenteData(Registro registro)
{
	criaArquivo();
	fstream arquivo("arquivo.dat", ios_base::in | ios_base::binary);
	bool acha = false; //flag;
	while ((not arquivo.eof()) and acha != true)
	{
		arquivo.read(reinterpret_cast<char*>(&registro_base), sizeof(class Registro));
		string dataCadastro = registro.getData();
		string dataBanco = registro_base.getData();
		if (dataCadastro == dataBanco && registro.getNome() == registro_base.getNome()) //se não se;
			acha = true;
	}
	arquivo.flush();
	arquivo.close();
	return acha;
}

void alteracaoData(int pos)
{
	string data;
	char dataArray[11];
	bool dataValida = false;
	do
	{
		cin >> data;
		strcpy(dataArray, data.c_str());
		dataValida = validate(dataArray);
		registro_base.setData(dataArray);
		if (!dataValida)
			cout << " Data inválida preencha novamente.";
		if (registroExistenteData(registro_base))
		{
			dataValida = false;
			cout << " Este registro já existe, tente novamente.\n";
		}
	} while (!dataValida);
	registro_base.setData(dataArray);
	fstream arquivo("arquivo.dat", ios_base::in | ios_base::out | ios_base::binary);
	arquivo.seekp(pos* sizeof(registro_base));
	arquivo.write(reinterpret_cast<char*>(&registro_base), sizeof(class Registro));
	arquivo.flush();
	arquivo.close();
}

void alterarDataNascimento()
{
	int id;
	int opcaoAlterar;
	do
	{
		limpa();
		string tipo = "Módulo de Alteração de data de nascimento";
		tracalinha(tipo);
		position(6, 2);
		cout << "Digite os dados abaixo:";
		position(8, 2);
		clearline();
		position(22, 2);
		cout << "Digite [0] para voltar ao menu: ";
		fstream arquivo("arquivo.dat", ios_base::in | ios_base::binary);
		position(10, 5);
		cout << "ID do registro que deseja alterar ...: ";
		cin >> id;
		position(22, 2);
		clearline();
		position(12, 2);
		if (id != 0)
		{
			int posicaoRegistro = encontrarPosicaoId(id);
			bool acha = false;
			while ((not arquivo.eof()) and acha != true)
			{
				arquivo.read(reinterpret_cast<char*>(&registro_base), sizeof(class Registro));
				if (id == registro_base.getId())
				{
					acha = true;
					registro_base.mostraDados();
					cout << " Deseja alterar este registro? (1) Sim; (2) Não;\n ";
					cin >> opcaoAlterar;
					if (opcaoAlterar == 1)
					{
						cout << " Digite a nova data: ";
						alteracaoData(posicaoRegistro);
						cout << " Registro alterado.\n";
					}
					else
						cout << " Registro não alterado.\n";
				}
			}
			if (acha == false)
				cout << " Registro não encontrado!" << endl;
			arquivo.flush();
			arquivo.close();
			cout << "\n Deseja pesquisar outro registro? (1) Sim; (0) Não, retornar ao Menu de Alteração;  " << endl;
			cin >> id;
		}
	} while (id != 0);
}

void opcoesMenuAlterar(int opcao)
{
	switch (opcao)
	{
	case 1:
		alterarNome();
		break;
	case 2:
		alterarDataNascimento();
		break;
	case 3:
		break;
	default:
		cout << "Opção inválida! Digite novamente.";
		break;
	}
}

void menuAlterar()
{
	int opcao;
	do
	{
		limpa();
		string tipo;
		tipo = "Menu de Alteração";
		tracalinha(tipo);
		position(8, 16);
		cout << "Alterar nome ....................: [1]" << "\n";
		position(10, 16);
		cout << "Alterar data de nascimento ......: [2]" << "\n";
		position(12, 16);
		cout << "Voltar ao menu ..................: [3]" << "\n";
		position(18, 16);
		cout << "Entre com a opção : ";
		cin >> opcao;
		opcoesMenuAlterar(opcao);
	} while (opcao != 3);
}

void opcoesMenu(int opcao)
{
	switch (opcao)
	{
	case 1:
		cadastrar();
		break;
	case 2:
		pesquisarNome();
		break;
	case 3:
		pesquisarDataNascimento();
		break;
	case 4:
		removerRegistro();
		break;
	case 5:
		menuAlterar();
		break;
	case 6:
		break;
	default:
		cout << "Opção inválida! Digite novamente.";
		break;
	}
}
