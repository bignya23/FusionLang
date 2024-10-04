#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include "../src/parser.hpp"



class Generator {
private:
    NodePrg m_prg;
    std::stringstream m_output;
    size_t m_stack_size = 0;
    int m_unique_msg_name = 0;


    void push(const std::string& reg) {
        m_output << "    push " << reg << "\n";
        m_stack_size++;
    }
    void pop(const std::string& reg) {
        m_output << "    pop " << reg << "\n";
        m_stack_size--;
    }

    struct Var {
        size_t m_size_loc;
    };

    std::unordered_map<std::string, Var> m_vars;

public:
    //Constructor initializer
    inline explicit Generator(NodePrg prg) : m_prg(std::move(prg))  {
    }

    // For generation of expressions
    void gen_expn(const NodeExpn& expn)  {

        struct ExpnVisitor {
            Generator* gen;
            // For INT_LITs
            void operator()(const NodeExpIntlit& int_lit_expn) const {
                gen->m_output << "    mov rax, " << int_lit_expn.int_lit.value.value() << "\n";
                gen->push("rax");
            }

            // For identifiers(There could be also expressions such as naam x = y;) | Searching for y
            void operator()(const NodeExpnIdent& ident_expn)  {
                if (!gen->m_vars.contains(ident_expn.ident.value.value())) {
                    std::cerr << "Could not find identifier " << ident_expn.ident.value.value() << "\n";
                    exit(EXIT_FAILURE);
                }


                const auto&[m_size_loc] = gen->m_vars.at(ident_expn.ident.value.value());
                std::stringstream output;
                // std::cerr << "Accessing variable '" << ident_expn.ident.value.value() << "' at stack location: "
                // << (gen->m_stack_size - m_size_loc - 1) * 8   << "\n";

                output << "QWORD [rsp + " << (gen->m_stack_size - m_size_loc -1) * 8 << "]";
                gen->push(output.str());

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
                gen->pop("rdi");
                gen->m_output << "    syscall\n";
            }
            void operator()(const NodeSmtnaam& smts_naam) const {
                if(gen->m_vars.contains(smts_naam.ident.value.value())) {
                    std::cerr << "Conflict in identifier " << smts_naam.ident.value.value() << std::endl;
                    exit(EXIT_FAILURE);
                }
                else {
                    gen->m_vars.insert({smts_naam.ident.value.value(), Var{.m_size_loc = gen->m_stack_size}});
                    gen->gen_expn(smts_naam.expn);
                }
            }
            void operator()(const NodeSmtPrint& smts_bolo) const
            {

            }
        };

        SmtsVisitor visitor{.gen = this};
        std::visit(visitor , smts.var);
    }





    //Combine all the statements
    [[nodiscard]] std::string gen() {
        m_output << "section .data\n";
        for(const auto& node : m_prg.smts) {
            if(auto* nstm = std::get_if<NodeSmtPrint>(&node.var))
            {
                m_output << "   msg_"<< m_unique_msg_name <<  " " << "db '" << nstm->ident.value.value() <<  "', 0xA\n";
            }
        }

        m_output << "section .text\n";
        m_output << "   global _start\n" ;
        m_output << "   _start:\n";

        for(const auto& node : m_prg.smts) {
            if(auto* nstm = std::get_if<NodeSmtPrint>(&node.var))
            {

            }
            else {
                gen_smts(node);
            }

        }
        m_output << "       mov rax, 60\n";
        m_output << "       mov rdi, 0\n";
        m_output << "       syscall\n";

        return m_output.str();
    }


};
