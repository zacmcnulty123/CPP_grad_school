#include <iostream>
#include "poker_hand.cpp"
#include "json/json.h"
#include<fstream>
#include <filesystem>
bool readInHands(const Json::Value& hands, std::vector<PokerHand> &pokerHands) {
    for (int i = 0; i < hands.size(); i++) {
        const Json::Value& values = hands[i]["cards"];
        const Json::Value& suits = hands[i]["suits"];
        if (values.size() != suits.size()) {
            std::cerr << "Difference between number of suits and values given" << std::endl;
            continue;
        }
        PokerHand pokerHand = PokerHand();
        for (int j = 0; j < values.size(); j++) {
            std::string key;
            key = values[j].asString();
            std::string suit = suits[j].asString();
            pokerHand.addCard(Card(key, suit));
        }
        pokerHands.push_back(pokerHand);
    }
    return true;
}

bool parseFile(const std::string filename, std::vector<PokerHand> & pokerHands) {
    Json::Value root;
    Json::Reader reader;
    std::ifstream file(filename.c_str());
    if (not file.is_open()) {
        std::cerr << filename << " couldn't be opened" << std::endl;
        return false;
    }
    file >> root;
    reader.parse(file, root, true);
    const Json::Value& hands = root["hands"];
    file.close();
    readInHands(hands, pokerHands);
    return true;
}

std::string getTestFilePath(const std::string fileRelativePath) {
    std::__fs::filesystem::path p = fileRelativePath;
    std::string fs = std::__fs::filesystem::absolute(p);
    return fs;
}

std::string verboseTestHandCompare(const std::vector<PokerHand> & pokerHands) {
    std::stringstream ss;
    for (int i = 1; i < pokerHands.size(); i+=2 ) {
        int j = pokerHands[i-1].compare(pokerHands[i]);
        if (j == 1) {
            ss << pokerHands[i-1] << " is greater than " <<
            pokerHands[i];
        }
        else if (j == 2) {
            ss << pokerHands[i-1] << " is less than " <<
            pokerHands[i];
        }
        else {
            ss << pokerHands[i-1] << " is equal to " <<
            pokerHands[i];
        }
        ss << "\n";
    }
    return ss.str();
}

bool straightFlushTest(const std::string fs) {
    std::cout << __func__ << std::endl;
    std::vector<PokerHand> pokerHands;
    parseFile(getTestFilePath(fs), pokerHands);
    std::cout << verboseTestHandCompare(pokerHands);
    return true;
}

bool OnePairTest(const std::string fs) {
    std::cout << __func__ << std::endl;
    std::vector<PokerHand> pokerHands;
    parseFile(getTestFilePath(fs), pokerHands);
    std::cout << verboseTestHandCompare(pokerHands);
    return true;
}

bool FourOfAKindTest(const std::string fs) {
    std::cout << __func__ << std::endl;
    std::vector<PokerHand> pokerHands;
    parseFile(getTestFilePath(fs), pokerHands);
    std::cout << verboseTestHandCompare(pokerHands);
    return true;
}

bool fullHouseTest(const std::string fs) {
    std::cout << __func__ << std::endl;
    std::vector<PokerHand> pokerHands;
    parseFile(getTestFilePath(fs), pokerHands);
    std::cout << verboseTestHandCompare(pokerHands);
    return true;
}

bool flushTest(const std::string fs) {
    std::cout << __func__ << std::endl;
    std::vector<PokerHand> pokerHands;
    parseFile(getTestFilePath(fs), pokerHands);
    std::cout << verboseTestHandCompare(pokerHands);
    return true;
}

bool threeOfAKindTest(const std::string fs) {
    std::cout << __func__ << std::endl;
    std::vector<PokerHand> pokerHands;
    parseFile(getTestFilePath(fs), pokerHands);
    std::cout << verboseTestHandCompare(pokerHands);
    return true;
}

bool twoPairTest(const std::string fs) {
    std::cout << __func__ << std::endl;
    std::vector<PokerHand> pokerHands;
    parseFile(getTestFilePath(fs), pokerHands);
    std::cout << verboseTestHandCompare(pokerHands);
    return true;
}

bool highCardTest(const std::string fs) {
    std::cout << __func__ << std::endl;
    std::vector<PokerHand> pokerHands;
    parseFile(getTestFilePath(fs), pokerHands);
    std::cout << verboseTestHandCompare(pokerHands);
    return true;
}

bool straightTest(const std::string fs) {
    std::cout << __func__ << std::endl;
    std::vector<PokerHand> pokerHands;
    parseFile(getTestFilePath(fs), pokerHands);
    std::cout << verboseTestHandCompare(pokerHands);
    return true;
}

int main(int, char**) {
    straightFlushTest("../test_hands/straight_flush_test.json");
    FourOfAKindTest("../test_hands/four_of_a_kind_test.json");
    fullHouseTest("../test_hands/full_house_test.json");
    flushTest("../test_hands/flush_test.json");
    straightTest("../test_hands/straight_test.json");
    threeOfAKindTest("../test_hands/three_of_a_kind_test.json");
    twoPairTest("../test_hands/two_pair_test.json");
    OnePairTest("../test_hands/OnePair-Test.json");
    highCardTest("../test_hands//high_card_test.json");
    return 0;
}