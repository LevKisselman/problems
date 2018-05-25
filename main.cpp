#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <array>
#include <algorithm>

using namespace std;


char inputText[]  = "HandA:Ac-Kd-Jd-3d HandB:5c-5d-6c-7d Board:Ah-Kh-5s-2s-Qd\n\
HandA:Ac-Kd-Jd-3d HandB:5c-5d-6c-6d Board:Ad-Kh-5s-2d-Qd\n\
HandA:Qc-Jd-Td-3d HandB:Tc-Jc-8h-6d Board:Ad-Kh-Qs-2d-3c\n\
HandA:Qh-4d-Tc-8s HandB:Qc-8c-7d-2h Board:Ad-As-3c-3d-5d\n\
HandA:Ah-2s-Qd-9S HandB:Ac-2d-6s-Jh Board:Kd-4h-Kh-5s-3c\n\
HandA:Ah-2s-Qd-9S HandB:Ac-2d-As-Jh Board:Kd-4h-Kh-5s-3c\n\
HandA:6d-6c-Kc-4d HandB:Jh-Js-Qs-8h Board:2s-3h-9c-As-Ac\n\
HandA:6d-Kh-Ac-4d HandB:Jh-2s-Ah-8h Board:Js-3h-9c-As-6c\n\
HandA:Qc-Jd-Td-3d HandB:3s-3h-8h-6d Board:Ad-Kh-Qs-Qd-3c";


class ComboFactory;

const char RANK[15] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' };
struct Card {
    int rank;
    char suit;

    Card(char assignedRank, char assignedSuit) : suit(assignedSuit)
    {
        rank = -1;
        for(int i = 0; i < 15; i++)
            if(RANK[i] == assignedRank)
            {
                rank = i;
                break;
            }
    }
    
    Card() 
    {
    }
    
    bool operator > (const Card &other)
    {
        return this->rank > other.rank;
    }
    
    bool operator < (const Card &other)
    {
        return this->rank < other.rank;
    }
};


class Combo {
 protected:
    
    enum comboRank { highCard = 0, pair, twoPair, threeKind, straight, flush, fullHouse, fourKind, straightFlush };
    const string comboRankName[9] = {"High Card", "One Pair", "Two Pair", "3-of-a-Kind", "Straight", "Flush", "Full House", "4-of-a-Kind", "Straight Flush"};
 public:

    // todo: rank and cards should be moved to private 
    comboRank rank = highCard;
    Card cards[5];
    
    // inputCards contains cards sorted by rank in desc order 
    Combo(Card inputCards[5])
    {
        for(int i = 0; i < 5; i++)
            cards[i] = inputCards[i];
    }
    
    // debug output
    void print(){
        cout<<cards[0].rank<<cards[0].suit<<" "<<cards[1].rank<<cards[1].suit<<" "<<cards[2].rank<<cards[2].suit<<" "<<cards[3].rank<<cards[3].suit<<" "<<cards[4].rank<<cards[4].suit<<" - "<<comboRankName[rank]<<endl;
    }
    
    string getRankName()
    {
        return comboRankName[rank];
    }
    
    virtual bool isGreaterThen (const Combo* other)
    {
        cout<<"Base operator = "<<this->rank<<endl;
        return this->rank > other->rank;
    }
};
 
 
class StraightFlush: public Combo {
private: 
    int topCardRank = 0;
public:
    StraightFlush(Card inputCards[5]): Combo(inputCards)
    {
        rank = comboRank::straightFlush;
        
        topCardRank = cards[0].rank;
        // card are arranged in order of rank.  In case A5432, ranking card is at index 1 (5)
        if(cards[0].rank == 14 && cards[1].rank != 13 )
            topCardRank = cards[1].rank;
    }
    
