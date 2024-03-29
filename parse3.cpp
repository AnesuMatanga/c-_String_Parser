#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <regex>
#include <algorithm>

using namespace std;

// Declaring an id struct to store the value of ids
struct Ids {
    string idName;
    string value;
};

// Declaring a struct to keep track of the types in an expression
struct Expressions {
    string type;
    string exprName;
};

// Global Variables
string input;
// Vector with keywords
vector<string> keyWords;
// Vector to keep record of ids variable name
vector<Ids> ids;
// Vector to store language constants
vector<string> constants;
// Vector to store expression word by word
vector<Expressions> expression;
// Vector to store words to later reverse
vector<string> words;

// Current position
size_t position = 0; 
// nextToken variable to work with parseExpression
string nextToken = "";
// final manipulated string according to constants/ id or just literal
string finalString;
// variable to check if user wants to exit
bool isExit = false;

// Function prototypes for the EBNF Tokens in bold
bool parseProgram();
bool parseStatement();
bool parseExpression();
bool parseValue(string value);

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
//void emptyVector();
bool isSpecialCharacter(char c);
bool isWordValid(string word);
bool isIdentifierValid(string identifier);
bool checkIdentifierExists(string id);
void declareConstants();
void getNextToken();
bool accept(string expected);
void printWords(const string& str);
void countWords(const string& str);
string resolveExpression();

// Global variables to help figure out if expression is either id|constant|literal
bool exprIsID = false;
bool exprIsLiteral = false;
bool exprIsConstant = false;

// Function to get next token in an expression for validation
void getNextToken() {
    // Get next token
    if(!keyWords.empty() && nextToken != ";"){
        nextToken = keyWords.front();
        // Now erase token from the keyWords table
        keyWords.erase(keyWords.begin());
    } else {
        nextToken = "\0";
    }
}

// Function to work with parseExpression by accepting/checking if after value, next char is +
bool accept(string expected){
    std::cout << endl;
    std::cout <<"accept()" << endl;
    std::cout << endl;

    // Get next token after value which should be + and return true
    getNextToken();
    std::cout <<"1: accepted getNextToken = " << nextToken << endl;
    if(nextToken != expected) {
        return false;
    }

    return true;
}

// Function to Print words utilizing the regex library for specific word pattern matching
void printWords(const string& str){
    // Declaring regex object with the pattern
    // Recall where you learnt this: https://cplusplus.com/reference/regex/ECMAScript/
    // \\b is for word boundary and pattern matches words that contain ' and -
    regex word_regex(R"(\b['\w-]+\b)");

    // Will use a regex iterator to be able to count words which match pattern
    auto words_begin = sregex_iterator(str.begin(), str.end(), word_regex);
    auto words_end = sregex_iterator();

    //cout <<"Found " << distance(words_begin, words_end) << " words" << endl;
    std::cout <<"Words are: " << endl;
    // Now iterate over all matches and print each word
    for (sregex_iterator i = words_begin; i != words_end; i++){
        smatch match = *i;
        std::cout << match.str() << endl;
    }
}

// Function to count the number of words which match the word boundary for the language
// Using const to avoid modifying the string and & to avoid copying for performance
void countWords(const string& str){
    // \\b is for word boundary and pattern matches words that contain ' and -
    regex word_regex(R"(\b['\w-]+\b)");

    // Will use a regex iterator to be able to count words which match pattern
    auto words_begin = sregex_iterator(str.begin(), str.end(), word_regex);
    auto words_end = sregex_iterator();

    std::cout <<"Wordcount is: " << distance(words_begin, words_end) << endl;
}

