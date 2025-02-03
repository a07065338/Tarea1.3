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

// Función para cargar el archivo y analizar los puertos atacados en horas sospechosas
void loadLogFile(const string& filename, map<string, int>& portSuspiciousAttempts) {
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
        
        // Si el intento ocurrió en un horario sospechoso (00:00 - 05:00), registrarlo
        if (hour >= 0 && hour < 5) {
            portSuspiciousAttempts[port]++;
        }
    }
    file.close();
}

// Función para encontrar el puerto más atacado en horas sospechosas
void findMostAttackedPort(const map<string, int>& portSuspiciousAttempts) {
    string mostAttackedPort;
    int maxAttempts = 0;

    for (const auto& entry : portSuspiciousAttempts) {
        if (entry.second > maxAttempts) {
            maxAttempts = entry.second;
            mostAttackedPort = entry.first;
        }
    }
    
    cout << "Puerto más atacado en horas sospechosas (00:00 - 05:00): " << mostAttackedPort << " con " << maxAttempts << " intentos." << endl;
}

int main() {
    string filename = "bitacora.txt";
    map<string, int> portSuspiciousAttempts;

    // Cargar datos del archivo y analizar intentos sospechosos
    loadLogFile(filename, portSuspiciousAttempts);

    // Encontrar el puerto más atacado en horas sospechosas
    findMostAttackedPort(portSuspiciousAttempts);
    
    return 0;
}
