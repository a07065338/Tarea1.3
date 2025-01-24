/*
 * Ordenar registros de bitácora usando una lista doblemente enlazada.
 * Autores:
 * José Leobardo Navarro Márquez - A01541324
 * Edgar Daniel Osorio Castaños - A07065338
 * Natalia Quiroga Colorado - A01722353
 * Fecha: 24/01/2025
*/

#include "doubly_linked_list.h"
#include <iostream>
#include <fstream>
using namespace std;

/*
 * Función principal del programa.
 * Carga un archivo de bitácora, ordena los registros por dirección IP, y permite buscar en un rango de IPs.
 */
int main() {
    DoublyLinkedList logs;
    string inputFile = "bitacora.txt";
    string outputFile = "sorted_by_ip.txt";

    // Cargar registros desde el archivo
    loadLogFile(inputFile, logs);

    // Ordenar registros por IP
    logs.sortByIP();

    // Guardar registros ordenados en un archivo
    logs.printToFile(outputFile);
    cout << "Registros ordenados por IP guardados en: " << outputFile << endl;

    // Solicitar rango de IPs al usuario
    string startIP, endIP;
    cout << "Ingrese la IP de inicio: ";
    cin >> startIP;
    cout << "Ingrese la IP de fin: ";
    cin >> endIP;

    // Crear archivo para el rango de búsqueda
    ofstream rangeFile("range_output.txt");
    if (!rangeFile.is_open()) {
        cerr << "Error al abrir el archivo de salida para el rango." << endl;
        return 1;
    }

    // Imprimir registros en el rango especificado
    logs.printRange(startIP, endIP, rangeFile);

    rangeFile.close();
    cout << "Registros en el rango guardados en: range_output.txt" << endl;

    return 0;
}
