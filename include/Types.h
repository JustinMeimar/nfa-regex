#pragma once
#include <map>
#include <tuple>
#include <memory>
#include "State.h"

class NFA;  // forward declaration

typedef std::tuple<std::shared_ptr<NFA>, const char> TransitionTuple; 
typedef std::map<std::tuple<std::shared_ptr<NFA>, const char>, 
    std::shared_ptr<State>>  
    TransitionTable;