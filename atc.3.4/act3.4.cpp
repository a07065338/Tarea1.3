/*
 * Act 3.4 Evidencia BST 
 * Programa para encontrar las 5 IPs con más accesos en un archivo de bitácora.
 * recibe un archivo de bitacora modificado ya que el original de la SP no cuenta con ips repetidas
 * Autores: 
 * José Leobardo Navarro Márquez - A01541324
 * Edgar Daniel Osorio Castaños - A07065338
 * Natalia Quiroga Colorado - A01722353
 * Fecha: 02/02/2025
 */

// Inclusión de bibliotecas necesarias
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <cstdio> // Para sscanf

using namespace std;

/**
 * Extrae los cuatro segmentos de la IP y el puerto de una cadena con formato "IP:PUERTO".
 * 
 
 * 
 * @complejidad O(1), ya que la operación de extracción siempre se realiza en tiempo constante.
 */
void parseIP(const string& ipStr, int& ip1, int& ip2, int& ip3, int& ip4, int& port) {
    sscanf(ipStr.c_str(), "%d.%d.%d.%d:%d", &ip1, &ip2, &ip3, &ip4, &port);
}

/**
 * Estructura para representar un nodo del Árbol Binario de Búsqueda (BST).
 * 
 * Almacena la cantidad de accesos (count) y un conjunto de IPs con esa cantidad de accesos.
 */
struct BSTNode {
    int count;          ///< Cantidad de accesos
    set<string> ips;    ///< Conjunto de IPs con la misma cantidad de accesos
    BSTNode* left;      ///< Puntero al hijo izquierdo
    BSTNode* right;     ///< Puntero al hijo derecho

    /**
     * Constructor de la estructura BSTNode.
     * 
     * @param c Número de accesos asociados a la IP.
     * @param ip Dirección IP que tuvo esos accesos.
     */
    BSTNode(int c, const string& ip) : count(c), left(nullptr), right(nullptr) { 
        ips.insert(ip);
    }
};

/**
 * Clase BST (Árbol Binario de Búsqueda) para almacenar los accesos por IP.
 */
class BST {
private:
    BSTNode* root; ///< Raíz del árbol

    /**
     * Inserta un nodo en el árbol.
     * Complejidad: O(log N) en promedio, donde N es la cantidad de nodos en el árbol.
     * @param node Nodo actual en el recorrido.
     * @param count Cantidad de accesos.
     * @param ip Dirección IP.
     * @return Puntero al nodo insertado.
     */
    BSTNode* insert(BSTNode* node, int count, const string& ip) {
        if (!node) return new BSTNode(count, ip);
        if (count < node->count) node->left = insert(node->left, count, ip);
        else if (count > node->count) node->right = insert(node->right, count, ip);
        else node->ips.insert(ip);
        return node;
    }
    /**
     * Realiza un recorrido en orden del árbol para obtener las 5 IPs con más accesos.
     */
    void inorderTraversal(BSTNode* node, vector<pair<int, string>>& topIPs) {
        if (!node || topIPs.size() >= 5) return;
        inorderTraversal(node->right, topIPs);
        for (const string& ip : node->ips) {
            topIPs.push_back({node->count, ip});
            if (topIPs.size() == 5) return;
        }
        inorderTraversal(node->left, topIPs);
    }
    // Función para destruir el árbol y liberar memoria.
    void destroyTree(BSTNode* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}

    void insert(int count, const string& ip) {
        root = insert(root, count, ip);
    }

    vector<pair<int, string>> getTop5IPs() {
        vector<pair<int, string>> topIPs;
        inorderTraversal(root, topIPs);
        return topIPs;
    }
/**
 * Destructor de la clase BST.
 */
    ~BST() {
        destroyTree(root);
    }
};

/**
 * Función principal del programa.
 * 
 * Lee un archivo de bitácora, cuenta los accesos por IP, los almacena en un BST y muestra las 5 IPs más frecuentes.
 * 
 * @return int Código de salida del programa (0 = éxito, 1 = error).
 */
int main() {
    // Abrir el archivo de entrada
    ifstream file("sorted_by_ip_modificado.txt");
    if (!file) {
        cerr << "Error al abrir el archivo" << endl;
        return 1;
    }

    map<string, int> ipCount; ///< Mapa para contar accesos por IP
    string line;

    // Leer línea por línea el archivo
    while (getline(file, line)) {
        stringstream ss(line);
        string date, time, ipPort, message;
        ss >> date >> time >> ipPort; // Se extraen los primeros tres elementos
        getline(ss, message); // Se ignora el mensaje de la bitácora

        // Extraer la IP sin el puerto
        int ip1, ip2, ip3, ip4, port;
        parseIP(ipPort, ip1, ip2, ip3, ip4, port);

        // Convertir la IP a una cadena estandarizada
        string ip = to_string(ip1) + "." + to_string(ip2) + "." + to_string(ip3) + "." + to_string(ip4);
        
        ipCount[ip]++; // Incrementar el contador de accesos para la IP
    }

    file.close(); // Cerrar el archivo

    // Insertar las IPs en el árbol BST
    BST bst;
    for (const auto& entry : ipCount) {
        bst.insert(entry.second, entry.first);
    }

    // Obtener las 5 IPs con más accesos
    vector<pair<int, string>> top5 = bst.getTop5IPs();

    // Mostrar el resultado
    cout << "Top 5 IPs con más accesos:" << endl;
    for (const auto& entry : top5) {
        cout << "IP: " << entry.second << " - Accesos: " << entry.first << endl;
    }

    return 0; // Indicar ejecución exitosa
}
