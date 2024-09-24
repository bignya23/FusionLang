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

    Tokenizer tokenizer(contents);
    std::vector<Token> tokens = tokenizer.tokenize();

    Parser parser(tokens);

    std::optional<NodePrg> tree = parser.parse_prg();

    if(!tree.has_value()) {
        std::cerr << "No valid values" << std::endl;
        exit(EXIT_FAILURE);
    }

    CodeGenerator generator;

    generator.generate_program(tree.value());

    generator.save_assembly_to_file("out.asm");



    system("nasm -f win64 -o out.o out.asm");
    system("ld -o output.exe out.o");
    system("output.exe");




    return EXIT_SUCCESS;
}
