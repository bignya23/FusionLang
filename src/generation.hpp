#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include "parser.hpp"

class CodeGenerator {
private:
    std::stringstream assembly_code;

public:
    CodeGenerator() {
        // Initialize with basic assembly directives if needed
        assembly_code << "section .data\n";
        assembly_code << "section .text\n";
        assembly_code << "global _start\n";
        assembly_code << "_start:\n";
    }

    // Function to handle expression nodes
    void generate_expression(const NodeExpn& expression) {
        if (std::holds_alternative<NodeExpIntlit>(expression.var)) {
            // Handle integer literal, load it into a register
            auto int_lit = std::get<NodeExpIntlit>(expression.var).int_lit;
            assembly_code << "mov eax, " << int_lit.value.value() << "\n";
        } else if (std::holds_alternative<NodeExpnIdent>(expression.var)) {
            // Handle identifier (variable), assuming we map them to registers or memory
            auto ident = std::get<NodeExpnIdent>(expression.var).ident;
            assembly_code << "mov eax, [" << ident.value.value() << "]\n";
        }
    }

    // Function to handle statement nodes
    void generate_statement(const NodeSmt& statement) {
        if (std::holds_alternative<NodeSmtExit>(statement.var)) {
            // Handle 'khatam' (return statement)
            auto exit_stmt = std::get<NodeSmtExit>(statement.var);
            generate_expression(exit_stmt.expn);
            assembly_code << "ret\n";  // Return the value in eax
        } else if (std::holds_alternative<NodeSmtjug>(statement.var)) {
            // Handle 'juggad' (variable assignment)
            auto jug_stmt = std::get<NodeSmtjug>(statement.var);
            generate_expression(jug_stmt.expn);
            assembly_code << "mov [" << jug_stmt.ident.value.value() << "], eax\n";  // Assign eax to variable
        }
    }

    // Function to handle the entire program
    void generate_program(const NodePrg& program) {
        for (const auto& stmt : program.smts) {
            generate_statement(stmt);
        }
    }

    // Save the generated assembly to a file
    void save_assembly_to_file(const std::string& filename) {
        std::ofstream outfile(filename);
        if (!outfile) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            return;
        }
        outfile << assembly_code.str();
        outfile.close();
    }
};
