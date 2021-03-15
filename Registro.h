#pragma once
#include <iostream>
#include <string>
#include "calculaData.h"
using namespace std;

class Registro
{
public:
	int getId()
	{
		return id;
	}
	void setId(int IdEntrada)
	{
		id = IdEntrada;
	}
	string getNome()
	{
		return nome;
	}
	void setNome(string nomeEntrada)
	{
		nome = nomeEntrada;
	}
	char* getData()
	{
		static char dataSaida[11];
		for (int i = 0; i <= 10; i++)
			dataSaida[i] = data[i];
		return dataSaida;
	}
	void setData(char dataEntrada[11])
	{
		for (int i = 0; i <= 10; i++)
			data[i] = dataEntrada[i];
	}
	void mostraDados()
	{
		cout << " ID ...................: " << id << "\n";
		cout << " Nome .................: " << nome << "\n";
		cout << " Data de nascimento ...: " << data << "\n";
		cout << " Idade ................: " << comparadorDatas(data) << "\n"; //argumento: data de nascimento;
	}

private:
	int id;
	string nome;
	char data[11];
};