//
//  Interpreter.hpp
//  Interpreter
//
//  Created by Jakup Güven on 2018-10-23.
//  Copyright © 2018 Cybercom. All rights reserved.
//

#ifndef Interpreter_hpp
#define Interpreter_hpp


#include "Interpreter.hpp"
#include <iostream>
#include <map>
#include <regex>


class Interpreter {
    std::map<std::string, int> symbolTable;
    std::vector<std::string> tokens;
    int config = 0; //0 is dec, 1 is hex, 2 is bin
    int position = 0;
    const std::string ETX = "\u0003";
//    std::regex variable("[a-zA-z][a-zA-z0-9]*");
//    std::regex integer("-?[0-9]+");
    
public:
    Interpreter(std::ostream& outStream);
    void evaluate(const std::vector<std::string>& tokens);
    std::vector<std::string> tokenize(std::string input);
    std::ostream& outStream;

    
    std::string peek();
    std::string peek(int steps);
    void consume(const std::string& token);
    
    void evaluate();
    void parseStmt();
    void parseConfigStmt();
    void parseAssgStmt();
    void parsePrintStmt();
    int parseMathExp();
    int parseSumExp();
    int parseProductExp();
    int parsePrimaryExp();
};

#endif /* Interpreter_hpp */
