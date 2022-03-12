```plantuml
@startuml

  package "Units Under Test" {
  class Card {
    __Constants__
    CARD_TO_VALUE_MAP
    SUITS
    __Constructor/Destructor__
    +Card(std::string val, std::string st)
    ~Card()
    __public methods__
    +unsigned int getValue()
    +std::string toString()
    +std::string getSuit()
    __private data__
    unsigned int value
    std::string displayValue
    std::string suit
    __operator overloads__
    Card <
    Card ==
    Card <<
  }

  class PokerHand {
    __TypeDefs__
    HandTypeE
    Props
    __Constructor/Destructor__
    +PokerHand()
    ~PokerHand()
    __Public methods__
    +std::string toString()
    +int compare(const PokerHand & handToCompare)
    +void addCard(const Card card)
    __Private methods__
    +HandTypeE getHandType()
    +Props getProperties()
    +void setHandProperties()
    +bool isStraightFlush()
    +bool isStraight()
    +bool isFlush()
    +bool isFullHouse()
    +bool isOnePair()
    +int countDuplicates()
    +std::map<unsigned int, int> getDuplicateCountMap()
    +int handleTypeTieBreak(const PokerHand & comp)
    +int handleTieBreakers(const std::vector<unsigned int> tieBreakers)
    __private data__
    std::vector<Card> hand
    HandTypeE handType
    Props handProps
    __operator overloads__
    PokerHand ==
    PokerHand <
    PokerHand >
    PokerHand <<
    HandTypeE <<
  }
  }
  Card --> PokerHand : Used by

package "Tester" {
  class Tester {
    __Helper methods__
    +void readInHands(Json::Value &hands, std::vector<PokerHand> &pokerHands)
    +bool parseFile(const std::string filename, std::vector<PokerHand> & pokerhands)
    +std::string getTestFilePath(const std::string fileRelativePath)
    __Testing methods__
    +std::string verboseTestHandCompare(const std::vector<PokerHand> &pokerHands)
    +void straightFlushTest(const std::string fs)
    +void onePairTest(const std::string fs)
    +void fourOfAKindTest(const std::string fs)
    +void fullHouseTest(const std::string fs)
    +void flushTest(const std::string fs)
    +void threeOfAKindTest(const std::string fs)
    +void twoPairTest(const std::string fs)
    +void highCardTest(const std::string fs)
    +void straightTest(const std::string fs)
    __main__
    Calls all testing methods each assert failures
  }
}

class jsoncpp {
  parse()
}

jsoncpp --> Tester : Used by
Card --> Tester : Used by

Tester --|> PokerHand : Testing

note bottom of PokerHand 
  PokerHand is responsible for managing the card vector as well as a set
  of properties regarding the hand. The PokerHand class also knows how to
  evalutes hand properties to one another to determine which hands are
  better than others.
end note

note top of Card
  Card is responsible for managing the properties of a card
  Card can also compare cards to one another to determine
  which are of greater value
end note

@enduml
```