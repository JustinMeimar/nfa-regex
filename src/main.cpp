#include <iostream>
#include "Parser.h"
#include <memory>
#include <vector>

int main(int argc, char** argv) {

    auto regexParser = std::make_shared<Parser>();   

    if (argc <= 1) {
        printf("no arguments supplied...\n");
        return 0;
    }

    std::string input = argv[1];
    regexParser->readStream(input);

    return 0;
}