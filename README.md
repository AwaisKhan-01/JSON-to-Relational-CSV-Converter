# README - JSON to Relational CSV Converter

## Overview
This project, `json2relcsv`, converts JSON files into relational CSV tables. It parses JSON input, constructs an Abstract Syntax Tree (AST), generates a relational schema, and outputs the data as CSV files. The implementation uses Flex for lexing, Bison for parsing, and custom C code for schema generation and CSV output.

- **Authors**: Awais Khan (22i-0997), Shayan Memon (22i-0773)
- **Section**: B
- **Date**: May 11, 2025

## Features
- Lexes JSON tokens (e.g., `{`, `}`, `[`, `]`, strings, numbers).
- Parses JSON into an AST, supporting objects, arrays, and primitive values.
- Generates relational tables from the JSON structure.
- Outputs tables as CSV files to a specified directory.
- Prints the AST for debugging.
- Handles basic error cases (e.g., file not found, syntax errors).

## Requirements
- GCC compiler
- Flex (lexer generator)
- Bison (parser generator)
- Make (for building)

## Setup
1. Clone the repository or copy the project files to your directory.
2. Ensure dependencies are installed:
   ```bash
   sudo apt-get install flex bison gcc make
   ```
3. Navigate to the project directory.

## Build Instructions
1. Run the following command to build the project:
   ```bash
   make all
   ```
   - This generates the executable `build/json2relcsv` from source files (`scanner.l`, `parser.y`, etc.).

2. Clean build artifacts if needed:
   ```bash
   make clean
   ```

## Usage
Run the program with a JSON file and optional output directory:
```bash
./build/json2relcsv <input_json_file> [--out-dir <output_directory>]
```
- `<input_json_file>`: Path to the JSON file (e.g., `tests/large.json`).
- `--out-dir <output_directory>`: Directory for CSV output (default is current directory).

### Example
```bash
./build/json2relcsv tests/large.json --out-dir build
```
- This processes `tests/large.json` and generates CSV files (e.g., `build/table0.csv`) in the `build` directory.

### Test
Run the test suite with predefined JSON files:
```bash
make test
```
- Tests `case1.json`, `case2.json`, and `large.json`, outputting CSVs to `build/`.

## File Structure
- `src/scanner.l`: Flex lexer definitions.
- `src/parser.y`: Bison parser grammar.
- `src/json2relcsv.c`: Main program and AST printing.
- `src/ast.c`, `src/ast.h`: AST data structures and operations.
- `src/schema.c`, `src/schema.h`: Schema generation logic.
- `src/csv.c`, `src/csv.h`: CSV output functions.
- `Makefile`: Build and test automation.
- `tests/`: Contains test JSON files (e.g., `large.json`).

## Output
- CSV files are generated based on the JSON structure (e.g., `table0.csv` for root, additional tables for nested data).
- The AST is printed to stdout for debugging (e.g., object hierarchies with indentation).

## Known Issues
- Large JSON files (>1MB) may cause segmentation faults if memory is not managed properly (mitigated with recent fixes).
- Excessive table creation was resolved by matching schemas, but performance with very large files needs optimization.
- Error handling could be improved (e.g., memory allocation failures).

## Future Improvements
- Add memory usage optimization for large files.
- Enhance error recovery and reporting.
- Include comprehensive documentation within code files.

## Credits
- **Awais Khan (22i-0997)**: Contributed to schema generation and debugging.
- **Shayan Memon (22i-0773)**: Contributed to parsing and AST implementation.
- Guided by xAI's Grok 3 for troubleshooting and code suggestions.

---

### Notes
- The README assumes the code is now complete based on your statement. It reflects the current state (e.g., 82/100 score from the evaluation, with fixes for table over-creation).
- Save this as `README.md` in your project directory for Markdown formatting.
- Adjust the "Known Issues" and "Future Improvements" sections if you identify additional points during final testing.

Let me know if you need further refinements or additional sections!
