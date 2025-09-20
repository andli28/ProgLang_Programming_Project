# COMS W4115 Fall 2025: Programming Assignment 1

## 1\. Overview of the Programming Project

This is the first of four programming assignments where you will build a compiler for MiniLang. This assignment focuses on the first phase: **Lexical Analysis**.

For this assignment, you will implement a lexical analyzer (scanner) in C++. This is an individual assignment.

A detailed lexical specification for MiniLang can be found in the ["MiniLang specsheet - Lexical"](https://docs.google.com/document/d/1QoHRme1exPaBUvD9djJaGjcuq7A--uN1LTLgmENxwws/edit?tab=t.0) document.

## 2\. Files Provided

The following files are provided to get you started:

  * `lexer.h`: Header file for the `Lexer` class.
  * `lexer.cpp`: The implementation file for the `Lexer` class, which you need to complete.
  * `exception.h`: Contains custom exceptions for the compiler.
  * `token.h`: Defines the `Token` class. A token includes a type, text, line, and column.
  * `Makefile`: Used for building and testing the project.
  * `main.cpp`: The main driver code to run your lexer.
  * `test/`: A directory containing sample inputs and expected outputs.

## 3\. Deliverables

### `lexer.cpp`

You must complete the `Token Lexer.getNextToken()` method in `lexer.cpp`.

**Requirements:**

  * The method should read from the input file stream.
  * Each call to `getNextToken()` must scan and return exactly one token based on the MiniLang lexical specification.
  * The lexer must accurately track the current `line` and `column` numbers in the source file.
  * Every token created must be added to the lexer's `tokens` vector.
  * Each `Token` object must be correctly populated with:
      * Token Type (e.g., Keyword, Identifier, Integer, etc.).
      * Token Value (the lexeme string).
      * Line Number (where the token begins).
      * Column Number (where the token begins).
  * Whitespace characters and comments must be skipped and should not generate tokens.
  * If an unrecognized character is found, a `ParserException` must be thrown immediately with the character's line and column number, and scanning must stop.
  * When the end of the input is reached, `getNextToken()` should return an `EndOfFile` (EOF) token.

### `DESIGN.md`

You must also submit a `DESIGN.md` file that discusses the following points:

1.  A description of your lexical scanner implementation.
2.  Any challenges you encountered during design or implementation.
3.  What you learned from this assignment.
4.  At least three new and distinct test cases you created that are not simple variations of the provided examples.

## 4\. Submission

  * Submit `lexer.cpp` and `DESIGN.md` to Gradescope. The filenames must be exact.
  * The autograder will run on your submission and provide feedback.
  * The autograder score is out of 80 points; the `DESIGN.md` file will be graded manually for the remaining points.
  * Your submission will also be evaluated against hidden test cases, so thorough testing is recommended.

## 5\. Testing

We have provided sample test cases to help you understand the requirements.

  * To run the provided tests, use the command:

    ```bash
    make test
    ```

    This command will execute your lexer against inputs in `test/input` and place the results in `test/result`.

  * **Example Test Case:**

    **Input:**

    ```
    var y := 20;
    ```

    **Output:**

    ```
    Keyword('var') at 1:1
    Identifier('y') at 1:5
    AssignOp(':=') at 1:7
    Integer('20') at 1:10
    Delimiter(';') at 1:12
    EOF('') at 1:12
    ```

## 6\. Logistics

  * No late submissions will be accepted.
  * Please see GradeScope for submission deadlines.
  * For questions, please use EdStem and attend TA office hours.
  * It is highly recommended to start early.

## 7\. Hints

  * Keep track of token locations by incrementing the column for each character and the line for each newline (resetting the column to 0).
  * Whitespace and newlines should be skipped and not produce tokens.
  * MiniLang comments start with `#` and extend to the end of the line; they should be ignored.
  * Always match the longest possible token (maximal munch). For example, `3456` should be a single integer token, not four separate digit tokens.
  * You are not required to use Regex, NFA, or DFA conversions for this assignment.
  * Consider using `istream::get()` for reading characters from the input stream.
  * Ensure your lexer emits a final `EndOfFile` token to mark the end of the input stream.