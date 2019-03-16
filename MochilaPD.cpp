#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stdio.h>
#include "Matriz.h"
#include <stdlib.h> 
#include <ctime>
#include <time.h>
#include <fstream>
using namespace std;

size_t const ITERACIONES_MIN = 300;
size_t const ITERACIONES_MAX = 3000;
size_t const PASO = 100;
double const RANDOM_MIN = 1.0;
double const RANDOM_MAX = 5.0;
size_t const RANDOM_NAT_MIN = 1;
size_t const RANDOM_NAT_MAX = 5;

struct tObjeto {
	size_t peso;
	double valor;
};


void mochilaPD(vector<tObjeto> & objetos, size_t M,
	vector<bool> & sol, double & valor, size_t & objetosMetidos) {
	int n = objetos.size();
	Matriz<double> mochila(n+1, M+1);

	//Inicializacion
	for (int i = 0; i <= n; ++i) mochila[i][0] = 0;
	for (int j = 1; j <= M; ++j) mochila[0][j] = 0;

	//Rellenamos la matriz
	for (int i = 1; i <= n; ++i)
		for (int j = 1; j <= M; ++j) {
			if (objetos[i-1].peso > j) mochila[i][j] = mochila[i - 1][j];
			else mochila[i][j] = max(mochila[i - 1][j], mochila[i - 1][j- objetos[i-1].peso] + objetos[i-1].valor);
		}
	valor = mochila[n][M];
	//Reconstruccion
	size_t resto = M;
	for (int i = n; i > 0; --i)
		if (mochila[i][resto] != mochila[i - 1][resto]) {
			sol[i] = true;
			objetosMetidos++;
			resto -= objetos[i-1].peso;
		}
}

void pruebaAleatorios(size_t N, ofstream &salida, ofstream &salidaGraficarX, ofstream &salidaGraficarY) {
	srand(time(NULL));
	size_t peso;
	double valor;
	size_t tam_mochila = 2 * N;
	size_t objetosMetidos = 0;
	vector<tObjeto> objetos;
	for (size_t i = 0; i < N; ++i) {
		peso = RANDOM_NAT_MIN + rand() % (RANDOM_NAT_MAX + 1 - RANDOM_NAT_MIN);
		valor = (RANDOM_MAX - RANDOM_MIN) * ((double)rand() / (double)RAND_MAX) + RANDOM_MIN;
		objetos.push_back({ peso, valor});
	}
	vector<bool> sol(objetos.size() + 1, false);
	double valorOptimo;
	int t0 = clock();
	mochilaPD(objetos, tam_mochila, sol, valor, objetosMetidos);
	int t1 = clock();
	double tiempo = double(t1 - t0) / CLOCKS_PER_SEC;
	salida << "N = " << N << '\n';
	salida << "Cantidad de objetos metidos: " << objetosMetidos << '\n';
	salida << "Tiempo requerido = " << tiempo << " segundos" << '\n';
	salida << "--------------------------\n";
	salidaGraficarX << N << '\n';
	salidaGraficarY << fixed;
	salidaGraficarY << setprecision(3) << tiempo << '\n';
}

void pruebaAleatoriosBucle() {
	ofstream salida("SalidaAleatoriosPD.txt");
	ofstream salidaGraficarX("SalidaAleatoriosXPD.txt");
	ofstream salidaGraficarY("SalidaAleatoriosYPD.txt");
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