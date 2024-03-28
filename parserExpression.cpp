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
    if(exprIsLiteral){
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
        cout <<"isExprLiteral?: " << exprIsLiteral << endl;
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
                        cout << "NOT String Literal/constant, so ID" << endl;
                        for(auto &idName: ids){
                            if(idName.idName == expressionWord){
                                cout <<"idName.value express = "<< idName.value << endl;
                                for(auto &idObj: ids){
                                    if(idObj.idName == id){
                                        cout <<"idObj.value id = "<< idObj.value << endl;
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
