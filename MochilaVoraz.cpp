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

size_t const ITERACIONES_MIN = 300000;
size_t const ITERACIONES_MAX = 3000000;
size_t const PASO = 100000;
double const RANDOM_MIN = 1.0;
double const RANDOM_MAX = 5.0;

struct tObjeto {
	double peso, valor;
	size_t id;
	bool operator >(tObjeto const& o) const {
		return (valor / peso) > (o.valor / o.peso);
	}
};

void resolver(vector<tObjeto> & objetos, double M,
	vector<double> & sol, double & valor, size_t & objetosMetidos) { // O(N log N)
	size_t N = objetos.size();
	sort(objetos.begin(), objetos.end(), greater<tObjeto>());
	double peso = 0; valor = 0;
	size_t i;
	for (i = 0; i < N && peso + objetos[i].peso <= M; ++i) {
		sol[objetos[i].id] = 1; // el objeto D[i].id cabe completo
		objetosMetidos++;
		peso += objetos[i].peso;
		valor += objetos[i].valor;
	}
	if (i < N) { // partir el objeto D[i].id
		sol[objetos[i].id] = (M - peso) / objetos[i].peso;
		valor += sol[objetos[i].id] * objetos[i].valor;
	}
}

/* Funcion que calcular el tiempo que tarda el algoritmo en resolver un caso random de tamaño N
y rellena tanto el archivo salida que da un información detallada de a prueba,
como salidaGraficarX y salidaGraficarY con las coordenadas para representar graficamente la misma
*/
void pruebaAleatorios(size_t N, ofstream &salida, ofstream &salidaGraficarX, ofstream &salidaGraficarY) {
	srand(time(NULL));
	double peso, valor;
	size_t tam_mochila = 2 * N;
	size_t objetosMetidos = 0;
	vector<tObjeto> objetos;
	for (size_t i = 0; i < N; ++i) {
		peso = (RANDOM_MAX - RANDOM_MIN) * ((double)rand() / (double)RAND_MAX) + RANDOM_MIN;
		valor = (RANDOM_MAX - RANDOM_MIN) * ((double)rand() / (double)RAND_MAX) + RANDOM_MIN;
		objetos.push_back({ peso, valor, i });
	}
	vector<double> sol(objetos.size(), 0);
	double valorOptimo;
	int t0 = clock();
	resolver(objetos, tam_mochila, sol, valorOptimo, objetosMetidos);
	int t1 = clock();
	double tiempo = double(t1 - t0) / CLOCKS_PER_SEC;
	salida << "N = " << N << '\n';
	salida << "Cantidad de objetos metidos: " << objetosMetidos << '\n';
	salida << "Tiempo requerido = " << tiempo <<  " segundos"<< '\n';
	salida << "--------------------------\n";
	salidaGraficarX << N << '\n';
	salidaGraficarY << fixed;
	salidaGraficarY << setprecision(3) << tiempo << '\n';
}

void pruebaAleatoriosBucle() {
	ofstream salida("SalidaAleatorios.txt");
	ofstream salidaGraficarX("SalidaAleatoriosX.txt");
	ofstream salidaGraficarY("SalidaAleatoriosY.txt");
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