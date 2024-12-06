
/*
Creative component:
Commented out in searchEngine function, removes stopwords from index.
*/

#pragma once

#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;


/*
This function reads in a string and gets rid of leading
or ending punctuation. This is done by checking if the 
first element of the string is punctuated, then reversing the string to
clean the back, and to end we reverse the string to put it
in original order.
*/
string cleanToken(string s) {
    bool hasLetters = false;
    
    for(int i = 0; i < s.size(); i++){
        if(isalpha(s[i])){
            hasLetters = true;
            break;
        }
        else{
            continue;
        }
    }

    while (ispunct(s[0]) != false){
        s.erase(0,1);
    }
    reverse(s.begin(), s.end());

    while (ispunct(s[0]) != false){
        s.erase(0,1);
    }

    reverse(s.begin(), s.end());

    for (int i = 0; i<s.size(); i ++){
        s[i] = tolower(s[i]);
    }

    if(s.size() == 0 || hasLetters == false){
        return "";  
    }
    else{
        return s;
    }
}

/*
In this function I split the inserted string by words.
This is done by using stringstream to seperate the
words by space.
*/
set<string> gatherTokens(string text) {
    set<string> tokens;

    stringstream stream(text);
    string word;

    while(stream>>word){
        word = cleanToken(word);
        if(isalpha(word[0]) || (isdigit(word[0]) && word.size()>1)){
            tokens.insert(word);
        }
    }
    return tokens;
}

/*
This function builds the index for the program. It does so by 
reading in lines, and if the line number is even, breaking the
contents of the line into tokens, adding the tokens to a map, and
updating the values of the tokens with the urls that came in the 
line before.
*/
int buildIndex(string filename, map<string, set<string>>& index) {
    int numURL = 0;
    set<string> tokens;
    string line;
    string url;
    ifstream inFile(filename);
    int lineNumber = 1;
    
    if (!inFile) {
        return 0;
    }
    else{
        while(!inFile.eof()){   
            set<string> urls; 
            getline(inFile, line);
            if(lineNumber % 2 != 0){
                url = line;
            }
            if(lineNumber % 2 == 0){
                numURL ++;
                set<string> tokensInLine = gatherTokens(line);
                urls.emplace(url);
                for(auto e : tokensInLine){
                    if(index.find(e) == index.end()){
                        index.insert({e, urls});
                    }
                    else{
                        (index.at(e)).emplace(url);
                    }
                }
            }     
            lineNumber++;
        }
        return numURL;
    }
}


