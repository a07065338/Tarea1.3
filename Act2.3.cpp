#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// Estructura para almacenar un registro de bitácora
struct LogEntry {
    string date;
    string time;
    string ip;
    string message;
};

// Nodo de la lista doblemente ligada
struct Node {
    LogEntry log;
    Node* prev;
    Node* next;

    Node(const LogEntry& entry) : log(entry), prev(nullptr), next(nullptr) {}
};

// Clase para la lista doblemente ligada
class DoublyLinkedList {
private:
    Node* head;
    Node* tail;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    ~DoublyLinkedList() {
        Node* current = head;
        while (current) {
            Node* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }

    // Agregar un registro al final de la lista
    void add(const LogEntry& entry) {
        Node* newNode = new Node(entry);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    // Buscar registros por IP y mostrarlos
    void searchByIP(const string& ip) const {
        Node* current = head;
        bool found = false;

        while (current) {
            if (current->log.ip == ip) {
                found = true;
                cout << current->log.date << " " << current->log.time << " "
                     << current->log.ip << " - " << current->log.message << endl;
            }
            current = current->next;
        }

        if (!found) {
            cout << "No se encontraron registros para la IP: " << ip << endl;
        }
    }
};

// Función para cargar registros desde un archivo
void loadLogsFromFile(const string& filename, DoublyLinkedList& list) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string date, time, ip, message;

        // Leer los componentes de la línea
        ss >> date >> time >> ip;
        getline(ss, message); // Leer el mensaje restante

        // Crear un registro y agregarlo a la lista
        LogEntry entry = {date, time, ip, message};
        list.add(entry);
    }

    file.close();
}

int main() {
    DoublyLinkedList logList;

    // Cargar registros desde un archivo
    string filename = "bitacora.txt";
    loadLogsFromFile(filename, logList);

    // Solicitar una IP al usuario
    string ip;
    cout << "Ingrese la IP a buscar: ";
    cin >> ip;

    // Buscar y mostrar registros relacionados con la IP
    logList.searchByIP(ip);

    return 0;
}
