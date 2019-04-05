
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stdio.h>
#include "VorazMochila.h"
#include "Matriz.h"
#include <stdlib.h> 
#include <ctime>
#include <time.h>
#include <fstream>
using namespace std;

size_t const ITERACIONES_MIN = 1000;
size_t const ITERACIONES_MAX = 1000;
size_t const PASO = 1;
float const RANDOM_MIN = 1.0;
float const RANDOM_MAX = 5.0;
size_t const RANDOM_NAT_MIN = 1;
size_t const RANDOM_NAT_MAX = 5;


void sumaParcial(int indice, pair<int,int> par, vector<pair<int, int>> const &objetos, pair<int, int> &sol, int M){
	if (indice == objetos.size()) {
		if (par.first <= M && par.second > sol.second) {
			sol.first = par.first;
			sol.second = par.second;
		}
	}
	else {
		indice++;
		sumaParcial(indice, par, objetos, sol, M);

		par.first += objetos[indice - 1].first;
		par.second += objetos[indice - 1].second;
		sumaParcial(indice, par, objetos, sol, M);
	}
}

int fuerza_bruta(int M, vector<pair<int, int>> const &mochila) {
	pair<int, int> par(0, 0);
	pair<int, int> sol(0, 0);
	sumaParcial(0, par, mochila, sol, M);
	int beneficio = sol.second;
	return beneficio;
}

void pruebaAleatorios(size_t N, ofstream &salida, ofstream &salidaGraficarX, ofstream &salidaGraficarY) {
	srand(time(NULL));
	size_t peso, tam_mochila;
	float valor;

	tam_mochila = 2 * N;

	size_t objetosMetidos = 0;
	vector<pair<int, int>> objetos;
	for (size_t i = 0; i < N; ++i) {
		peso = (RANDOM_MAX - RANDOM_MIN) * ((float)rand() / (float)RAND_MAX) + RANDOM_MIN;
		valor = (RANDOM_MAX - RANDOM_MIN) * ((float)rand() / (float)RAND_MAX) + RANDOM_MIN;
		objetos.push_back({ peso, valor });
	}
	vector<bool> sol(objetos.size() + 1, false);
	float valorOptimo;
	int t0 = clock();
	fuerza_bruta(tam_mochila, objetos);
	int t1 = clock();
	float tiempo = float(t1 - t0) / CLOCKS_PER_SEC;
	salida << "N = " << N << '\n';
	salida << "Cantidad de objetos metidos: " << objetosMetidos << '\n';
	salida << "Tiempo requerido = " << tiempo << " segundos" << '\n';
	salida << "--------------------------\n";
	salidaGraficarX << N << '\n';
	salidaGraficarY << fixed;
	salidaGraficarY << setprecision(3) << tiempo << '\n';

	cout <<  "N = " << N << '\n';
	cout << "Tiempo requerido = " << tiempo << " segundos" << '\n';
}



void pruebaAleatoriosBucle() {
	ofstream salida("SalidaAleatoriosFBruta.txt");
	ofstream salidaGraficarX("SalidaAleatoriosXFBruta.txt");
	ofstream salidaGraficarY("SalidaAleatoriosYFBruta.txt");
	for (size_t i = ITERACIONES_MIN; i <= ITERACIONES_MAX; i = i + PASO)
		pruebaAleatorios(i, salida, salidaGraficarX, salidaGraficarY);
	salida.close();
	salidaGraficarX.close();
	salidaGraficarY.close();
}


int main() {
	pruebaAleatoriosBucle();
	system("pause");
	return 0;
}