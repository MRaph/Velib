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
    exit(1);
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
    } else if (Options::args->explore == "strict"){
        // In this case, accepts the neighbour if it improves the score
        this->found = solve_pure_descent();
    } else {
        printf("Argument --explore has to be set to all or strict\n");
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
        }
    }
    if (log4()) {
        logn4("DescentSolver::solve_pure_descent END");
    }
    return true;
}

// Effectue une mutation sur la solution en paramètre et renvoie un voisin.
void DescentSolver::mutate(Solution* sol) {
    // Deux possibilités pour trouver un voisin :
    // - changer la position de deux stations au sein d'un meme circuit (2opt)
    // - retirer une station d'un circuit pour l'ajouter à un autre
    // Le choix de l'un ou l'autre est fait avec une probabilité 50-50
    if (log4()) {
        logn4("DescentSolver::mutate BEGIN");
    }

    bool are_different_circuits;
    int circuit_1_int, circuit_2_int, length_circuit_1, length_circuit_2, nb_circuits, single_position;
    int remove_position, add_position;

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
    if (are_different_circuits) {
        if (length_circuit_1 < 2 && length_circuit_2 < 2) {
            // Switch remorque between the two circuits
            // Even if it seems useless, remorque capacity can modify the score
            Station* station_circuit_1 = circuit_1->erase(0);
            Station* station_circuit_2 = circuit_2->erase(0);
            circuit_1->insert(station_circuit_2, 0);
            circuit_2->insert(station_circuit_1, 0);
            return;
        } else if (length_circuit_1 == 1 && length_circuit_2 > 0) {
            // Remove circuit 1 and add station of the circuit to the other circuit
            single_position = rand() % length_circuit_2+1; // +1 to insert at the end
            Station* station_to_move = circuit_1->erase(0);
            circuit_2->insert(station_to_move, single_position);
            return;
        } else if (length_circuit_2 == 1 && length_circuit_1 > 0) {
            // Remove circuit 2 and add station of the circuit to the other circuit
            single_position = rand() % length_circuit_1+1; // +1 to insert at the end
            Station* station_to_move = circuit_2->erase(0);
            circuit_1->insert(station_to_move, single_position);
            return;
        } else {
            // We remove a station from circuit 1 and add it to circuit 2
            remove_position = rand() % length_circuit_1;
            add_position = rand() % length_circuit_2+1; // +1 to insert at the end
            Station* station_to_move = circuit_1->erase(remove_position);
            circuit_2->insert(station_to_move, add_position);
            return;
        }
    } else {
        // Circuits chosen are the same, we have to mutate two stations in the same circuit
        if (circuit_1->stations->size()>=2) {
            int k = rand() % (circuit_1->stations->size()-1);
            int l = rand() % (circuit_1->stations->size()-1);
            if (k != l) {
                // mutate_2opt implies that first argument is lower than second one.
                circuit_1->mutate_2opt(min(k,l), max(k,l));
            } else {
                // if k is the last position, we switch the last two stations.
                l = k+1;
                circuit_1->mutate_2opt(k, l);
            }
		}
    }

    if (log4()) {
        logn4("DescentSolver::mutate END");
    }
};

//./