/*
This function finds urls that match the keys inputed by the user.
It also takes into account the first character of the words the user 
enters, splitting the words by spaces. If the first character is a 
+, then set intersection is performed. If the first character is a -,
then set difference is performed.
*/
set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
    set<string> result;
    vector<string> searchWords;   

    for(int i = 0; i < sentence.size(); i++){
        sentence[i] = tolower(sentence[i]);
    }
    
    string searchWord;
    stringstream stream(sentence);

    while (stream>>searchWord){
        searchWords.push_back(searchWord);
    }

    searchWords[0] = cleanToken(searchWords[0]);    

    while(searchWords.size() >= 2){
        vector<string> pair;
        pair.push_back(searchWords[0]);
        pair.push_back(searchWords[1]);
        
        if(isalpha(pair[1][0])){
            for(auto e : index){
                if(e.first == pair[0] || e.first == pair[1]){
                    for(auto d : e.second){
                        result.emplace(d);
                    }
                }

            }
        }
        if(pair[1][0] == '+'){
            set<string> result0;
            set<string> result1;
            pair[1].erase(0,1);
            for(auto e : index){
                if(e.first == pair[0]){
                    for(auto d : e.second){
                        result0.emplace(d);
                    }
                }
                
                if(e.first == pair[1]){
                    for(auto d : e.second){
                        result1.emplace(d);
                    }
                }
                
                
            }
            set_intersection(result0.begin(), result0.end(), result1.begin(), result1.end(), inserter(result, result.begin()));
        }
        if(pair[1][0] == '-'){
            set<string> result0;
            set<string> result1;
            pair[1].erase(0,1);
            for(auto e : index){
                if(e.first == pair[0]){
                    for(auto d : e.second){
                        result0.emplace(d);
                    }
                }
                
                if(e.first == pair[1]){
                    for(auto d : e.second){
                        result1.emplace(d);
                    }
                }
                
                
            }
            set_difference(result0.begin(), result0.end(), result1.begin(), result1.end(), inserter(result, result.begin()));
        }
        searchWords.erase(searchWords.begin());
        searchWords.erase(searchWords.begin());
    }
    if(searchWords.size() == 1){
        if(isalpha(searchWords[0][0])){
            for(auto e : index){
                if(e.first == searchWords[0]){
                    for(auto d : e.second){
                        result.emplace(d);
                    }
                }

            }
        }
        if(searchWords[0][0] == '+'){
            set<string> result0;
            set<string> newResult;
            searchWords[0].erase(0,1);
            for(auto e : index){
                if(e.first == searchWords[0]){
                    for(auto d : e.second){
                        result0.emplace(d);
                    }
                }               
                
            }
            set_intersection(result0.begin(), result0.end(), result.begin(), result.end(), inserter(newResult, newResult.begin()));
            result = newResult;
        }
        if(searchWords[0][0] == '-'){
            set<string> result0;
            set<string> newResult;
            searchWords[0].erase(0,1);
            for(auto e : index){
                if(e.first == searchWords[0]){
                    for(auto d : e.second){
                        result0.emplace(d);
                    }
                }               
                
            }
            set_difference(result.begin(), result.end(), result0.begin(), result0.end(), inserter(newResult, newResult.begin()));
            result = newResult;
        }  
    }

    cout << "Found " << result.size() << " matching pages" << endl;

    for(auto e : result){
        cout << e << endl;
    }
    
    return result;
}

/*
This function runs the search engine. It takes in input
until the input is the enter key. It displays the amount
of pages processed, the amount of words processed, and
finally displays all the pages matching the users input.
*/
void searchEngine(string filename) {
    map<string, set<string>> index;
    string sentence;
    
    // set<string> stopWords = {"A","ABOUT","ACTUALLY","ALMOST","ALSO","ALTHOUGH","ALWAYS","AM","AN",
    //                          "AND","ANY","ARE","AS","AT","BE","BECAME","BECOME","BUT","BY","CAN",
    //                          "COULD","DID","DO","DOES","EACH","EITHER","ELSE","FOR","FROM","HAD","HAS",
    //                          "HAVE","HENCE","HOW","I","IF","IN","IS","IT","ITS","JUST","MAY","MAYBE","ME",
    //                          "MIGHT","MINE","MUST","MY","MINE","MUST","MY","NEITHER","NOR","NOT","OF","OH",
    //                          "OK","WHEN","WHERE","WHEREAS","WHEREVER","WHENEVER","WHETHER","WHICH","WHILE",
    //                          "WHO","WHOM","WHOEVER","WHOSE","WHY","WILL","WITH","WITHIN","WITHOUT","WOULD",
    //                          "YES","YET","YOU","YOUR"};
    // for(auto e : stopWords){
    //     for(int i = 0; i < e.size(); i++){
    //         e[i] = tolower(e[i]);
            
    //     }
    // }

    int pagesIndexed = buildIndex(filename, index);
    int wordsChecked = 0;
    for(auto e : index){
        wordsChecked++;
    }

    // for(auto e : index){
    //     if(stopWords.count(e.first)){
    //         cout << "YES" << endl;
    //     }
    // }

    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << pagesIndexed << " pages containing ";
    cout << wordsChecked << " unique terms\n" << endl;
    cout << "Enter query sentence (press enter to quit): ";
    getline(cin, sentence);
    while(sentence != ""){
        findQueryMatches(index, sentence);
        cout << "\nEnter query sentence (press enter to quit): ";
        getline(cin, sentence);
    }
    cout << "Thank you for searching!" << endl;
}

