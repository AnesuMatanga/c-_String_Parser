#include <iostream>
#include <string>

using namespace std;

// Global Vars

// Input string and current position in string
string input;
size_t pos = 0;

// Function Prototypes
bool ParseS();

// Function to get the next char from input
char getNext() {
    if (pos < input.length()){
        return input[pos++];
    }
    // Return null char for end of the string
    return '\0';
}

// Function to parse 'S' non-terminal
bool parseS() {
    // Get the next char
    char next = getNext();
    cout <<"bool ParseS()" << endl;
    cout <<"next1 = " << next << endl;
    if (next == 'a'){
        cout <<"next2 = " << next << endl;
        if (!parseS()) { // Recursively parse 'S'
            cout <<"If !parseS() return false" << endl; 
            return false;
        }
        // Get the next char
        next = getNext();
        cout <<"next3 = " << next << endl;
        if (next != 'b') {
            cout <<"Next != 'b" << endl;
            return false;
        }
        cout <<"Return True" << endl;
        return true;
    } else {
        // If the next char is not 'a', it should be the end of the string
        cout <<"ELSE" << endl;
        bool isEnd = false;
        if (next == '\0'){
            isEnd = true;
        }
        cout <<"END? : " << isEnd << endl;

        return next == '\0' || next == 'b';
    }
}

int main() {
    cout << "Enter a string: ";
    //fflush(stdin); // Flush the input buffer
    cin >> input;
    //input = "aabb";
    cout <<"Input: " << input << endl;

    // Reset position and parse the input
    pos = 0;
    if (parseS()) { // Check if the whole input is parsed
        cout <<"parseS TRUE. Pos = " << pos << endl;
        if(pos == input.length()){
            cout << "Success: The string belongs to the language." << endl;
        }
    } else {
        cout << "Error: The string does not belong to the language." << endl;
    }

    return 0;
}