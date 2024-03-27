#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// Global Vars
vector<string> keyWords;
string nextToken;
string input;
bool exprIsLiteral = false;
bool exprIsID = false;
bool exprIsConstant = false;

// Function prototypes
bool parseExpression();
bool parseValue(string value);

void addTokensToVector(){
    stringstream ss(input);
    string word;
    // Add each word in the expression to tokenVector keyWords
    while(ss >> word){
        keyWords.push_back(word);
    }
    // Now end the end of the expression identifier
    //keyWords.push_back(";");
}

void emptyVector(){
    while(!keyWords.empty()){
        keyWords.pop_back();
    }
}

// Function to get next token in expression keyWord 
void getNextToken(){
    // Get next token
    if(!keyWords.empty()){
        nextToken = keyWords.front();
        // Now erase the token from the keyWords table
        keyWords.erase(keyWords.begin());
    } else {
        nextToken = ";";
    }
}

// Function to accept by checking if after value next char is +
bool accept(string expected){
    cout << endl;
    cout <<"accept()" << endl;
    cout << endl;
    //get next token after value so + and return true
    getNextToken();
    cout <<"1: accepted getNextToken = " << nextToken << endl;
    if(nextToken != expected){
        return false;
    }
    
    return true;
}

// Function to parse Expression Recursively. Recursive Descent parser
// Using expression := value + Expression | value (for recursiveness) 
bool parseExpression(){
    cout << endl;
    cout <<"parseExpression()" << endl;
    cout << endl;
    // First Check if the nextToken is valid
    getNextToken();
    cout <<"1: Outside if getNextToken = " << nextToken << endl;
    
    if(!parseValue(nextToken)) {
        cout <<"Value Invalid" << endl;
        // token not valid
        return false;
    }

    // Check if there are more tokens & if the next token is a +
    if(!keyWords.empty() && keyWords.front() == "+") {
        // Consume the '+' token
        getNextToken();
        cout <<"2: Consumed + next token = " << nextToken << endl;

        // Recursively parseExpression
        return parseExpression();
    }

    // If there is no +, we've reached the end of the expression
    return true;
}

// Function to parseValue by checking if the value is a valid value or not
bool parseValue(string value){
    cout << endl;
    cout <<"parseValue()" << endl;
    cout << endl;
    cout <<"Next Token = " << nextToken << endl;
    // Check if value is string
    if(value.front() == '"'){
        cout <<"Is String" << endl;
        exprIsLiteral = true;
        return true;
        // Check if value is constant
    } else if (value == "SPACE" || value == "TAB" || value == "NEWLINE"){
        cout <<"Is Constant" << endl;
        exprIsConstant = true;
        return true;
        // Check if value is id
    } else if (value == "id"){
        cout <<"Is ID" << endl;
        exprIsID = true;
        return true;
    }

    // Else if value is invalid return false
    return false;
} 

int main() {
    input = "";
    while (input != "exit;"){
        std::cout << "Enter a string: ";
        getline(cin, input);
        // Add top keyWords Vector
        addTokensToVector();
        std::cout << "While Input: " << input << endl;

        if(parseExpression() && (keyWords.empty())) {
            cout <<"Success: parsed Expression = " << input << endl;
        } else {
            cout <<"Error: parsing failed." << endl;
        }

        // Empty vector
        emptyVector();
    }

}
