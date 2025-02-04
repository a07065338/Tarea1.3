/*
 * Act 4.3 Grafos: Evidencia de Conceptos Básicos y Algoritmos Fundamentales
*Descripción: Programa para analizar un archivo de bitácora y detectar posibles ataques cibernéticos.
*Se identifica el puerto más atacado en horas sospechosas (00:00 - 05:00) y se determina un posible bot master.
 * Autores: 
 * José Leobardo Navarro Márquez - A01541324
 * Edgar Daniel Osorio Castaños - A07065338
 * Natalia Quiroga Colorado - a01722353
 * Fecha: 03/02/2025
*/

// Librerías necesarias para el programa
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

// Estructura para almacenar un intento de acceso de la bitácora
struct LogEntry {
    string date;
    string time;
    string ip;
    int port;
    string message;
};

/*
    Función: parseIP
    Descripción: Extrae la dirección IP y el número de puerto desde una cadena con formato "IP:Puerto".
    Parámetros:
        - ipStr (string): Dirección IP con puerto adjunto.
        - ip1, ip2, ip3, ip4 (int&): Variables para almacenar los segmentos de la IP.
        - port (int&): Variable para almacenar el puerto extraído.
    Retorno:
        - Ninguno.
*/
void parseIP(const string& ipStr, int& ip1, int& ip2, int& ip3, int& ip4, int& port) {
    sscanf(ipStr.c_str(), "%d.%d.%d.%d:%d", &ip1, &ip2, &ip3, &ip4, &port);
}

/*
    Función: extractHour
    Descripción: Extrae la hora de un formato "hh:mm:ss".
    Parámetros:
        - time (string): Hora en formato de cadena "hh:mm:ss".
    Retorno:
        - (int): La hora en formato entero.
*/
int extractHour(const string& time) {
    stringstream ss(time);
    int hour, minute, second;
    char separator;
    ss >> hour >> separator >> minute >> separator >> second;
    return hour;
}

/*
    Función: loadLogFile
    Descripción: Carga el archivo de bitácora y almacena los intentos en una lista de adyacencia.
    Parámetros:
        - filename (string): Nombre del archivo de bitácora.
        - logs (vector<LogEntry>&): Vector donde se almacenarán los registros.
        - portAdjacencyList (map<int, set<string>>&): Lista de adyacencia de puertos atacados.
    Retorno:
        - Ninguno.
*/
void loadLogFile(const string& filename, vector<LogEntry>& logs, map<int, set<string>>& portAdjacencyList) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error al abrir el archivo " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string month, day, time, ipPort, message;

        ss >> month >> day >> time >> ipPort;
        getline(ss, message);

        int ip1, ip2, ip3, ip4, port;
        parseIP(ipPort, ip1, ip2, ip3, ip4, port);
        int hour = extractHour(time);
        
        // Si el intento ocurrió en un horario sospechoso (00:00 - 05:00), registrarlo
        if (hour >= 0 && hour < 5) {
            portAdjacencyList[port].insert(ipPort);
            logs.push_back({month + "-" + day, time, ipPort, port, message});
        }
    }
    file.close();
}

/*
    Función: findMostAttackedPortAndBotMaster
    Descripción: Encuentra el puerto más atacado y determina un posible bot master.
    Parámetros:
        - logs (const vector<LogEntry>&): Vector con los registros.
        - portAdjacencyList (const map<int, set<string>>&): Lista de adyacencia con los puertos atacados.
    Retorno:
        - Ninguno.
*/
void findMostAttackedPortAndBotMaster(const vector<LogEntry>& logs, const map<int, set<string>>& portAdjacencyList) {
    int mostAttackedPort = -1;
    int maxFanOut = 0;
    
    for (const auto& entry : portAdjacencyList) {
        if (entry.second.size() > maxFanOut) {
            maxFanOut = entry.second.size();
            mostAttackedPort = entry.first;
        }
    }
    
    cout << "\nPuerto más atacado en horas sospechosas: " << mostAttackedPort << " con " << maxFanOut << " IPs atacantes distintas." << endl;
    cout << "\nRegistros asociados a este puerto:" << endl;
    
    string possibleBotMaster;
    for (const auto& log : logs) {
        if (log.port == mostAttackedPort) {
            cout << log.date << " " << log.time << " " << log.ip << " - " << log.message << endl;
            if (log.message.find("admin") != string::npos) {
                possibleBotMaster = log.ip;
            }
        }
    }
    
    if (!possibleBotMaster.empty()) {
        cout << "\nPosible Bot Master: " << possibleBotMaster << " intentó acceder como admin." << endl;
    } else {
        cout << "\nNo se encontró un intento de acceso a 'admin'." << endl;
    }
}

/*
    Función: main
    Descripción: Función principal que ejecuta el programa.
    Parámetros:
        - Ninguno.
    Retorno:
        - (int): Código de salida del programa (0 si ejecuta correctamente).
*/
int main() {
    string filename = "bitacora.txt";
    vector<LogEntry> logs;
    map<int, set<string>> portAdjacencyList;

    // Cargar datos del archivo y analizar intentos sospechosos
    loadLogFile(filename, logs, portAdjacencyList);

    // Encontrar el puerto más atacado y un posible bot master
    findMostAttackedPortAndBotMaster(logs, portAdjacencyList);
    
    return 0;
}
