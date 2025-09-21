#include "lexer.h"
#include "exception.h"
#include <cctype>
#include <sstream>
#include <unordered_set>
#include <regex>

using namespace std;

// Do Maximal Munch, considering the longest valid token
// Then, apply priority rules if needed between identifiers and keywords
// Identifier, Keyword, Integer, AssignOp, Op, Delimiter, EndOfFile

// Identifiers are any other alphanumerics + underscore that start with [a-zA-Z_]
// Integers are any [0-9]+

std::unordered_set<std::string> delimiters = {"(", ")", "{", "}", ":", ";", ","};
std::unordered_set<std::string> ops = {"+", "-", "*", "/", "==", "!=", "<", ">", "<=", ">="};
std::unordered_set<std::string> assignOps = {":="};
std::unordered_set<std::string> keywords = {"func", "var", "let", "if", "else", "while", "print", "return"};
std::regex identifierRegex("[a-zA-Z_][a-zA-Z0-9_]*");
std::regex integerRegex("[0-9]+");
std::unordered_set<char> firstChars = { '!', ')', '(', '*', '+', ',', '-', '/', ':', ';', '<', '=', '>', '{', '}' };

Lexer::Lexer(const char *filename) : in(filename) {
    if (!in) {
        throw std::runtime_error("Failed to open file");
    }
}

Lexer::~Lexer() {
    in.close();
}

void Lexer::syncPointer(int line, int col) {
    this->line = line;
    this->col = col;
}

// Create function that checks a string if it matches any lexeme, and returns the corresponding Token
// If it doesn't match anything, return Token() which means line=0 (impossible otherwise)
Token checkForMatch(const std::string &currText, int line, int col) {
    // check if currText matches anything
    if (keywords.find(currText) != keywords.end()) {
        return Token(Token::Keyword, currText, line, col);
    }
    else if(assignOps.find(currText) != assignOps.end()) {
        return Token(Token::AssignOp, currText, line, col);
    }
    else if(ops.find(currText) != ops.end()) {
        return Token(Token::Op, currText, line, col);
    }
    else if(delimiters.find(currText) != delimiters.end()) {
        return Token(Token::Delimiter, currText, line, col);
    }
    else if(std::regex_match(currText, identifierRegex)) {
        return Token(Token::Identifier, currText, line, col);
    }
    else if(std::regex_match(currText, integerRegex)) {
        return Token(Token::Integer, currText, line, col);
    }

    return Token(); // No match found
}

Token Lexer::getNextToken() {
    std::string currText = "";
    int currLine = this->line;
    int currCol = this->col;
    
    // Loop until currText cannot be extended to form a valid token
    char ch;
    while (in.get(ch)) {
        // std::cout << currText << "," << ch << std::endl; // --- IGNORE ---
        currCol++;
        // Handle token separators
        if(std::isspace(ch) || ch == '#') {
            if (ch == '\n') {
                currLine++;
                currCol = 0;
            }
            else if (ch == '#') {
                std::string comment;
                std::getline(in, comment); // skip the rest of the line
                currLine++;
                currCol = 0;
            }

            if (!currText.empty()) {
                break; // End of current token
            } else {
                syncPointer(currLine, currCol);
                continue; // Skip leading separators
            }
        }
        
        // Other way tokens can end is if we can't form a valid token by adding the next character
        if (currText.empty() && firstChars.count(ch) > 0) {
            // Valid tokens can still exist
            currText += ch;
            continue;
        }

        std::string testText = currText + ch;
        Token testToken = checkForMatch(testText, this->line, this->col);
        if (testToken.line == 0) {
            in.unget() && currCol--; //backtrack
            break; // No valid token can be formed by adding ch, end of current token
        }

        currText += ch;
    }

    // First check for EOF
    if(in.eof() && currText.empty()) {
        Token t = Token(Token::EndOfFile, "", this->line, this->col);
        tokens.push_back(t);
        return t;
    }

    // If we reach here, this is the end of the current token
    int startLine = this->line;
    int startCol = this->col + 1;
    syncPointer(currLine, currCol);
    if (!currText.empty()) {
        Token t = checkForMatch(currText, startLine, startCol);
        // std::cout << "After sync: " << this->line << "," << this->col << std::endl; // --- IGNORE ---
        if (t.line != 0) {
            tokens.push_back(t);
            return t;
        } else {
            throw LexerException(startLine, startCol);
        }
    }
    else {
        throw LexerException(startLine, startCol);
    }

    // Form Token Object (Type, Text, Line, Column)
    // Should tokens vector contain EOF token?
    return Token(Token::EndOfFile, "", this->line, this->col);
}

void Lexer::printTokens() {
    for (const auto& token : tokens) {
        token.print();
    }
}
