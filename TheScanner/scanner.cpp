// scanner.cpp

#include "dfa.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

unordered_set<string> keywords = {
    "start", "finish", "then", "if", "repeat", "var", "int", "float", "do", "read",
    "print", "void", "return", "dummy", "program"
};

unordered_set<string> relops = {"==", "!=", "=>", "="};

bool isRelOp(const string& s) {
    return relops.count(s);
}

bool isIdentifier(const string& s) {
    if (s.empty()  !isalpha(s[0])  s.length() > 8)
        return false;
    for (char c : s) {
        if (!isalpha(c)) return false;
    }
    return true;
}

vector<pair<string, string>> tokenizeLine(const string& line, DFA& dfa, int row) {
    vector<pair<string, string>> tokens;
    string token;
    int col = 1;

    for (size_t i = 0; i < line.size();) {
        char c = line[i];

        if (isspace(c)) {
            i++;
            col++;
            continue;
        }

        size_t start = i;
        string current;
        int last_accepting = -1;
        string last_token_type;

        DFAState* state = dfa.getStartState();

        while (i < line.size() && state->transitions.count(line[i])) {
            state = state->transitions[line[i]];
            current += line[i];
            if (state->is_accepting) {
                last_accepting = i;
                last_token_type = state->token_type;
            }
            i++;
        }

        if (last_accepting != -1) {
            token = current;
            tokens.emplace_back(last_token_type, token);
            col += token.length();
        } else {
            tokens.emplace_back("ERROR", string(1, line[start]));
            i = start + 1;
            col++;
        }
    }

    return tokens;
}

int main() {
    ifstream input("input.abc");
    if (!input.is_open()) {
        cerr << "Failed to open input.abc" << endl;
        return 1;
    }

    DFA dfa;
    // Add DFAs for each token type
    for (const auto& word : keywords) {
        dfa.addRegex(word, "KEYWORD");
    }
    for (const auto& op : relops) {
        dfa.addRegex(op, "RELOP");
    }
    dfa.addRegex("[a-zA-Z][a-zA-Z]{0,7}", "IDENTIFIER");

    dfa.build();

    string line;
    int row = 1;
    while (getline(input, line)) {
        auto tokens = tokenizeLine(line, dfa, row);
        int col = 1;
        for (const auto& token : tokens) {
            cout << "(" << token.first << ", \"" << token.second << "\", row=" << row << ", col=" << col << ")" << endl;
            col += token.second.length() + 1;
        }
        row++;
    }

    return 0;
}
