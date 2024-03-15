#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// Global Variables
string input;
// Vector with keywords
vector<string> keyWords;
// Vector to keep record of ids variable name
vector<string> ids;
// Vector to keep track of constants variable names
vector<string> constants;
// Vector to keep track of the literal
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
bool parseExitStatement();
bool parsePrintStatement();
bool parsePrintLengthStatement();
bool parsePrintWordsStatement();
bool parsePrintWordCountStatement();
bool parseSetStatement();
bool parseReverseStatement();

// Function prototype to get a vector of words from string
void getWordsVector();


// Function to divide the input into keywords and put in vector
void getWordsVector(){
    // Initialise stringstream with the object.
    stringstream ss(input);
    string word;
    string stringLiteral = "";
    int countHiphens = 0;
    bool isStringLiteral = false;
    
    cout << endl;
    cout <<"getWordsVector()" << endl;

    // While loop to extract the words 
    while (ss >> word){
        cout <<"word[0]: " << word[0] << endl;
        cout <<"word: " << word << endl;
        cout <<"Word.length: " << word.length() << endl; 
        // Check if in the nput there is a string literal identified by ""
        if (((word[0] == '"') || (word[word.length() - 1] == '"')) || ((countHiphens > 0) && (countHiphens < 2))){
            // Count the hiphens until you know its the end (2 hiphens)
            if ((word[0] == '"') || (word[word.length() - 1] == '"')){
                countHiphens++;
            }
            cout <<"Is StringLiteral word: " << word << endl;
            stringLiteral = stringLiteral + word + " ";
            cout <<"StringLiteral: " << stringLiteral << endl;
        } else if(countHiphens == 0){
            // Add to the vector
            cout <<"Is a Keyword: " << word << endl;
            keyWords.push_back(word);
        } else {
            cout <<"2 Hiphens Counted. Adding sentence: " << stringLiteral << endl; 
            isStringLiteral = true;
            // Push back the whole string
            cout <<"stringLiteral = " << stringLiteral << endl;
            keyWords.push_back(stringLiteral);
            // Reset so it can continue adding words
            countHiphens = 0;
        }

        
    }
}

// Function to parse Program non-terminal
bool parseProgram(){
    //Put the Statement keywords into vector
    getWordsVector();
    // Check if the keyWords still has words
    while(!keyWords.empty()){
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
    //DEBUG
    cout << endl;
    cout <<"parseStatement()" << endl;
    cout <<"Keywords: " << endl;
    for (const auto& keyword: keyWords){
        cout << keyword << " ";
    }
    string firstKeyword = keyWords.front();
    cout <<"keyWords Front = " << firstKeyword << endl;
    if(firstKeyword == "append"){
        // Parse append statement to check the validity
        return parseAppendStatement();
    } else if(firstKeyword == "list"){
        // Parse list statement to check validity
        return parseListStatement();
    } else if(firstKeyword == "exit"){
        // Parse exit statement to check validity
        return parseExitStatement();
    } else if(firstKeyword == "print"){
        // Parse print statement to check validity
        return parsePrintStatement();
    } else if(firstKeyword == "printlength"){
        // Parse printlength list statement to check validity
        return parsePrintLengthStatement();
    } else if(firstKeyword == "printwords"){
        // Parse printwords statement to check validity
        return parsePrintWordsStatement();
    } else if(firstKeyword == "printwordcount"){
        // Parse printwordcount statement to check validity
        return parsePrintWordCountStatement();
    } else if(firstKeyword == "set"){
        // Parse set statement to check validity
        return parseSetStatement();
    } else if(firstKeyword == "reverse"){
        // Parse reverse statement to check validity
        return parseReverseStatement();
    } else {
        // Unexpected token, first keyword not valid
        // Error Handling
        cout <<"Error: Keyword " << firstKeyword <<" unrecognized" << endl;
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
        

        // Pop again to check the last end of statement keyword
        if (!keyWords.empty()){
            keyWords.erase(keyWords.begin());
        }
        
        // Now check if the current keyword is valid
        if(keyWords.front() == ";"){
            // Reached the end of the statement
            return true;
        } else {
            // Error Handling
            cout <<"Error: Expected end of statement identifier" << endl;
            return false;
        }
    } else {
        // Error Handling
        cout <<"Error: Expected 'id' identifier" << endl;
        return false;
    }
}

// Function to parse list statement
bool parseListStatement(){
    return true;
}

// Function to parse list statement
bool parseExitStatement(){
    return true;
}

// Function to parse list statement
bool parsePrintStatement(){
    return true;
}

// Function to parse list statement
bool parsePrintLengthStatement(){
    return true;
}

// Function to parse list statement
bool parsePrintWordsStatement(){
    return true;
}

// Function to parse list statement
bool parsePrintWordCountStatement(){
    return true;
}

// Function to parse list statement
bool parseSetStatement(){
    return true;
}

// Function to parse list statement
bool parseReverseStatement(){
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
