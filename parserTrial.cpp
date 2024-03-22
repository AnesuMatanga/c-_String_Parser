#include <iostream>
#include <regex>
#include <string>

using namespace std;

int main(){
    // Check if the identifier matches the pattern id = [a-zA-Z][a-zA-Z0-9]* using regex library
    string id = "Hey78@";

    const std::regex id_regex("[a-zA-Z][a-zA-Z0-9]*");

    if (!regex_match(id, id_regex)){
        //Error
        cout <<"Error: Invalid Identifier";
    } else {
        cout <<"Success: Valid Identifier";
    }

}