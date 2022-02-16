#include <iostream>
#include "poker_hand.cpp"
#include "json/json.h"
#include<fstream>
int main(int, char**) {
    Json::Value root;
    Json::Reader reader;
    std::ifstream file("./test_hands/straight_flush_test.json");
    file >> root;
    reader.parse(file, root, true);
    const Json::Value& hands = root["hands"];
    const int size = hands.size();
    std::array<PokerHand, 6> pokerHands;
    for (int i = 0; i < hands.size(); i++) {
        const Json::Value& values = hands[i]["cards"];
        const Json::Value& suits = hands[i]["suits"];
        if (values.size() != suits.size()) {
            continue;
        }
        
        if (std::find(SUITS.begin(), SUITS.end(), suits[i].asString()) == SUITS.end()) {
            continue;
        }

        if (CARD_TO_VALUE_MAP.find(values[i].asString()) == CARD_TO_VALUE_MAP.end()) {
            continue;
        }
        for (int j = 0; j < values.size(); j++) {
            Card temp;
            std::string key;
            key = values[j].asString();
            temp.value = CARD_TO_VALUE_MAP.at(key);
            temp.displayValue = key;
            temp.suit = suits[j].asString();
            pokerHands[i].addCard(temp);
        }
        std::cout << pokerHands[i];
        pokerHands[i].getHandValue();
    }
}
