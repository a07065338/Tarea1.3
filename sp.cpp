#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

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

// Función para obtener el número del mes desde su nombre abreviado
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

// Función para cargar los datos desde el archivo
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

        // Formatear la fecha como MM-DD
        if (day.back() == ',') day.pop_back(); // Eliminar cualquier coma
        string date = monthNumber + "-" + day;

        // Almacenar el registro en el vector
        logs.push_back({date, time, ip, message});
    }

    file.close();
}

// Función para escribir los registros ordenados en un archivo
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

void quickSort(vector<LogEntry>& logs, int low, int high) {
    if (low < high) {
        int pi = partition(logs, low, high);
        quickSort(logs, low, pi - 1);
        quickSort(logs, pi + 1, high);
    }
}




int main() {
    vector<LogEntry> logs;
    string inputFile = "bitacora.txt";
    string outputFile = "sorted_logs.txt";

    // Cargar datos del archivo
    loadLogFile(inputFile, logs);
    // manejo de excepción
    if (logs.empty()) {
        cout << "No se encontraron registros para procesar." << endl;
        return 1;
    }

    // Ordenar registros usando Quick Sort
    quickSort(logs, 0, logs.size() - 1);

    // Guardar registros ordenados en un archivo
    writeLogsToFile(outputFile, logs);
    /*

    // Solicitar fechas al usuario
    string startDate, endDate;
    cout << "Ingrese la fecha de inicio (MM-DD): ";
    cin >> startDate;
    cout << "Ingrese la fecha de fin (MM-DD): ";
    cin >> endDate;

*/


    return 0;
}
