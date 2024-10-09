#pragma once
#include <unordered_map>
#include <variant>
#include "tokenization.hpp"

struct NodeExpIntlit {
    Token int_lit;
};

struct NodeExpnIdent {
    Token ident;
};

struct NodeExpStrLit {
    Token string_lit;
};

struct NodeExpn {
    std::variant<NodeExpIntlit, NodeExpnIdent,NodeExpStrLit> var;
};

struct NodeSmtExit {
    NodeExpn expn;
};

struct NodeSmtnaam {
    Token ident;
    NodeExpn expn;

};

struct NodeSmtPrint
{
    std::vector<Token> string_lit;
    int size{};

};

struct NodeSmt {
    std::variant<NodeSmtExit, NodeSmtnaam, NodeSmtPrint> var;
};

struct NodePrg {
    std::vector<NodeSmt> smts;
};

class Parser {
private:
    std::vector<Token> m_tokens;
    size_t m_index{};
    // To go to next token
    [[nodiscard]] inline std::optional<Token> peek(int offset = 0) const {
        if(m_index + offset >= m_tokens.size()) {
            return {};
        }
        else {
            return m_tokens.at(m_index + offset);
        }
    }
    // To consume tokens
    inline Token consume() {
        return m_tokens.at(m_index++);
    }
    std::unordered_map<std::string, int> m_stack_v;
public:
    inline explicit Parser(std::vector<Token> tokens): m_tokens(std::move(tokens)) {
    }
    // TO get the int_lit , Identifiers
    std::optional<NodeExpn> parse_expn() {
        if (peek().has_value() && peek().value().type == TokenType::INT_LIT) {
            return NodeExpn{.var = NodeExpIntlit{.int_lit = consume()}}; // Consumes integer literal
        }
        if (peek().has_value() && peek().value().type == TokenType::IDENT) {
            return NodeExpn{.var = NodeExpnIdent{.ident = consume()}}; // Consumes identifier
        }
        if (peek().has_value() && peek().value().type == TokenType::STRING_LIT) {
            return NodeExpn{.var = NodeExpStrLit{.string_lit = consume()}}; // Consumes String literal
        }
        return {}; // Return an empty optional if no valid expression is found


    }

    // TO return the statements by combining all the identifiers, int_lit
    std::optional<NodeSmt> parse_smt() {
        // Parse 'khatam' (return) statement
        if (peek().has_value() && peek().value().type == TokenType::RETURN) {
            consume();  // consume 'khatam'

            NodeSmt smt_node;
            if (auto node_expr = parse_expn()) {
                smt_node = NodeSmt{.var = NodeSmtExit{.expn = node_expr.value()}};
            } else {
                std::cerr << "Error: No input integer given for 'khatam'" << std::endl;
                exit(EXIT_FAILURE);
            }

            if (peek().has_value() && peek().value().type == TokenType::SEMI) {
                consume();  // consume ';'
            } else {
                std::cerr << "Expected ';' after 'khatam'" << std::endl;
                exit(EXIT_FAILURE);
            }

            return smt_node;
        }

        // Parse 'naam' (variable assignment) statement
        else if (peek().has_value() && peek().value().type == TokenType::NAAM &&
            peek(1).has_value() && peek(1).value().type == TokenType::IDENT &&
            peek(2).has_value() && peek(2).value().type == TokenType::EQUAL &&
            peek(3).has_value()) {

            consume();  // consume 'naam'

            auto stmt_naam = NodeSmtnaam({.ident = consume()});  // consume IDENT
            consume();  // consume '='
            if(auto node_expr = parse_expn())
            {
                stmt_naam.expn = node_expr.value();
            } else {
                std::cerr << "Error: Failed to parse expression for 'naam'" << std::endl;
                exit(EXIT_FAILURE);
            }

            if (peek().has_value() && peek().value().type == TokenType::SEMI) {
                consume();  // consume ';'
            } else {
                std::cerr << "Expected ';' after 'naam' statement" << std::endl;
                exit(EXIT_FAILURE);
            }

            return NodeSmt{.var = stmt_naam};
        }
        //Parse "bolo" Print Statement
        if(peek().has_value() && peek(0).value().type == TokenType::BOLO &&
            peek(1).has_value() && peek(1).value().type == TokenType::OPEN_P)
        {
            consume();
            consume();
            std::vector<Token> string_var;
            int com_size{};

            while(peek().value().type != TokenType::CLOSE_P)
            {
                com_size += peek().value().value->size();
                string_var.push_back(consume());
            }
            consume();

            if(peek().has_value() && peek().value().type == TokenType::SEMI)
            {
                consume();
            }
            else
            {
                std::cerr << "Expected ';' after 'bolo' statement" << std::endl;
                exit(EXIT_FAILURE);
            }
            return NodeSmt{NodeSmtPrint{.string_lit = string_var, .size = com_size}};
        }

        // If no valid statement is found
        return {};

    }

    // To combine all the statements in a single Program
    std::optional<NodePrg> parse_prg() {
        NodePrg prg;
        while(peek().has_value()) {
            if(auto smts = parse_smt()) {
                prg.smts.push_back(smts.value());
            }
            else {
                std::cerr << "Error: Expression" << std::endl;
                exit(EXIT_FAILURE);
            }

        }

        return prg;
    }


};
