#include <iostream>
#include "poker_hand.cpp"
#include "json/json.h"
#include<fstream>

bool readInHands(const Json::Value& hands, std::vector<PokerHand> &pokerHands) {
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
        PokerHand pokerHand = PokerHand();
        for (int j = 0; j < values.size(); j++) {
            Card temp;
            std::string key;
            key = values[j].asString();
            temp.value = CARD_TO_VALUE_MAP.at(key);
            temp.displayValue = key;
            temp.suit = suits[j].asString();
            pokerHand.addCard(temp);
        }
        pokerHands.push_back(pokerHand);
    }
    return true;
}

bool straightFlushTest() {
    std::cout << __func__ << std::endl;
    Json::Value root;
    Json::Reader reader;
    std::ifstream file("./test_hands/straight_flush_test.json");
    file >> root;
    reader.parse(file, root, true);
    const Json::Value& hands = root["hands"];
    file.close();
    std::vector<PokerHand> pokerHands;
    readInHands(hands, pokerHands);
    for (auto hand : pokerHands) {
        std::cout << hand;
    }
    return true;
}

bool OnePairTest() {
    std::cout << __func__ << std::endl;
    Json::Value root;
    Json::Reader reader;
    std::ifstream file("./test_hands/OnePair-Test.json");
    file >> root;
    reader.parse(file, root, true);
    const Json::Value& hands = root["hands"];
    file.close();
    std::vector<PokerHand> pokerHands;
    readInHands(hands, pokerHands);
    for (auto hand : pokerHands) {
        std::cout << hand;
    }
    return true;
}

bool FourOfAKindTest() {
    std::cout << __func__ << std::endl;
    Json::Value root;
    Json::Reader reader;
    std::ifstream file("./test_hands/four_of_a_kind_test.json");
    file >> root;
    reader.parse(file, root, true);
    const Json::Value& hands = root["hands"];
    file.close();
    std::vector<PokerHand> pokerHands;
    readInHands(hands, pokerHands);
    for (auto hand : pokerHands) {
        std::cout << hand;
    }
    return true;
}

bool fullHouseTest() {
    std::cout << __func__ << std::endl;
    Json::Value root;
    Json::Reader reader;
    std::ifstream file("./test_hands/full_house_test.json");
    file >> root;
    reader.parse(file, root, true);
    const Json::Value& hands = root["hands"];
    file.close();
    std::vector<PokerHand> pokerHands;
    readInHands(hands, pokerHands);
    for (auto hand : pokerHands) {
        std::cout << hand;
    }
    return true;
}

int main(int, char**) {
    straightFlushTest();
    OnePairTest();
    FourOfAKindTest();
    fullHouseTest();
    return 0;
}