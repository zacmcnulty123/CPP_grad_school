#include<iostream>
#include"poker_hand.cpp"
#include"json/json.h"
#include<fstream>
#include<filesystem>
#include<cassert>

//@Brief - Reads in PokerHands using jsoncpp and populates a list
//@Param[in] Json::Value - Hands in json format to parse
//@Param[inout] std::vector<PokerHand> - Set of PokerHands to populate
void readInHands(const Json::Value& hands, std::vector<PokerHand> &pokerHands) {
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
}

//@Brief - Parses json files and populates pokerhands
//@Param[in] filename - Name of file to parse
//@Param[inout] pokerHands - List of pokerhands to populate
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

//@Brief - Gets the absolute path of the configuration file
//@Param[int] fileRelativePath - Relative Path to the current build
//@return std::string absolute path to configuration file
std::string getTestFilePath(const std::string fileRelativePath) {
  std::__fs::filesystem::path p = fileRelativePath;
  std::string fs = std::__fs::filesystem::absolute(p);
  return fs;
}

//@Brief verbosely compares pokerhands
//@Param[in] pokerHands - Hands to compare
//@Return std::string - verbose output
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

//Set of tests to run for the hand comparer
void straightFlushTest(const std::string fs) {
  std::cout << __func__ << std::endl;
  std::vector<PokerHand> pokerHands;
  parseFile(getTestFilePath(fs), pokerHands);
  assert(pokerHands[0] > pokerHands[1]);
  assert(pokerHands[2] > pokerHands[3]);
  assert(pokerHands[4] == pokerHands[5]);
  assert(pokerHands[6] > pokerHands[7]);
  std::cout << verboseTestHandCompare(pokerHands);
}

void onePairTest(const std::string fs) {
  std::cout << __func__ << std::endl;
  std::vector<PokerHand> pokerHands;
  parseFile(getTestFilePath(fs), pokerHands);
  assert(pokerHands[0] > pokerHands[1]);
  assert(pokerHands[2] > pokerHands[3]);
  assert(pokerHands[4] > pokerHands[5]);
  assert(pokerHands[6] > pokerHands[7]);
  assert(pokerHands[8] > pokerHands[9]);
  assert(pokerHands[10] == pokerHands[11]);
  std::cout << verboseTestHandCompare(pokerHands);
}

void fourOfAKindTest(const std::string fs) {
  std::cout << __func__ << std::endl;
  std::vector<PokerHand> pokerHands;
  parseFile(getTestFilePath(fs), pokerHands);
  assert(pokerHands[0] > pokerHands[1]);
  assert(pokerHands[2] > pokerHands[3]);
  assert(pokerHands[4] == pokerHands[5]);
  std::cout << verboseTestHandCompare(pokerHands);
}

void fullHouseTest(const std::string fs) {
  std::cout << __func__ << std::endl;
  std::vector<PokerHand> pokerHands;
  parseFile(getTestFilePath(fs), pokerHands);
  assert(pokerHands[0] > pokerHands[1]);
  assert(pokerHands[2] > pokerHands[3]);
  assert(pokerHands[4] == pokerHands[5]);
  std::cout << verboseTestHandCompare(pokerHands);
}

void flushTest(const std::string fs) {
  std::cout << __func__ << std::endl;
  std::vector<PokerHand> pokerHands;
  parseFile(getTestFilePath(fs), pokerHands);
  assert(pokerHands[0] > pokerHands[1]);
  assert(pokerHands[2] > pokerHands[3]);
  assert(pokerHands[4] > pokerHands[5]);
  assert(pokerHands[6] > pokerHands[7]);
  assert(pokerHands[8] > pokerHands[9]);
  assert(pokerHands[10] == pokerHands[11]);
  std::cout << verboseTestHandCompare(pokerHands);
}

void threeOfAKindTest(const std::string fs) {
  std::cout << __func__ << std::endl;
  std::vector<PokerHand> pokerHands;
  parseFile(getTestFilePath(fs), pokerHands);
  assert(pokerHands[0] > pokerHands[1]);
  assert(pokerHands[2] > pokerHands[3]);
  assert(pokerHands[4] > pokerHands[5]);
  assert(pokerHands[6] == pokerHands[7]);
  std::cout << verboseTestHandCompare(pokerHands);
}

void twoPairTest(const std::string fs) {
  std::cout << __func__ << std::endl;
  std::vector<PokerHand> pokerHands;
  parseFile(getTestFilePath(fs), pokerHands);
  assert(pokerHands[0] > pokerHands[1]);
  assert(pokerHands[2] > pokerHands[3]);
  assert(pokerHands[4] > pokerHands[5]);
  assert(pokerHands[6] == pokerHands[7]);
  std::cout << verboseTestHandCompare(pokerHands);
}

void highCardTest(const std::string fs) {
  std::cout << __func__ << std::endl;
  std::vector<PokerHand> pokerHands;
  parseFile(getTestFilePath(fs), pokerHands);
  assert(pokerHands[0] > pokerHands[1]);
  assert(pokerHands[2] > pokerHands[3]);
  assert(pokerHands[4] > pokerHands[5]);
  assert(pokerHands[6] > pokerHands[7]);
  assert(pokerHands[8] > pokerHands[9]);
  assert(pokerHands[10] == pokerHands[11]);
  std::cout << verboseTestHandCompare(pokerHands);
}

void straightTest(const std::string fs) {
  std::cout << __func__ << std::endl;
  std::vector<PokerHand> pokerHands;
  parseFile(getTestFilePath(fs), pokerHands);
  assert(pokerHands[0] > pokerHands[1]);
  assert(pokerHands[2] > pokerHands[3]);
  assert(pokerHands[4] == pokerHands[5]);
  std::cout << verboseTestHandCompare(pokerHands);
}

int main() {
  std::cout << "Starting Tests" << std::endl;
  straightFlushTest("../test_hands/straight_flush_test.json");
  fourOfAKindTest("../test_hands/four_of_a_kind_test.json");
  fullHouseTest("../test_hands/full_house_test.json");
  flushTest("../test_hands/flush_test.json");
  straightTest("../test_hands/straight_test.json");
  threeOfAKindTest("../test_hands/three_of_a_kind_test.json");
  twoPairTest("../test_hands/two_pair_test.json");
  onePairTest("../test_hands/OnePair-Test.json");
  highCardTest("../test_hands//high_card_test.json");
  std::cout << "All Tests Passed" << std::endl;
  return 0;
}