// archivo de implementación de la lista doblemente enlazada
#include "doubly_linked_list.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
using namespace std;

/*
 * Constructor del nodo.
 * @param log Registro de bitácora a almacenar.
 */
Node::Node(const LogEntry& log) : data(log), next(nullptr), prev(nullptr) {}

/*
 * Constructor de la lista doblemente enlazada.
 */
DoublyLinkedList::DoublyLinkedList() : head(nullptr), tail(nullptr) {}

/**
 * Destructor de la lista doblemente enlazada.
 * Libera la memoria ocupada por los nodos de la lista.
 */
DoublyLinkedList::~DoublyLinkedList() {
    Node* current = head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

/*
 * Agrega un nuevo registro al final de la lista.
 * Complejidad: O(1).
 * @param log Registro de bitácora a agregar.
 */
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

/*
 * Fusiona dos sublistas ordenadas.
 * Complejidad: O(n).
 * @param left Puntero al primer nodo de la sublista izquierda.
 * @param right Puntero al primer nodo de la sublista derecha.
 * @return Puntero al primer nodo de la lista fusionada.
 */
Node* DoublyLinkedList::merge(Node* left, Node* right) {
    if (!left) return right;
    if (!right) return left;

    if (left->data.ip <= right->data.ip) {
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

/*
 * Ordena una sublista utilizando merge sort.
 * Complejidad: O(n log n).
 * @param head Puntero al primer nodo de la sublista.
 * @return Puntero al primer nodo de la lista ordenada.
 */
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

/**
 * Ordena la lista por dirección IP.
 * Complejidad: O(n log n).
 */
void DoublyLinkedList::sortByIP() {
    head = mergeSort(head);
    tail = head;
    while (tail && tail->next) tail = tail->next;
}

/*
 * Imprime los registros que están en un rango de IPs especificado.
 * Complejidad: O(n).
 * @param startIP IP inicial del rango.
 * @param endIP IP final del rango.
 * @param outFile Archivo de salida donde se guardarán los registros.
 */
void DoublyLinkedList::printRange(const string& startIP, const string& endIP, ofstream& outFile) {
    Node* current = head;
    bool found = false;

    while (current) {
        if (current->data.ip >= startIP && current->data.ip <= endIP) {
            outFile << current->data.date << " " << current->data.time << " "
                    << current->data.ip << " - " << current->data.message << endl;
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        cout << "No se encontraron registros en el rango especificado." << endl;
    }
}

/*
 * Imprime todos los registros en un archivo de salida.
 * Complejidad: O(n).
 * @param filename Nombre del archivo de salida.
 */
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

/*
 * Carga los registros de la bitácora desde un archivo.
 * Complejidad: O(n), donde n es el número de líneas en el archivo.
 * @param filename Nombre del archivo de entrada.
 * @param list Lista doblemente enlazada donde se almacenarán los registros.
 */
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

        // Leer componentes de la línea
        ss >> month >> day >> time >> ip;
        getline(ss, message);

        // Convertir mes a número
        string monthNumber;
        unordered_map<string, string> monthMap = {
            {"Jan", "01"}, {"Feb", "02"}, {"Mar", "03"}, {"Apr", "04"},
            {"May", "05"}, {"Jun", "06"}, {"Jul", "07"}, {"Aug", "08"},
            {"Sep", "09"}, {"Oct", "10"}, {"Nov", "11"}, {"Dec", "12"}};
        monthNumber = monthMap[month];

        if (day.back() == ',') day.pop_back();
        if (stoi(day) < 10) day = "0" + day;

        string date = monthNumber + "-" + day;

        // Agregar registro a la lista
        list.append({date, time, ip, message});
    }

    file.close();
}
