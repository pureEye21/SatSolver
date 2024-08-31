#include "solver.hpp"



void solver::add_clause(vector<int> (&clause)){
    
    set <int> clause_ta; // * Clause to add
    bool tautology = false;
    
    for(auto i:clause){
        if(clause_ta.find(-i) != clause_ta.end()){
            tautology = true;
            break;
        }
        clause_ta.insert(i);
    }

    if(!tautology){ // Add the clause only if the clause is not a tautology
        this->clauses.insert(clause_ta);
    }
}

void solver::add_clauses(vector< vector<int> > (&clauses)){
    for(auto c : clauses){
        this->add_clause(c);
    }
}

void solver::pure_literal(){ // * Adds pure literals to the set of "fixed" literals
    for(int x = 1; x <= num_literals; x++){
        // If the literal is already added to set of fixed literals or don't cares , continue
        if(fixed.find(x) != fixed.end() || fixed.find(-x) != fixed.end()){ 
            continue;
        }else if(dont_care.find(x) != dont_care.end() || dont_care.find(-x) != dont_care.end()){
            continue;
        }

        int pres = 0;
        bool pure = true;
        
        for(auto it : clauses){
            if(it.find(x) != it.end()){
                if(!pres){
                    pres = x;
                }else if(pres == -x){ // Both x and -x present, not pure
                    pure = false;
                    break;
                }
            }
            
            if(it.find(-x) != it.end()){
                if(!pres){
                    pres = -x;
                }else if(pres == x){ // Both x and -x present, not pure
                    pure = false;
                    break;
                }
            }
        }

        if(!pres){ // * If neither x nor -x is present in the CNF, then it is a don't care
            dont_care.insert(x);
        }else if(pure){ // * Fix the pure literal x along with its polarity
            fixed.insert(pres);
        }
    }
}

void solver::unit_literal(){ // * Adds literals present in unit clauses to set of "fixed" literals
    for(auto it : clauses){

        if(it.size() == 1){
            int x = *(it.begin());
            fixed.insert(x);
        }
    }
    
}

void solver::clean(){ // * Cleans each clause conditioned to the literals which are fixed
    for(auto x : fixed){
        if(updated.find(x) != updated.end()){ // * Already updated w.r.t. x
            continue;
        }else{
            change = true;
        }

        set< set<int> > to_remove, to_modify;
        
        for(auto c : clauses){
            if(c.find(x) != c.end()){
                to_remove.insert(c); // * Add clauses to be removed as a result of x being fixed
            }else if(c.find(-x) != c.end()){
                to_modify.insert(c); // * Remove negation of x from every clause if present
            }
        }

        for(auto c : to_remove){
            clauses.erase(c);
        }
        for(auto c : to_modify){
            clauses.erase(c);
            c.erase(-x);
            clauses.insert(c);
        }

        updated.insert(x);
    }
}

void solver::clean_DPLL(){
    change = true;

    clean();

    while(change){ // * Continuously clean the clauses till no new unit literal(s) is(are) formed
        change = false;

        unit_literal();
        clean();
    }

    change = true;

    while(change){ // * Continuously clean the clauses till no new pure literal(s) is(are) formed
        change = false;

        pure_literal();
        clean();
    }
    
}

bool solver::solve(){

    // * Standard DPLL

    clean_DPLL();
    
    if(clauses.size() == 0){
        return true;
    }

    for(auto c : clauses){
        if(c.size() == 0){
            return false;
        }
    }

    // * Choose the literal optimally to send in DPLL algorithm using some branching heuristics
    int l=0;
    {
        l = max_two_clause(); 

        if(!l){
            l = max_three_clause();
        }
        
        if(!l){
            l = max_occurrence();
        }
    }

    // * Backup the current state of the variables
    set< set<int> > clone_clauses(clauses);
    set<int> clone_fixed(fixed);
    set<int> clone_updated(updated);
    set<int> clone_dont_care(dont_care);

    fixed.insert(l);

    if(solve()){ // * l helps in SAT
        return true;
    }else{ // * l did not help with SAT, trying with -l

        // * Resetting the variables with original value before fixing l
        clauses = clone_clauses;
        fixed = clone_fixed;
        updated = clone_updated;
        dont_care = clone_dont_care;
        
        fixed.insert(-l);

        return solve();
    }

}

void solver::set_num_lit(int n){
    num_literals = n;
}

void solver::read_cnf(string file_name){ // * Reads the input cnf file 

    freopen(file_name.c_str(), "r", stdin);

    string extra;
    do{
        cin>>extra;
    }while(extra != "p");

    cin>>extra;
    assert(extra == "cnf");

    int num_clause;
    cin>>num_literals>>num_clause;

    while(num_clause--){
        set<int> clause;
        int p;

        cin>>p;
        while(p != 0){
            clause.insert(p);
            cin>>p;
        }

        clauses.insert(clause);
    }

    fclose(stdin);
}

vector<int> solver::get_model(){
    vector<int> assignment;

    for(int i=1; i<=num_literals; i++){
        // * Fixed literals are assigned as is, dont care literals are assigned randomly
        if(fixed.find(i) != fixed.end()){
            assignment.push_back(i);
        }else if(fixed.find(-i) != fixed.end()){
            assignment.push_back(-i);
        }else if(dont_care.find(i) != dont_care.end()){
            assignment.push_back((rand() % 2 ? 1 : -1) * i);
        }
    }

    return assignment;
}

void solver::print_model(){
    // * Prints the assignment of the literals: +x denotes x should be set T, -x denotes x should be set F
    cout<<"\nHere is a model:\n";
    auto assignment = get_model();

    for(auto x : assignment){
        cout<<x<<' ';
    }
    cout<<'\n';
}

int solver::max_two_clause(){
    int cand=0, cand_c=0; // * Count of current candidate literal
    map<int, int> occurrence;

    for(auto c : clauses){
        if(c.size() == 2){
            for(auto x : c){
                occurrence[max(x, -x)]++;
                if(cand_c < occurrence[max(x, -x)]){
                    cand = x;
                    cand_c = occurrence[max(x, -x)];
                }
            }
        }
    }

    return cand;
}

int solver:: max_three_clause(){
    int cand=0, cand_c=0; // * Count of current candidate literal
    map<int, int> occurrence;

    for(auto c : clauses){
        if(c.size() == 3){
            for(auto x : c){
                occurrence[max(x, -x)]++;
                if(cand_c < occurrence[max(x, -x)]){
                    cand = x;
                    cand_c = occurrence[max(x, -x)];
                }
            }
        }
    }

    return cand;
}

int solver::max_occurrence(){
    int cand=0, cand_c=0; // * Count of current candidate literal
    map<int, int> occurrence;

    for(auto c : clauses){
        for(auto x : c){
            occurrence[max(x, -x)]++;
            if(cand_c < occurrence[max(x, -x)]){
                cand = x;
                cand_c = occurrence[max(x, -x)];
            }
        }
    }

    return cand;
}

void solver::reset(){ // * Re-initialize for new input CNF
    clauses.clear();
    fixed.clear();
    updated.clear();
    dont_care.clear();
    num_literals = 0;

    return;
}