#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// Global Variables
string input;
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
vector<string> getWordsVector();


// Function to divide the input into keywords and put in vector
vector<string> getWordsVector(){
    // Initialise stringstream with the object.
    stringstream ss(input);
    string word;
    // Initialise a vector
    vector<string> keyWord;
    // While loop to extract the words 
    while (ss >> word){
        // Add to the vector
        keyWord.push_back(word);
    }

    return keyWord;
}

// Function to parse Program non-terminal
bool parseProgram(){
    // Check if the input string has words
    while(input.length() > 0){
        // Check if the statement is valid
        parseStatement();
        // Return true
        return true;
    }
    // else return false
    return false;
}

// Function to parse the statements of the program(Check validity)
bool parseStatement() {
    // Check the first keyword of a statement is valid before diving in
    // Check if first keyword is append
    if(input[0] == "append"){
        // Parse append statement to check the validity
        return parseAppendStatement();
    }
}

int main() {
    cout << "Enter a string: ";
    cin >> input;
    cout << "Input: " << input << endl;

    if (parseProgram() && position == input.length()) {  // Check if the whole input is parsed
        cout << "Success: The string belongs to the language." << endl;
    } else {
        cout << "Error: The string does not belong to the language." << endl;
    }

    return 0;
}
