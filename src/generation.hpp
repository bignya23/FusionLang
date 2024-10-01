#pragma once
#include <fstream>
#include <string>
#include "../src/parser.hpp"

class Generator {
private:
    NodePrg m_prg;
    std::stringstream m_output;

public:
    //Constructor initializer
    inline explicit Generator(NodePrg prg) : m_prg(std::move(prg))  {
    }

    // For generation of expressions
    void gen_expn(const NodeExpn& expn)  {

        struct ExpnVisitor {
            Generator* gen;
            void operator()(const NodeExpIntlit& int_lit_expn) const {
                gen->m_output << "    mov rax, " << int_lit_expn.int_lit.value.value() << "\n";
                gen->m_output << "    push rax\n";
            }
            void operator()(const NodeExpnIdent& ident_expn) const {

            }


        };

        ExpnVisitor expn_generate{.gen = this};
        std::visit(expn_generate, expn.var);
     }

    //Generate individual statements
    void gen_smts(const NodeSmt& smts) {
        struct SmtsVisitor {
            Generator* gen;
            void operator()(const NodeSmtExit& smts_exit) const {
                gen->gen_expn(smts_exit.expn);
                gen->m_output << "    mov rax, 60\n";
                gen->m_output << "    pop rdi\n";
                gen->m_output << "    syscall\n";
            }
            void operator()(const NodeSmtnaam& smts_naam) const {

            }
        };

        SmtsVisitor visitor{.gen = this};
        std::visit(visitor , smts.var);
    }

    //Combine all the statements
    [[nodiscard]] std::string gen() {

        m_output << "global _start\n" ;
        m_output << "_start:\n";

        for(const auto& node : m_prg.smts) {
            gen_smts(node);

        }


        m_output << "    mov rax , 60\n";
        m_output << "    mov rdi ,0\n";
        m_output << "    syscall\n";
        return m_output.str();
    }

};
