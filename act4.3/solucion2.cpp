#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>

using namespace std;

// Función para extraer el puerto de la dirección IP
string extractPort(const string& ipPort) {
    size_t pos = ipPort.find(":");
    return (pos != string::npos) ? ipPort.substr(pos + 1) : "";
}

// Función para extraer la hora en formato entero
int extractHour(const string& time) {
    stringstream ss(time);
    int hour, minute, second;
    char separator;
    ss >> hour >> separator >> minute >> separator >> second;
    return hour;
}

// Función para cargar el archivo y construir la lista de adyacencia basada en puertos atacados en horas sospechosas
void loadLogFile(const string& filename, map<string, set<string>>& portAdjacencyList) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error al abrir el archivo " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string month, day, time, ipOrigen, message;

        ss >> month >> day >> time >> ipOrigen;
        getline(ss, message); // Capturar el mensaje restante

        // Extraer el puerto
        string port = extractPort(ipOrigen);
        
        // Extraer la hora del intento
        int hour = extractHour(time);
        
        // Si el intento ocurrió en un horario sospechoso (00:00 - 05:00), registrarlo en la lista de adyacencia
        if (hour >= 0 && hour < 5) {
            portAdjacencyList[port].insert(ipOrigen);
        }
    }
    file.close();
}

// Función para encontrar el puerto con mayor fan-out (más IPs atacantes distintas)
void findPortWithMostFanOut(const map<string, set<string>>& portAdjacencyList) {
    string mostAttackedPort;
    int maxFanOut = 0;

    for (const auto& entry : portAdjacencyList) {
        if (entry.second.size() > maxFanOut) {
            maxFanOut = entry.second.size();
            mostAttackedPort = entry.first;
        }
    }
    
    cout << "Puerto con mayor fan-out en horas sospechosas (00:00 - 05:00): " << mostAttackedPort << " con " << maxFanOut << " IPs atacantes distintas." << endl;
}

int main() {
    string filename = "bitacora.txt";
    map<string, set<string>> portAdjacencyList;

    // Cargar datos del archivo y analizar intentos sospechosos
    loadLogFile(filename, portAdjacencyList);

    // Encontrar el puerto con mayor fan-out en horas sospechosas
    findPortWithMostFanOut(portAdjacencyList);
    
    return 0;
}
