#include <string>
#include "../headers/Stock.h"
#include <fstream>
#include <iostream>
#include <list>

Stock::Stock(std::string key, std::string name, std::string wkn) {
    this->key = key;
    this->name = name;
    this->wkn = wkn;
}

bool Stock::import(std::string path) {
    std::ifstream file(path);
    if (file.is_open()) {
        std::string line;
        std::list<std::string> lines;
        while (std::getline(file, line)) { // lines are added to list in reverse order, because in the csv file they are sorted chronologically
            lines.push_front(line);
        }

        int i = 0;
        std::string delimiter = ",";
        std::string data;
        for (std::string line : lines) { // begin at front of list, reading newest data rows first
            for (int j = 0; j < 7; j++) {
                data = line.substr(0, line.find(delimiter));
                stockData[i][j] = data;
                line.erase(0, line.find(delimiter) + delimiter.length());
            }
            i++;
            if (i >= 30) { // stop when the newest 30 rows are loaded
                break;
            }
        }
        file.close();
        return true;
    } else {
        return false;
    }
}

std::string Stock::getKey() {
    return this->key;
}

std::string Stock::getName() {
    return this->name;
}

std::string Stock::getWKN() {
    return this->wkn;
}

bool Stock::plot() {
    if (this->stockData[0][0].empty()) {
        return false;
    }
    const int plotterLines = 20;
    std::string plotter[plotterLines];
    plotter[0] = "           |                                                                                          |";
    for (int i = 1; i < plotterLines - 2; i++) {
        plotter[i] = plotter[0];
    }
    plotter[plotterLines - 2] = "           |__________________________________________________________________________________________|";
    plotter[19] = "                                                                                                      ";

    double min = std::stod(this->stockData[0][4]);
    double max = min;
    for (int i = 1; i < 30; i++) { // find lowest and highest value in closing prices
        double temp = std::stod(this->stockData[i][4]);
        if(temp <= min) {
            min = temp;
        } else if (temp >= max) {
            max = temp;
        }
    }
    double dY = max - min; // range of y axis
    double dYtemp;
    double dYprev;
    double dYnext;
    for(int i = 0; i < 30; i++) {
        dYtemp = std::stod(this->stockData[29 - i][4]) - min; // height of this point relative to min value
        int height = 17 * dYtemp / dY; // height of this point in lines
        plotter[17 - height][13 + i * 3] = 'o'; // plot point as 'o'

        if (i > 0) { // create line to previous point for all but the first point
            dYprev = (std::stod(this->stockData[29 - i + 1][4]) - min) - dYtemp; // height of last point relative to this point
            int prevHeight = 17 * (dYtemp + dYprev / 3) / dY; // height of interpolation point 1/3 of way between this point and previous point
            plotter[17 - prevHeight][13 - 1 + i * 3] = prevHeight - height < 0 ? '/' : (prevHeight - height == 0 ? '-' : '\\'); // decide between /, \ and - for better look
        }
        if (i < 29) { // create line to next point for all but the last one
            dYnext = (std::stod(this->stockData[29 - (i + 1)][4]) - min) - dYtemp; // height of next point relative to this point
            int nextHeight = 17 * (dYtemp + dYnext / 3) / dY; // height of interpolation point 1/3 of way between this point and next point
            plotter[17 - nextHeight][13 + 1 + i * 3] = nextHeight - height < 0 ? '\\' : (nextHeight - height == 0 ? '-' : '/'); // decide between /, \ and - for better look
        }
    }
    std::string high = std::to_string(max);
    std::string low = std::to_string(min);
    std::string label;
    for (int i = 0; i <= 17; i++) { // y-axis labels
        label = std::to_string(max - i * (max - min) / 17);
        plotter[i] = label + plotter[i].substr(label.length(), plotter[i].length() - label.length());
    }
    int j = 29;
    for (int i = 13; i < plotter[19].length(); i += 3) { // setting days as x-axis labels
        plotter[19][i] = this->stockData[j][0][8];
        plotter[19][i+1] = this->stockData[j][0][9];
        j--;
    }

    std::cout << "Closing prices between " << this->stockData[29][0] << " and " << this->stockData[0][0] << std::endl
        << "            __________________________________________________________________________________________" << std::endl;
    for (int i = 0; i< 20; i++) {
        std::cout << plotter[i] << std::endl;
    }
    return true;
}

std::string Stock::serialize() { // serializes all stock content into one block of text
    std::string metaData = this->key + ";" + this->name + ";" + this->wkn + "\n";
    std::string values = "";

    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 7; j++) {
            values += this->stockData[i][j] + ";";
        }
        values += "\n";
    }

    return metaData + values;
}

void Stock::deserialize(std::string content) { // deserializes a block of max. 30 data rows back into the stockDate array
    int i = 0;
    std::string delimiter = ";";
    for (int j = 0; ; j++) {
        stockData[i][j] = content.substr(0, content.find(delimiter));
        content.erase(0, content.find(delimiter) + delimiter.length());
        if(j == 6) {
            j = -1;
            i++;
        }
        if (content.length() == 0 || i >= 30) {
            break;
        }
    }
}

int Stock::getRowCount() {
    for(int i = 0; i < 30; i++) {
        if (this->stockData[i][0].empty()) {
            return i;
        }
    }
    return 30;
}

std::string Stock::getLatestRow() {
    if (this->stockData[0][0].empty()) {
        return "None";
    }
    std::string r = "(";
    for (int i = 0; i < 7; i++) {
        r += stockData[0][i];
        if (i < 6) {
            r += ",";
        }
    }
    return r + ")";
}
