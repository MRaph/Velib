#include "solver.hpp"

TabooSolver::TabooSolver(Instance* inst) : Solver::Solver(inst) {
    name = "TabooSolver";
    desc = "Solver par méthode tabou";
    cerr << "TabooSolver non implémenté !" << endl;
    exit(1);
}
TabooSolver::~TabooSolver()  {
}
// Méthode principale de ce solver, principe :
//
bool TabooSolver::solve() {
    found = false;
    return found;
}
//./