// Function to help resolve an expression to get a final string
string resolveExpression(){

    string manipulatedString = "";
    string expressionWord;
    string expressionType;
    while(!expression.empty()){
        expressionWord = expression.front().exprName;
        expressionType = expression.front().type;

        // Now pop the expressionWord & Type off the table
        expression.erase(expression.begin());

        // Now append since statement is valid
        if (expressionType == "id"){    
           std::cout << "NOT String Literal/constant, so ID" << endl;
            // Add id.value to string
            for (auto &idObj: ids) {
                if(idObj.idName == expressionWord){
                    std::cout <<"idObj.value id = "<< idObj.value << endl;
                    // Append contents
                    manipulatedString += idObj.value;
                }
            }
                        
        } else if (expressionType == "literal"){
            // Just add to the string
            manipulatedString += expressionWord;
        } else if (expressionType == "constant"){
            if(expressionWord == "SPACE"){
                manipulatedString += " ";
            }
            if(expressionWord == "TAB"){
                 manipulatedString += "\t";
            }
            if (expressionWord == "NEWLINE"){
                manipulatedString += "\n";
            }
        }
    }
    return manipulatedString;
}

// Function to Declaring language constants and storing them in constants vector
void declareConstants() {
    // Add the 3 language constants to the vector
    constants.push_back("SPACE");
    constants.push_back("TAB");
    constants.push_back("NEWLINEW");
}

// Function to check if id has been set
bool checkIdentifierExists(string id){
    bool doesExist = false;

    std::cout << endl;
    std::cout <<"checkIdentifierExists" << endl;
    std::cout << endl;

    for(const auto& idNm: ids){
        std::cout <<"idName: " << idNm.idName << endl;
    }

    // Use for loop to traverse the vector
    for(const auto& idName: ids){
        // Check if its in the vector
        if(idName.idName == id){
            doesExist = true;
        }
    }

    // Error message if idName not matched
    if(!doesExist){
        std::cout <<"Error: Stated identifier invalid or not set." << endl;
    }

    return doesExist;
}

// Function to define special characters of the language
bool isSpecialCharacter(char c){
    // Special characters
    string specialCharacters = "~`!@#$%^&*()_+=-{}[]|/;:',.<>?\\";

    // Check if char c is a special char using find
    bool isSpecial = specialCharacters.find(c);

    return isSpecial;
}

// Function to validate the identifier using the regex library
bool isIdentifierValid(string identifier){
    bool isValid = true;

    std::cout << endl;
    std::cout << "isIdentifierValid" << endl;
    std::cout << endl;
    // Declare the regex object id_regex
    const regex id_regex("[a-zA-Z][a-zA-Z0-9]*");

    // Check if there is a match with the pattern using regex_match
    if(!regex_match(identifier, id_regex)){
        // Error
        isValid = false;
        std::cout <<"Error: Invalid, Identifier doesnt match correct pattern" << endl;
    } else {
        std::cout <<"Identifier pattern CORRECT" << endl;
    }
    
    return isValid;

}

