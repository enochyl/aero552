//
//  main.cpp
//  automaton
//
//  Created by Enoch Lee on 10/16/17.
//  Copyright © 2017 Enoch Lee. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <streambuf>
#include <algorithm>

using namespace std;

class automata {
public:
    void add_transition(string i, char e, string tr) {
        transitions[make_pair(i,e)] = tr;
    }

    void add_finalState(string c) {
        finalStates.push_back(c);
    }

    void add_initialState(string c) {
        initialState.push_back(c);
    }

    void add_alphabet(string c) {
        alphabet.push_back(c);
    }

    void add_states(string c) {
        states.push_back(c);
    }

    void printAutomaton() {

        printf("Alphabets:\n");
        for (vector<string>::iterator i = alphabet.begin(); i != alphabet.end(); ++i)
            cout << *i << " ";
        printf("\n\n");
        printf("States:\n");
        for (vector<string>::iterator i = states.begin(); i != states.end(); ++i)
            cout << *i << " ";
        printf("\n\n");
        printf("Initial states:\n");
        for (vector<string>::iterator i = initialState.begin(); i != initialState.end(); ++i)
            cout << *i << " ";
        printf("\n\n");
        cout << "Final states:" << endl;
        for (vector<string>::iterator i = finalStates.begin(); i != finalStates.end(); ++i)
            cout << *i << " ";
        printf("\n\n");

    }

    void constructAutomaton(string file) {
        int newline = 0; string temp;
        printf("Checking automaton...\n\nTransitions:\n");

        for (int i = 0; i < file.size(); i++) {
            if (file[i] == '\n')
                newline++;
            // Construct alphabet
            if (newline == 1 && file[i] != ' ' && file[i] != '\n') {
                string temp(1,file[i]);
                add_alphabet(temp);
            }
            // Construct states
            else if (newline == 3 && file[i] != ' ' && file[i] != '\n') {
                if (isalnum(file[i+1])) {
                    string temp1(1,file[i]); string temp2(1,file[i+1]);
                    add_states(temp1+temp2);
                }
                else if((file[i-1] == ' ' || file[i-1] == '\n') && ~isalnum(file[i+1])) {
                    string temp(1,file[i]);
                    add_states(temp);
                }
            }
            // Construct initial state
            else if (newline == 5 && file[i] != ' ' && file[i] != '\n') {
                if (isalnum(file[i+1])) {
                    string temp1(1,file[i]); string temp2(1,file[i+1]);
                    add_initialState(temp1+temp2);
                }
                else {
                    string temp(1,file[i]);
                    add_initialState(temp);
                }
            }
            // Construct final states
            else if (newline == 7 && file[i] != ' ' && file[i] != '\n') {
                if (isalnum(file[i+1])) {
                    string temp1(1,file[i]); string temp2(1,file[i+1]);
                    add_finalState(temp1+temp2);
                }
                else if((file[i-1] == ' ' || file[i-1] == '\n') && ~isalnum(file[i+1])) {
                    string temp(1,file[i]);
                    add_finalState(temp);
                }
            }
            // Construct transitions based on relative positions. Incorrect formatting of input file will result in weird results.
            else if (newline >= 9 && file[i] != ' ' && file[i] != '\n') {
                if (file[i+1] != ' ' && file[i+12] != ' ' && file[i+14] == '\n') { // 2-char origin to 2-char destination
                    string temp1(1,file[i]); string temp2(1,file[i+1]); string temp3(1,file[i+12]); string temp4(1,file[i+13]);
                    add_transition(temp1+temp2,file[i+6],temp3+temp4);
                    cout << temp1+temp2 << " " << file[i+6] << " " << temp3+temp4 << endl;
                    i = i + 13;
                }
                else if (file[i+1] != ' ' && file[i+13] == '\n') { // 2-char origin to 1-char destination
                    string temp1(1,file[i]); string temp2(1,file[i+1]); string temp3(1,file[i+12]);
                    add_transition(temp1+temp2,file[i+6],temp3);
                    cout << temp1+temp2 << " " << file[i+6] << " " << temp3 << endl;;
                    i = i + 12;
                }
                else if (file[i+1] == ' ' && file[i+13] == '\n') { // 1-char origin to 2-char destination
                    string temp1(1,file[i]); string temp3(1,file[i+11]); string temp4(1,file[i+12]);
                    add_transition(temp1,file[i+5],temp3+temp4);
                    cout << temp1 << " " << file[i+5] << " " << temp3+temp4 << endl;
                    i = i + 12;
                }
                else if (file[i-1] == '\n') { // 1-char origin to 1-char destination
                    string temp1(1,file[i]); string temp3(1,file[i+11]);
                    add_transition(temp1,file[i+5],temp3);
                    cout << file[i] << " " << file[i+5] << " " << file[i+11] << endl;
                    i = i + 11;
                }

            }
        }
        printf("\n");
    }

    bool readAutomaton(string input) {
        string current = initialState[0];
        printf("Running automaton...\n");
        cout << "Initial state: " << current << endl;
        for (int i = 0; i < input.size(); i++) {
            current = transitions[make_pair(current,input[i])];
            cout << "Input: " << input[i] << ", New state: " << current << endl;
        }
        cout << "Final state: " << current << endl;
        if(find(finalStates.begin(), finalStates.end(), current) != finalStates.end())
            return true;
        else
            return false;
    }

private:
    vector<string> alphabet;
    vector<string> states;
    vector<string> initialState;
    vector<string> finalStates;
    map<pair<string, char>, string> transitions;
};

bool automaton (string file, string input) {
    automata a;

    a.constructAutomaton(file);
    a.printAutomaton();

    if(a.readAutomaton(input))
        return true;
    else
        return false;
}

int main() {

    ifstream inFile;
//    inFile.open("automaton1.txt"); // Original automaton
//    inFile.open("automaton2.txt");
//    inFile.open("automaton3.txt");
//    inFile.open("automaton4.txt");
    inFile.open("automaton5.txt"); // Automaton with "negative" states

    string inputAutomaton( (std::istreambuf_iterator<char>(inFile) ),
                           (std::istreambuf_iterator<char>()     ) );
    inFile.close();

//    string input = "aaabb"; // Invalid input will result in rejection.
//    string input = "aaabcbcc"; // Test case with invalid input "c" for automaton 1 and 2
//    string input = "aaabcbccabbbbbababbbcbbcbbbabbbabbcbbcbabca"; // Test case 4
//    string input = ""; // Test case 5 for empty input. Expected output: Reject.
//    string input = "thisisaninvalidinput"; // Test case for invalid input.
    string input = "aab";

    if(automaton (inputAutomaton, input))
        printf("\nOutput: Accept\n\n");
    else
        printf("\nOutput: Reject\n\n");

    return 0;
}
