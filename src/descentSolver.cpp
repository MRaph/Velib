#include "solver.hpp"
#include <math.h>

// Constructeur de la classe : il crée une solution basique puis appelle le solve().
DescentSolver::DescentSolver(Instance* inst) : Solver::Solver(inst) {
    name = "DescentSolver";
    desc = "Résolution par acceptation systématique d'un voisinage\n";
    if (log1()) {
        logn1(name + ": " + desc + " inst: " + inst->name);
    }
    // Creation of a basic solution with the solve_stupid algorithm.
    this->testsol = new Solution(inst);
    testsol->solve_stupid();
    this->cursol = new Solution(this->testsol);
    this->bestsol = new Solution(this->testsol);
    // Improve the previous solution using the descent algorithm
    solve();
    if (log1()) {
        logn1(name + ": " + desc + " inst: " + inst->name);
    }
}

// Destructeur
DescentSolver::~DescentSolver()  {
    // If testsol exists, we delete it.
    if (this->testsol) {
        delete this->testsol;
    }
    // If bestsol exists, we delete it.
    if (this->bestsol) {
        delete this->bestsol;
    }
    // If cursol exists, we delete it.
    if (this->cursol) {
        delete this->cursol;
    }
}

// Algorithme de résolution de la classe
bool DescentSolver::solve() {
    if (log4()) {
        logn4("DescentSolver::solve BEGIN");
    }
    if (Options::args->explore == "all") {
        // In this case, accepts systematically the neighbour
        this->found = solve_explore_everything();
        if (log1()) {
            logn1("Explore everything");
        }
    } else if (Options::args->explore == "strict"){
        // In this case, accepts the neighbour if it improves the score
        this->found = solve_pure_descent();
        if (log1()) {
            logn1("Explore everything");
        }
    } else {
        cerr << "Argument --explore has to be set to all or strict" << endl;
    }

    if (log4()) {
        logn4("End solve DescentSolver");
    }
    this->found = true;
    return found;
}

// On accepte systématiquement chaque voisin
bool DescentSolver::solve_explore_everything() {
    if (log4()) {
        logn4("DescentSolver::solve_explore_everything BEGIN");
    }
    Options* args = Options::args;
    int nb_iter = args->itermax, i, diff;
	Solution* solution_current = new Solution(this->cursol);
    // For a number of iterations, we explore the niehgborhood
    for (i=0; i<nb_iter; i++) {
        // We apply a mutation on the current solution
        mutate(solution_current);
        // We compute the difference of score
        diff = solution_current->get_cost() - this->bestsol->get_cost();
        if (diff < 0) {
            // If it improves, we remember the solution as the best one
            this->bestsol->copy(solution_current);
        }
    }

    if (log4()) {
        logn4("DescentSolver::solve_explore_everything END");
    }
    return true;
}

// On accepte que les voisins améliorants
bool DescentSolver::solve_pure_descent() {
    if (log4()) {
        logn4("DescentSolver::solve_pure_descent BEGIN");
    }
    Options* args = Options::args;
    int nb_iter = args->itermax, i, diff;
	Solution* solution_current = new Solution(this->cursol);
    // For a number of iterations, we explore the niehgborhood
    for (i=0; i<nb_iter; i++) {
        // We apply a mutation on the current solution
        mutate(solution_current);
        // We compute the difference of score
        diff = solution_current->get_cost() - this->bestsol->get_cost();
        if(diff < 0) {
            // It improves, we remember the solution as the best one
            this->bestsol->copy(solution_current);
		} else {
			// It doesn't improve, we come back to the best solution remembered.
            solution_current->copy(this->bestsol);
            solution_current->update();
        }
    }
    if (log4()) {
        logn4("DescentSolver::solve_pure_descent END");
    }
    return true;
}

