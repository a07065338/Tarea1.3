/**
 * Corrrección de ordenamiento de ips en bitácora
 */


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
using namespace std;

// Estructura para almacenar un registro de bitácora
struct LogEntry {
    string date;
    string time;
    string ip;
    string message;
};

// Nodo para la lista doblemente enlazada
struct Node {
    LogEntry data;
    Node* next;
    Node* prev;
    Node(const LogEntry& log) : data(log), next(nullptr), prev(nullptr) {}
};

// Clase para la lista doblemente enlazada
class DoublyLinkedList {
private:
    Node* head;
    Node* tail;
    Node* mergeSort(Node* head);
    Node* merge(Node* left, Node* right);

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}
    ~DoublyLinkedList();

    void append(const LogEntry& log);
    void sortByIP();
    void printRange(const string& startIP, const string& endIP, ofstream& outFile);
    void printToFile(const string& filename);
};

DoublyLinkedList::~DoublyLinkedList() {
    Node* current = head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

void DoublyLinkedList::append(const LogEntry& log) {
    Node* newNode = new Node(log);
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

void DoublyLinkedList::printToFile(const string& filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Error al abrir el archivo de salida: " << filename << endl;
        return;
    }

    Node* current = head;
    while (current) {
        outFile << current->data.date << " " << current->data.time << " "
                << current->data.ip << " - " << current->data.message << endl;
        current = current->next;
    }
    outFile.close();
}

void DoublyLinkedList::printRange(const string& startIP, const string& endIP, ofstream& outFile) {
    Node* current = head;
    while (current) {
        if (current->data.ip >= startIP && current->data.ip <= endIP) {
            outFile << current->data.date << " " << current->data.time << " "
                    << current->data.ip << " - " << current->data.message << endl;
        }
        current = current->next;
    }
}

void parseIP(const string& ipStr, int& ip1, int& ip2, int& ip3, int& ip4, int& port) {
    sscanf(ipStr.c_str(), "%d.%d.%d.%d:%d", &ip1, &ip2, &ip3, &ip4, &port);
}

Node* DoublyLinkedList::merge(Node* left, Node* right) {
    if (!left) return right;
    if (!right) return left;

    int leftIP1, leftIP2, leftIP3, leftIP4, leftPort;
    int rightIP1, rightIP2, rightIP3, rightIP4, rightPort;

    parseIP(left->data.ip, leftIP1, leftIP2, leftIP3, leftIP4, leftPort);
    parseIP(right->data.ip, rightIP1, rightIP2, rightIP3, rightIP4, rightPort);

    if (tie(leftIP1, leftIP2, leftIP3, leftIP4, leftPort) <= tie(rightIP1, rightIP2, rightIP3, rightIP4, rightPort)) {
        left->next = merge(left->next, right);
        left->next->prev = left;
        left->prev = nullptr;
        return left;
    } else {
        right->next = merge(left, right->next);
        right->next->prev = right;
        right->prev = nullptr;
        return right;
    }
}

Node* DoublyLinkedList::mergeSort(Node* head) {
    if (!head || !head->next) return head;

    Node* slow = head;
    Node* fast = head->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    Node* mid = slow->next;
    slow->next = nullptr;
    if (mid) mid->prev = nullptr;

    Node* left = mergeSort(head);
    Node* right = mergeSort(mid);

    return merge(left, right);
}

void DoublyLinkedList::sortByIP() {
    head = mergeSort(head);
    tail = head;
    while (tail && tail->next) tail = tail->next;
}

void loadLogFile(const string& filename, DoublyLinkedList& list) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string month, day, time, ip, message;
        ss >> month >> day >> time >> ip;
        getline(ss, message);
        list.append({month + "-" + day, time, ip, message});
    }
    file.close();
}

int main() {
    DoublyLinkedList logs;
    string inputFile = "bitacora.txt";
    string outputFile = "sorted_by_ip.txt";
    string rangeOutputFile = "range_output.txt";

    loadLogFile(inputFile, logs);
    logs.sortByIP();
    logs.printToFile(outputFile);
    cout << "Registros ordenados por IP guardados en: " << outputFile << endl;

    // Solicitar rango de IPs al usuario
    string startIP, endIP;
    cout << "Ingrese la IP de inicio: ";
    cin >> startIP;
    cout << "Ingrese la IP de fin: ";
    cin >> endIP;

    // Guardar registros dentro del rango en un archivo
    ofstream rangeFile(rangeOutputFile);
    if (!rangeFile.is_open()) {
        cerr << "Error al abrir el archivo de salida para el rango." << endl;
        return 1;
    }
    logs.printRange(startIP, endIP, rangeFile);
    rangeFile.close();

    cout << "Registros en el rango guardados en: " << rangeOutputFile << endl;
    return 0;
}