#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cctype>

using namespace std;

// Token types for lexical analysis
enum TokenType {
    KEYWORD,
    IDENTIFIER,
    MATH_OPERATOR,
    LOGICAL_OPERATOR,
    NUMERIC_VALUE,
    OTHER
};

// Token structure for lexical analysis
struct Token {
    TokenType type;
    string value;
};

// Check if a string is a keyword
int isKeyword(const string& str) {
    static const string keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default",
        "do", "double", "else", "enum", "extern", "float", "for", "goto",
        "if", "int", "long", "register", "return", "short", "signed",
        "sizeof", "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"
    };

    for (const auto& keyword : keywords) {
        if (str == keyword) {
            return 1; // Keyword found
        }
    }
    return 0; // Keyword not found
}

// Tokenize the input expression
vector<Token> tokenizeExpression(const string& expression) {
    vector<Token> tokens;
    string numberBuffer;

    for (char c : expression) {
        if (isspace(c)) {
            // Ignore whitespace
            continue;
        } else if (isdigit(c) || c == '.') {
            // If it's a digit or decimal point, it's part of a number
            numberBuffer += c;
        } else {
            // Flush number buffer if not empty
            if (!numberBuffer.empty()) {
                tokens.push_back({NUMERIC_VALUE, numberBuffer});
                numberBuffer.clear();
            }

            // Handle operators and parentheses
            switch (c) {
                case '+':
                case '-':
                case '*':
                case '/':
                    tokens.push_back({MATH_OPERATOR, string(1, c)});
                    break;
                case '(':
                case ')':
                    tokens.push_back({OTHER, string(1, c)});
                    break;
                default:
                    tokens.push_back({OTHER, string(1, c)});
                    break;
            }
        }
    }

    // Add the last number token if present
    if (!numberBuffer.empty()) {
        tokens.push_back({NUMERIC_VALUE, numberBuffer});
    }

    return tokens;
}

// Function to perform lexical analysis on input code
void performLexicalAnalysis(const string& filename) {
    char ch;
    ifstream fin(filename);

    if (!fin.is_open()) {
        cout << "Error while opening the file " << filename << endl;
        exit(EXIT_FAILURE);
    }

    vector<Token> tokens;
    vector<int> mark(256, 0); // To keep track of characters already processed

    while (!fin.eof()) {
        fin.get(ch);

        // Check for operators, parentheses, and other characters
        if (ispunct(ch) || isspace(ch)) {
            if (!mark[ch]) {
                tokens.push_back({OTHER, string(1, ch)});
                mark[ch] = 1;
            }
        }

        // Check for keywords and identifiers
        if (isalpha(ch)) {
            string buffer;
            buffer += ch;

            while (fin.get(ch) && (isalnum(ch) || ch == '_')) {
                buffer += ch;
            }
            fin.unget();

            if (isKeyword(buffer)) {
                tokens.push_back({KEYWORD, buffer});
            } else {
                tokens.push_back({IDENTIFIER, buffer});
            }
        }
    }

    fin.close();

    // Output token categories and counts
    cout << "Token Categories and Counts:" << endl;
    cout << "Keywords: ";
    for (const auto& token : tokens) {
        switch (token.type) {
            case KEYWORD:
                cout << token.value << ", ";
                break;
            default:
                break;
        }
    }
    cout << endl;

    cout << "Identifiers: ";
    for (const auto& token : tokens) {
        switch (token.type) {
            case IDENTIFIER:
                cout << token.value << ", ";
                break;
            default:
                break;
        }
    }
    cout << endl;

    cout << "Math Operators: ";
    for (const auto& token : tokens) {
        switch (token.type) {
            case MATH_OPERATOR:
                cout << token.value << ", ";
                break;
            default:
                break;
        }
    }
    cout << endl;

    cout << "Logical Operators: ";
    for (const auto& token : tokens) {
        switch (token.type) {
            case LOGICAL_OPERATOR:
                cout << token.value << ", ";
                break;
            default:
                break;
        }
    }
    cout << endl;

    cout << "Numeric Values: ";
    for (const auto& token : tokens) {
        switch (token.type) {
            case NUMERIC_VALUE:
                cout << token.value << ", ";
                break;
            default:
                break;
        }
    }
    cout << endl;

    cout << "Others: ";
    for (const auto& token : tokens) {
        switch (token.type) {
            case OTHER:
                cout << token.value << ", ";
                break;
            default:
                break;
        }
    }
    cout << endl;
}

int main() {
    cout << "Choose an option:\n1. Tokenize a mathematical expression\n2. Perform lexical analysis on input code\n";
    int choice;
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        string expression;
        cout << "Enter a mathematical expression: ";
        getline(cin, expression);

        vector<Token> tokens = tokenizeExpression(expression);

        // Print tokens
        cout << "Tokens:" << endl;
        for (const auto& token : tokens) {
            switch (token.type) {
                case NUMERIC_VALUE:
                    cout << "Numeric Value: " << token.value << endl;
                    break;
                case MATH_OPERATOR:
                    cout << "Math Operator: " << token.value << endl;
                    break;
                case OTHER:
                    cout << "Other: " << token.value << endl;
                    break;
                default:
                    break;
            }
        }
    } else if (choice == 2) {
        string filename;
        cout << "Enter the filename for input code: ";
        getline(cin, filename);

        performLexicalAnalysis(filename);
    } else {
        cout << "Invalid choice" << endl;
    }

    return 0;
}