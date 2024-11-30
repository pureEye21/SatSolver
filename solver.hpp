#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <bits/stdc++.h>
using namespace std;

class solver{ // * Solver class
    public:
        set< set<int> > clauses; // * Set of clauses that solver will act on

        void set_num_lit(int n); // * Sets the number of literals
        void add_clauses(vector< vector<int> > &clauses); // * Adds clauses to the solver
        void read_cnf(string file); // * Reads CNF from a file in DIMACS format
        bool solve(); 
        vector<int> get_model(); // ! Returns a model; Should be used only if given clauses are sat
        void print_model(); // ! Prints a model; Should be used only if given clauses are sat
        void reset();

    private:
        int num_literals; // * Number of literals/ Value of the highest literal used
        set<int> fixed; // * Set of literals whose values are fixed; Union of units and pures
        set<int> updated; // * To prevent duplicate updation
        set<int> dont_care; // * Literals who can be assigned both true or false
        bool change; // * Tracks if unit propagation/ pure literal formation has stabilised or not

        void add_clause(vector<int> &clause); 
        void unit_literal(); // * Finds and adds unit literals
        void pure_literal(); // * Finds and adds pure literals
        void clean(); // * Updates clauses whenever new fixed literals found
        void clean_DPLL(); // * Updates clauses
        int max_two_clause(); // * Counts literal (including its negation) with maximum occurrence in clauses with just two literals
        int max_three_clause(); // * Counts literal (including its negation) with maximum occurrence in clauses with just three literals
        int max_occurrence(); // * Returns the literal that is present maximum number of times in the clauses
};

#endif