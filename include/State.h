#pragma once
#include <stdint.h>

enum StateType {
    ACCEPT,
    REJECT
};

class State {
    public: 
        StateType type; //is accept or reject 
        uint32_t id; 
        State(StateType type) : type(type) { id = max_id; max_id++; }
        uint32_t getId();
    private:
        static uint32_t max_id;
};