void DescentSolver::mutate(Solution* sol) {
    // Deux possibilités pour trouver un voisin :
    // - changer la position de deux stations au sein d'un meme circuit (2opt)
    // - retirer une station d'un circuit pour l'ajouter à un autre
    // Le choix de l'un ou l'autre est fait avec une probabilité 50-50
    if (log4()) {
        logn4("AnnealingSolver::mutate BEGIN");
    }

    bool are_different_circuits;
    int circuit_1_int, circuit_2_int, length_circuit_1, length_circuit_2, nb_circuits;
    int remove_position, add_position;
    Station* station_circuit_2;
    Station* station_circuit_1;
    Station* station_to_move;

    // We pick two random circuits
    nb_circuits = sol->circuits->size();
    circuit_1_int = rand() % nb_circuits;
    circuit_2_int = rand() % nb_circuits;
    // We test to see if the circuits are different
    are_different_circuits = (circuit_1_int == circuit_2_int);
    // We pick two random stations in each circuit
    Circuit* circuit_1 = sol->circuits->at(circuit_1_int);
    Circuit* circuit_2 = sol->circuits->at(circuit_2_int);
    length_circuit_1 = circuit_1->stations->size();
    length_circuit_2 = circuit_2->stations->size();
    bool is_circuit_1_empty = (length_circuit_1 == 0);
    bool is_circuit_2_empty = (length_circuit_2 == 0);

    if (is_circuit_2_empty && is_circuit_1_empty) {
        if (log3()) {
            logn3("AnnealingSolver::mutate - Both circuits are empty.");
        }
        // Do nothing
        return;
    } else if (is_circuit_1_empty && !is_circuit_2_empty) {
        if (log3()) {
            logn3("AnnealingSolver::mutate - Circuit 1 is empty.");
        }
        // Move station of circuit 2 to 1
        remove_position = rand() % length_circuit_2;
        station_circuit_2 = sol->circuits->at(circuit_2_int)->erase(remove_position);
        sol->circuits->at(circuit_1_int)->insert(station_circuit_2, 0);
        return;
    } else if (!is_circuit_1_empty && is_circuit_2_empty) {
        if (log3()) {
            logn3("AnnealingSolver::mutate - Circuit 2 is empty.");
        }
        // Move station of circuit 1 to 2
        remove_position = rand() % length_circuit_1;
        station_circuit_1 = sol->circuits->at(circuit_1_int)->erase(remove_position);
        sol->circuits->at(circuit_2_int)->insert(station_circuit_1, 0);
        return;
    } else {
        // Both circuits aren't empty
        if (are_different_circuits) {
            // Circuits are different and non-empty
            if (log3()) {
                logn3("AnnealingSolver::mutate - Switching stations");
            }
            // We remove a station from circuit 1 and add it to circuit 2
            remove_position = rand() % length_circuit_1;
            add_position = rand() % (length_circuit_2+1); // +1 to insert at the end
            station_to_move = sol->circuits->at(circuit_1_int)->erase(remove_position);
            sol->circuits->at(circuit_2_int)->insert(station_to_move, add_position);
            return;
        } else {
            // Same circuit, non-empty
            if (circuit_1->stations->size()>1) {
                if (log3()) {
                    logn3("AnnealingSolver::mutate - Intern mutation accepted.");
                }
                // Pick two random positions
                int k = rand() % (circuit_1->stations->size());
                int l = rand() % (circuit_1->stations->size());
                if (k != l) {
                    // mutate_2opt implies that first argument is lower than second one.
                    this->mutate_2opt(sol->circuits->at(circuit_1_int), min(k,l), max(k,l));
                } else  if (k==0) {
                    // if k is the last position, we switch the last two stations.
                    l = k+1;
                    this->mutate_2opt(sol->circuits->at(circuit_1_int), k, l);
                } else {
                    l = k;
                    k -= 1;
                    this->mutate_2opt(sol->circuits->at(circuit_1_int), k, l);
                }
            } else {
                if (log3()) {
                    logn3("AnnealingSolver::mutate - Intern mutation refused : Circuit is too short.");
                }
            }
        }
    }

    sol->update();
    if (log4()) {
        logn4("AnnealingSolver::mutate END");
    }
};
//./
