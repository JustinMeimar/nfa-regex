#pragma once
#include <map>

enum StateType {
    ACCEPT,
    REJECT
};

class State {
    public: 
        StateType type; //is accept or reject  
        State(StateType type) : type(type) {}
};