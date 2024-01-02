#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>
#include <set>
#include <algorithm>

enum class HandType : uint8_t {
    highCard = 1 << 0, 
    onePair = 1 << 1, 
    twoPair = 1 << 2, 
    threeOfAKind = 1 << 3, 
    fullHouse = 1 << 4, 
    fourOfAKind = 1 << 5,
    fiveOfAKind = 1 << 6, 
};

namespace util {
    std::map<char, int> cardVal {{'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'T', 10}, {'J', 11}, {'Q', 12},  {'K', 13}, {'A', 14}};
    std::map<int, HandType> handVal {{2, HandType::onePair}, {3, HandType::threeOfAKind}, {4, HandType::fourOfAKind}, {5, HandType::fiveOfAKind}};
}


class Hand {
private:
    std::multiset<int> hand;
    std::vector<int> handUnsorted;
public:
    Hand(std::string str) {
        for (int x = 0; x < 5; x++) {
            int val = util::cardVal[str[x]];
            hand.insert(val);
            handUnsorted.push_back(val);
        }
    }

    HandType score() const {
        std::multiset<HandType> pairs;
        for (int v = 2; v <= 14; v++) {
            int count = hand.count(v);
            if (count >= 4) {return util::handVal[count];}
            else if (count > 1) {pairs.insert(util::handVal[count]);}
        }
        return bestHand(pairs);
    }

    HandType bestHand (std::multiset<HandType>& pairs) const { //guaranteed no 5 or 4
        if (pairs.size() == 0) {
            return HandType::highCard;
        } 
        if (pairs.count(HandType::onePair) > 1) {
            return HandType::twoPair;
        }
        if (pairs.count(HandType::onePair) == 1 && pairs.count(HandType::threeOfAKind)) {
            return HandType::fullHouse;
        }
        return *pairs.cbegin();
    }

    auto operator<=>(const Hand& other) const {
        if (auto cmp = this->score() <=> other.score(); cmp != 0) {
            return cmp;
        } else {
            for(int i = 0; i < this->handUnsorted.size(); i++) {
                if (auto cmp = this->handUnsorted[i] <=> other.handUnsorted[i]; cmp != 0) {
                    return cmp;
                }
            }
            return (0 <=> 0);
        } 
    }

};


int main() {
    std::ifstream input("input13.txt", std::ios::in);

    std::vector<std::pair<Hand, int>> hands;
    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        hands.push_back({Hand(line.substr(0, 5)), std::stoi(line.substr(5))});
    }

    std::sort(hands.begin(), hands.end(), [](const std::pair<Hand, int>& lhs, const std::pair<Hand, int>& rhs) {
      return lhs.first < rhs.first;
    });

    int total = 0;
    for (int x = 0; x < hands.size(); x++) {
        total += (x+1) * hands[x].second;
    }
    std::cout << total << std::endl;
    return 0;
}