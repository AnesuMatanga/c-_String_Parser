#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

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
bool getWordsVector();

// Other Helper Functions Prototypes
void emptyVector();
bool isSpecialCharacter(char c);
bool isWordValid(string word);

// Function to define special characters of the language
bool isSpecialCharacter(char c){
    // Special characters
    string specialCharacters = "~`!@#$%^&*()_+=-{}[]|/;:',.<>?\\";

    // Check if char c is a special char using find
    bool isSpecial = specialCharacters.find(c);

    return isSpecial;
}

// Function to check the validity of a word in the string literal
bool isWordValid(string word){
    bool isValid = true;
    
    // Get the length of the word
    int wordLength = word.length();

    // Iterate through each char in word
    for (int i = 0; i < wordLength; i++){
        // Use isalpha from cctype to check word char is alpha
        if(!isalpha(word[i])){
            std::cout <<"Word: " << word[i] << " is not Alpha." << endl;
            // Check if char is special
            if(isSpecialCharacter(word[i]) && word[i] != '"'){
                cout <<"Word[i] Special = " << word[i] << endl;
                // Check if the special char escape clause is first '\'
                if(word[i] != '\\'){
                    // Now check if the char before it is escape clause
                    if(word[i - 1] != '\\'){
                        // Word is not valid
                        isValid = false;
                        std::cout <<"Error: missing escape clause for special char" << word[i] << " in string literal" << endl;
                        return isValid;
                    }
                } else if(word[i] == '\\'){
                    // Now check that there is another special char infront
                    if(isSpecialCharacter(word[i + 1])){
                        isValid = true;
                    } else {
                        //Escape clause is not followed by sepcial char
                        isValid = false;
                        std::cout <<"Error: Escape clause in string literal not followed by any special char" << endl;
                        return isValid;
                    }
                }
            }
        }
    }
    cout <<"Word is Valid?: " << isValid << endl;
    return isValid;
}


// Function to divide the input into keywords and put in vector
bool getWordsVector(){
    // Initialise stringstream with the object.
    stringstream ss(input);
    string word;
    string stringLiteral = "";
    int countHiphens = 0;
    bool isStringLiteral = false;
    bool isValidWord = true;
    
    std::cout << endl;
    std::cout <<"getWordsVector()" << endl;
    std::cout <<"Input Length: " << input.length() << endl;

    // While loop to extract the words 
    while (ss >> word){
        std::cout <<"word[0]: " << word[0] << endl;
        std::cout <<"word: " << word << endl;
        std::cout <<"Word.length: " << word.length() << endl; 
        // Check if in the nput there is a string literal identified by ""

        // If expression a string literal
        if(isStringLiteral) {
            // Add the word to the string literal
            stringLiteral += word + " ";
            // Check if the word ends with unescaped quote
            // String is same as vector so can access it that way
            if(word.back() == '"' && (word.length() == 1 || word[word.length() - 2] != '\\')) {
                // Add to the string Literal, add to keyWords Vector
                keyWords.push_back(stringLiteral);
                // Reached the end of literal, so put isStringLiteral to false
                isStringLiteral = false; 
            }
        } else {
            // Check if the word starts with quote "
            if(word.front() == '"'){
                // Start of stringLiteral so put to true
                isStringLiteral = true;
                stringLiteral = word + " ";
                // Special case, check for string literal with only one word
                if(word.back() == '"' && word.length() > 1) {
                    keyWords.push_back(stringLiteral);
                    // Stop looping 
                    isStringLiteral = false;
                }
            } else {
                // Add regular words(commands like append) to the vector
                keyWords.push_back(word);
            }
        }
    }

    // If isStringLiteral true after whileLoop, means string not closed in quotes
    if(isStringLiteral){
        // Error, string literal incorrectly inputted
        cout <<"Error: String literal not enclosed within quotes." << endl;
        isValidWord = false;
    }
    return isValidWord;
}

// Function to parse Program non-terminal
bool parseProgram(){
    //Put the Statement keywords into vector
    bool isValid = getWordsVector();

    if(!isValid){
        return false;
    }
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
    std::cout << endl;
    std::cout <<"parseStatement()" << endl;
    std::cout <<"Keywords: " << endl;
    for (const auto& keyword: keyWords){
        std::cout << keyword << " ";
    }
    string firstKeyword = keyWords.front();
    std::cout <<"keyWords Front = " << firstKeyword << endl;
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
        std::cout <<"Error: Keyword " << firstKeyword <<" unrecognized" << endl;
        return false;
    }
}

// Function to parse the append Statement
bool parseAppendStatement(){
    // Append then id then expression and end
    //Check if second keyword is what its supposed to be
    //Remove front() first by erasing

    //DEBUG
    std::cout <<"Keywords: " << endl;
    for (const auto& keyword: keyWords){
        std::cout << keyword << " ";
    }
    std::cout <<"parseAppendStatement" << endl;
    std::cout <<"Front keyword b4 erasing: " << keyWords.front() << endl;
    keyWords.erase(keyWords.begin());
    std::cout <<"Front keyword AFTER erasing: " << keyWords.front() << endl;

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
            std::cout <<"Error: Expected end of statement identifier" << endl;
            return false;
        }
    } else {
        // Error Handling
        std::cout <<"Error: Expected 'id' identifier" << endl;
        return false;
    }
}

// Function to empty the vector
void emptyVector(){
    while(!keyWords.empty()){
        keyWords.pop_back();
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
    input = "";
    while (input != "exit;"){
        std::cout << "Enter a string: ";
        getline(cin, input);
        std::cout << "While Input: " << input << endl;

        if (parseProgram()) {  // Check if the whole input is parsed
            std::cout << "Success: The string belongs to the language." << endl;
        } else {
            std::cout << "Error: The string does not belong to the language." << endl;
        }

        // Empty Vector
        emptyVector();
    }

    return 0;
}
