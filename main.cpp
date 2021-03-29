#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <math.h>
#include <fstream>
#include "headers/Stock.h"
#include "headers/HashTable.h"
#include "headers/StockTable.h"

void helpSection();
Stock* add(std::string command);
bool import(std::string command, Stock* stock);
bool del(std::string command, Stock* stock, StockTable* stocks);
Stock* search(std::string command, StockTable* stocks);
bool save(std::string command, StockTable* stocks);
bool load(std::string command, StockTable* stocks);

int main() {
    StockTable* stocks = new StockTable();

    Stock* currentStock = NULL;
    std::string command = "HELP";
    std::string firstCommand;
    while(true) {
        firstCommand = command.substr(0, command.find(" "));
        // ################# HELP ##################
        if (command.compare("HELP") == 0 || command.compare("help") == 0) {
            helpSection();
        }
        // ################# PRINT ###################
        else if (firstCommand.compare("PRINT") == 0 || firstCommand.compare("print") == 0) {
            stocks->printTable();
        }
        // ################# ADD ###################
        else if (firstCommand.compare("ADD") == 0 || firstCommand.compare("add") == 0) {
            Stock* temp = add(command);
            if(temp != NULL) {
                switch (stocks->add(temp)) {
                    case -1:
                        delete temp;
                        std::cout << "Error: Key already exists!" << std::endl;
                        break;
                    case -2:
                        delete temp;
                        std::cout << "Error: Can't add stock - hash value is overused!" << std::endl;
                        break;
                    case -3:
                        delete temp;
                        std::cout << "Error: Name already exists!" << std::endl;
                        break;
                    case -4:
                        delete temp;
                        std::cout << "Error: Can't add stock name - hash value is overused!" << std::endl;
                        break;
                    default:
                        std::cout << "Stock added!" << std::endl;
                        currentStock = temp;
                        break;
                }
            } else {
                std::cout << "Error interpreting ADD command. Enter HELP for further information." << std::endl;
            }
        }
        // ################# IMPORT ###################
        else if (firstCommand.compare("IMPORT") == 0 || firstCommand.compare("import") == 0) {
            if(currentStock != NULL) {
                if (import(command, currentStock)) {
                    std::cout << "Successfully imported " << currentStock->getRowCount() << " rows of data!" << std::endl;
                } else {
                    std::cout << "IMPORT failed! Please check file path and use correct syntax. Enter HELP for further information." << std::endl;
                }
            } else {
                std::cout << "No stock loaded. Please ADD new Stock or SEARCH for existing Stock before trying to IMPORT." << std::endl;
            }
        }
        // ################# SEARCH ###################
        else if (firstCommand.compare("SEARCH") == 0 || firstCommand.compare("search") == 0) {
            Stock* temp = search(command, stocks);
            if (temp != NULL) {
                std::cout << "Stock found!" << std::endl;
                currentStock = temp;
            } else {
                std::cout << "Stock not found!" << std::endl;
            }
        }
        // ################# DEL ###################
        else if (firstCommand.compare("DEL") == 0 || firstCommand.compare("del") == 0) {
            if(del(command, currentStock, stocks)) {
                currentStock = NULL;
                std::cout << "Stock deleted!" << std::endl;
            } else {
                std::cout << "Error deleting stock. No stock opened or no stock with that name exists!" << std::endl;
            }
        }
        // ################# PLOT ###################
        else if (firstCommand.compare("PLOT") == 0 || firstCommand.compare("plot") == 0) {
            if (currentStock != NULL) {
                if (!currentStock->plot()) {
                    std::cout << "No data to plot." << std::endl;
                }
            } else {
                std::cout << "No stock loaded. ADD or SEARCH for a stock first." << std::endl;
            }
        }
        // ################# SAVE ###################
        else if (firstCommand.compare("SAVE") == 0 || firstCommand.compare("save") == 0) {
            if(save(command, stocks)) {
                std::cout << "Database saved!" << std::endl;
            } else {
                std::cout << "Error saving database! Please check file path and use correct syntax. Enter HELP for further information." << std::endl;
            }
        }
        // ################# LOAD ###################
        else if (firstCommand.compare("LOAD") == 0 || firstCommand.compare("load") == 0) {
            StockTable* temp = new StockTable();
            if(load(command, temp)) {
                delete stocks;
                stocks = temp;
                currentStock = NULL;
                std::cout << "Database loaded!" << std::endl;
            } else {
                delete temp;
                std::cout << "Error loading database! Please check file path and use correct syntax. Enter HELP for further information." << std::endl;
            }
        }
        // ################## QUIT ###################
        else if (command.compare("QUIT") == 0 || command.compare("quit") == 0) {
            break;
        }
        // ######### UNKNOWN COMMAND ERROR ###########
        else {
            if(!command.empty()) {
                std::cout << "Invalid command \"" + command + "\" entered. Enter HELP for further information." << std::endl;
            }
        }
        // ############# CURRENT STOCK INFO ###########
        if (currentStock != NULL) {
            std::cout << std::endl
                      << "##################################################################################################################" << std::endl
                      << "## Current stock:   " << currentStock->getName() << std::endl
                      << "## Stock WKN:       " << currentStock->getWKN() << std::endl
                      << "## Stock symbol:    " << currentStock->getKey() << std::endl
                      << "## Stock data rows: " << currentStock->getRowCount() << std::endl
                      << "## Latest data row: (Date,Open,High,Low,Close, Volume, Adj Close)" << std::endl
                      << "##                  " << currentStock->getLatestRow() << std::endl // Format not right!!!
                      << "##################################################################################################################" << std::endl;
        }
        std::cout << std::endl << ">> ";
        std::getline(std::cin, command);
    }
    delete stocks;
}

