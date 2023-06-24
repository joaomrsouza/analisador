console.log(5 + 10);
console.log(5 - 10);
console.log(5 * 10);
console.log(5 / 10);

// Desired Lexical output for console.log(5 + 10);
/*
token: COMMAND    lexeme: console.log
token: LPAREN     lexeme: (
token: FACTOR     lexeme: 5
token: PLUS_OP    lexeme: +
token: FACTOR     lexeme: 10
token: RPAREN     lexeme: )
token: SEMICOLON  lexeme: ;
token: EOF        lexeme: EOF
*/
