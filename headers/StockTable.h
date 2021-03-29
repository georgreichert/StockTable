#ifndef STOCKTABLE_H
#define STOCKTABLE_H

#include <string>
#include "Stock.h"
#include "HashTable.h"

class StockTable {
    private:
        HashTable<int> nameKeys; // maps names to indexes of the other table
        HashTable<Stock> stocks; // maps symbols to Stock objects

    public:
        StockTable();
        ~StockTable();
        int add(Stock* stock);
        bool remove(std::string searchString);
        Stock* search(std::string searchString);
        bool save(std::string path);
        bool load(std::string path);
        void printTable(); // only left in for debugging purposes, can be activated by typing "print" or "PRINT"
};

#endif // STOCKTABLE_H
