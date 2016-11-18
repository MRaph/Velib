#include "solver.hpp"

DescentSolver::DescentSolver(Instance* inst) : Solver::Solver(inst) {
    name = "DescentSolver";
    desc = "Résolution par acceptation systématique d'un voisinage\n"
            ;
    logn1(name + ": " + desc + " inst: " + inst->name);
    // this->solution = new Solution(inst);
    // logn1("Apres  DescentSolver::DescentSolver pour instance " + inst->name);

    this->testsol = new Solution(inst);
    testsol->solve_stupid();
    this->cursol = new Solution(this->testsol);
    this->bestsol = new Solution(this->testsol);

    cerr << "DescentSolver non implémenté" << endl;
    logn1(name + ": " + desc + " inst: " + inst->name);
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
    logn4("DescentSolver::mutate BEGIN");
    // RIEN POUR L'INSTANT !
    logn4("DescentSolver::mutate END");
};

//./
