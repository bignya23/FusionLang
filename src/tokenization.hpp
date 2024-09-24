#pragma once


//Types of Tokens
enum TokenType{
    RETURN,INT_LIT,SEMI,OPEN_P,CLOSE_P,IDENT,JUGAAD,EQUAL,BOLO
};
// Token Type and val which is optional
struct Token {
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer {
private:
    std::string m_str;
    int m_index{};
    // function to peek to the next character
     [[nodiscard]] inline std::optional<char> peek(int offset = 0) const {
        if(m_index + offset >= m_str.length()) {
            return {};
        }
        else {
            return m_str.at(m_index + offset);
        }
    }
    // To consume the character
    inline char consume() {
        return m_str.at(m_index++);
    }
public:
    // To Convert to Tokens
    inline explicit Tokenizer(std::string str) : m_str(std::move(str)) {}
    inline std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        std::string buf;
        while(peek().has_value()) {
            if(std::isalpha(peek().value())) { // need to move x to down

                buf.push_back(consume());
                while(std::isalnum(peek().value())) {
                    buf.push_back(consume());
                }

                if(buf == "khatam") {
                    tokens.push_back({.type = TokenType::RETURN});
                }
                else if(buf == "juggad") {
                    tokens.push_back({.type = TokenType::JUGAAD});

                }
                // else if(buf == "bolo") {
                //     tokens.push_back({.type = TokenType::BOLO});
                // }
                else {
                    tokens.push_back({.type = TokenType::IDENT,.value = buf});
                }
                buf.clear();

            }
            else if(peek().has_value() && std::isdigit(peek().value())) {
                buf.push_back(consume());
                while(std::isdigit(peek().value())) {
                    buf.push_back(consume());
                }

                tokens.push_back({.type = TokenType::INT_LIT,.value = buf});
                buf.clear();

            }
            else if(peek().value() == '(') {
                consume();
                tokens.push_back({.type = TokenType::OPEN_P});
            }
            else if(peek().value() == ')') {
                consume();
                tokens.push_back({.type = TokenType::CLOSE_P});
            }
            else if(std::isspace(peek().value())) {
                consume();
            }
            else if(peek().value() == ';') {
                tokens.push_back({.type = TokenType::SEMI});
                consume();
            }
            else if(peek().value() == '=') {
                tokens.push_back({.type = TokenType::EQUAL});
                consume();
            }
            else {
                std::cerr << "Unrecognized character: " << peek().value() << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        m_index = 0;
        return tokens;
    }

};





