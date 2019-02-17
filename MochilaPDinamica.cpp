#include <vector>
#include <algorithm>
#include "Matriz.h"
using namespace std;

struct tObjeto {
	size_t valor;
	size_t peso;
};
vector<bool> mochilaPDMatriz(vector<tObjeto> objetos, size_t P) {
	int n = objetos.size();
	Matriz<size_t> mochila(n + 1, P + 1);

	//Casos base
	for (int i = 0; i < n + 1; ++i) mochila[i][0] = 0;
	for (int j = 1; j < P + 1; ++j) mochila[0][j] = 0;

	for (int i = 0; i < n + 1; ++i)
		for (int j = 1; j < P + 1; ++j) {
			if (objetos[i].peso > j) mochila[i][j] = mochila[i - 1][j];
			else mochila[i][j] = max(mochila[i - 1][j], mochila[i - 1][j - objetos[i].peso] + objetos[i].valor);
		}

	vector<bool> sol(n, false);
	//Reconstrucción
	size_t resto = P;
	for (int i = n - 1; n >= 0; --i) {
		if (mochila[i][resto] != mochila[i - 1][resto]) {
			sol[i] = 1;
			resto -= objetos[i].peso;
		}
	}
	return sol;
}



int mochilaPDVector(vector<tObjeto> objetos, size_t P) {
	int n = objetos.size();
	vector<size_t> mochila(P + 1, 0);
	for (int i = 0; i < n + 1; ++i)
		for (int j = P; j >= 0; --j) {
			if (objetos[i].peso <= j) mochila[j] = max(mochila[j], mochila[j - objetos[i].peso] + objetos[i].valor);
		}
	return mochila[P];
}