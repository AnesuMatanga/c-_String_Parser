#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// Global Variables
string input;
// Vector with keywords
vector<string> keyWords;
// Current position
size_t position = 0;  

// Function prototypes for the EBNF Tokens in bold
bool parseProgram();
bool parseStatement();
bool parseExpression();
bool parseValue();

// Function prototypes for the Lexer tokens first keywords
bool parseAppendStatement();
bool parseListStatement();

// Function prototype to get a vector of words from string
void getWordsVector();


// Function to divide the input into keywords and put in vector
void getWordsVector(){
    // Initialise stringstream with the object.
    stringstream ss(input);
    string word;

    // While loop to extract the words 
    while (ss >> word){
        // Add to the vector
        keyWords.push_back(word);
    }
}

// Function to parse Program non-terminal
bool parseProgram(){
    // Check if the input string has words
    while(input.length() > 0){
        //Put the Statement keywords into vector
        getWordsVector();
        // Check if the statement is valid
        return parseStatement();
    }
    // else return false
    return false;
}

// Function to parse the statements of the program(Check validity)
bool parseStatement() {
    // Check the first keyword of a statement is valid before diving in
    // Check if first keyword is append
    string firstKeyword = keyWords.front();
    cout <<"keyWords Front = " << firstKeyword << endl;
    if(firstKeyword == "append"){
        // Parse append statement to check the validity
        return parseAppendStatement();
    } else if(firstKeyword == "list"){
        // Parse list statement to check validity
        return parseListStatement();
    } else {
        // Unexpected token, first keyword not valid
        return false;
    }
}

// Function to parse the append Statement
bool parseAppendStatement(){
    // Append then id then expression and end
    //Check if second keyword is what its supposed to be
    //Remove front() first by erasing

    //DEBUG
    cout <<"Keywords: " << endl;
    for (const auto& keyword: keyWords){
        cout << keyword << " ";
    }
    cout <<"parseAppendStatement" << endl;
    cout <<"Front keyword b4 erasing: " << keyWords.front() << endl;
    keyWords.erase(keyWords.begin());
    cout <<"Front keyword AFTER erasing: " << keyWords.front() << endl;

    string token = keyWords.front();

    //Check if its id
    if (token == "id"){
        //Should be followed by expression which can be recursive
        //Remove the id and check the next expression
        keyWords.erase(keyWords.begin());

        // For now, lets pretend the expression will always be any word 
        // Pop again to check the last keyword
        keyWords.erase(keyWords.begin());
        
        // Now check if the current keyword is valid
        if(keyWords.front() == ";"){
            // Reached the end of the statement
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

// Function to parse list statement
bool parseListStatement(){
    return true;
}

int main() {
    cout << "Enter a string: ";
    getline(cin, input);
    cout << "Input: " << input << endl;

    if (parseProgram()) {  // Check if the whole input is parsed
        cout << "Success: The string belongs to the language." << endl;
    } else {
        cout << "Error: The string does not belong to the language." << endl;
    }

    return 0;
}
