# Interpreter
A simple interpreter for a simple programming language.

Run it on the command line without arguments to start the interpreter in REPL mode.
Alternatively enter the path to a text file with source code as an argument like to run your program like so:<br />
    interpreter sourcecode.txt<br />
 
Some example code: <br />



  config dec<br />
  print 1 + 1<br />
  print 3 + 3 * 3<br />
  x = 5 * ( 5 + 15 - 10 )<br />
  y = x * -2<br />
  print x<br />
  print y<br />
  x2 = x / 5 + 5<br />
  print x2<br />
  config hex<br />
  print x2<br />
  config bin<br />
  print x2<br />
 
 
Grammar for the language: <br />
  Stmt:= ConfigStmt | AssgStmt | PrintStmt<br />
  ConfigStmt := "config" [ "dec" | "hex" | "bin” ]<br />
  AssgStmt := Variable "=" MathExp<br />
  PrintStmt := "print" MathExp<br />
  MathExp := SumExp<br />
  SumExp := ProductExp [ "+" ProductExp | "–" ProductExp ]*<br />
  ProductExp := PrimaryExp [ "*" PrimaryExp | "/" PrimaryExp ]*<br />
  PrimaryExp := Int | Variable | "(" MathExp ")"<br />

  Variable := [a-zA-z][a-zA-z0-9]*<br />
  Int := -?[0-9]+<br />
