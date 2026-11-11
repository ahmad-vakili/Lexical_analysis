#include "dfa.h"
#include <unordered_set>
#include <queue>
#include <sstream>

using namespace std;

int stateIdCounter = 0;

DFAState* DFA::createDFAState(const set<NFAState*>& nfaStates) {
    DFAState* state = new DFAState(stateIdCounter++);
    state->nfaStates = nfaStates;
    for (NFAState* nfaState : nfaStates) {
        if (nfaState->isAccepting) {
            state->isAccepting = true;
        }
    }
    return state;
}

set<NFAState*> DFA::epsilonClosure(const set<NFAState*>& states) {
    stack<NFAState*> stk;
    set<NFAState*> closure = states;
    for (NFAState* s : states) stk.push(s);

    while (!stk.empty()) {
        NFAState* state = stk.top(); stk.pop();
        for (auto& tran : state->transitions) {
            if (tran.first == "") {
                for (NFAState* target : tran.second) {
                    if (closure.find(target) == closure.end()) {
                        closure.insert(target);
                        stk.push(target);
                    }
                }
            }
        }
    }
    return closure;
}

set<NFAState*> DFA::move(const set<NFAState*>& states, const string& symbol) {
    set<NFAState*> result;
    for (NFAState* state : states) {
        if (state->transitions.count(symbol)) {
            for (NFAState* target : state->transitions[symbol]) {
                result.insert(target);
            }
        }
    }
    return result;
}

void DFA::convertFromNFA(NFA* nfa) {
    set<NFAState*> startClosure = epsilonClosure({nfa->startState});
    DFAState* startDFA = createDFAState(startClosure);
    dfaStates[startClosure] = startDFA;
    startState = startDFA;

    queue<DFAState*> q;
    q.push(startDFA);

    while (!q.empty()) {
        DFAState* current = q.front(); q.pop();

        unordered_set<string> inputSymbols;
        for (NFAState* s : current->nfaStates) {
            for (auto& tran : s->transitions) {
                if (!tran.first.empty()) {
                    inputSymbols.insert(tran.first);
                }
            }
        }

        for (const string& symbol : inputSymbols) {
            set<NFAState*> moveResult = move(current->nfaStates, symbol);
            set<NFAState*> closure = epsilonClosure(moveResult);

            if (!dfaStates.count(closure)) {
                DFAState* newState = createDFAState(closure);
                dfaStates[closure] = newState;
                q.push(newState);
            }
            current->transitions[symbol] = dfaStates[closure];
        }
    }
}

DFA::~DFA() {
    unordered_set<DFAState*> deleted;
    for (auto& entry : dfaStates) {
        if (!deleted.count(entry.second)) {
            delete entry.second;
            deleted.insert(entry.second);
        }
    }
}
