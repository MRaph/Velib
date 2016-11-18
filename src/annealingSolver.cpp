#include "solver.hpp"

AnnealingSolver::AnnealingSolver(Instance* inst) : Solver::Solver(inst) {
    name = "AnnealingSolver";
    desc = "Solver par recuit simulé";
    cursol = NULL;
    testsol = NULL;
    bestsol = NULL;

    cerr << "\nAnnealingSolver non implémenté : AU BOULOT !" << endl;
    exit(1);

}
AnnealingSolver::~AnnealingSolver()  {
    // delete cursol;
    // delete testsol;
    // delete bestsol;
}

// Méthode principale de ce solver
//
bool AnnealingSolver::solve() {
    found = false;
    return found;
}

void AnnealingSolver::mutate(Solution* sol) {
    logn4("AnnealingSolver::mutate BEGIN");
    // On choisit et on effectue le type de mouvement à effectuer selon
    // différents critères (équilibrage atteint, ...)

    // ...
    logn4("AnnealingSolver::mutate END");
}

//./
