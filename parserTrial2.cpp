#include <iostream>
#include <regex>
#include <iterator>
#include <string>

using namespace std;

void parsePrintWordsStatement(const string& str){
    // Declaring regex object with the pattern
    // Recall where you learnt this: https://cplusplus.com/reference/regex/ECMAScript/
    // \\b is for word boundary
    regex word_regex(R"(\b['\w-]+\b)");

    // Will use a regex iterator to be able to count words which match pattern
    auto words_begin = sregex_iterator(str.begin(), str.end(), word_regex);
    auto words_end = sregex_iterator();

    cout <<"Found " << distance(words_begin, words_end) << " words" << endl;
    cout <<"List of words found: " << endl;
    // Now iterate over all matches and print each word
    for (sregex_iterator i = words_begin; i != words_end; i++){
        smatch match = *i;
        cout << match.str() << endl;
    }

}

int main() {
    //string testString = "To be, or not to be - that is the question: Whether 'tis nobler in the mind to suffer The slings and arrows of outrageous fortune Or to take arms against a sea of troubles, And by opposing end them. To die - to sleep - No more, and by a sleep to say we end The heartache, and the thousand natural shocks That flesh is heir to. 'Tis a consummation Devoutly to be wish'd. To die - to sleep. To sleep - perchance to dream: ay, there's the rub! For in that sleep of death what dreams may come When we have shuffled off this mortal coil, Must give us pause. There's the respect That makes calamity of so long life. For who would bear the whips and scorns of time, The oppressor's wrong, the proud man's contumely, The pangs of despised love, the law's delay, The insolence of office, and the spurns That patient merit of the unworthy takes, When he himself might his quietus make";
    string testString = "To be, or not to be - that is the question: Whether 'tis nobler in the mind to suffer The slings and arrows of outrageous fortune Or to take arms against a sea of troubles, And by opposing end them. To die - to sleep - No more; and by a sleep to say we end The heartache, and the thousand natural shocks That flesh is heir to. 'Tis a consummation Devoutly to be wish'd. To die - to sleep. To sleep - perchance to dream: ay, there's the rub! For in that sleep of death what dreams may come When we have shuffled off this mortal coil, Must give us pause. There's the respect That makes calamity of so long life. For who would bear the whips and scorns of time, The oppressor's wrong, the proud man's contumely, The pangs of despised love, the law's delay, The insolence of office, and the spurns That patient merit of the unworthy takes, When he himself might his quietus make With a bare bodkin? Who would these fardels bear, To grunt and sweat under a weary life, But that the dread of something after death - The undiscover'd country, from whose bourn No traveller returns - puzzles the will, And makes us rather bear those ills we have Than fly to others that we know not of? Thus conscience does make cowards of us all, And thus the native hue of resolution Is sicklied o'er with the pale cast of thought, And enterprises of great pith and moment With this regard their currents turn awry And lose the name of action.";
    parsePrintWordsStatement(testString);
    /*string input = "";
    while(input != "exit"){
        cout <<"Enter a string: "  << endl;
        getline(cin, input);
        parsePrintWordsStatement(input);
    } */
}