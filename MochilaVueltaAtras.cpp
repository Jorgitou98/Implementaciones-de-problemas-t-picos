#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h> 
#include <ctime>
#include <time.h>
#include <fstream>
using namespace std;

size_t const ITERACIONES_MIN = 25;
size_t const ITERACIONES_MAX = 30;
size_t const PASO = 1;
float const RANDOM_MIN = 1.0;
float const RANDOM_MAX = 5.0;
size_t const RANDOM_NAT_MIN = 1;
size_t const RANDOM_NAT_MAX = 5;


struct DatosProblema
{
	float tam_Mochila;
	vector<float> pesos;
	vector<float> valores;
};



void VueltaAtrasFactibilidad(vector<int>const & objetos, DatosProblema const& dp, float valor_candidato, float peso_candidato, float& sol, int indiceElemento, size_t & nodos) {
	
	nodos++;

	if (valor_candidato + dp.valores[objetos[indiceElemento]] > sol && peso_candidato + dp.pesos[objetos[indiceElemento]] <= dp.tam_Mochila) {
		sol = valor_candidato + dp.valores[objetos[indiceElemento]];
	}

	/*	Si el elemento actual entra, lo agrego y busco las soluciones que lo contengan
		Ademas si el elemento del proximo indiceElemento entra
		vemos las soluciones de ver ese indiceElemento. Si no, al estar ordenado 'objetos'
		En orden creciente por tamaño, las demas soluciones tampoco entran por lo que los ignoramos. Estaremos podando */

	if (indiceElemento + 1 < objetos.size()) {
		// Marcado y desmarcado
		if (peso_candidato + dp.pesos[objetos[indiceElemento]] <= dp.tam_Mochila) {
			valor_candidato += dp.valores[objetos[indiceElemento]];
			peso_candidato += dp.pesos[objetos[indiceElemento]];
			if (peso_candidato + dp.pesos[objetos[indiceElemento + 1]] <= dp.tam_Mochila)
				VueltaAtrasFactibilidad(objetos, dp, valor_candidato, peso_candidato, sol, indiceElemento + 1, nodos);
			valor_candidato -= dp.valores[objetos[indiceElemento]];
			peso_candidato -= dp.pesos[objetos[indiceElemento]];
		}

	 /* Sin considerar el objeto actual, si el elemento del proximo indiceElemento entra
		Vemos las soluciones de ver ese indiceElemento. Si no, al estar ordenado objetos
		en orden creciente por tamaño, las demas soluciones tampoco entran
		Por lo que los ignoramos */

		if (peso_candidato + dp.pesos[objetos[indiceElemento + 1]] <= dp.tam_Mochila)
			VueltaAtrasFactibilidad(objetos, dp, valor_candidato, peso_candidato, sol, indiceElemento + 1, nodos);
	}
}


void pruebaAleatorios(size_t N, ofstream &salida, ofstream &salidaGraficarX, ofstream &salidaGraficarY) {
	srand(time(NULL));
	size_t tam_mochila;
	float valor, peso;

	vector<int> v_indices;
	DatosProblema datosProblema;
	datosProblema.tam_Mochila = N;
	for (size_t i = 0; i < N; ++i) {
		peso = (RANDOM_MAX - RANDOM_MIN) * ((float)rand() / (float)RAND_MAX) + RANDOM_MIN;
		valor = (RANDOM_MAX - RANDOM_MIN) * ((float)rand() / (float)RAND_MAX) + RANDOM_MIN;
		v_indices.push_back(i);
		datosProblema.pesos.push_back(peso);
		datosProblema.valores.push_back(valor);
	}
	size_t nodos = 0;
	float valorOptimo;
	int t0 = clock();
	sort(v_indices.begin(), v_indices.end(), [datosProblema](int const & a, const int& b) {
		return (datosProblema.pesos[a] < datosProblema.pesos[b]);
	});
	VueltaAtrasFactibilidad(v_indices, datosProblema, 0, 0, valorOptimo, 0, nodos);
	int t1 = clock();
	float tiempo = float(t1 - t0) / CLOCKS_PER_SEC;
	salida << "N = " << N << '\n';
	salida << "Nodos visitados " << nodos << '\n';
	salida << "Tiempo requerido = " << tiempo << " segundos" << '\n';
	salida << "--------------------------\n";
	salidaGraficarX << N << '\n';
	salidaGraficarY << fixed;
	salidaGraficarY << setprecision(3) << tiempo << '\n';

	cout << "N = " << N << '\n';
	cout << "Tamano de la mochila " << datosProblema.tam_Mochila << '\n';
	cout << "Nodos visitados " << nodos << '\n';
	cout << "Tiempo requerido = " << tiempo << " segundos" << '\n';
}

void pruebaAleatoriosBucle() {
	ofstream salida("SalidaAleatoriosVAtrasFact.txt");
	ofstream salidaGraficarX("SalidaAleatoriosXVAtrasFact.txt");
	ofstream salidaGraficarY("SalidaAleatoriosYVAtrasFact.txt");
	for (size_t i = ITERACIONES_MIN; i <= ITERACIONES_MAX; i = i + PASO)
		pruebaAleatorios(i, salida, salidaGraficarX, salidaGraficarY);
	salida.close();
	salidaGraficarX.close();
	salidaGraficarY.close();
}



int main() {
	/*
	vector<int> v_indices = { 0,1,2,3,4 };
	DatosProblema objetos;
	objetos.tam_Mochila = 25;
	objetos.pesos = { 10,15,5,10,5 };
	objetos.valores = { 5,4,13,8,8 }
	float valorOptimo;
	size_t nodos = 0;
	VueltaAtrasFactibilidad(v_indices, objetos, 0, 0, valorOptimo, 0, nodos);
	cout << valorOptimo;
	*/
	pruebaAleatoriosBucle();
	system("pause");
	return 0;
}