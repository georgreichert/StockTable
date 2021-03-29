#ifndef STOCK_H
#define STOCK_H

#include <string>

class Stock {
    private:
        std::string key;
        std::string name;
        std::string wkn;
        std::string stockData[30][7];
        int stocksHashtableIndex;
        int nameKeyHashtableIndex;

    public:
        Stock(std::string key, std::string name, std::string wkn);
        bool import(std::string path);
        bool plot();
        std::string getKey();
        std::string getName();
        std::string getWKN();
        std::string serialize();
        void deserialize(std::string content);
        int getRowCount();
        std::string getLatestRow();
};

#endif // STOCK_H