void helpSection() {
    std::cout << "######################## HELP ########################" << std::endl
        << "All commands can be entered in exclusively UPPERCASE or exclusively lowercase letters." << std::endl
        << "<VARIABLES> have to be entered without \"<>\". No spaces are allowed in <VARIABLES>." << std::endl << std::endl
        << "--------------------- COMMANDS ---------------------" << std::endl
        << "HELP............................Print this help section again" << std::endl << std::endl
        << "PLOT............................Plot closing prices of the opened stock for the last 30 days" << std::endl << std::endl
        << "ADD <NAME> <WKN> <SYMBOL>.......Adds a new Stock to the database, and opens it for manipulation" << std::endl << std::endl
        << "IMPORT <FILENAME>...............Imports data from csv-file into currently opened stock. csv-file must be in subdirectory "
        << "\"/csv\" or path must be entered relative to that directory." << std::endl << std::endl
        << "DEL <OPTIONAL_SEARCH_PARAM>.....Deletes currently opened stock from the database. If optional search parameter is given, "
        << " searches database for Stock with that name or symbol to delete instead." << std::endl << std::endl
        << "SEARCH <SEARCH_PARAM>...........Searches database for stock with that symbol or name, and opens it." << std::endl << std::endl
        << "SAVE <FILENAME>.................Saves database with specified filename in subdirectory \"/saved\". "
        << "Alternate paths must be entered relative to that directory." << std::endl << std::endl
        << "LOAD <FILENAME>.................Loads database with specified filename from subdirectory \"/saved\". "
        << "Alternate paths must be entered relative to that directory." << std::endl << std::endl
        << "QUIT............................Quit program" << std::endl;
}

// all the following functions basically do the same, take the input command and evaluate the arguments
// and execute command if everything is correct

Stock* add(std::string command) {
    std::string commandBuffer;
    std::string evaluate;
    std::string name;
    std::string wkn;
    std::string key;
    commandBuffer = command;
    if (commandBuffer.find(" ") != std::string::npos) {
        commandBuffer.erase(0, commandBuffer.find(" ") + 1);
        evaluate = commandBuffer.substr(0, commandBuffer.find(" "));
        if(evaluate.empty()) {
            return NULL;
        }
        name = evaluate;
    } else {
        return NULL;
    }
    if (commandBuffer.find(" ") != std::string::npos) {
        commandBuffer.erase(0, commandBuffer.find(" ") + 1);
        evaluate = commandBuffer.substr(0, commandBuffer.find(" "));
        if(evaluate.empty()) {
            return NULL;
        }
        wkn = evaluate;
    } else {
        return NULL;
    }
    if (commandBuffer.find(" ") != std::string::npos) {
        commandBuffer.erase(0, commandBuffer.find(" ") + 1);
        evaluate = commandBuffer.substr(0, commandBuffer.find(" "));
        if(evaluate.empty()) {
            return NULL;
        }
        key = evaluate;
    } else {
        return NULL;
    }
    return new Stock(key, name, wkn);
}

bool import(std::string command, Stock* stock) {
    std::string commandBuffer;
    std::string evaluate;
    std::string path;
    commandBuffer = command;
    if (commandBuffer.find(" ") != std::string::npos) {
        commandBuffer.erase(0, commandBuffer.find(" ") + 1);
        evaluate = commandBuffer.substr(0, commandBuffer.find(" "));
        if(evaluate.empty()) {
            return false;
        }
        path = evaluate;
    } else {
        return false;
    }
    return stock->import("csv/" + path);
}

bool del(std::string command, Stock* stock, StockTable* stocks) {
    std::string commandBuffer;
    std::string evaluate;
    std::string searchValue;
    commandBuffer = command;
    if (commandBuffer.find(" ") != std::string::npos) {
        commandBuffer.erase(0, commandBuffer.find(" ") + 1);
        evaluate = commandBuffer.substr(0, commandBuffer.find(" "));
        if(evaluate.empty()) {
            if (stock == NULL) {
                return false;
            }
            return stocks->remove(stock->getKey());
        }
        searchValue = evaluate;
    } else {
        if (stock == NULL) {
            return false;
        }
        return stocks->remove(stock->getKey());
    }
    return stocks->remove(searchValue);
}

Stock* search(std::string command, StockTable* stocks) {
    std::string commandBuffer;
    std::string evaluate;
    std::string searchValue;
    commandBuffer = command;
    if (commandBuffer.find(" ") != std::string::npos) {
        commandBuffer.erase(0, commandBuffer.find(" ") + 1);
        evaluate = commandBuffer.substr(0, commandBuffer.find(" "));
        if(evaluate.empty()) {
            return NULL;
        }
        searchValue = evaluate;
    } else {
        return NULL;
    }
    return stocks->search(searchValue);
}

bool save(std::string command, StockTable* stocks) {
    std::string commandBuffer;
    std::string evaluate;
    std::string path;
    commandBuffer = command;
    if (commandBuffer.find(" ") != std::string::npos) {
        commandBuffer.erase(0, commandBuffer.find(" ") + 1);
        evaluate = commandBuffer.substr(0, commandBuffer.find(" "));
        if(evaluate.empty()) {
            return false;
        }
        path = evaluate;
    } else {
        return false;
    }
    return stocks->save("saved/" + path);
}

bool load(std::string command, StockTable* stocks) {
    std::string commandBuffer;
    std::string evaluate;
    std::string path;
    commandBuffer = command;
    if (commandBuffer.find(" ") != std::string::npos) {
        commandBuffer.erase(0, commandBuffer.find(" ") + 1);
        evaluate = commandBuffer.substr(0, commandBuffer.find(" "));
        if(evaluate.empty()) {
            return false;
        }
        path = evaluate;
    } else {
        return false;
    }
    return stocks->load("saved/" + path);
}
