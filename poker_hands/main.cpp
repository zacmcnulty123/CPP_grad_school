#include <iostream>
#include "poker_hand.cpp"
#include "json/json.h"
#include<fstream>
int main(int, char**) {
    std::cout << "Hello, world!\n";
    Json::Value root;
    Json::Reader reader;
    std::ifstream file("./test_hands/straight_flush_test.json");
    file >> root;
    if (not reader.parse(file, root, true)) {
        std::cout << "Failed to parse configuration\n" << reader.getFormattedErrorMessages();
    }
    std::cout << root;
}
