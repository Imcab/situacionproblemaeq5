/*
 * Actividad Integral 3: lee la bitacora ordenada por IP destino resultante de
 * la Actividad Integral 2, cuenta los accesos de cada IP destino, los acomoda
 * en un heap y despliega las N IPs con mas accesos
 * Charbel German (A01799433), Imad Jared Cabrera Trejo (A01798415)
 * Fecha de modificacion: 23-11-2026
 * Enlace de version online: https://www.jdoodle.com/ia/jdfp-397f-f7rg
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <string>
#include <iomanip>
#include <utility>
#include <algorithm>
#include <vector>
#include <limits>

using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::pair;
using std::ifstream;
using std::stringstream;
using std::endl;

// Representa una direccion IP (o1.o2.o3.o4); 0.0.0.0 representa "-"
struct ip {
	int o1, o2, o3, o4;
};

// Representa un evento de la bitacora (conexion entre dos nodos)
// ts: fecha y hora del evento
// ip_o, port_o, domain_o: IP, puerto y dominio del nodo origen
// ip_d, port_d, domain_d: IP, puerto y dominio del nodo destino
struct event {
	struct std::tm ts;
	struct ip ip_o;
	string port_o;
	string domain_o;
	struct ip ip_d;
	string port_d;
	string domain_d;
};

// Imprime una IP; si o1 == 0 imprime '-'
// os: flujo de salida
// i: IP a imprimir
// Regresa: el mismo flujo de salida
std::ostream& operator<<(std::ostream &os, const ip &i) {
	if (i.o1 == 0) {
		os << "-";
	} else {
		os << i.o1 << "." << i.o2 << "." << i.o3 << "." << i.o4;
	}
	return os;
}

// Imprime un par con el formato "ip: accesos"
// os: flujo de salida
// p: par (IP, numero de accesos) a imprimir
// Regresa: el mismo flujo de salida
std::ostream& operator<<(std::ostream &os, const pair<ip, int> &p) {
	os << p.first << ": " << p.second;
	return os;
}

// Determina si dos IPs son iguales
// ip1, ip2: IPs a comparar
// Regresa: true si los cuatro octetos coinciden
bool operator==(const ip &ip1, const ip &ip2) {
	return ip1.o1 == ip2.o1 && ip1.o2 == ip2.o2 && ip1.o3 == ip2.o3 && ip1.o4 == ip2.o4;
}

// Compara dos pares por su numero de accesos; make_heap y pop_heap usan este
// operador para dejar al frente el par con mas accesos (mayor prioridad)
// p1, p2: pares (IP, numero de accesos) a comparar
// Regresa: true si p1 tiene menos accesos que p2
bool operator<(const pair<ip, int> &p1, const pair<ip, int> &p2) {
	return p1.second < p2.second;
}

// Convierte un texto "o1.o2.o3.o4" o "-" en una IP
// ipStr: texto a convertir
// Regresa: la IP leida (0.0.0.0 si es "-")
ip parseIP(const string &ipStr) {
	ip parsedIp = {0, 0, 0, 0};
	if (ipStr != "-") {
		char dot = ' ';
		stringstream ss(ipStr);
		ss >> parsedIp.o1 >> dot >> parsedIp.o2 >> dot >> parsedIp.o3 >> dot >> parsedIp.o4;
	}
	return parsedIp;
}

// Convierte una linea de la bitacora en un evento
// line: linea con formato fecha,hora,ipO,puertoO,dominioO,ipD,puertoD,dominioD
// Regresa: el evento correspondiente
event parseEvent(const string &line) {
	stringstream ss(line);
	string date = "";
	string time = "";
	string ipOrigin = "";
	string ipDest = "";
	event e{};

	getline(ss, date, ',');
	getline(ss, time, ',');
	getline(ss, ipOrigin, ',');
	getline(ss, e.port_o, ',');
	getline(ss, e.domain_o, ',');
	getline(ss, ipDest, ',');
	getline(ss, e.port_d, ',');
	getline(ss, e.domain_d, '\r');

	stringstream ssDate(date + "-" + time);
	ssDate >> std::get_time(&e.ts, "%d-%m-%Y-%H:%M:%S");
	e.ts.tm_isdst = -1;

	e.ip_o = parseIP(ipOrigin);
	e.ip_d = parseIP(ipDest);
	return e;
}

// Lee la bitacora y cuenta los accesos de cada IP destino. Como el archivo
// esta ordenado por IP destino, los eventos con la misma IP vienen seguidos:
// si la IP es igual a la del ultimo par se incrementa su contador, si no se
// agrega un par nuevo con contador 1
// fileName: nombre del archivo a leer
// counts: vector donde se guardan los pares (IP destino, numero de accesos)
// Regresa: true si el archivo pudo abrirse
bool readCounts(const string &fileName, vector<pair<ip, int>> &counts) {
	ifstream file(fileName);
	bool opened = file.is_open();
	if (!opened) {
		cout << "Error al abrir el archivo " << fileName << endl;
	} else {
		string line = "";
		while (getline(file, line)) {
			if (!line.empty() && line != "\r") {
				event e = parseEvent(line);
				if (!counts.empty() && counts.back().first == e.ip_d) {
					++counts.back().second;
				} else {
					counts.push_back(std::make_pair(e.ip_d, 1));
				}
			}
		}
		file.close();
	}
	return opened;
}

// Pide al usuario un numero N hasta que capture uno valido (entre 1 y maxN)
// maxN: valor maximo permitido (numero de IPs distintas)
// Regresa: el numero capturado
int askN(int maxN) {
	int n = 0;
	bool valid = false;
	while (!valid && cin) {
		cout << "\nCuantas IPs con mas accesos deseas ver? (1-" << maxN << "): ";
		cin >> n;
		valid = !cin.fail() && n >= 1 && n <= maxN;
		if (!valid && !cin.eof()) {
			cout << "Valor invalido, ingresa un entero entre 1 y " << maxN << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
	return n;
}

// Despliega las N IPs con mas accesos. En cada paso front() da el par de
// mayor prioridad, pop_heap lo manda al final del vector y reacomoda el resto
// como heap, y pop_back lo elimina
// heap: vector acomodado como heap (se le quitan los N elementos desplegados)
// n: numero de IPs a desplegar
void printTop(vector<pair<ip, int>> &heap, int n) {
	cout << "\nLas " << n << " IPs con mas accesos:" << endl;
	for (int i = 1; i <= n && !heap.empty(); ++i) {
		cout << i << ". " << heap.front() << endl;
		std::pop_heap(heap.begin(), heap.end());
		heap.pop_back();
	}
}

int main() {
	const string INPUT_FILE = "equipo5_ordenado_ip.csv";

	vector<pair<ip, int>> counts{};
	int exitCode = 1;

	if (readCounts(INPUT_FILE, counts) && !counts.empty()) {
		cout << "IPs destino distintas: " << counts.size() << endl;

		// Reacomoda el vector como heap usando el operador < (por accesos)
		std::make_heap(counts.begin(), counts.end());

		int n = askN(static_cast<int>(counts.size()));
		if (n > 0) {
			printTop(counts, n);
		}
		exitCode = 0;
	}
	return exitCode;
}
