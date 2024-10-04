#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>
#include "tokenization.hpp"
#include "parser.hpp"
#include "generation.hpp"

int main(const int argc , char *argv[]) {

    if(argc != 2) {
        std::cerr << "Incorrect Usage" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <filename.jd>" << std::endl;
        return EXIT_FAILURE;


    }

    std:: string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1] , std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }
    //Convert into tokens
    Tokenizer tokenizer(contents);
    std::vector<Token> tokens = tokenizer.tokenize();

    //Parse into tokens ans generate vectors of
    Parser parser(tokens);
    std::optional<NodePrg> prog = parser.parse_prg();

    if(!prog.has_value()) {
        std::cerr << "Some Invalid statements are there!!" << std::endl;
        exit(EXIT_FAILURE);
    }

    Generator generator((prog.value()));
    std::string asm_code = generator.gen();
    {
        std::fstream file("out.asm" , std::ios::out);
        file << asm_code;

    }

     //For Unix
     system("nasm -f elf64 -o out.o out.asm");
     system("ld -o output out.o");
     system("./output");

    // For Windows
    // system("nasm -f win64 -o out.o out.asm");
    // system("ld -o output.exe out.o");
    // system("output.exe");

    return EXIT_SUCCESS;
}
