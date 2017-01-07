#include "solver.hpp"

Solver::Solver(Instance* inst) {

    name = "S_0_abstract";
    desc = "Par itération : un simplex + un mip + un seul sous tour";

    this->inst = inst;
    found = false;
}
Solver::~Solver() {
    // todo;
}

// Méthode principale de ce solver, principe :
//
bool Solver::solve() {
    found = false;
    return found;
}

void Solver::mutate_2opt(Circuit* circuit, int i, int j) {
    if (i>=j){
        cout << "Erreur de mutate_2opt : i=" << i << " supérieur à j=" << j << endl;
    } else {
        list<Station*>::iterator it_i = circuit->stations->begin();
        list<Station*>::iterator it_j = circuit->stations->begin();
        std::advance(it_i, i);
        std::advance(it_j, j);
        list<Station*>* reverse_list = new list<Station*>();
        for (auto it = it_i; it != it_j; it++) {
            reverse_list->push_front(*it);
        }
        circuit->stations->erase(it_i, it_j);
        circuit->stations->insert(it_j, reverse_list->begin(), reverse_list->end());
        delete reverse_list;
    }
   	circuit->update();
    return;
}

void Solver::mutate_2opt_best(Circuit* circuit) {
    Circuit* curr_circuit = new Circuit(circuit);
    int score = circuit->get_cost();
    int curr_score;

    for (int i=0; i<circuit->stations->size()-1; i++) {
        for (int j=i+1; j<circuit->stations->size(); j++) {
           this->mutate_2opt(curr_circuit, i, j);
           //curr_circuit->partial_clear();
           //curr_circuit->optimal_initial_load();
           //curr_circuit->length = inst->get_dist(curr_circuit->stations, curr_circuit->remorque);
           //curr_circuit->equilibrate();
           curr_score = curr_circuit->get_cost();
           if (curr_score < score) {
               score = curr_score;
               circuit->copy(curr_circuit);
           }
        }
    }
    delete curr_circuit;
    return;
}
// ./
