/*
 * Act 1.3 Evidencia Conceptos Básicos y Algoritmos Fundamentales
 Programa para ordenar registros de bitácora por fecha
 * Autores: 
 * José Leobardo Navarro Márquez - A01541324
 * Edgar Daniel Osorio Castaños - A07065338
 * Natalia Quiroga Colorado - a01722353
 * Fecha: 18/01/2025
*/

// Incluir bibliotecas necesarias
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Estructura para almacenar un registro de bitácora
struct LogEntry {
    string date;  
    string time;
    string ip;
    string message;

    // Comparación para ordenamiento
    bool operator<(const LogEntry& other) const {
        return date + time < other.date + other.time;
    }
};


/*
* Función para obtener el número del mes desde su nombre abreviado
* Complejidad: O(1).
* Parametros:
* month Nombre abreviado del mes
* Return: 
*  Número del mes en formato de dos dígitos
*/
string getMonthNumber(const string& month) {
    unordered_map<string, string> monthMap = {
        {"Jan", "01"}, {"Feb", "02"}, {"Mar", "03"}, {"Apr", "04"},
        {"May", "05"}, {"Jun", "06"}, {"Jul", "07"}, {"Aug", "08"},
        {"Sep", "09"}, {"Oct", "10"}, {"Nov", "11"}, {"Dec", "12"}
    };

    auto it = monthMap.find(month);
    if (it != monthMap.end()) {
        return it->second;
    }
    return "00"; // En caso de que el mes no sea válido
}


/*
* Función para cargar los datos desde el archivo
* Complejidad: O(n), donde n es la cantidad de líneas en el archivo.
* Parametros:
* filename Nombre del archivo a cargar
* logs Vector donde se almacenarán los registros
*/
void loadLogFile(const string& filename, vector<LogEntry>& logs) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string month, day, time, ip, message;

        // Leer componentes de la línea
        ss >> month >> day >> time >> ip;
        getline(ss, message); // Leer el mensaje restante

        // Convertir mes a número
        string monthNumber = getMonthNumber(month);

        // Formatear el día a dos dígitos
        if (day.back() == ',') day.pop_back(); // Eliminar cualquier coma
        if (stoi(day) < 10) { // Si el día es menor a 10, agregar un 0
            day = "0" + day;
        }

        // Formatear la fecha como MM-DD
        string date = monthNumber + "-" + day;

        // Almacenar el registro en el vector
        logs.push_back({date, time, ip, message});
    }

    file.close();
}


/*
* Función para escribir los registros ordenados en un archivo
* Complejidad: O(n), donde n es la cantidad de registros.
* Parametros:
* outputFile Nombre del archivo de salida
* logs Vector con los registros a escribir
*/
void writeLogsToFile(const string& outputFile, const vector<LogEntry>& logs) {
    ofstream file(outputFile);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo de salida: " << outputFile << endl;
        return;
    }

    for (const auto& log : logs) {
        file << log.date << " " << log.time << " " << log.ip << " - " << log.message << endl;
    }

    file.close();
    cout << "Registros ordenados guardados en el archivo: " << outputFile << endl;
}


// Implementación de Quick Sort
/*
* Función para particionar el vector de registros
* Complejidad: O(n), donde n es la cantidad de registros.
* Parametros:
* logs Vector con los registros a ordenar
* low Índice del primer elemento
* high Índice del último elemento
*/
int partition(vector<LogEntry>& logs, int low, int high) {
    LogEntry pivot = logs[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (logs[j] < pivot) {
            ++i;
            swap(logs[i], logs[j]);
        }
    }
    swap(logs[i + 1], logs[high]);
    return i + 1;
}


/*
* Función para ordenar los registros usando Quick Sort
* Complejidad: O(n log n) en promedio, O(n^2) en el peor caso.
* Parametros:
* logs Vector con los registros a ordenar
* low Índice del primer elemento
* high Índice del último elemento
*/
void quickSort(vector<LogEntry>& logs, int low, int high) {
    if (low < high) {
        int pi = partition(logs, low, high);
        quickSort(logs, low, pi - 1);
        quickSort(logs, pi + 1, high);
    }
}


// Función para realizar búsqueda binaria en el rango de fechas
/*
* Busca registros dentro de un rango de fechas
* Complejidad: O(log n) para cada búsqueda.
* Parametros:
* logs Vector con los registros ordenados
* startDate Fecha de inicio del rango
* endDate Fecha de fin del rango
* Return: Par de índices que delimitan el rango de fechas
*/
pair<int, int> binarySearch(const vector<LogEntry>& logs, const string& startDate, const string& endDate) {
    auto startIt = lower_bound(logs.begin(), logs.end(), LogEntry{startDate, "", "", ""});
    auto endIt = upper_bound(logs.begin(), logs.end(), LogEntry{endDate, "23:59:59", "", ""});
    
    if (endIt == logs.end() || endIt->date < startDate) {
        return {-1, -1}; // No hay registros en el rango
    }
    
    if (startIt == logs.end() || startIt->date > startDate) {
    int endIndex = (endIt == logs.end()) ? logs.size() - 1 : distance(logs.begin(), endIt) - 1;
    return {distance(logs.begin(), startIt), endIndex};
    }
    
    return {distance(logs.begin(), startIt), distance(logs.begin(), endIt) - 1};
}

// Función principal de la aplicación
int main() {
    // Variables para almacenar registros
    vector<LogEntry> logs;
    string inputFile = "bitacora.txt";
    string outputFile = "sorted_logs.txt";

    // Cargar datos del archivo
    loadLogFile(inputFile, logs);
    // Manejo de excepción
    if (logs.empty()) {
        cout << "No se encontraron registros para procesar." << endl;
        return 1;
    }

    // Ordenar registros usando Quick Sort
    quickSort(logs, 0, logs.size() - 1);

    // Guardar registros ordenados en un archivo
    writeLogsToFile(outputFile, logs);

    // Solicitar fechas al usuario
    string startDate, endDate;
    cout << "Ingrese la fecha de inicio (MM-DD): ";
    cin >> startDate;
    cout << "Ingrese la fecha de fin (MM-DD): ";
    cin >> endDate;

    // Buscar registros dentro del rango de fechas usando búsqueda binaria
    auto range = binarySearch(logs, startDate, endDate);

    if (range.first == -1) {
        cout << "No se encontraron registros en el rango de fechas especificado." << endl;
    } else {
        cout << "Registros encontrados en el rango de fechas:" << endl;
        for (int i = range.first; i <= range.second; ++i) {
            cout << logs[i].date << " " << logs[i].time << " " << logs[i].ip << " - " << logs[i].message << endl;
        }
    }

    return 0;
}

/* Complejidades de los algoritmos utilizados:
 * - Carga del archivo (`loadLogFile`): O(n), donde n es la cantidad de líneas en el archivo.
 * - Quick Sort (`quickSort`): O(n log n) en promedio, O(n^2) en el peor caso.
 * - Búsqueda binaria (`binarySearchRange`): O(log n) para cada búsqueda.
 * - Escritura en el archivo (`writeLogsToFile`): O(n).
 * Complejidad total aproximada del programa: O(n log n).
 */