    static bool match(Card cards[5])
    {
        //If all 5 suits don't match false
        for(int i = 0; i < 4; i++)
            if(cards[i].suit != cards[i+1].suit)
                return false;
                
        // if all 5 ranks aren't sequential, false
        for(int i = 0; i < 4; i++)
        {
            if(cards[i].rank != cards[i+1].rank+1)
            {
                if(cards[0].rank == 14 && cards[1].rank == 5 && cards[2].rank == 4 && cards[3].rank == 3 && cards[4].rank == 2)
                {
                    return true; // Case where Ace has value 1
                } else 
                {
                    return false;
                }
            }
        }
        
        return true;
    }
    
    virtual bool isGreaterThen(const Combo* other)
    {
        if(this->rank != other->rank)
            return this->rank > other->rank;
        
        if(this->topCardRank > ((StraightFlush*)other)->topCardRank)
            return true;
            
        return false;    
    }
};


class FourKind: public Combo {
private:
    int MatchedCardRank = 0;
public:
    FourKind(Card inputCards[5]): Combo(inputCards)
    {
        rank = comboRank::fourKind;
        
        // first 4 cards have matching rank
        if( cards[0].rank == cards[1].rank && 
            cards[1].rank == cards[2].rank &&
            cards[2].rank == cards[3].rank)
        {
            MatchedCardRank = cards[0].rank;
        } else 
        {
            MatchedCardRank = cards[4].rank;
        }
    }
    
    static bool match(Card cards[5])
    {
        // first 4 cards have matching rank
        if( cards[0].rank == cards[1].rank && 
            cards[1].rank == cards[2].rank &&
            cards[2].rank == cards[3].rank)
            return true;
        
        // last 4 cards have matching rank
        if( cards[1].rank == cards[2].rank &&
            cards[2].rank == cards[3].rank &&
            cards[3].rank == cards[4].rank)
            return true;
            
        return false;
    }
    
    virtual bool isGreaterThen(const Combo* other)
    {
        if(this->rank != other->rank)
            return this->rank > other->rank;
        
        if(this->MatchedCardRank > ((FourKind*)other)->MatchedCardRank)
            return true;
        else if(this->MatchedCardRank < ((FourKind*)other)->MatchedCardRank)
            return false;
        
        // check kicker
        for(int i = 0; i < 5; i++){
            if(this->cards[i].rank > other->cards[i].rank)
                return true;
            else if(this->cards[i].rank < other->cards[i].rank)
                return false;
        }

        return false;    
    }
};


class FullHouse: public Combo {
private:
    int matchedCardRankMajor = 0;
    int matchedCardRankMinor = 0;
public:
    FullHouse(Card inputCards[5]): Combo(inputCards)
    {
        rank = comboRank::fullHouse;
        // XXYYY pattern
        if( cards[0].rank == cards[1].rank && 
            cards[2].rank == cards[3].rank &&
            cards[3].rank == cards[4].rank)
        {
            matchedCardRankMajor = cards[2].rank;
            matchedCardRankMinor = cards[0].rank;
        }
        
        // XXXYY pattern
        if( cards[0].rank == cards[1].rank &&
            cards[1].rank == cards[2].rank &&
            cards[3].rank == cards[4].rank)
        {
            matchedCardRankMajor = cards[0].rank;
            matchedCardRankMinor = cards[3].rank;
        }

    }
    
    static bool match(Card cards[5])
    {
        // XXYYY pattern
        if( cards[0].rank == cards[1].rank && 
            cards[2].rank == cards[3].rank &&
            cards[3].rank == cards[4].rank)
            return true;
        
        // XXXYY pattern
        if( cards[0].rank == cards[1].rank &&
            cards[1].rank == cards[2].rank &&
            cards[3].rank == cards[4].rank)
            return true;
            
        return false;
    }
    
    virtual bool isGreaterThen(const Combo* other)
    {
        if(this->rank != other->rank)
            return this->rank > other->rank;
        
        if(this->matchedCardRankMajor > ((FullHouse*)other)->matchedCardRankMajor)
            return true;
        else if(this->matchedCardRankMajor < ((FullHouse*)other)->matchedCardRankMajor)
            return false;
        else
        {
            if(this->matchedCardRankMinor > ((FullHouse*)other)->matchedCardRankMinor)
                return true;
            else if(this->matchedCardRankMinor < ((FullHouse*)other)->matchedCardRankMinor)
                return false;
        }
            
        return false;    
    }
};


