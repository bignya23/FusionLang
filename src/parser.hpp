#pragma once
#include <variant>
#include "tokenization.hpp"

struct NodeExpIntlit {
    Token int_lit;
};
struct NodeExpnIdent {
    Token ident;
};
struct NodeExpn {
    std::variant<NodeExpIntlit, NodeExpnIdent> var;
};
struct NodeSmtExit {
    NodeExpn expn;
};
struct NodeSmtnaam {
    Token ident;
    NodeExpn expn;
};
struct NodeSmt {
    std::variant<NodeSmtExit, NodeSmtnaam> var;
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
public:
    inline explicit Parser(std::vector<Token> tokens): m_tokens(std::move(tokens)) {
    }
    // TO get the int_lit , Identifiers
    std::optional<NodeExpn> parse_expn() {
        if (peek().has_value() && peek().value().type == TokenType::INT_LIT) {
            return NodeExpn{.var = NodeExpIntlit{.int_lit = consume()}}; // Consumes integer literal
        }
        else if (peek().has_value() && peek().value().type == TokenType::IDENT) {
            return NodeExpn{.var = NodeExpnIdent{.ident = consume()}}; // Consumes identifier
        }
        else {
            return {}; // Return an empty optional if no valid expression is found
        }
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
            peek(2).has_value() && peek(2).value().type == TokenType::EQUAL) {

            consume();  // consume 'naam'

            auto stmt_naam = NodeSmtnaam({.ident = consume()});  // consume IDENT
            consume();  // consume '='

            if (auto node_expr = parse_expn()) {
                stmt_naam.expn = node_expr.value();
            } else {
                std::cerr << "Error: Failed to parse expression for 'Naam'" << std::endl;
                exit(EXIT_FAILURE);
            }

            if (peek().has_value() && peek().value().type == TokenType::SEMI) {
                consume();  // consume ';'
            } else {
                std::cerr << "Expected ';' after 'Naam' statement" << std::endl;
                exit(EXIT_FAILURE);
            }

            return NodeSmt{.var = stmt_naam};
        }

        // If no valid statement is found
        else {
            return {};
        }
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
