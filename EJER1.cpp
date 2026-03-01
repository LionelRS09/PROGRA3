#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <unordered_set>
#include <random>

using namespace std;
using namespace chrono;

const int TOTAL_NUMBERS = 10000000;
const int SEARCHES = 1000;
const string FILENAME = "datos.txt";


// =========================
// ARBOL BINARIO DE BUSQUEDA
// =========================
struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int value) : data(value), left(nullptr), right(nullptr) {}
};

class BST {
private:
    Node* root;

    Node* insert(Node* node, int value) {
        if (node == nullptr)
            return new Node(value);

        if (value < node->data)
            node->left = insert(node->left, value);
        else
            node->right = insert(node->right, value);

        return node;
    }

    bool search(Node* node, int value) {
        if (node == nullptr)
            return false;

        if (node->data == value)
            return true;

        if (value < node->data)
            return search(node->left, value);
        else
            return search(node->right, value);
    }

public:
    BST() { root = nullptr; }

    void insert(int value) {
        root = insert(root, value);
    }

    bool search(int value) {
        return search(root, value);
    }
};


// =========================
// GENERAR ARCHIVO
// =========================
void generateFile() {
    ofstream file(FILENAME);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(-50000000, 50000000);

    for (int i = 0; i < TOTAL_NUMBERS; i++) {
        file << dis(gen) << "\n";
    }

    file.close();
}


// =========================
// MAIN
// =========================
int main() {

    cout << "Generando archivo...\n";
    generateFile();

    cout << "Cargando datos...\n";

    vector<int> data;
    ifstream file(FILENAME);
    int number;

    while (file >> number) {
        data.push_back(number);
    }
    file.close();

    // ======================
    // ARREGLO ORDENADO
    // ======================
    vector<int> sortedArray = data;
    sort(sortedArray.begin(), sortedArray.end());

    // ======================
    // BST
    // ======================
    BST tree;
    for (int val : data) {
        tree.insert(val);
    }

    // ======================
    // HASH TABLE
    // ======================
    unordered_set<int> hashTable;
    for (int val : data) {
        hashTable.insert(val);
    }

    // ======================
    // BUSQUEDAS ALEATORIAS
    // ======================
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(-50000000, 50000000);

    double timeArray = 0;
    double timeBST = 0;
    double timeHash = 0;

    for (int i = 0; i < SEARCHES; i++) {

        int target = dis(gen);

        // ---- Arreglo Ordenado (binary_search)
        auto start = high_resolution_clock::now();
        binary_search(sortedArray.begin(), sortedArray.end(), target);
        auto stop = high_resolution_clock::now();
        timeArray += duration<double, milli>(stop - start).count();

        // ---- BST
        start = high_resolution_clock::now();
        tree.search(target);
        stop = high_resolution_clock::now();
        timeBST += duration<double, milli>(stop - start).count();

        // ---- Hash
        start = high_resolution_clock::now();
        hashTable.find(target);
        stop = high_resolution_clock::now();
        timeHash += duration<double, milli>(stop - start).count();
    }

    // ======================
    // RESULTADOS
    // ======================
    cout << "\n===== TABLA COMPARATIVA =====\n\n";
    cout << "Estructura\t\tTiempo Promedio (ms)\tComplejidad\n";
    cout << "--------------------------------------------------------------\n";
    cout << "Arreglo Ordenado\t" << timeArray / SEARCHES << "\t\tO(log n)\n";
    cout << "Arbol Binario\t\t" << timeBST / SEARCHES << "\t\tO(log n)*\n";
    cout << "Tabla Hash\t\t" << timeHash / SEARCHES << "\t\tO(1)\n";

    cout << "\n*Nota: El BST puede degradarse a O(n) si se desbalancea.\n";

    return 0;
}
