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


# FusionLang Documentation

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

Here's how you can include installation instructions for CMake and NASM in your `README.md` under the "Getting Started" section:


# Getting Started

To get started with **FusionLang**, follow these steps:

## Prerequisites

Make sure you have the following installed on your system:

- **CMake**
- **NASM**

### Installing CMake

#### On Ubuntu

You can install CMake using the following command:

```bash
sudo apt update
sudo apt install cmake
```

#### On Windows

1. Download the CMake installer from the [CMake official website](https://cmake.org/download/).
2. Run the installer and follow the instructions.

#### On macOS

You can install CMake using Homebrew:

```bash
brew install cmake
```

### Installing NASM

#### On Ubuntu

You can install NASM using the following command:

```bash
sudo apt update
sudo apt install nasm
```

#### On Windows

1. Download the NASM installer from the [NASM official website](https://www.nasm.us).
2. Run the installer and follow the instructions.

#### On macOS

You can install NASM using Homebrew:

```bash
brew install nasm
```

## Building the Project

1. Clone the repository:

    ```bash
    git clone https://github.com/bignya23/FusionLang.git
    ```

2. Navigate into the project directory:

    ```bash
    cd FusionLang
    ```

3. Create a build directory:

    ```bash
    mkdir build
    cd build
    ```

4. Run CMake to generate the project files:

    ```bash
    cmake ..
    ```

5. Build the project:

    ```bash
    cmake --build .
    ```

## Running FusionLang

To run your FusionLang files, use the following command in the terminal:

```bash
./Fusionlang <filename.fl>
```

Replace `<filename.fl>` with the name of your FusionLang file.

### Example

If you have a file named `example.fl`, you would run:

```bash
./Fusionlang example.fl
```


### Summary
- The installation instructions for both CMake and NASM include commands for Ubuntu and methods for Windows and macOS, making it accessible for users on different platforms.
- The README now provides clear steps for users to set up their environment before building and running the project.

Feel free to make any adjustments or ask for more details!

