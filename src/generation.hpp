#pragma once
#include <fstream>
#include <queue>
#include <string>
#include <unordered_map>


#include "../src/parser.hpp"




class Generator {
private:
    NodePrg m_prg;
    std::stringstream m_output;
    size_t m_stack_size = 0;
    int m_unique_msg_name = 0;
    std::queue<std::pair<std::string,int>> m_queue_data;

    void push(const std::string& reg) {
        m_output << "      push " << reg << "\n";
        m_stack_size++;
    }
    void pop(const std::string& reg) {
        m_output << "      pop " << reg << "\n";
        m_stack_size--;
    }

    struct Var {
        size_t m_size_loc;
    };

    std::unordered_map<std::string, Var> m_vars;
    std::unordered_map<std::string, int> m_value_stack;


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
                gen->m_output << "      mov rax, " << int_lit_expn.int_lit.value.value() << "\n";

                gen->push("rax");
            }

            // For identifiers(There could be also expressions such as naam x = y;) | Searching for y
            void operator()(const NodeExpnIdent& ident_expn) const {
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
                gen->m_output << "      mov rax, 60\n";
                gen->pop("rdi");
                gen->m_output << "      syscall\n";
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
                gen->m_output << "      mov rax , 1\n";
                gen->m_output << "      mov rdi , 1\n";
                gen->m_output << "      mov rsi , " << gen->m_queue_data.front().first << "\n";
                gen->m_output << "      mov rdx , " << gen->m_queue_data.front().second  << "\n";
                gen->m_queue_data.pop();
                gen->m_output << "      syscall\n";

            }
        };

        SmtsVisitor visitor{.gen = this};
        std::visit(visitor , smts.var);
    }


    //Combine all the statements
    [[nodiscard]] std::string gen() {

        for (const auto& node : m_prg.smts)
        {
            if (auto* smts_naam = std::get_if<NodeSmtnaam>(&node.var))
            {
                if (auto smts_naam_value = std::get_if<NodeExpIntlit>(&smts_naam->expn.var)) {
                    m_value_stack[smts_naam->ident.value.value()] = std::stoi(smts_naam_value->int_lit.value.value());
                }
            }
        }
        m_output << "section .data\n";
        for (const auto& node : m_prg.smts) {

            if (auto* nstm = std::get_if<NodeSmtPrint>(&node.var)) {
                std::string msg_content; // To build the complete message

                // Iterate through the string literals
                for (const auto& string_lit : nstm->string_lit) {
                    if (string_lit.type == TokenType::IDENT) {
                        // Check if the identifier is in m_value_stack
                        if (m_value_stack.contains(string_lit.value.value())) {
                            int value = m_value_stack[string_lit.value.value()];
                            msg_content += std::to_string(value);
                            // Append the integer value to the message
                        } else {
                            std::cerr << "Undefined identifier: " << string_lit.value.value() << "\n";
                            exit(EXIT_FAILURE);
                        }
                    } else {
                        // For literal strings, append the literal value directly
                        msg_content += string_lit.value.value();
                    }
                }

                // Now output the combined message in .data section
                m_output << "   msg_" << m_unique_msg_name << " db '" << msg_content << "', 0xA \n"; // Add 0 for null-termination
                int com_size = msg_content.length() + 1;
                // Store the label to queue for later reference
                std::string temp = "msg_" + std::to_string(m_unique_msg_name);
                m_queue_data.emplace(temp , com_size);
                m_unique_msg_name++;
            }
        }




        m_output << "section .text\n";
        m_output << "   global _start\n" ;
        m_output << "   _start:\n";

        for(const auto& node : m_prg.smts) {
            gen_smts(node);
        }
        m_output << "      mov rax, 60\n";
        m_output << "      mov rdi, 0\n";
        m_output << "      syscall\n";

        return m_output.str();
    }


};
