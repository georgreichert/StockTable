#include "../headers/HashTable.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include "../headers/Stock.h"

template <class T>
HashTable<T>::HashTable() {
    for (int i = 0; i < this->size; i++) { // Initialize all value pointers as NULL and all deleted flags as false
        this->values[i] = NULL;
        this->deleted[i] = false;
    }
}

template <class T>
HashTable<T>::~HashTable() {
    for (int i = 0; i < this->size; i++) {
        if (this->values[i] != NULL) {
            delete this->values[i];
        }
    }
}

template <class T>
int HashTable<T>::hashFunction(std::string input) {
    unsigned int key = 0;
    int n = input.length();

    for (int i = 0; i < n; i++) {
        key += input[i] * integerPow(31, n - (i + 1));
    }

    return key % this->size;
}

template <class T>
int HashTable<T>::integerPow(int base, int power) { // own integer pow() function because working with standard pow() function
    int r = 1;                                      // increases likelihood of errors due to double return value
    for (int i = 0; i < power; i++) {
        r *= base;
    }
    return r;
}

template <class T>
int HashTable<T>::add(T* t, std::string key){
    unsigned int index = hashFunction(key);
    int start = index;
    for (int i = 1; !this->keys[index].empty(); i += 2) {   // quadratic probing indexes are computed by always adding the next odd number to the
        if (this->keys[index].compare(key) == 0) {          // previous quadratic number, 1 + 3 = 4, 4 + 5 = 9, 9 + 7 = 16, ...
            return -1;  // Stock could not be added to Namekeys because name already exists
        }
        index += i;
        index %= this->size;
        if(index == start) { // if loop reaches its starting point again (after max. size/2 iterations), break
            return -2; // Stock could not be added to Namekeys because hash value is overused
        }
    }
    this->values[index] = t; // adding stock
    this->keys[index] = key;
    this->deleted[index] = false;
    return index;
}

template <class T>
void HashTable<T>::remove(std::string key){
    unsigned int index = hashFunction(key);
    int start = index;
    for (int i = 1; !this->keys[index].empty() || this->deleted[index]; i += 2) { // quadratic probing indexes are computed by always adding the next odd number to the
        if (!this->deleted[index] && this->keys[index].compare(key) == 0) {       // previous quadratic number, 1 + 3 = 4, 4 + 5 = 9, 9 + 7 = 16, ...
            delete this->values[index];                                           // deleted[index] keeps loop searching if a value was deleted and left a hole in the probing chain
            this->values[index] = NULL;
            this->keys[index].clear();
            if (!this->keys[(index + i) % this->size].empty() || this->deleted[(index + i) % this->size]) { // if next probing index after the deleted one is taken
                this->deleted[index] = true;                                                                // deleted flag is set
            }
            return;
        }
        index += i;
        index %= this->size;
        if(index == start) {
            break;
        }
    }
}

template <class T>
T* HashTable<T>::search(std::string key) {
    unsigned int index = hashFunction(key);
    int start = index;
    for (int i = 1; !this->keys[index].empty() || this->deleted[index]; i += 2) { // quadratic probing indexes are computed by always adding the next odd number to the
        if (!this->deleted[index] && this->keys[index].compare(key) == 0) {       // previous quadratic number, 1 + 3 = 4, 4 + 5 = 9, 9 + 7 = 16, ...
            return this->values[index];                                           // deleted[index] keeps loop searching if a value was deleted and left a hole in the probing chain
        }
        index += i;
        index %= this->size;
        if(index == start) {
            break;
        }
    }
    return NULL;
}

template <class T>
void HashTable<T>::printTable() { // only left in for debugging purposes, can be activated by typing "print" or "PRINT"
    for (int i = 0; i < this->size; i++) {
        std::cout << "[" << i << "] " << this->keys[i] << "|" << this->values[i] << "|" << this->deleted[i] << std::endl;
    }
}

template <class T>
T* HashTable<T>::get(int index) {
    return this->values[index];
}

template class HashTable<int>;
template class HashTable<Stock>;
