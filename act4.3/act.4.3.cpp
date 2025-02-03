#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>

using namespace std;

// Función para extraer solo la dirección IP sin el puerto
string extractIP(const string& ipPort) {
    size_t pos = ipPort.find(":");
    return (pos != string::npos) ? ipPort.substr(0, pos) : ipPort;
}

// Función para extraer la hora en formato entero
int extractHour(const string& time) {
    stringstream ss(time);
    int hour, minute, second;
    char separator;
    ss >> hour >> separator >> minute >> separator >> second;
    return hour;
}

// Función para cargar el archivo y construir la lista de adyacencia basada en mensajes y horas sospechosas
void loadLogFile(const string& filename, map<string, set<string>>& ipToMessages, map<string, int>& suspiciousAdminAccess) {
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

        // Extraer solo la dirección IP
        string cleanIpOrigen = extractIP(ipOrigen);

        // Asociar la IP con el tipo de mensaje
        ipToMessages[cleanIpOrigen].insert(message);
        
        // Extraer la hora del intento
        int hour = extractHour(time);
        
        // Si es un acceso a 'admin' en horario sospechoso (00:00 - 05:00), registrarlo
        if (hour >= 0 && hour < 5 && message.find("admin") != string::npos) {
            suspiciousAdminAccess[cleanIpOrigen]++;
        }
    }
    file.close();
}

// Función para encontrar la IP con más conexiones a diferentes tipos de mensajes
void findMostDiverseAttacker(const map<string, set<string>>& ipToMessages) {
    string mostDiverseIP;
    int maxMessageTypes = 0;

    for (const auto& entry : ipToMessages) {
        if (entry.second.size() > maxMessageTypes) {
            maxMessageTypes = entry.second.size();
            mostDiverseIP = entry.first;
        }
    }
    
    cout << "IP con más tipos de intentos de acceso (posible bot master): " << mostDiverseIP << " con " << maxMessageTypes << " tipos de intentos." << endl;
}

// Función para encontrar la IP con más intentos de acceso a 'admin' en horas sospechosas
void findSuspiciousAdminAttacker(const map<string, int>& suspiciousAdminAccess) {
    string mostSuspiciousIP;
    int maxAttempts = 0;

    for (const auto& entry : suspiciousAdminAccess) {
        if (entry.second > maxAttempts) {
            maxAttempts = entry.second;
            mostSuspiciousIP = entry.first;
        }
    }
    
    cout << "IP con más intentos de acceso a 'admin' en horas sospechosas (00:00 - 05:00): " << mostSuspiciousIP << " con " << maxAttempts << " intentos." << endl;
}

int main() {
    string filename = "bitacora.txt";
    map<string, set<string>> ipToMessages;
    map<string, int> suspiciousAdminAccess;

    // Cargar datos del archivo y analizar intentos sospechosos
    loadLogFile(filename, ipToMessages, suspiciousAdminAccess);

    // Encontrar la IP con más conexiones a diferentes tipos de intentos
    findMostDiverseAttacker(ipToMessages);
    
    // Encontrar la IP con más intentos de acceso a 'admin' en horas sospechosas
    findSuspiciousAdminAttacker(suspiciousAdminAccess);
    
    return 0;
}
