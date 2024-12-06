#include <iostream>
#include "search.h"
using namespace std;

int main() {
    
    // Use this function to call and test the functions you write inside of
    // search.h.  If you do not call the functions here, nothing will run.
    // cout << "hello------" << endl;
    // string token = cleanToken(" For 16bit-width character types,");
    // cout << token << endl;
    // set<string> tokens = gatherTokens("...to!!! .be.. .or... .not. ..to.. .be....");
    // set<string> tokensANS;
    // tokensANS.insert("to");
    // tokensANS.insert("be");
    // tokensANS.insert("or");
    // tokensANS.insert("not");
    // cout << "1. " << (tokens.size() == 4) << endl;
    // cout << "2. " << (tokens == tokensANS) << endl;
    // gatherTokens(" For 16bit-width character types,");
    // map<string, set<string>> index;
    // buildIndex("tiny.txt", index);
    // findQueryMatches(index, "vector");
    searchEngine("cplusplus.txt");
    // removeStopWords(index);



    return 0;
}
