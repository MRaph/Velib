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


    //exit(1);
}
Steepest2optSolver::~Steepest2optSolver()  {
}


// Cod� par Mathilde :

// 1) Construction solution initiale par glouton al�atoire :----------------------------------------------------------------------------
// Pareil que StupidSolver::solve() mais retourne une sol et non pas un bool :
Solution* Steepest2optSolver::Stupid_solver(Solution* sol) {
    if (log3()) {
        cout << "\n---StupidSolver::solve START instance: "
             << inst->name << " de taille "
             << inst->stations->size() << "\n";
    }
    sol->clear();
    // Principe : On prend les stations dans l'ordre de l'instance, puis
    // on les affecte a chaque remorque � tour de r�le.

    int remorque_id = -1;
    for (unsigned j = 0; j < inst->stations->size(); j++) {
        // on extrait la prochaine station � visiter
        Station* station = inst->stations->at(j);
        // s�lection des remorques � tour de r�le
        remorque_id = (remorque_id + 1) % inst->remorques->size();
        // on extrait le circuit associ� � la remorque s�lectionn�e
        Circuit* circuit = sol->circuits->at(remorque_id);
        // on ajoute la station en fin de ce circuit
        logn5("StupidSolver::solve: ajout de la station " + station->name +
              " � la remorque " + circuit->remorque->name);
        circuit->stations->push_back(station);
        // update inutile ici car n'a pas (encore) besoin de mesurer la
        // stupidit� de cette insertion !
    }

    logn4("StupidSolver::solve: avant appel � sol->update");
    sol->update();
    logn3("StupidSolver::solve: END");
    return sol;
};



// 2)M�thode stupid de ce solver :----------------------------------------------------------------------------
// "Effectue une seule descente � partir de la solution (deterministe) issue du glouton StupidSolver"
bool Steepest2optSolver::solve_stupid() {

    this->cursol = new Solution(inst);
    this->cursol = Stupid_solver(this->cursol);
    logn1("Stupid_solveur a pour valeur "+this->cursol->get_cost_string());
    mutate(this->cursol); 
    logn1(" Apr�s descente, valeur "+this->cursol->get_cost_string()+"\n");
    this->bestsol->copy(this->cursol);
    this->found = true;
    return found;
};




// 3)M�thode principale de ce solver :----------------------------------------------------------------------------
// S'encha�ne un nombre param�trable de fois : on utilise la classe Option !
bool Steepest2optSolver::solve() {

// Modifier la classe options pour choisir it�ration


}


// 4) Effectue une mutation sur la solution en param�tre :----------------------------------------------------------------------------

void Steepest2optSolver::mutate(Solution* sol) {
    logn4("Steepest2optSolver::mutate BEGIN");

    Circuit* circuit;
	
    for(int i=0;i<sol->circuits->size();i++){
        circuit=sol->circuits->at(i);
        cout << "Circuit avant la descente" << endl;
        cout << circuit->to_s() << endl;
        circuit = circuit->mutate_2opt_steepest(10000000);
        cout << "Circuit apres la descente" << endl;
        cout << circuit->to_s() << endl;       
    }

    sol->update();
    logn4("Steepest2optSolver::mutate END");
};
