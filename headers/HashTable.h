#ifndef HASHTABLE_H
#define HASHTABLE_H

#define TABLE_SIZE      2003

#include <string>

template <class T>
class HashTable {
    private:
        static const int size = TABLE_SIZE; // 1009 bzw 2003
        int hashFunction(std::string input);
        int integerPow(int base, int power);
        T* values[size];
        std::string keys[size];
        bool deleted[size]; // deleted flags for performant deletion

    public:
        HashTable();
        ~HashTable();
        T* search(std::string key);
        int add(T* t, std::string key);
        void remove(std::string key);
        void printTable(); // for debugging
        T* get(int index);
};

#endif // HASHTABLE_H
