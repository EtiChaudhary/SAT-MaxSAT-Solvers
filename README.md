# SAT-MaxSAT-Solvers
SAT Solving using DPLL and CDCL. MaxSAT Solver based on MiniSAT.

## Instructions to run DPLL and CDCL Solvers
Both the solvers accept a DIMACS format. To run them, follow the instructions given below:
```bash
cd <source-directory>
g++ -o dpll -O2 --std=c++11 dpll.cpp
./dpll < dimacs-file
```
Replace *dpll* with *cdcl* to run the CDCL Solver.
The solver will output *UNSAT* if the input formula is UNSAT. Otherwise, it will output *SAT* along with the satisfying assignment.

## Instructions to run MaxSAT Solver
MaxSAT solver is built on top of Minisat 2.2.0 as the underlying solver.

Before compiling the source, make sure:
  - $MROOT points to the top-level minisat directory 
  - A static linked libminisat.a is generated (using make in core directory and then renaming lib_release.a to libminisat.a)

Then follow the instructions below:
```bash
cd <source-directory>
g++ -O2 -I $MROOT -L $MROOT/core maxsat.cpp -lminisat
./a.out <dimacs-file>
```
The program outputs the max number of satisfiable clauses and the corresponding SAT assignment.

