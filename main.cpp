#include "solver.hpp"
using namespace std;
#include <ctime>

int main(){


    solver s;
    string file = "./tests/test1.cnf";
    s.read_cnf(file);

    if(s.solve()){
        cout<<"Satisfiable\n";
        s.print_model();
    }else{
        cout<<"Unsatisfiable.";
    }
    

    return 0;
}