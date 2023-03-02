#pragma once
#include <map>

enum StateType {
    ACCEPT,
    REJECT
};

class State {
    public: 
        //Sha-256 Hash we dont want duplicate state id for mergeing two NFA
        std::string id; //unique 
        StateType type; //is accept or reject 
        
        State() {}
}