class Flush: public Combo {
public:
    Flush(Card inputCards[5]): Combo(inputCards)
    {
        rank = comboRank::flush;
    }
    
    static bool match(Card cards[5])
    {
        //If all 5 suits don't match false
        for(int i = 0; i < 4; i++)
            if(cards[i].suit != cards[i+1].suit)
                return false;
        
        return true;
    }
    
    virtual bool isGreaterThen(const Combo* other)
    {
        if(this->rank != other->rank)
            return this->rank > other->rank;
        
        for(int i = 0; i < 5; i++){
            if(this->cards[i].rank > other->cards[i].rank)
                return true;
            else if(this->cards[i].rank < other->cards[i].rank)
                return false;
        }
            
        return false;    
    }
};


class Straight: public Combo {
private: 
    int topCardRank = 0;
public:
    Straight(Card inputCards[5]): Combo(inputCards)
    {
        rank = comboRank::straight;
        
        topCardRank = cards[0].rank;
        if(cards[0].rank == 14 && cards[1].rank != 13 )
            topCardRank = cards[1].rank;
    }
    
    static bool match(Card cards[5])
    {
        // if all 5 ranks aren't sequential, false
        for(int i = 0; i < 4; i++)
        {
            if(cards[i].rank != cards[i+1].rank+1)
            {
                if(cards[0].rank == 14 && cards[1].rank == 5 && cards[2].rank == 4 && cards[3].rank == 3 && cards[4].rank == 2)
                {
                    return true; // Case where Ace has value 1
                } else 
                {
                    return false;
                }
            }
        }
        
        
        return true;
    }
    
    virtual bool isGreaterThen(const Combo* other)
    {
        if(this->rank != other->rank)
            return this->rank > other->rank;
        
        if(this->topCardRank > ((Straight*)other)->topCardRank)
            return true;
            
        return false;    
    }
};


class ThreeKind: public Combo {
private: 
    int matchedCardRank = 0;
    
public:
   ThreeKind(Card inputCards[5]): Combo(inputCards)
    {
        rank = comboRank::threeKind;
        
        // first 3 cards have matching rank
        if( cards[0].rank == cards[1].rank && cards[1].rank == cards[2].rank )
            matchedCardRank = cards[0].rank;
        
        //  cards 2-4 have matching rank
        else if( cards[1].rank == cards[2].rank && cards[2].rank == cards[3].rank )
            matchedCardRank = cards[1].rank;
        
        //  cards 2-4 have matching rank
        else if( cards[2].rank == cards[3].rank && cards[3].rank == cards[4].rank )
            matchedCardRank = cards[2].rank;
        
    }
    
    static bool match(Card cards[5])
    {
        // first 3 cards have matching rank
        if( cards[0].rank == cards[1].rank && 
            cards[1].rank == cards[2].rank )
            return true;
        
        //  cards 2-4 have matching rank
        if( cards[1].rank == cards[2].rank &&
            cards[2].rank == cards[3].rank )
            return true;
        
        //  cards 2-4 have matching rank
        if( cards[2].rank == cards[3].rank &&
            cards[3].rank == cards[4].rank )
            return true;
            
        return false;
    }
    
    virtual bool isGreaterThen(const Combo* other)
    {
        if(this->rank != other->rank)
            return this->rank > other->rank;
        
        if(this->matchedCardRank > ((ThreeKind*)other)->matchedCardRank)
            return true;
            
        for(int i = 0; i < 5; i++){
            if(this->cards[i].rank > other->cards[i].rank)
                return true;
            else if(this->cards[i].rank < other->cards[i].rank)
                return false;
        }
        
        return false;    
    }
};


