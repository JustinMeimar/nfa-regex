#pragma once

enum StateType {
    ACCEPT,
    REJECT
};

class State {
    public: 
        StateType type; //is accept or reject 
        int id; 
        State(StateType type) : type(type) { id = max_id; max_id++; }
        int getId();
    private:
        static int max_id;
};