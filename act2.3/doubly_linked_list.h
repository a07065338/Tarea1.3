// Header para la lista doblemente enlazada
#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <string>
using namespace std;

struct LogEntry {
    string date;
    string time;
    string ip;
    string message;
};

struct Node {
    LogEntry data;
    Node* next;
    Node* prev;

    Node(const LogEntry& log);
};

class DoublyLinkedList {
private:
    Node* head;
    Node* tail;
    Node* mergeSort(Node* head);
    Node* merge(Node* left, Node* right);

public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    void append(const LogEntry& log);
    void sortByIP();
    void printRange(const string& startIP, const string& endIP, ofstream& outFile);
    void printToFile(const string& filename);
};

void loadLogFile(const string& filename, DoublyLinkedList& list);

#endif