class TwoPair: public Combo {
private:
    int matchedCardRankHi = 0;
    int matchedCardRankLow = 0;
public:
    TwoPair(Card inputCards[5]): Combo(inputCards)
    {
        rank = comboRank::twoPair;
        
        for(int i = 0; i< 4; i++)
            if(cards[i].rank == cards[i+1].rank)
            {
                if(matchedCardRankHi == 0)
                    matchedCardRankHi = cards[i].rank;
                else if(matchedCardRankHi >= cards[i].rank)
                    matchedCardRankLow = cards[i].rank;
                else
                {
                    matchedCardRankLow = matchedCardRankHi;
                    matchedCardRankHi = cards[i].rank;
                }
            }
    }
    
    static bool match(Card cards[5])
    {
        int matches = 0;
        for(int i = 0; i< 4; i++)
            if(cards[i].rank == cards[i+1].rank)
                matches++;   
        
        if(matches == 2)
            return true;
        return false;
    }
    
    virtual bool isGreaterThen(const Combo* other)
    {
        if(this->rank != other->rank)
            return this->rank > other->rank;
        
        if(this->matchedCardRankHi > ((TwoPair*)other)->matchedCardRankHi)
             return true;
        else if(this->matchedCardRankHi < ((TwoPair*)other)->matchedCardRankHi)
             return false;
        else if(this->matchedCardRankLow > ((TwoPair*)other)->matchedCardRankLow)
             return true;
        else if(this->matchedCardRankLow < ((TwoPair*)other)->matchedCardRankLow)
             return false;
             
        //check kicker
        for(int i = 0; i < 5; i++){
            if(this->cards[i].rank > other->cards[i].rank)
                return true;
            else if(this->cards[i].rank < other->cards[i].rank)
                return false;
        }
        
        return false;    
    }
};


class Pair: public Combo {
private:
    int matchedCardRank;
    
public:
    Pair(Card inputCards[5]): Combo(inputCards)
    {
        rank = comboRank::pair;
        
        for(int i = 0; i< 4; i++)
            if(cards[i].rank == cards[i+1].rank)
            {
                matchedCardRank = cards[i].rank;
                break;
            }
        
    }
    
    static bool match(Card cards[5])
    {
        int matches = 0;
        for(int i = 0; i< 4; i++)
            if(cards[i].rank == cards[i+1].rank)
                matches++;   
        
        if(matches == 1)
            return true;
        return false;
    }
    
    virtual bool isGreaterThen(const Combo* other)
    {
        if(this->rank != other->rank)
            return this->rank > other->rank;
        
        if(this->matchedCardRank > ((Pair*)other)->matchedCardRank)
            return true;
            
        //check kicker
        for(int i = 0; i < 5; i++){
            if(this->cards[i].rank > other->cards[i].rank)
                return true;
            else if(this->cards[i].rank < other->cards[i].rank)
                return false;
        }
        
        return false;    
    }
};


class HighCard: public Combo {
public:
    HighCard(Card inputCards[5]): Combo(inputCards)
    {
        rank = comboRank::highCard;
    }
    
    static bool match(Card cards[5])
    {
        return true;
    }
    
    virtual bool isGreaterThen (const Combo* other)
    {
        if(this->rank != other->rank)
            return this->rank > other->rank;
            
        for(int i = 0; i < 5; i++)
            if(this->cards[i] > other->cards[i])
                return true;
            else 
                continue;
        
        return false;    
    }
};


class ComboFactory{
// factory method
public:
static Combo* generateCombo(Card cards[5])//stop passing on stack
    {
        list<Card> cardsList;
        for(int i = 0; i < 5; i++)
            cardsList.push_back(cards[i]);
        cardsList.sort();
        
        // this is dumb, just use lists everywhere
        Card sotedCards[5];
        for(int i = 0; i < 5; i++)
        {
            sotedCards[i] = cardsList.back();
            cardsList.pop_back();
        }
        
        if(StraightFlush::match(sotedCards))
            return new StraightFlush((sotedCards));
        if(FourKind::match(sotedCards))
            return new FourKind((sotedCards));
        if(FullHouse::match(sotedCards))
            return new FullHouse((sotedCards));
        if(Flush::match(sotedCards))
            return new Flush((sotedCards));
        if(Straight::match(sotedCards))
            return new Straight((sotedCards));
        if(ThreeKind::match(sotedCards))
            return new ThreeKind((sotedCards));
        if(TwoPair::match(sotedCards))
            return new TwoPair((sotedCards));
        if(Pair::match(sotedCards))
            return new Pair((sotedCards));
        if(HighCard::match(sotedCards))
            return new HighCard((sotedCards));
        
        return new Combo(sotedCards);
    }
};