// Function to check the validity of a word in the string literal
bool isWordValid(string word){
    bool isValid = true;

    std::cout << endl;
    std::cout <<"isWordValid()" << endl;
    std::cout << endl;

    // Declare & instantiate string literal expression regex object
    //const regex expression_regex(R"("[a-zA-Z0-9\s@!~`#$%^&*()_\-+={}\[\]|/><,.;:?']*(\\\")?[a-zA-Z0-9\s@!~`#$%^&*()_\-+={}\[\]|/><,.;:?']*")");
    // Try this pattern "([a-zA-Z0-9\s@!~`#$%^&*()_\-+={}\[\]|\/><,.;:?']*(\\\")?)*"
    
    // To recall where i learnt this: https://cplusplus.com/reference/regex/ECMAScript/
    // ^ for negating the target chars like " and backslash
    // OR except any char after backslash which is what . stands for
    const regex expression_regex(R"("([^"\\]|\\.)*")");

    // Check if the expression valid, matches the pattern
    if(!regex_match(word, expression_regex)){
        isValid = false;
        // Error, invalid stringLiteral
        std::cout <<"Error: String Literal invalid, doesnt match pattern" << endl;
    } else {
        std::cout <<"String Literal expression CORRECT" << endl;
    }
    std::cout <<"Word is Valid?: " << isValid << endl;
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
    bool isStringValid = false;
    exprIsLiteral = false;
    exprIsConstant = false;
    exprIsID = false;
    
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
            // Check if the last word is right next to ; without space
            if(word.back() == ';' && word.length() > 1 && word[word.length() - 2] == '"'){
                // Update exprIsLiteral global var to indicate expression is literal
                exprIsLiteral = true;
                // Split the last word into two strings, for ; to be added to vector
                stringLiteral += word.substr(0, word.length() - 2);
                // Check if String Literal is valid fiorst before adding to vector
                isStringValid = isWordValid(stringLiteral);
                // Now take the substr without the string open and close quotes
                stringLiteral = stringLiteral.substr(1, stringLiteral.length() - 1);
                std::cout <<"STRINGLITERAL Without quotes: " << stringLiteral << endl;
                // Add the substrings to keyWord vector
                keyWords.push_back(stringLiteral);
                keyWords.push_back(";");
                 // Reached the end of literal, so put isStringLiteral to false
                isStringLiteral = false;
                // Check if the word ends with unescaped quote
                // String is same as vector so can access it that way
            } else if(word.back() == '"' && (word.length() == 1 || word[word.length() - 2] != '\\')) {
                // Update exprIsLiteral global var to indicate expression is literal
                exprIsLiteral = true;
                // Add to the string Literal, add to keyWords Vector
                stringLiteral += word;
                std::cout <<"# StringLiteralLength: " << stringLiteral.length() << endl;
                keyWords.push_back(stringLiteral.substr(1, stringLiteral.length()-2));
                // Reached the end of literal, so put isStringLiteral to false
                isStringLiteral = false; 
            } else {
                // Add the word to the string literal
                stringLiteral += word + " ";
            } 
        } else {
            // Check if the word starts with quote "
            if(word.front() == '"'){

                // Start of stringLiteral so put to true
                isStringLiteral = true;
                //stringLiteral = word + " ";
                std::cout <<"Word.length() - 2 = " << word[word.length() - 2] << endl;
                // Special case, check for string literal with only one word
                if((word.back() == '"' || (word.back() == ';' && word[word.length() - 2] == '"')) && word.length() > 1) {
                     // Split the last word into two strings, for ; to be added to vector
                    stringLiteral += word.substr(0, word.length() - 1);
                    // Check if String Literal is valid fiorst before adding to vector
                    isStringValid = isWordValid(stringLiteral);
                    // Now take the substr without the string open and close quotes
                    stringLiteral = stringLiteral.substr(1, stringLiteral.length() - 2);
                    std::cout <<"STRINGLITERAL Without quotes: " << stringLiteral << endl;
                    // Add the substrings to keyWord vector
                    keyWords.push_back(stringLiteral);
                    keyWords.push_back(";");
                    // Stop looping 
                    isStringLiteral = false;
                } else {
                    stringLiteral = word.substr() + " ";
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
        std::cout <<"Error: String literal not enclosed within quotes." << endl;
        isValidWord = false;
    }

    cout <<"isExprLiteral?: " << exprIsLiteral << endl;
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

// Function to parse Expression Recursively. Recursive Descent parser
// Using expression := value + Expression | value (for recursiveness) 
bool parseExpression(){
    std::cout << endl;
    std::cout <<"parseExpression()" << endl;
    std::cout << endl;

    // First Check if the nextToken is valid
    getNextToken();
    std::cout <<"1: Outside if getNextToken = " << nextToken << endl;

    // Base Case
    if(!parseValue(nextToken)){
        std::cout <<"Error: Value Invalid, missing value after + sign to append" << endl;
        // token not valid
        return false;
    }

    // Check if there are more tokens & if the next token is a +
    if(!keyWords.empty() && keyWords.front() == "+"){
        // Consume the '+' token
        getNextToken();
        std::cout <<"2: Consumed + next token = " << nextToken << endl;

        // Recursively parseExpression since there is a + to make sure its followed by value
        return parseExpression();
    }

    // If there is no +, reached the end of the expression
    return true;
}

// Function to parseValue by checking if the value is a valid value or not
bool parseValue(string value){
    std::cout << endl;
    std::cout <<"parseValue()" << endl;
    std::cout << endl;
    std::cout <<"Next Token = " << nextToken << endl;

    // Declare a struct to store the expression types
    Expressions express;
    // Check if value is string (For now just testing with literals)
    if(exprIsLiteral){
        std::cout <<"Is String" << endl;
        // Add string to expressions table for later manipulation
        express.type = "literal";
        express.exprName = value;
        expression.push_back(express);
        return true;
    } else if (value == "SPACE" || value == "TAB" || value == "NEWLINE"){
        exprIsConstant = true;
        std::cout <<"Is Constant" << endl;
        express.type = "constant";
        express.exprName = value;
        expression.push_back(express);
        return true;
    } else if(checkIdentifierExists(value)){
        std::cout <<"Is ID" << endl;
        exprIsID = true;
        express.type = "id";
        express.exprName = value;
        expression.push_back(express);
        return true;
    }

    return false;
}

// Function to parse the append Statement
bool parseAppendStatement(){
    // Append then id then expression and end
    //Check if second keyword is what its supposed to be
    //Remove front() first by erasing

    std::cout << endl;
    std::cout <<"parseAppendStatement()" << endl;
    std::cout << endl;

    //DEBUG
    std::cout <<"Keywords: " << endl;
    for (const auto& keyword: keyWords){
        std::cout << keyword << " ";
    }
    std::cout << endl;
    std::cout <<"Front keyword b4 erasing: " << keyWords.front() << endl;
    keyWords.erase(keyWords.begin());
    std::cout <<"Front keyword AFTER erasing: " << keyWords.front() << endl;

    string token = keyWords.front();
    // Declare a struct to store the ids
    string id = "";
    string express = "";
    // Check if its id is valid
    if (!keyWords.empty() && isIdentifierValid(keyWords.front())){
        std::cout <<"ID: " << keyWords.front() << endl;
        // Should be followed by expression which can be recursive

        // Save the id
        id = keyWords.front();
        // Now check if Id is valid whose value going to be appended to
        bool idExists = checkIdentifierExists(id);
        if(!idExists){
            return false;
        }

        // Remove the id and check the next expression
        keyWords.erase(keyWords.begin());

        std::cout <<"Expression: " << keyWords.front() << endl;
        std::cout <<"isExprLiteral?: " << exprIsLiteral << endl;
        // For now, check the expression is valid 
        if (!keyWords.empty()){
            express = keyWords.front();
            // Remove the expression from the vector
            //keyWords.erase(keyWords.begin());


            // Declare strings to hold expression name and type
            string expressionWord;
            string expressionType;
            // Call parseExpression to pass the expression
            // Now check if the current keyword is valid
            if(keyWords.back() == ";" && parseExpression()){
                // If exprIsLiteral is false, it means expression is either ID/Constant
                // Check if expression ID/Constant, for now lets pretend if not literal
                // Get first word in expression and check its type to manipulate
                while(!expression.empty()){
                    expressionWord = expression.front().exprName;
                    expressionType = expression.front().type;

                    // Now pop the expressionWord and type off the table
                    expression.erase(expression.begin());

                    // Now append since statement is valid 
                    if (expressionType == "id"){
                        std::cout << "NOT String Literal/constant, so ID" << endl;
                        for(auto &idName: ids){
                            if(idName.idName == expressionWord){
                                std::cout <<"idName.value express = "<< idName.value << endl;
                                for(auto &idObj: ids){
                                    if(idObj.idName == id){
                                        std::cout <<"idObj.value id = "<< idObj.value << endl;
                                        // Append contents
                                        idObj.value += idName.value;
                                    }
                                }
                            }
                        }
                    } else if (expressionType == "literal"){
                        // use & to alter the actual object
                        for (auto &idName: ids){
                            if (idName.idName == id){
                                // Concatenta new expression to existing idValue
                                idName.value += expressionWord;
                            
                            }
                        }
                    } else if (expressionType == "constant"){
                        // use & to alter the actual object
                        for (auto &idName: ids){
                            if (idName.idName == id){
                                // Concatenta new expression to existing idValue
                                if(expressionWord == "SPACE"){
                                    idName.value += " ";
                                }
                                if(expressionWord == "TAB"){
                                    idName.value += "\t";
                                }
                                if(expressionWord == "NEWLINE"){
                                    idName.value += "\n";
                                }

                            }
                        }
                    }
                }
            
                // Reached the end of the statement
                return true;
            } else {
                // Error Handling
                std::cout <<"Error: Expected end of statement identifier" << endl;
                return false;
            }
        }
    } else {
        // Error Handling
        std::cout <<"Error: Expected 'id' identifier" << endl;
        return false;
    }
        
        // Pop again to check the last end of statement keyword
        //if (!keyWords.empty()){
            //keyWords.erase(keyWords.begin());
        //} 
        return false;    
}


// Function to empty any vector
template <typename T>
void emptyVector(vector<T>& vec){
    // Empty vector
    vec.clear();
}

// Function to parse list statement
bool parseListStatement(){
    // Check if ids list not empty first
    if(!ids.empty()){
        // List everything
        cout <<"Identififer list(" << ids.size() <<"):" << endl;
        for(const auto &id: ids){
            cout <<id.idName <<": " <<"\"" <<id.value <<"\"" << endl;
        }
        return true;
    }
    //Error
    cout <<"Error: No Id's set yet, empty list" << endl;
    
    return false;
}

// Function to parse list statement
bool parseExitStatement(){
    std::cout << endl;
    std::cout <<"parseExitStatement()" << endl;
    std::cout << endl;

    //DEBUG
    std::cout <<"Keywords: " << endl;
    for (const auto& keyword: keyWords){
        std::cout << keyword << " ";
    }
    std::cout << endl;
    std::cout <<"Front keyword b4 erasing: " << keyWords.front() << endl;
    keyWords.erase(keyWords.begin());
    std::cout <<"Front keyword AFTER erasing: " << keyWords.front() << endl;

    if(!keyWords.empty() && keyWords.front() == ";" && keyWords.size() == 1){
        isExit = true;
        return true;
    }
    // Error
    cout <<"Error: Exit Statement invalid" << endl;
    return false;
}

// Function to parse Print statement
bool parsePrintStatement(){

    std::cout << endl;
    std::cout <<"parsePrintStatement()" << endl;
    std::cout << endl;

    //DEBUG
    std::cout <<"Keywords: " << endl;
    for (const auto& keyword: keyWords){
        std::cout << keyword << " ";
    }
    std::cout << endl;
    std::cout <<"Front keyword b4 erasing: " << keyWords.front() << endl;
    keyWords.erase(keyWords.begin());
    std::cout <<"Front keyword AFTER erasing: " << keyWords.front() << endl;

    string token = keyWords.front();
    string manipulatedString = "";
    // Declare a struct to store the ids
    struct Ids id;
    //Check if its id is valid
    if (!keyWords.empty()){
        //Should be followed by expression which can be recursive
        std::cout <<"Expression: " << keyWords.front() << endl;
        // For now, check the expression is valid 
        if (!keyWords.empty()){

            // Call parseExpression to pass the expression
            if(keyWords.back() == ";" && parseExpression()){
                // Check if expression ID/Constant, for now lets pretend if not literal
                // Get first word in expression and check its type to manipulate
            
                // Now print the words which conform to a specific pattern utilizing the regex library
                manipulatedString = resolveExpression();
                // Now printLength
                cout << manipulatedString << endl;

                // Reached the end of the statement
                return true;
            } else {
                // Error Handling
                std::cout <<"Error: Expected end of statement identifier" << endl;
                return false;
            }
        }
    } else {
        // Error Handling
        std::cout <<"Error:Missing Expression after printwords" << endl;
        return false;
    }
    return true;   
}

// Function to parse list statement
bool parsePrintLengthStatement(){

    std::cout << endl;
    std::cout <<"parsePrintLengthStatement()" << endl;
    std::cout << endl;

    //DEBUG
    std::cout <<"Keywords: " << endl;
    for (const auto& keyword: keyWords){
        std::cout << keyword << " ";
    }
    std::cout << endl;
    std::cout <<"Front keyword b4 erasing: " << keyWords.front() << endl;
    keyWords.erase(keyWords.begin());
    std::cout <<"Front keyword AFTER erasing: " << keyWords.front() << endl;

    string token = keyWords.front();
    string manipulatedString = "";
    // Declare a struct to store the ids
    struct Ids id;
    //Check if its id is valid
    if (!keyWords.empty()){
        //Should be followed by expression which can be recursive
        std::cout <<"Expression: " << keyWords.front() << endl;
        // For now, check the expression is valid 
        if (!keyWords.empty()){

            // Call parseExpression to pass the expression
            if(keyWords.back() == ";" && parseExpression()){
                // Check if expression ID/Constant, for now lets pretend if not literal
                // Get first word in expression and check its type to manipulate
            
                // Now print the words which conform to a specific pattern utilizing the regex library
                manipulatedString = resolveExpression();
                // Now printLength
                cout <<"Length is: " << manipulatedString.length() << endl;

                // Reached the end of the statement
                return true;
            } else {
                // Error Handling
                std::cout <<"Error: Expected end of statement identifier" << endl;
                return false;
            }
        }
    } else {
        // Error Handling
        std::cout <<"Error:Missing Expression after printwords" << endl;
        return false;
    }
    return true;
}

// Function to parse list statement
bool parsePrintWordsStatement(){

    std::cout << endl;
    std::cout <<"parsePrintWordsStatement()" << endl;
    std::cout << endl;

    //DEBUG
    std::cout <<"Keywords: " << endl;
    for (const auto& keyword: keyWords){
        std::cout << keyword << " ";
    }
    std::cout << endl;
    std::cout <<"Front keyword b4 erasing: " << keyWords.front() << endl;
    keyWords.erase(keyWords.begin());
    std::cout <<"Front keyword AFTER erasing: " << keyWords.front() << endl;

    string token = keyWords.front();
    string manipulatedString = "";
    // Declare a struct to store the ids
    struct Ids id;
    //Check if its id is valid
    if (!keyWords.empty()){
        //Should be followed by expression which can be recursive
        std::cout <<"Expression: " << keyWords.front() << endl;
        // For now, check the expression is valid 
        if (!keyWords.empty()){

            // Call parseExpression to pass the expression
            if(keyWords.back() == ";" && parseExpression()){
                // Check if expression ID/Constant, for now lets pretend if not literal
                // Get first word in expression and check its type to manipulate
            
                // Now print the words which conform to a specific pattern utilizing the regex library
                manipulatedString = resolveExpression();
                printWords(manipulatedString);

                // Reached the end of the statement
                return true;
            } else {
                // Error Handling
                std::cout <<"Error: Expected end of statement identifier" << endl;
                return false;
            }
        }
    } else {
        // Error Handling
        std::cout <<"Error:Missing Expression after printwords" << endl;
        return false;
    }
           
    return true;
}


// Function to parse list statement
bool parsePrintWordCountStatement(){

    std::cout << endl;
    std::cout <<"parsePrintWordCountStatement()" << endl;
    std::cout << endl;

    //DEBUG
    std::cout <<"Keywords: " << endl;
    for (const auto& keyword: keyWords){
        std::cout << keyword << " ";
    }
    std::cout << endl;
    std::cout <<"Front keyword b4 erasing: " << keyWords.front() << endl;
    keyWords.erase(keyWords.begin());
    std::cout <<"Front keyword AFTER erasing: " << keyWords.front() << endl;

    string token = keyWords.front();
    string manipulatedString = "";
    // Declare a struct to store the ids
    struct Ids id;
    //Check if its id is valid
    if (!keyWords.empty()){
        //Should be followed by expression which can be recursive
        std::cout <<"Expression: " << keyWords.front() << endl;
        // For now, check the expression is valid 
        if (!keyWords.empty()){

            // Call parseExpression to pass the expression
            if(keyWords.back() == ";" && parseExpression()){
                // Check if expression ID/Constant, for now lets pretend if not literal
                // Get first word in expression and check its type to manipulate
            
                // Now print the words which conform to a specific pattern utilizing the regex library
                manipulatedString = resolveExpression();
                countWords(manipulatedString);

                // Reached the end of the statement
                return true;
            } else {
                // Error Handling
                std::cout <<"Error: Expected end of statement identifier" << endl;
                return false;
            }
        }
    } else {
        // Error Handling
        std::cout <<"Error:Missing Expression after printwords" << endl;
        return false;
    }

    return true;
}

// Function to parse list statement
bool parseSetStatement(){

    // Append then id then expression and end
    //Check if second keyword is what its supposed to be
    //Remove front() first by erasing

    std::cout << endl;
    std::cout <<"parseSetStatement()" << endl;
    std::cout << endl;

    //DEBUG
    std::cout <<"Keywords: " << endl;
    for (const auto& keyword: keyWords){
        std::cout << keyword << " ";
    }
    std::cout << endl;
    std::cout <<"Front keyword b4 erasing: " << keyWords.front() << endl;
    keyWords.erase(keyWords.begin());
    std::cout <<"Front keyword AFTER erasing: " << keyWords.front() << endl;

    string token = keyWords.front();
    string manipulatedString = "";
    // Declare a struct to store the ids
    struct Ids id;
    //Check if its id is valid
    if (!keyWords.empty() && isIdentifierValid(keyWords.front())){
        std::cout <<"ID: " << keyWords.front() << endl;
        //Should be followed by expression which can be recursive
        // Save Id in the vector<Ids> ids that stores ids
        id.idName = keyWords.front();
        
        //Remove the id and check the next expression
        keyWords.erase(keyWords.begin());

        std::cout <<"Expression: " << keyWords.front() << endl;
        // For now, check the expression is valid 
        if (!keyWords.empty()){
            // Save the value of the id to later add to the vector ids
            //id.value = keyWords.front();
            // Now save to the vector
            // ids.push_back(id);
            // cout <<"id.value = " << ids.back().value << endl;
            
            // Declare strings to hold expression name and type
            string expressionWord;
            string expressionType;
            bool alreadySet = false;

            // Remove the expression from the vector
            //keyWords.erase(keyWords.begin());
            // Now check if the current keyword is valid
            // Call parseExpression to pass the expression
            if(keyWords.back() == ";" && parseExpression()){
                // Check if expression ID/Constant, for now lets pretend if not literal
                // Get first word in expression and check its type to manipulate
                // Check if the id to set is already taken if so overwrite it
                if(!ids.empty()){
                    // Allow overwrite of id
                    for(auto &idName: ids){
                        if(idName.idName == id.idName){
                            idName.value = "";
                            alreadySet = true;
                        }
                    }
                }

                if (alreadySet == false){
                    ids.push_back(id);
                    keyWords.erase(keyWords.begin());
                }
                while(!expression.empty()){
                    expressionWord = expression.front().exprName;
                    expressionType = expression.front().type;

                    // Now pop the expressionWord & Type off the table
                    expression.erase(expression.begin());

                    // Now append since statement is valid
                    if (expressionType == "id"){
                        std::cout << "NOT String Literal/constant, so ID" << endl;
                        // Add id.value to string
                        for (auto &idObj: ids) {
                            if(idObj.idName == expressionWord){
                                std::cout <<"idObj.value id = "<< idObj.value << endl;
                                // Append contents
                                manipulatedString += idObj.value;
                            }
                        }
                        
                    } else if (expressionType == "literal"){
                        // Just add to the string
                        manipulatedString += expressionWord;
                    } else if (expressionType == "constant"){
                        if(expressionWord == "SPACE"){
                            manipulatedString += " ";
                        }
                        if(expressionWord == "TAB"){
                            manipulatedString += "\t";
                        }
                        if (expressionWord == "NEWLINE"){
                            manipulatedString += "\n";
                        }
                    }
                }
                // Now add to the ids vector
                id.value = manipulatedString;
                ids.push_back(id);
                std::cout <<"id.value = " << ids.back().value << endl;
                // Reached the end of the statement
                return true;
            } else {
                // Error Handling
                std::cout <<"Error: Expected end of statement identifier" << endl;
                return false;
            }
        }
    } else {
        // Error Handling
        std::cout <<"Error: Expected 'id' identifier" << endl;
        return false;
    }
           
    return true;
}

// Function to parse list statement
bool parseReverseStatement(){
    
    std::cout << endl;
    std::cout <<"parseReverseStatement()" << endl;
    std::cout << endl;

    //DEBUG
    std::cout <<"Keywords: " << endl;
    for (const auto& keyword: keyWords){
        std::cout << keyword << " ";
    }
    std::cout << endl;
    std::cout <<"Front keyword b4 erasing: " << keyWords.front() << endl;
    keyWords.erase(keyWords.begin());
    std::cout <<"Front keyword AFTER erasing: " << keyWords.front() << endl;

    string reversedString = "";
    // Declare a struct to store the ids
    string id = "";
    string word;
    // Check if its id is valid
    if (!keyWords.empty() && checkIdentifierExists(keyWords.front())){
        std::cout <<"ID: " << keyWords.front() << endl;
        // Should be followed by expression which can be recursive
        id = keyWords.front();
        // Remove the id and check the next expression
        keyWords.erase(keyWords.begin());

        if(keyWords.front() == ";"){
            for(auto &iD: ids){
                if (iD.idName == id){
                    // Then reverse the string using algorithm library
                    //reversedString = iD.value;
                    //reverse(iD.value.begin(), iD.value.end());
                    //iD.value = reversedString;
                    // To reverse using istringstream to split the string into words
                    istringstream iss(iD.value);

                    //While loop to add words to vector
                    while(iss >> word){
                        // Push into words vector
                        words.push_back(word);
                    }

                    // Now reversing the order of the words in the vector using reverse from algorithm library
                    reverse(words.begin(), words.end());

                    // Now join the words to form a string
                    // Also using size_t incase larger values
                    for(size_t i = 0; i < words.size(); i++){
                        // Add to reversed string
                        reversedString += words[i];
                        // Add space between words 
                        if(i != words.size() - 1){
                            reversedString += " ";
                        }
                    }
                    // Now update the iD value
                    iD.value = reversedString;
                    //Also empty the vector Reset
                    emptyVector(words);
                    // Reached the end of the statement
                    return true;
                }
            }
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
        
    return false;   
}


int main() {
    // Declare language constants (SPACE|NEWLINE|TAB)
    declareConstants();

    input = "";
    while (!isExit){
        std::cout << "Enter a string: ";
        getline(cin, input);
        std::cout << "While Input: " << input << endl;

        if (parseProgram()) {  // Check if the whole input is parsed
            std::cout << "Success: The string belongs to the language." << endl;
        } else {
            std::cout << "Error: The string does not belong to the language." << endl;
        }

        // Empty Vectors
        emptyVector(keyWords);
        emptyVector(expression);
    }

    return 0;
}
