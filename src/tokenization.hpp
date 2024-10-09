#pragma once

#include <variant>

//Types of Tokens
enum TokenType{
    RETURN,INT_LIT,SEMI,OPEN_P,CLOSE_P,IDENT,NAAM,EQUAL,BOLO,STRING_LIT,NEW_LINE
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

    std::string m_buf;
public:
    // To Convert to Tokens
    explicit Tokenizer(std::string str) : m_str(std::move(str)) {}
    // Main Token function
    std::vector<Token> tokenize()
    {
        std::vector<Token> tokens;
        while(peek().has_value()) {

            // For Comments
            if(peek().value() == '/' && peek(1).value() == '/')
            {
                while(peek().value() != '\n')
                {
                    consume();
                }
            }
            if(peek().value() == '/' && peek(1).value() == '*')
            {
                consume();
                consume();
                while(peek().value() != '*' && peek(1).value() != '/')
                {
                    consume();
                }
                consume();
                consume();
            }

            if(std::isalpha(peek().value()))
            {
                tokens.push_back(tokenizeIdentifier());
            }
            else if(peek().has_value() && std::isdigit(peek().value()))
            {
                tokens.push_back(tokenizeInteger());
            }
            else if(peek().value() == '"') {
                std::vector<Token> string_tokens = tokenizeString();
                for(Token& i : string_tokens)
                {
                    tokens.push_back(i);
                }
            }

            else if(std::isspace(peek().value()))
            {
                consume();
            }
            else
            {
                tokens.push_back(other_token());
            }

        }
        m_index = 0;
        return tokens;
    }
    
    // Function for identifiers
    Token tokenizeIdentifier()
    {
        m_buf.clear();
        m_buf.push_back(consume());
        while(peek().has_value() && std::isalnum(peek().value())) {
            m_buf.push_back(consume());
        }

        if(m_buf == "khatam") {
            return {.type = TokenType::RETURN};
        }
        if(m_buf == "naam")
        {
            return {.type = TokenType::NAAM};
        }
        if(m_buf == "bolo") {
            return {.type = TokenType::BOLO};
        }
        return {.type = TokenType::IDENT,.value = m_buf};

    }
    // For integers values
    Token tokenizeInteger()
    {
        m_buf.clear();
        m_buf.push_back(consume());
        while(peek().has_value() && std::isdigit(peek().value())) {
            m_buf.push_back(consume());
        }
        return {.type = TokenType::INT_LIT,.value = m_buf};

    }
    // For strings placed within ""
    std::vector<Token> tokenizeString()
    {
        consume();
        std::string string_ident{};
        std::vector<Token> tokens;

        while(peek().has_value() && peek().value() != '"')
        {
            if(peek().has_value() && peek().value() == '{')
            {
                if(!string_ident.empty())
                {
                    tokens.push_back({.type = TokenType::STRING_LIT, .value = string_ident});
                    string_ident.clear();
                }
                std::string place_holder{};
                consume(); //consume '{'
                while(peek().value() != '}')
                {
                    place_holder.push_back(consume());
                }
                consume(); // consume '}'
                tokens.push_back({.type = TokenType::IDENT, .value = place_holder});
            }
            else
            {
                string_ident.push_back(consume());
            }
        }
        consume();
        tokens.push_back({.type = STRING_LIT, .value = string_ident});
        return tokens;
    }
    // For any other tokens
    Token other_token()
    {
        switch (peek().value())
        {
        case '(':
            consume();
            return {.type = TokenType::OPEN_P};

        case ')':
            consume();
            return {.type = TokenType::CLOSE_P};
        case ';':
            consume();
            return {.type = TokenType::SEMI};
        case '=':
            consume();
            return {.type = TokenType::EQUAL};
        default:
            std::cerr << "Unrecognized character: " <<  peek().value() << std::endl;
            exit(EXIT_FAILURE);
        }


    }

};





