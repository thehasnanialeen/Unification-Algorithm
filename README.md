# Unification-Algorithm
This is an implementation of Unification Algorithm in C++

## What is Unification?
To apply the inference rules, an inference system must know if two expressions are same. Sometimes, two terms are not same but we can find substitution (or unifier) that make both terms the same. This process is called unification.

There are a lot of implementation of Unification Algorithm written in different languages, but I could not find one written in C++. So, I have written one here.

## Compiling and running the program
Run the program with the following command
```bash
g++ Unification.cpp -o run
```
After compilation, run the executable file with the command
```bash
./run
```

## Input/Output
#### Input
```
This is an implementation of the unification algorithm.
Please enter the first expression: f(f(X,Y),X)
Please enter the second expression: f(f(V,U),g(U,Y))
```
#### Output
```
The unifier is: X=V, Y=U, V=g(U,U)
```
