#include <iostream>
#include <string>

using namespace std;

// Global Variables
string input;
size_t pos = 0;  // Use a position index instead of modifying the input string

// Function Prototypes
bool parseS();

// Function to parse 'S' non-terminal
bool parseS() {
    if (pos < input.length() && input[pos] == 'a') {
        pos++;  // Consume 'a'
        if (!parseS()) {
            return false;
        }
        if (pos >= input.length() || input[pos] != 'b') {
            return false;  // Expecting 'b' after 'aS'
        }
        pos++;  // Consume 'b'
        return true;
    }
    // Base case: empty string or reaching a 'b' after processing 'aSb'
    return true;
}

int main() {
    cout << "Enter a string: ";
    cin >> input;
    cout << "Input: " << input << endl;

    if (parseS() && pos == input.length()) {  // Check if the whole input is parsed
        cout << "Success: The string belongs to the language." << endl;
    } else {
        cout << "Error: The string does not belong to the language." << endl;
    }

    return 0;
}
