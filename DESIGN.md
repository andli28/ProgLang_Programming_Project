# Design of lexical scanner

### Implementation
My goal was to lex the simple [basic test](test/input/basic_assignment.minilang) correctly first, and then move on to the edge cases.

At first I tackled white space - I thought this was the most straightforward, as we either just skip it and move the line and col pointers, or we cut off the current read text and see if it's a valid token.

I realized quickly that I would need to check for if a given string was a valid token multiple times, so I refactored that part into a seperate function. It's fairly straightforward, checking for membership in several sets, and then checking if it matches the identifier or number regexes provided in the minilang spec sheet.

The rest of the implementation involved looping through the characters using `ifstream`, and appropriately breaking from the loop whenever we are certain we can't form a longer token. We then check if the current text scanned is a valid token or not, and return or throw an error accordingly.

### Challenges
I think figuring out the best way to approach this problem was the main challenge. There was some edge cases I had to consider. First, after each character read, I wanted to throw an error if the current text so far didn't match a token. But then I realized there were cases such as "!" and "=" which are not valid tokens but they can be extended to valid tokens. This means that I have to keep reading characters until I can for sure say that the current text cannot be any token, in order to throw a Lexer error.

To do this, I created a set of all characters of length one, and would check if a character such as "!" or "=" showed up. In this case, I would keep reading characters. This is because the maximal length of any lexeme except identifiers, keywords, and numbers in this language is of length two.

If operators or delimiters could be longer, I would look into better ways to figure out if the current text is a prefix of them. I looked into this online and found the trie data structure. I noted that it could be useful in the future, but for now it wasn't necessary.

### Learning
I learned about looking ahead by one to figure out if the current token being lexed matches a valid token.

I also learned about some of the challenges with implementing lexers, mentioned above.

### Extra test cases

1. [new_lines](test/myinputs/new_lines.minilang) tests that we correctly move the line and col pointers when we have new lines and not just spaces.
2. [maximal munch](test/myinputs/maximalmunch.minilang) tests that we perform maximal munch correctly, and that we can lex tokens even without whitespaces as seperators
3. [tabs](test/myinputs/tabs.minilang) tests that we treat tabs as whitespaces. It also checks that alphanumeric as tokenized as identifiers correctly, and that we throw a lexer error when we encounter a token that invalidates the current content in our token buffer.