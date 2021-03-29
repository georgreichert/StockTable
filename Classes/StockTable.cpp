#include "../headers/StockTable.h"
#include <string>
#include <math.h>
#include <iostream>
#include <fstream>

StockTable::StockTable() {

}

StockTable::~StockTable() {
}

int StockTable::add(Stock* stock) {
    int result = this->stocks.add(stock, stock->getKey()); // result is index where it was added or error value
    if (result >= 0) { // Stock was added to stocks
        result = this->nameKeys.add(new int(result), stock->getName()); // result is index where it was added or error value
    } else {
        return result;
    }
    if (result >= 0) { // Stock was added to namekeys too
        return result;
    } else if (result == -1) { // Stock could not be added to Namekeys because name already exists
        this->stocks.remove(stock->getKey());
        return -3;
    } else if (result <= -2) { // Stock could not be added to Namekeys because hash value is overused
        this->stocks.remove(stock->getKey());
        return -4;
    }
}

bool StockTable::remove(std::string searchString) {
    Stock* toRemove = NULL;
    toRemove = search(searchString);
    if (toRemove != NULL) {
        this->nameKeys.remove(toRemove->getName());
        this->stocks.remove(toRemove->getKey());
        return true;
    }
    return false;
}

Stock* StockTable::search(std::string searchString) {
    Stock* r = NULL;
    r = this->stocks.search(searchString);
    if (r == NULL) { // only search namekeys table if search for symbol was unseccessful
        int* index = NULL;
        index = this->nameKeys.search(searchString);
        if (index != NULL) {
            r = this->stocks.get(*index);
        }
    }
    return r;
}


void StockTable::printTable() { // only left in for debugging purposes, can be activated by typing "print" or "PRINT"
    std::cout << "###### PRINT STOCKTABLE ######" << std::endl;
    std::cout << "########### STOCKS ###########" << std::endl;
    this->stocks.printTable();
    std::cout << "######### NAMEKEYS ###########" << std::endl;
    this->nameKeys.printTable();
    std::cout << "##############################" << std::endl;
}

bool StockTable::save(std::string path) {
    std::string fileContent;

    Stock* tmp = NULL;
    for (int i = 0; i < TABLE_SIZE; i++) { // iterate through the whole table and build string containing all serialized stocks
        tmp = this->stocks.get(i);         // seperated by '#'
        if (tmp != NULL) {
           fileContent += "#" + tmp->serialize();
        }
    }

    std::fstream fs;
    fs.open(path, std::fstream::out);
    if (!fs)
        return false;
    fs << fileContent;
    fs.close();
    return true;
}

bool StockTable::load(std::string path) {
    std::fstream fs;
    fs.open(path, std::fstream::in);

    if (!fs)
        return false;

    std::string line;
    std::string stockContent;
    Stock* tmp = NULL;
    while(std::getline(fs, line)) {
        if (line[0] == '#') { // when next stock is reached in file
            if (tmp != NULL) {//and it is not the first iteration
               tmp->deserialize(stockContent); // deserialize the data
            }
            stockContent.clear(); // erase already deserialized data
            std::string delimiter = ";";
            std::string key = line.substr(1, line.find(delimiter) - 1);  // read symbol
            line.erase(0, line.find(delimiter) + delimiter.length());
            std::string name = line.substr(0, line.find(delimiter)); // read name
            line.erase(0, line.find(delimiter) + delimiter.length());
            std::string wkn = line.substr(0, line.find(delimiter)); // read WKN

            tmp = new Stock(key, name, wkn); // generate new stock from read input
            add(tmp); // add new stock to the table
        } else { // if the next stock is not reached yet, add current line to current stocks data
            stockContent += line;
        }
    }
    if (tmp != NULL) { // necessary so last stock will be deserialized too
           tmp->deserialize(stockContent);
    }
    return true;
}

