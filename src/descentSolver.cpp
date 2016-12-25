#include "solver.hpp"

DescentSolver::DescentSolver(Instance* inst) : Solver::Solver(inst) {
    name = "DescentSolver";
    desc = "Résolution par acceptation systématique d'un voisinage\n";
    if (log1()) {
        logn1(name + ": " + desc + " inst: " + inst->name);
    }

    this->testsol = new Solution(inst);
    testsol->solve_stupid();
    this->cursol = new Solution(this->testsol);
    this->bestsol = new Solution(this->testsol);

    cerr << "DescentSolver non implémenté" << endl;
    if (log1()) {
        logn1(name + ": " + desc + " inst: " + inst->name);
    }
    exit(1);
}
DescentSolver::~DescentSolver()  {
}

// Méthode principale de ce solver, principe :
//
bool DescentSolver::solve() {
    // ...
    // On pourra exploiter ici l'option booléenne Options::args->explore pour
    // choisir entre :
    // - une descente pure (on n'accepte que les voisins améliorants).
    // - une exploration (acceptation systématique de tout voisin)
    // ...
    this->found = true;
    return found;
}
// Effectue une mutation sur la solution en paramètre.
//
void DescentSolver::mutate(Solution* sol) {
    if (log4()) {
        logn4("DescentSolver::mutate BEGIN");
    }
    // RIEN POUR L'INSTANT !
    if (log4()) {
        logn4("DescentSolver::mutate END");
    }
};

//./
