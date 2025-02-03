/*
 *  Act 4.3 Evidencia Grafos
 * Programa para construir un grafo de eventos de bitácora
 * Autores:
 * José Leobardo Navarro Márquez - A01541324
 * Edgar Daniel Osorio Castaños - A07065338
 * Natalia Quiroga Colorado - a01722353
 * Fecha: 03/02/2025
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <set>
using namespace std;

// Estructura para representar un nodo de evento
struct EventNode {
    string fecha;     // Fecha del evento
    string hora;      // Hora del evento
    string ip;        // IP de origen
    string tipo;      // Tipo de entrada (Illegal user, Failed password, etc.)
    vector<EventNode*> conexiones; // Conexiones a otros nodos

    EventNode(string f, string h, string origen, string entrada)
            : fecha(f), hora(h), ip(origen), tipo(entrada) {}
};

// Estructura del grafo de eventos
class EventGraph {
private:
    // Mapa donde la clave es la fecha y el valor es una lista de nodos
    map<string, vector<EventNode*>> grafo;

    // Mapa para contar ocurrencias de IPs
    unordered_map<string, int> contadorIPs;

    // Función auxiliar para extraer IP sin puerto
    string extractIP(const string& ipPort) {
        size_t pos = ipPort.find(":");
        return (pos != string::npos) ? ipPort.substr(0, pos) : ipPort;
    }

public:
    // Agregar un evento al grafo
    void agregarEvento(const string& fecha, const string& hora,
                       const string& ipPort, const string& tipo) {
        string ip = extractIP(ipPort);
        EventNode* nuevoNodo = new EventNode(fecha, hora, ip, tipo);

        // Si la fecha no existe, crearla
        if (grafo.find(fecha) == grafo.end()) {
            grafo[fecha] = vector<EventNode*>();
        }

        grafo[fecha].push_back(nuevoNodo);

        // Contar ocurrencias de IPs
        contadorIPs[ip]++;
    }

    // Conectar nodos dentro de la misma fecha
    void conectarNodos() {
        for (auto& fechaEntrada : grafo) {
            auto& nodos = fechaEntrada.second;

            // Ordenar nodos por hora
            sort(nodos.begin(), nodos.end(),
                 [](const EventNode* a, const EventNode* b) {
                     return a->hora < b->hora;
                 });

            // Conectar nodos con el mismo tipo
            for (size_t i = 0; i < nodos.size() - 1; ++i) {
                if (nodos[i]->tipo == nodos[i+1]->tipo) {
                    nodos[i]->conexiones.push_back(nodos[i+1]);
                }
            }
        }
    }

    // Calcular fan-out de cada nodo
    unordered_map<string, int> calcularFanOut() {
        unordered_map<string, int> fanOut;

        for (const auto& fechaEntrada : grafo) {
            for (const auto* nodo : fechaEntrada.second) {
                // El fan-out es el número de conexiones del nodo
                fanOut[nodo->ip] = nodo->conexiones.size();
            }
        }

        return fanOut;
    }

    // Encontrar nodos con mayor fan-out
    vector<pair<string, int>> encontrarNodosMayorFanOut(int topN = 5) {
        unordered_map<string, int> fanOut = calcularFanOut();

        // Convertir mapa a vector para ordenar
        vector<pair<string, int>> nodosFanOut(fanOut.begin(), fanOut.end());

        // Ordenar de mayor a menor
        sort(nodosFanOut.begin(), nodosFanOut.end(),
             [](const pair<string, int>& a, const pair<string, int>& b) {
                 return a.second > b.second;
             });

        // Devolver top N
        return vector<pair<string, int>>(
                nodosFanOut.begin(),
                nodosFanOut.begin() + min(topN, static_cast<int>(nodosFanOut.size()))
        );
    }

    // Encontrar IP presumiblemente del boot master
    string encontrarBootMaster() {
        // Criterios para identificar boot master:
        // 1. Mayor número de intentos de conexión
        // 2. Mayor fan-out
        // 3. Mayor variedad de tipos de eventos

        unordered_map<string, int> fanOut = calcularFanOut();

        // Combinar múltiples criterios
        unordered_map<string, int> scoreBootMaster;

        for (const auto& par : contadorIPs) {
            const string& ip = par.first;
            int intentos = par.second;
            int fanOutScore = fanOut[ip];

            // Calcular puntuación compuesta
            scoreBootMaster[ip] = intentos + fanOutScore;
        }

        // Encontrar IP con mayor puntuación
        auto bootMaster = max_element(scoreBootMaster.begin(), scoreBootMaster.end(),
                                      [](const pair<const string, int>& a, const pair<const string, int>& b) {
                                          return a.second < b.second;
                                      });

        return bootMaster != scoreBootMaster.end() ? bootMaster->first : "No encontrado";
    }

    // Imprimir grafo
    void imprimirGrafo() {
        for (const auto& fechaEntrada : grafo) {
            cout << "Fecha: " << fechaEntrada.first << endl;

            for (const auto* nodo : fechaEntrada.second) {
                cout << "  Hora: " << nodo->hora
                     << ", IP: " << nodo->ip
                     << ", Tipo: " << nodo->tipo << endl;

                if (!nodo->conexiones.empty()) {
                    cout << "    Conexiones:" << endl;
                    for (const auto* conexion : nodo->conexiones) {
                        cout << "      -> " << conexion->hora
                             << ", IP: " << conexion->ip
                             << ", Tipo: " << conexion->tipo << endl;
                    }
                }
            }
            cout << endl;
        }
    }

    // Destructor para liberar memoria
    ~EventGraph() {
        for (auto& par : grafo) {
            for (auto* nodo : par.second) {
                delete nodo;
            }
        }
    }
};

int main() {
    ifstream archivo("bitacora.txt");
    if (!archivo) {
        cerr << "Error al abrir el archivo" << endl;
        return 1;
    }

    EventGraph grafoEventos;
    string linea;

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string fecha, hora, ipPort, resto;

        ss >> fecha >> hora >> ipPort;
        getline(ss, resto);

        // Determinar tipo de entrada
        string tipo = (resto.find("Illegal user") != string::npos)
                      ? "Illegal user"
                      : "Failed password";

        grafoEventos.agregarEvento(fecha, hora, ipPort, tipo);
    }

    archivo.close();

    // Conectar nodos del grafo
    grafoEventos.conectarNodos();

    // Encontrar nodos con mayor fan-out
    cout << "IPs con Mayor Fan-Out:" << endl;
    vector<pair<string, int>> nodosMayorFanOut = grafoEventos.encontrarNodosMayorFanOut();
    for (const auto& par : nodosMayorFanOut) {
        cout << "IP: " << par.first << ", Fan-Out: " << par.second << endl;
    }

    // Encontrar Boot Master
    string bootMaster = grafoEventos.encontrarBootMaster();
    cout << "\nIP Presumible de Boot Master: " << bootMaster << endl;

    return 0;
}
