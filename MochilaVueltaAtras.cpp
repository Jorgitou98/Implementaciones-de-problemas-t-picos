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

enum poda {
	FACTIBILIDAD, OPTIMALIDAD, AMBAS
};

struct DatosProblema
{
	float tam_Mochila;
	vector<float> pesos;
	vector<float> valores;
};



float cotaSupOpt(vector<int> const& objetos, DatosProblema const& dp, float peso, float beneficio, int k) {
	float opt;
	int n = objetos.size();
	float hueco = dp.tam_Mochila - peso; opt = beneficio;
	int j = k + 1;
	while (j <= n && dp.pesos [objetos[j - 1]]<= hueco) {
		hueco -= dp.pesos[objetos[j - 1]];
		opt += dp.valores[objetos[j - 1]];
		j++;
	}
	if (j <= n) {
		opt += opt + (hueco / dp.pesos[objetos[j - 1]]) * dp.valores[objetos[j - 1]];
	}
	return opt;
}



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


void VueltaAtrasOptimalidad(vector<int>const & objetos, DatosProblema const& dp, float valor_candidato, float peso_momentaneo, float& sol, int indiceElemento, size_t & nodos) {

	nodos++;

	if (valor_candidato + dp.valores[objetos[indiceElemento]] > sol && peso_momentaneo + dp.pesos[objetos[indiceElemento]] <= dp.tam_Mochila) {
		sol = valor_candidato + dp.valores[objetos[indiceElemento]];
	}

	if (indiceElemento + 1 < objetos.size()) {
		// Marcado y desmarcado
		if (peso_momentaneo + dp.pesos[objetos[indiceElemento]] <= dp.tam_Mochila) {
			valor_candidato += dp.valores[objetos[indiceElemento]];
			peso_momentaneo += dp.pesos[objetos[indiceElemento]];
			VueltaAtrasOptimalidad(objetos, dp, valor_candidato, peso_momentaneo, sol, indiceElemento + 1, nodos);
			valor_candidato -= dp.valores[objetos[indiceElemento]];
			peso_momentaneo -= dp.pesos[objetos[indiceElemento]];
		}

		if (cotaSupOpt(objetos, dp, valor_candidato, peso_momentaneo, indiceElemento +1) > sol)
			VueltaAtrasOptimalidad(objetos, dp, valor_candidato, peso_momentaneo, sol, indiceElemento + 1, nodos);
	}
}


void VueltaAtrasAmbasPodas(vector<int>const & objetos, DatosProblema const& dp, float valor_candidato, float peso_candidato, float& sol, int indiceElemento, size_t & nodos) {

	nodos++;

	if (valor_candidato + dp.valores[objetos[indiceElemento]] > sol && peso_candidato + dp.pesos[objetos[indiceElemento]] <= dp.tam_Mochila) {
		sol = valor_candidato + dp.valores[objetos[indiceElemento]];
	}

	if (indiceElemento + 1 < objetos.size()) {
		// Marcado y desmarcado
		if (peso_candidato + dp.pesos[objetos[indiceElemento]] <= dp.tam_Mochila) {
			valor_candidato += dp.valores[objetos[indiceElemento]];
			peso_candidato += dp.pesos[objetos[indiceElemento]];
			if (peso_candidato + dp.pesos[objetos[indiceElemento + 1]] <= dp.tam_Mochila)
				VueltaAtrasAmbasPodas(objetos, dp, valor_candidato, peso_candidato, sol, indiceElemento + 1, nodos);
			valor_candidato -= dp.valores[objetos[indiceElemento]];
			peso_candidato -= dp.pesos[objetos[indiceElemento]];
		}

		if (cotaSupOpt(objetos, dp, valor_candidato, peso_candidato, indiceElemento + 1) > sol)
			VueltaAtrasAmbasPodas(objetos, dp, valor_candidato, peso_candidato, sol, indiceElemento + 1, nodos);
	}
}


void pruebaAleatorios(size_t N, ofstream &salida, ofstream &salidaGraficarX, ofstream &salidaGraficarY, poda p) {
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
	switch (p)
	{
	case FACTIBILIDAD:
		sort(v_indices.begin(), v_indices.end(), [datosProblema](int const & a, int const& b) {
			return (datosProblema.pesos[a] < datosProblema.pesos[b]);
		});
		VueltaAtrasFactibilidad(v_indices, datosProblema, 0, 0, valorOptimo, 0, nodos);
		break;
	case OPTIMALIDAD:
		sort(v_indices.begin(), v_indices.end(), [datosProblema](const int & a, const int& b) {
			return (datosProblema.valores[a] / datosProblema.pesos[a]) >= (datosProblema.valores[b] / datosProblema.pesos[b]);
		});
		VueltaAtrasOptimalidad(v_indices, datosProblema, 0, 0, valorOptimo, 0, nodos);
		break;
	case AMBAS:
		sort(v_indices.begin(), v_indices.end(), [datosProblema](int const & a, int const& b) {
			return ((datosProblema.valores[a] / datosProblema.pesos[a]) >= (datosProblema.valores[b] / datosProblema.pesos[b]));
		});
		VueltaAtrasAmbasPodas(v_indices, datosProblema, 0, 0, valorOptimo, 0, nodos);
		break;
	default:
		break;
	}
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



void pruebaAleatoriosBucleFact() {
	ofstream salida("SalidaAleatoriosVAtrasFact.txt");
	ofstream salidaGraficarX("SalidaAleatoriosXVAtrasFact.txt");
	ofstream salidaGraficarY("SalidaAleatoriosYVAtrasFact.txt");
	for (size_t i = ITERACIONES_MIN; i <= ITERACIONES_MAX; i = i + PASO)
		pruebaAleatorios(i, salida, salidaGraficarX, salidaGraficarY, FACTIBILIDAD);
	salida.close();
	salidaGraficarX.close();
	salidaGraficarY.close();
}

void pruebaAleatoriosBucleOpt() {
	ofstream salida("SalidaAleatoriosVAtrasOpt.txt");
	ofstream salidaGraficarX("SalidaAleatoriosXVAtrasOpt.txt");
	ofstream salidaGraficarY("SalidaAleatoriosYVAtrasOpt.txt");
	for (size_t i = ITERACIONES_MIN; i <= ITERACIONES_MAX; i = i + PASO)
		pruebaAleatorios(i, salida, salidaGraficarX, salidaGraficarY, OPTIMALIDAD);
	salida.close();
	salidaGraficarX.close();
	salidaGraficarY.close();
}

void pruebaAleatoriosBucleAmbas() {
	ofstream salida("SalidaAleatoriosVAtrasAmbas.txt");
	ofstream salidaGraficarX("SalidaAleatoriosXVAtrasAmbas.txt");
	ofstream salidaGraficarY("SalidaAleatoriosYVAtrasAmbas.txt");
	for (size_t i = ITERACIONES_MIN; i <= ITERACIONES_MAX; i = i + PASO)
		pruebaAleatorios(i, salida, salidaGraficarX, salidaGraficarY, AMBAS);
	salida.close();
	salidaGraficarX.close();
	salidaGraficarY.close();
}



int main() {
	pruebaAleatoriosBucleAmbas();
	system("pause");
	return 0;
}