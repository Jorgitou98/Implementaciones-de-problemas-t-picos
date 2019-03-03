#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
struct tObjeto {
	double peso, valor;
	size_t id;
	bool operator >(tObjeto const& o) const {
		return (valor / peso) > (o.valor / o.peso);
	}
};

void resolver(vector<tObjeto> & objetos, double M,
	vector<double> & sol, double & valor) { // O(N log N)
	size_t N = objetos.size();
	sort(objetos.begin(), objetos.end(), greater<tObjeto>());
	double peso = 0; valor = 0;
	size_t i;
	for (i = 0; i < N && peso + objetos[i].peso <= M; ++i) {
		sol[objetos[i].id] = 1; // el objeto D[i].id cabe completo
		peso += objetos[i].peso;
		valor += objetos[i].valor;
	}
	if (i < N) { // partir el objeto D[i].id
		sol[objetos[i].id] = (M - peso) / objetos[i].peso;
		valor += sol[objetos[i].id] * objetos[i].valor;
	}
}

int main() {
	return 0;
}
