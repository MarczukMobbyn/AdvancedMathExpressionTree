# Mathematical Expression Tree

A C++11 interactive calculator built around a prefix-notation expression tree. Supports arithmetic and trigonometric operators, symbolic variables, subtree composition, and result persistence — with a custom reference-counted smart pointer and a Result-type error handling system.

---

## Table of Contents

- [Overview](#overview)
- [Architecture](#architecture)
- [Requirements](#requirements)
- [Building](#building)
- [Usage](#usage)
- [Expression Syntax](#expression-syntax)
- [File Structure](#file-structure)

---

## Overview

The program parses mathematical formulas written in prefix (Polish) notation into a binary expression tree. Each node is either an operator, a numeric literal, or a named variable. The tree can be evaluated by supplying concrete values for all variables, extended via a JOIN operation that grafts a second tree onto the first available leaf, and serialized to disk.

The project demonstrates several advanced C++ techniques:

- **Custom smart pointer** with a shared reference counter and a tracked list of all live instances (`CMySmartPointer`, `CRefCounter`)
- **Result type** for error propagation without exceptions (`CResult<T, E>`)
- **Deep polymorphism** through a `CNode` hierarchy covering operators, numbers, and variables
- **Move semantics** — explicit move constructor and move-assignment operator on `CTree`, with counters to verify elision behavior under `-fno-elide-constructors`

---

## Architecture

```
CNode  (abstract base)
├── CNumber      literal double value
├── CVar         named variable
└── COperator    +  -  *  /  sin  cos
      └── v_children[]   argument nodes

CTree
├── c_parseExpression()   tokenize + parse -> CResult<CTree, CError>
├── operator+()           non-mutating join, returns new tree
├── v_join()              mutating join, replaces first leaf in-place
└── d_comp()              evaluate with a variable->value map

CResult<T, E>             discriminated union: value or error list
CMySmartPointer<T>        intrusive shared_ptr with pointer registry
CInterface                REPL loop, command dispatch
CResultWriter             serializes CResult<CTree, CError> to file
```

---

## Requirements

- C++11-compliant compiler (GCC 5+, Clang 3.4+, or MSVC 2015+)
- CMake 3.0 or later

---

## Building

```bash
git clone https://github.com/<username>/TEP_Lista_3.git
cd TEP_Lista_3

mkdir build && cd build
cmake ..
cmake --build .

./TEP_Lista_3
```

The build enables `-fno-elide-constructors` to suppress return-value optimization, making copy and move counts from `CTree::i_copyCount` / `CTree::i_moveCount` accurate during testing.

---

## Usage

The program presents a text-based REPL. Commands are case-insensitive.

| Command  | Arguments              | Description                                               |
|----------|------------------------|-----------------------------------------------------------|
| `ENTER`  | `<expression>`         | Parse a prefix expression and set it as the active tree   |
| `COMP`   | `<v1> <v2> ...`        | Evaluate the tree; supply one value per variable in order |
| `JOIN`   | `<expression>`         | Graft a second tree onto the first leaf of the active tree|
| `PRINT`  |                        | Print the active tree in prefix notation                  |
| `VARS`   |                        | List all distinct variable names in the tree              |
| `LEAVES` |                        | Print the number of leaf nodes                            |
| `SAVE`   | `<filename>`           | Write the tree in prefix notation to a file               |
| `EXIT`   |                        | Quit                                                      |

### Example session

```
> ENTER + * x 2 sin y
Tree parsed successfully.
Current tree: + * x 2 sin y

> VARS
Variables: x, y

> COMP 3 1.5707963
Result: 7.000000

> JOIN - a b
T1 before: + * x 2 sin y
T2:        - a b
T1 after:  + * - a b 2 sin y

> SAVE output.txt
Tree written to: output.txt

> EXIT
```

---

## Expression Syntax

Expressions are written in **prefix notation**: the operator precedes its operands.

### Supported operators

| Operator | Arity | Example  | Meaning |
|----------|-------|----------|---------|
| `+`      | 2     | `+ x 1`  | x + 1   |
| `-`      | 2     | `- a b`  | a - b   |
| `*`      | 2     | `* 3 x`  | 3 * x   |
| `/`      | 2     | `/ x 2`  | x / 2   |
| `sin`    | 1     | `sin x`  | sin(x)  |
| `cos`    | 1     | `cos x`  | cos(x)  |

### Parsing rules

- **Numbers** — non-negative integers only (`0`, `1`, `42`).
- **Variables** — any alphanumeric token containing at least one letter (`x`, `var1`, `alpha`). Characters other than letters and digits are stripped with a warning.
- **Missing arguments** — automatically substituted with `0`; a warning is recorded in the result.
- **Excess tokens** — tokens appearing after a complete expression are discarded with a warning.
- **Division by zero** — raises `CDivisionByZeroException` at evaluation time, caught and reported by the interface layer.

---

## File Structure

```
.
├── CMakeLists.txt
├── main.cpp
├── CInterface.h
├── CInterface.cpp
├── CTree.h
├── CTree.cpp
├── CNode.h
├── CNumber.h
├── CNumber.cpp
├── CVar.h
├── CVar.cpp
├── COperator.h
├── COperator.cpp
├── CResult.h
├── CResultWriter.h
├── CError.h
├── CError.cpp
├── CTreeException.h
├── CMySmartPointer.h
└── CRefCounter.h
```

| File | Description |
|------|-------------|
| `CMakeLists.txt` | Build configuration |
| `main.cpp` | Entry point and manual tests |
| `CInterface.h/.cpp` | REPL loop and command handlers |
| `CTree.h/.cpp` | Tree logic, parser, join, evaluation |
| `CNode.h` | Abstract node base class |
| `CNumber.h/.cpp` | Literal numeric node |
| `CVar.h/.cpp` | Variable node |
| `COperator.h/.cpp` | Operator node with child management |
| `CResult.h` | `Result<T,E>` template, including `void` specialization |
| `CResultWriter.h` | File serialization of `CResult<CTree, CError>` |
| `CError.h/.cpp` | Error message wrapper |
| `CTreeException.h` | Exception hierarchy (`CDivisionByZeroException`) |
| `CMySmartPointer.h` | Reference-counted smart pointer |
| `CRefCounter.h` | Shared reference counter with pointer registry |