class Game {
 public:
    Game(string input)
    {
        handA[0] = Card(input[6], input[7]);
        handA[1] = Card(input[9], input[10]);
        handA[2] = Card(input[12], input[13]);
        handA[3] = Card(input[15], input[16]);
        
        handB[0] = Card(input[24], input[25]);
        handB[1] = Card(input[27], input[28]);
        handB[2] = Card(input[30], input[31]);
        handB[3] = Card(input[33], input[34]);
        
        board[0] = Card(input[42], input[43]);
        board[1] = Card(input[45], input[46]);
        board[2] = Card(input[48], input[49]);
        board[3] = Card(input[51], input[52]);
        board[4] = Card(input[54], input[55]);
    }

    void play()
    {
        // Generate  all possible hands for both players:
        int currentComboIndex = 0;
        for(int handCardIndex1 = 0; handCardIndex1 < 4; handCardIndex1++)
            for(int handCardIndex2 = handCardIndex1 + 1; handCardIndex2 < 4; handCardIndex2++)
                for(int boardCardIndex1 = 0; boardCardIndex1 < 5; boardCardIndex1++)
                    for(int boardCardIndex2 = boardCardIndex1 + 1; boardCardIndex2 < 5; boardCardIndex2++)
                        for(int boardCardIndex3 = boardCardIndex2 + 1; boardCardIndex3 < 5; boardCardIndex3++)
                        {
                            Card comboCardsHandA [5] = {handA[handCardIndex1], handA[handCardIndex2], board[boardCardIndex1], board[boardCardIndex2], board[boardCardIndex3]};
                            Card comboCardsHandB [5] = {handB[handCardIndex1], handB[handCardIndex2], board[boardCardIndex1], board[boardCardIndex2], board[boardCardIndex3]};
                            
                            
                            handACombos[currentComboIndex] = ComboFactory::generateCombo(comboCardsHandA);
                            handBCombos[currentComboIndex] = ComboFactory::generateCombo(comboCardsHandB);
                            currentComboIndex++;
                        }

        
        std::sort(handACombos.begin(), handACombos.end(), [] ( Combo* a, Combo* b ){ return a->isGreaterThen(b); } );
        std::sort(handBCombos.begin(), handBCombos.end(), [] ( Combo* a, Combo* b ){ return a->isGreaterThen(b); } );
        
        if(handACombos[0]->isGreaterThen(handBCombos[0]))
            cout<<"HandA wins ("<<handACombos[0]->getRankName();
        else if(handBCombos[0]->isGreaterThen( handACombos[0]))
            cout<<"HandB wins ("<<handBCombos[0]->getRankName();
        else 
            cout<<"Split Pot ("<<handACombos[0]->getRankName();
            
        cout<<")"<<endl<<endl;
        
        // free allocated memory
        for(int i = 0; i < 60; i++)
        {
            delete handACombos[i];
            delete handBCombos[i];
        }
    }
    
 private:
    Card handA [4] ;
    Card handB [4];
    Card board [5];
    array <Combo*, 60> handACombos;
    array <Combo*, 60> handBCombos;
};


int main()
{
    istringstream str;
    str.rdbuf()->pubsetbuf(inputText, sizeof(inputText));
    string line;
    while(getline(str, line))
    {
        if (*line.rbegin() == '\r') line.erase(line.end() - 1);
        Game g (line);
        g.play();
    }
}
