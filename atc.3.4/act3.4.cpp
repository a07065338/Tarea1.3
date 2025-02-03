/*
 * Act 3.4 Evidencia BST 
* Programa para encontrar las 5 IPs con más accesos en un archivo de bitácora
 * Autores: 
 * José Leobardo Navarro Márquez - A01541324
 * Edgar Daniel Osorio Castaños - A07065338
 * Natalia Quiroga Colorado - a01722353
 * Fecha: 02/02/2025
*/

// Incluir bibliotecas necesarias
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <vector>
using namespace std;

// Función para extraer la IP sin el puerto
/** Complejidad: O(n), donde n es la longitud de la cadena.
 * 
 *
*/
string extractIP(const string& ipPort) {
    size_t pos = ipPort.find(":");
    return (pos != string::npos) ? ipPort.substr(0, pos) : ipPort;
}

/**
* Estructura para almacenar un nodo del árbol binario de búsqueda
* Complejidad: O(n), donde n es el número de IPs distintas.
* 
*/
struct BSTNode {
    int count;
    set<string> ips; // Puede haber varias IPs con la misma cantidad de accesos
    BSTNode* left;
    BSTNode* right;

    BSTNode(int c, const string& ip) : count(c), left(nullptr), right(nullptr) { 
        ips.insert(ip);
    }
};

// Clase BST para almacenar los accesos por IP
class BST {
private:
    BSTNode* root;
    
    BSTNode* insert(BSTNode* node, int count, const string& ip) {
        if (!node) return new BSTNode(count, ip);
        if (count < node->count) node->left = insert(node->left, count, ip);
        else if (count > node->count) node->right = insert(node->right, count, ip);
        else node->ips.insert(ip);
        return node;
    }
    
    void inorderTraversal(BSTNode* node, vector<pair<int, string>>& topIPs) {
        if (!node || topIPs.size() >= 5) return;
        inorderTraversal(node->right, topIPs);
        for (const string& ip : node->ips) {
            topIPs.push_back({node->count, ip});
            if (topIPs.size() == 5) return;
        }
        inorderTraversal(node->left, topIPs);
    }
    
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
    
    ~BST() {
        destroyTree(root);
    }
};

int main() {
    ifstream file("sorted_by_ip_modificado.txt");
    if (!file) {
        cerr << "Error al abrir el archivo" << endl;
        return 1;
    }
    
    map<string, int> ipCount;
    string line;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string date, time, ipPort, message;
        ss >> date >> time >> ipPort; // Se asume que los primeros tres elementos son estos
        getline(ss, message); // Ignoramos el mensaje
        
        string ip = extractIP(ipPort);
        ipCount[ip]++;
    }
    
    file.close();
    
    BST bst;
    for (const auto& entry : ipCount) {
        bst.insert(entry.second, entry.first);
    }
    
    vector<pair<int, string>> top5 = bst.getTop5IPs();
    
    cout << "Top 5 IPs con más accesos:" << endl;
    for (const auto& entry : top5) {
        cout << "IP: " << entry.second << " - Accesos: " << entry.first << endl;
    }
    
    return 0;
}

