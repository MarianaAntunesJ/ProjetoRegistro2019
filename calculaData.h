#pragma once
#include <iostream>
#include <ctime>
#include <windows.h>
using namespace std;

int coverterAnoToDias(int entrada)
{
	return (entrada * 365); 
}

int converterMesToDias(int entrada)
{
	return (entrada * 30);
}

int converterDataToDias(int dia, int mes, int ano)
{
	return (coverterAnoToDias(ano) + converterMesToDias(mes) + dia); //conversão em dias total.
}

int dataStringToDataInt(char dataEntrada[])
{
	string dia, mes, ano, dataEntrda;

	ano.push_back(dataEntrada[6]); //concatenação.
	ano.push_back(dataEntrada[7]);
	ano.push_back(dataEntrada[8]);
	ano.push_back(dataEntrada[9]);

	mes.push_back(dataEntrada[3]);
	mes.push_back(dataEntrada[4]);

	dia.push_back(dataEntrada[0]);
	dia.push_back(dataEntrada[1]);

	int dataEntradaInt = converterDataToDias(stoi(dia), stoi(mes), stoi(ano)); //string to int.

	return dataEntradaInt;
}

string idadeEmData(int diasEntrada)
{
	int anos = (int)(diasEntrada / 365);  //parse explicíto, (para não virar um double).
	int totalDias = diasEntrada - anos * 365;
	int meses = (int)(totalDias / 30);
	totalDias = totalDias - meses * 30;

	string result = to_string(anos) + " ano(s), " + to_string(meses) + " mes(es) e " + to_string(totalDias) + " dia(s)."; //voltar para string;
	return result;
}

string comparadorDatas(char nascimentoEntrada[])
{
	int nascimentoInt = dataStringToDataInt(nascimentoEntrada);
	int ano, mes, dia;
	SYSTEMTIME tm;
	GetSystemTime(&tm);
	ano = tm.wYear; mes = tm.wMonth; dia = tm.wDay; //data atual;

	int dataAtualInt = converterDataToDias(dia, mes, ano); 

	int idadeIntDias = dataAtualInt - nascimentoInt;

	return idadeEmData(idadeIntDias);
}



