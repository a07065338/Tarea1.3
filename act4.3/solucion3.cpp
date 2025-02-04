/*
 * Act 4.3 Grafos: Evidencia de Conceptos Básicos y Algoritmos Fundamentales
 * Descripción: Programa para analizar un archivo de bitácora y detectar posibles ataques cibernéticos.
 * Se identifica el puerto más atacado en horas sospechosas (00:00 - 05:00) y se determina un posible bot master.
 * Autores: 
 * José Leobardo Navarro Márquez - A01541324
 * Edgar Daniel Osorio Castaños - A07065338
 * Natalia Quiroga Colorado - a01722353
 * Fecha: 03/02/2025
 */

//Importación de librerías necesarias
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

//Estructura para almacenar un registro de la bitácora
struct LogEntry {
    string date;
    string time;
    string ip;
    int port;
    string message;
};

/**
 * Función para extraer los componentes de una dirección IP y puerto.
 * Parametros: ipStr - Dirección IP y puerto en formato "xxx.xxx.xxx.xxx:xxxx".
 * salida: ip1, ip2, ip3, ip4 - Componentes de la dirección IP.
 */
void parseIP(const string& ipStr, int& ip1, int& ip2, int& ip3, int& ip4, int& port) {
    sscanf(ipStr.c_str(), "%d.%d.%d.%d:%d", &ip1, &ip2, &ip3, &ip4, &port);
}

/**
 * Función para extraer la hora de un registro de la bitácora.
 * Parametros: time - Hora en formato "hh:mm:ss".
 * Salida: Hora en formato entero.
 */
int extractHour(const string& time) {
    stringstream ss(time);
    int hour, minute, second;
    char separator;
    ss >> hour >> separator >> minute >> separator >> second;
    return hour;
}

/**
 * Función para cargar el archivo de bitácora y construir la lista de registros y la lista de adyacencia basada en puertos atacados en horas sospechosas.
 * Parametros: filename - Nombre del archivo de bitácora.
 *             logs - Vector para almacenar los registros de la bitácora.
 *             portAdjacencyList - Mapa para almacenar la lista de adyacencia basada en puertos atacados en horas sospechosas.
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
        
        if (hour >= 0 && hour < 5) {
            portAdjacencyList[port].insert(ipPort);
            logs.push_back({month + "-" + day, time, ipPort, port, message});
        }
    }
    file.close();
}

/**
 * Merge de dos subarreglos ordenados.
 */
void merge(vector<LogEntry>& logs, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    vector<LogEntry> leftArr(n1);
    vector<LogEntry> rightArr(n2);
    
    for (int i = 0; i < n1; i++)
        leftArr[i] = logs[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = logs[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i].port <= rightArr[j].port) {
            logs[k] = leftArr[i];
            i++;
        } else {
            logs[k] = rightArr[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        logs[k] = leftArr[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        logs[k] = rightArr[j];
        j++;
        k++;
    }
}

/**
 * Función para ordenar el vector de registros de la bitácora.
 * Parametros: logs - Vector de registros de la bitácora.
 *             left - Índice del primer elemento del subarreglo.
 *             right - Índice del último elemento del subarreglo.
 */
void mergeSort(vector<LogEntry>& logs, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(logs, left, mid);
        mergeSort(logs, mid + 1, right);
        merge(logs, left, mid, right);
    }
}

/**
 * Función para encontrar el puerto más atacado en horas sospechosas y determinar un posible Bot Master.
 * Parametros: logs - Vector de registros de la bitácora.
 *             portAdjacencyList - Mapa de la lista de adyacencia basada en puertos atacados en horas sospechosas.
 */
void findMostAttackedPortAndBotMaster(vector<LogEntry>& logs, const map<int, set<string>>& portAdjacencyList) {
    mergeSort(logs, 0, logs.size() - 1);
    
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

/**
 * Función principal.
 */
int main() {
    string filename = "bitacora.txt";
    vector<LogEntry> logs;
    map<int, set<string>> portAdjacencyList;

    loadLogFile(filename, logs, portAdjacencyList);
    findMostAttackedPortAndBotMaster(logs, portAdjacencyList);
    
    return 0;
}

//Complejidad total del programa: O(n log n) + O(n) = O(n log n) donde n es el número de registros en la bitácora.

