#ifndef DFA_H
#define DFA_H

#include "nfa.h"
#include <map>
#include <set>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct DFAState {
    int id;
    bool is_accept;
    unordered_map<char, int> transitions;

    DFAState(int i, bool accept = false) : id(i), is_accept(accept) {}
};

class DFA {
public:
    unordered_map<int, DFAState> states;
    int start_state;
    unordered_set<char> alphabet;

    DFA() : start_state(0) {}

    void build_from_nfa(const NFA& nfa);
    void print();
};

#endif
