#include "solver.hpp"

Steepest2optSolver::Steepest2optSolver(Instance* inst) : Solver::Solver(inst) {
    name = "Steepest2optSolver";
    desc = "Résolution par acceptation systématique d'un voisinage\n"
            ;
    logn1(name + ": " + desc + " inst: " + inst->name);
    // this->solution = new Solution(inst);
    // logn1("Apres  Steepest2optSolver::Steepest2optSolver pour instance " + inst->name);

    this->testsol = new Solution(inst);
    testsol->solve_stupid();
    this->cursol = new Solution(this->testsol);
    this->bestsol = new Solution(this->testsol);

    cerr << "\nSteepest2optSolver non implémenté" << endl;
    exit(1);
}
Steepest2optSolver::~Steepest2optSolver()  {
}

// Méthode principale de ce solver, principe :
//
bool Steepest2optSolver::solve() {
    this->found = true;
    return found;
}
// Effectue une mutation sur la solution en paramètre.
//
void Steepest2optSolver::mutate(Solution* sol) {
    logn4("Steepest2optSolver::mutate BEGIN");
    // NE FAIT RIEN POUR L'INSTANT !
    logn4("Steepest2optSolver::mutate END");
};

//./
