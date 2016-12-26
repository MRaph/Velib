#include "solver.hpp"
#include <math.h>

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
// On part d'un circuit fixé C0, d'un température T0
// On effectue deux types d'itérations :
// La première correspond à une baisse de la température du système
// La deuxième correspond à une tentative d'amélioration de circuits
// Dans l'amélioration du circuit :
// - Si le poids est inférieur, on accepte le nouveau circuit
// - Si le poids est supérieur, on accepte si Uniforme(0,1) < exp(-(cost(new)-cost(old))/T0)
// On accepte beaucoup de circuit au départ, puis au fur et à mesure, de moins en moins
// car la température du système est updaté suivant : T(n+1) = temperature_update * T(n)
bool DescentSolver::solve() {
    if (log4()) {
        logn4("Start solve DescentSolver");
    }
    // ...
    // On pourra exploiter ici l'option booléenne Options::args->explore pour
    // choisir entre :
    // - une descente pure (on n'accepte que les voisins améliorants).
    // - une exploration (acceptation systématique de tout voisin)
    // ...
    if (Options::args->explore == false) {
        this->found = solve_recuit_simule();
    } else if (Options::args->explore == true) {
        this->found = solve_explore_everything();
    } else {
        this->found = solve_pure_descent();
    }

    if (log4()) {
        logn4("End solve DescentSolver");
    }
    this->found = true;
    return found;
}

bool DescentSolver::solve_explore_everything() {
    return true;
}

bool DescentSolver::solve_pure_descent() {
    return true;
}

// Algorithme du recuit simulé pour une temperature initiale
// non-nulle et non-infinie
bool DescentSolver::solve_recuit_simule() {
    int temperature_init = 1000000;
    float temperature_update = 0.95;
    int nb_iterations_temperature = 1;
    int temperature_current = temperature_init;
    float criteria_stop = 0.00001*temperature_init;

    int nb_iterations_ameliorations;
    int diff;
    double r;
    while (temperature_current > criteria_stop) {
        nb_iterations_ameliorations = 0;
        while (nb_iterations_ameliorations < nb_iterations_temperature) {
            mutate(this->cursol);
            diff = this->cursol->get_cost() - this->bestsol->get_cost();
            if (diff < 0) {
                // Copy this->cursol into this->bestsol
                if (log7()) {
                    logn7("New solution with lower cost has been found");
                }
            } else {
                // Take a random number between 0 and 1
                r = ((double) rand() / (RAND_MAX));
                // Compare it with exp(-diff/temperature_init)
                if (r < exp(-diff/temperature_init)) {
                    // Copy this->cursol into this->bestsol
                    if (log7()) {
                        logn7("New solution with higher cost has been found.");
                    }
                }
            }
            nb_iterations_ameliorations += 1;
        }
        temperature_current = temperature_current*temperature_update;
    }

    return true;
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
