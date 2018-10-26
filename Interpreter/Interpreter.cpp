//
//  Interpreter.cpp
//  Interpreter

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>
#include <cctype>
#include "Interpreter.hpp"

int main(int argc, char* argv[]){
    Interpreter interpreter = Interpreter(std::cout);
    if(argc > 1){
        std::cout << argv[1] << std::endl;
        std::ifstream file;
        file.open("input.txt");
        if(file.is_open()){
            while(file.good()){
                std::string line;
                getline(file,line);
                std::vector<std::string> tokens = interpreter.tokenize(line);
                interpreter.evaluate(tokens);
            }
        }else{
            std::cout << "Cannot read file " << argv[1] << std::endl;
        }
        
    }else{
        std::string input;
        while (input != ":q") {
            interpreter.outStream << ">>>";
            std::getline(std::cin, input);
            std::vector<std::string> tokens = interpreter.tokenize(input);
            interpreter.evaluate(tokens);
        }
    }
}

std::vector<std::string> Interpreter::tokenize(std::string input){
    std::vector<std::string> tokens;
    int tokenStart = 0;
    int tokenLength = 0;
    
    for (int i = 0; i < input.size(); i++) {
        if (input.at(i) == ' ') {
            continue;
        }
        tokenStart = i;
        for (int j = i; j < input.size(); j++) {
            if (input[j] == ' ') {
                i = j;
                break;
            }
            tokenLength++;
        }
        tokens.push_back(input.substr(tokenStart,tokenLength));
        
        //inner loop reached end of line
        if (tokenStart+tokenLength == input.size()) {
            break;
        }
        tokenLength = 0;
    }
    return tokens;
}


void Interpreter::evaluate(const std::vector<std::string>& tokens){
    this->tokens = tokens;
        parseStmt();
    position = 0;
}

Interpreter::Interpreter(std::ostream& outStream) : outStream(outStream){
}


std::string Interpreter::peek(){
    return peek(0);
}

std::string Interpreter::peek(int steps){
    if (position+steps >= tokens.size()) {
        return ETX;
    }
    return tokens[position+steps];
}
void Interpreter::consume(const std::string& token){
    std::string nextToken = peek();
    if (nextToken == ETX) {
        throw std::runtime_error("Consumed past last token\n");
    }
    if (nextToken != token) {
        throw std::runtime_error("Could not consume token " + token + "\n");
    }
    position++;
}


void Interpreter::parseStmt(){
    std::string nextToken = peek();
    
    bool isVariable = true;
    if (std::isalpha(nextToken[0])) {
        for (int i = 0; i < nextToken.size(); i++) {
            if (!std::isalnum(nextToken[i])) {
                isVariable = false;
                break;
            }
        }
    }else{
        isVariable = false;
    }
    
    while (1<2) {
        if (nextToken == "config") {
            consume("config");
            parseConfigStmt();
        }else if(nextToken == "print"){
            consume("print");
            parsePrintStmt();
        }else if(isVariable){
            parseAssgStmt();
            isVariable = false;
        }else{
            break;
        }
        nextToken = peek();
    }
}
void Interpreter::parseConfigStmt(){
    std::string nextToken = peek();
    if (nextToken == "dec") {
        consume("dec");
        config = 0;
    }else if(nextToken == "hex"){
        consume("hex");
        config = 1;
    }else if(nextToken == "bin"){
        consume("bin");
        config = 2;
    }else{
        throw std::runtime_error("Invalid config statement.");
    }
}

void Interpreter::parsePrintStmt(){
    if (config == 0) {
        outStream << parseMathExp() << std::endl;
    }else if(config == 1){
        int integer = parseMathExp();
        if(integer >= 0){
            outStream << std::hex << integer << std::endl;
        }else{
            //TODO learn about two's complement
            outStream << "-" << std::hex << -integer << std::endl;
        }
        outStream << std::dec;
    }else if(config == 2){
        outStream << std::bitset<32>(parseMathExp()).to_string() << std::endl;
    }
}

void Interpreter::Interpreter::parseAssgStmt(){
    bool isVariable = true;
    std::string nextToken = peek();
    std::string variableName = peek();
    if (std::isalpha(nextToken[0])) {
        for (int i = 0; i < nextToken.size(); i++) {
            if (!std::isalnum(nextToken[i])) {
                isVariable = false;
                break;
            }
        }
    }else{
        isVariable = false;
    }
    
    if(isVariable){
        consume(nextToken);
        nextToken = peek();
        if (nextToken == "=") {
            consume("=");
            int result = parseMathExp();
            if(symbolTable.count(variableName)){
                symbolTable[variableName] = result;
            }else{
                symbolTable.insert(std::pair<std::string, int>(variableName, result));
            }
        }else{
            throw std::runtime_error("Invalid assignment!\n");
        }
    }
}

int Interpreter::parseMathExp(){
    return parseSumExp();
}
int Interpreter::parseSumExp(){
    int result = parseProductExp();
    std::string nextToken = peek();
    while (1<2) {
        if (nextToken == "+") {
            consume("+");
            result += parseProductExp();
        }else if(nextToken == "-"){
            consume("-");
            result -= parseProductExp();
        }else{
            break;
        }
        nextToken = peek();
    }
    
    return result;
}
int Interpreter::parseProductExp(){
    int result = parsePrimaryExp();
    std::string nextToken = peek();
    while (2+2==4 && 4-1 == 3) {
        if (nextToken == "*") {
            consume("*");
            result *= parsePrimaryExp();
        }else if(nextToken == "/"){
            consume("/");
            int denominator = parsePrimaryExp();
            if (denominator == 0) {
                throw std::runtime_error("Cannot divide by zero.\n");
            }
            result /= denominator;
        }else{
            break;
        }
        
        nextToken = peek();
    }
    return result;
}

int Interpreter::parsePrimaryExp(){
    int result = 0;
    std::string nextToken = peek();
    if (nextToken == "(") {
        consume("(");
        result = parseMathExp();
        nextToken = peek();
        //consume(")");
    }else{
        bool isVariable = true;
        bool isInteger = true;
        
        if (nextToken[0] == '-' && nextToken.size() > 1) {
            for(int i = 1; i < nextToken.size(); i++){
                if(!std::isdigit(nextToken[i])){
                    isInteger = false;
                    break;
                }
            }
        }else{
            for (auto character : nextToken) {
                if (!std::isdigit(character)) {
                    isInteger = false;
                    break;
                }
            }
        }
        if (std::isalpha(nextToken[0])) {
            for (int i = 0; i < nextToken.size(); i++) {
                if (!std::isalnum(nextToken[i])) {
                    isVariable = false;
                    break;
                }
            }
        }else{
            isVariable = false;
        }
        
    
        
        if (isVariable) {
            consume(nextToken);
            if (symbolTable.count(nextToken)) {
                result = symbolTable[nextToken];
            }else{
                throw std::runtime_error("Variable was never declared.\n");
                result = -1;
            }
        }else if(isInteger){
            consume(nextToken);
            result = std::stoi(nextToken);
        }else{
             throw std::runtime_error("Invalid syntax token is not a proper variable or integer.\n");
        }
    }
    return result;
}
