# FusionLang


## Overview

**FusionLang** is a new programming language designed for flexibility and ease of use, incorporating innovative syntax inspired by everyday language. This project aims to provide developers with a unique programming experience.

## Features

- Simple and intuitive syntax
- Support for various programming constructs
- Future enhancements planned for comprehensive functionality

## Grammar

Fusion Lang's grammar is defined using ANTLR, supporting key constructs:

- **Statements**: Includes print statements, variable declarations, function definitions, conditionals, loops, and exit statements.
- **Keywords**:

  - `bolo`: For printing output. // `IMPLEMENTED`
  - `naam`: To declare variables. // `IMPLEMENTED (for int literals)`
  - `bulao`: For defining functions.
  - `khatam`: To exit functions or statements. // `IMPLEMENTED`
  - `agar, nahi_toh, warna`: For conditional statements.
  - `firse`: For for loops.
  - `jab_tak` : For while loops

Here's the README file in Markdown format, styled for GitHub so you can copy it directly:


# My Programming Language Documentation

## Table of Contents
- [Comments](#comments)
- [Variable Declaration](#variable-declaration)
- [Output Statement](#output-statement)
- [Program Termination](#program-termination)


### Comments
Comments in the code are used for adding explanatory notes or temporarily disabling code. There are two types of comments:
  
- **Single-Line Comments**: Denoted by `//`. Anything following `//` on the same line is a comment and will not be executed.
  ```plaintext
  // This is a single-line comment
  ```

- **Multiline Comments**: Wrapped between `/*` and `*/`. Text within this range is treated as a comment and ignored by the compiler.
  ```plaintext
  /* This is a 
     multiline 
     comment */
  ```

### Variable Declaration
Variables are declared using the keyword `naam` followed by the variable name, an equals sign `=`, and the value. Statements end with a semicolon `;`.
  ```plaintext
  naam x = 1340;
  naam y = 2000;
  ```

### Output Statement
The `bolo` command is used to print messages to the screen. Text to be printed is placed within quotation marks and parentheses. To include variable values within the output, use `{variable_name}` within the text.
  ```plaintext
  bolo("Hello World");
  bolo("The value of x is {x}"); 
  ```

### Program Termination
End the program with the `khatam` command and an exit code.
  ```plaintext
  khatam 0;
  ```


Copy this text into your GitHub README file to document your programming language syntax!
## Getting Started

To get started with Fusion Lang:

1. Create a file with the `.fl` extension for your code (e.g., `example.fl`).
2. Write your code using the Simple Lang syntax in the `.fl` file.
3. Run your code using the appropriate interpreter or compiler (details coming soon).


