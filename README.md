# Interpreter
A simple interpreter for a simple programming language.

Run it on the command line without arguments to start the interpreter in REPL mode.
Alternatively enter the path to a text file with source code as an argument like to run your program like so:
    interpreter sourcecode.txt
 
Some example code: 
  config dec
  print 1 + 1
  print 3 + 3 * 3
  x = 5 * ( 5 + 15 - 10 )
  y = x * -2
  print x
  print y
  x2 = x / 5 + 5
  print x2
  config hex
  print x2
  config bin
  print x2
 
 
Grammar for the language: 
  Stmt:= ConfigStmt | AssgStmt | PrintStmt
  ConfigStmt := "config" [ "dec" | "hex" | "bin” ]
  AssgStmt := Variable "=" MathExp
  PrintStmt := "print" MathExp
  MathExp := SumExp
  SumExp := ProductExp [ "+" ProductExp | "–" ProductExp ]*
  ProductExp := PrimaryExp [ "*" PrimaryExp | "/" PrimaryExp ]*
  PrimaryExp := Int | Variable | "(" MathExp ")"

  Variable := [a-zA-z][a-zA-z0-9]*
  Int := -?[0-9]+
