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
        struct ExprVisitor {
            void operator()(const NodeExpIntlit& intLit) const {
                codegen->assembly_code << "mov eax, " << intLit.int_lit.value.value() << "\n";
            }
            void operator()(const NodeExpnIdent ident) const {
                codegen->assembly_code << "mov eax, [" << ident.ident.value.value() << "]\n";
            }
            CodeGenerator* codegen;
        };
        ExprVisitor visitor {.codegen = this};
        std::visit(visitor, expression.var);
    }

    // Function to handle statement nodes
    void generate_statement(const NodeSmt& statement) {
        struct StmtVisitor {
            void operator()(const NodeSmtExit& exitStmt) const {
                codegen->generate_expression(exitStmt.expn);
                codegen->assembly_code << "ret\n";
            }
            void operator()(const NodeSmtjug& jugStmt) const {
                codegen->generate_expression(jugStmt.expn);
                codegen->assembly_code << "mov [" << jugStmt.ident.value.value() << "], eax\n";  // Assign eax to variable
            }
            CodeGenerator* codegen;
        };
        StmtVisitor visitor {.codegen =  this};
        // Visit the function and on the basis of what is contained in var
        std::visit(visitor, statement.var);
    }

    // Function to handle the entire program
    void generate_program(const NodePrg& program) {
        for (const auto& stmt : program.smts) {
            generate_statement(stmt);
        }
    }

    // Save the generated assembly to a file
    void save_assembly_to_file(const std::string& filename) const {
        std::ofstream outfile(filename);
        if (!outfile) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            return;
        }
        outfile << assembly_code.str();
        outfile.close();
    }
};
