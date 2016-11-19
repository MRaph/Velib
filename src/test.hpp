void test_initial_load() {
    Instance* inst = Instance::new_velib_mini();
    vector<Remorque*>* remorques = inst->getRemorques();
    Remorque* rem = remorques->at(0);
    Circuit circuit = Circuit(inst, rem);

    int k = 0;
    printf("Optimal initial load : %i", k);
}

void test_circuit_reverse_branch() {
    logn1("Test reverse_branch sur new_velib_mini_mono_remorque\n");
    Instance* inst = Instance::new_velib_mini_mono_remorque();
    // cout << inst->to_s();

    // Création d'une solution initiale
    Solution* sol = new Solution(inst);
    sol->solve_stupid();


    list<Station*>* stations = sol->circuits->at(0)->stations;

    // cout << "Solution initiale : \n"<< sol->to_s();
    cout << "Solution initiale : \n  "
         << inst->to_s(stations) << endl
         << "  => coût : " << sol->get_cost_string() << endl;


    sol->circuits->at(0)->reverse_branch(3,7);
    sol->update();
    cout << "Solution après revers_branch(3,7) : \n  "
         << inst->to_s(stations) << endl
         << "  => coût : " << sol->get_cost_string() << endl;

    sol->circuits->at(0)->reverse_branch(3,7);
    sol->update();
    cout << "Solution après nouveau revers_branch(3,7) : \n  "
         << inst->to_s(stations) << endl
         << "  => coût : " << sol->get_cost_string() << endl;

    sol->circuits->at(0)->reverse_branch(0,8);
    sol->update();
    cout << "Solution après nouveau revers_branch(0,8) : \n  "
         << inst->to_s(stations) << endl
         << "  => coût : " << sol->get_cost_string() << endl;

    cout << "Test reverse_branch : FIN \n\n";
}

void test_circuit_move_branch_to() {
    logn1("Test move_branch_to sur mini_gen\n");
    Instance* inst = Instance::new_velib_mini();
    // cout << inst->to_s();

    // Création d'une solution initiale
    Solution* sol_init = new Solution(inst);
    sol_init->solve_stupid();

    Solution* sol = new Solution(sol_init);
    // cout << sol->to_s();

    Circuit* c1 = sol->circuits->at(0);
    Circuit* c2 = sol->circuits->at(1);

    // cout << "Solution initiale : \n"<< sol->to_s();
    cout << "Solution initiale : \n"
         << "  " << inst->to_s(c1->stations) << endl
         << "  " << inst->to_s(c2->stations) << endl
         << "  => coût : " << sol->get_cost_string() << endl;


    // c1->move_branch_to(0,2,c2,0);
    // sol->update();
    // cout << "Solution après c1->move_branch_to(0,2,c2,0) : \n"
    //      << "  " << inst->to_s(c1->stations) << endl
    //      << "  " << inst->to_s(c2->stations) << endl
    //      << "  => coût : " << sol->get_cost_string() << endl;
    // c2->move_branch_to(0,2,c1,0);
    // sol->update();
    // cout << "Restauration par c2->move_branch_to(0,2,c1,0) : \n"
    //      << "  " << inst->to_s(c1->stations) << endl
    //      << "  " << inst->to_s(c2->stations) << endl
    //      << "  => coût : " << sol->get_cost_string() << endl;

    // c1->move_branch_to(0,2,c2,4);
    // sol->update();
    // cout << "Solution après c1->move_branch_to(0,2,c2,4) : \n"
    //      << "  " << inst->to_s(c1->stations) << endl
    //      << "  " << inst->to_s(c2->stations) << endl
    //      << "  => coût : " << sol->get_cost_string() << endl;
    // c2->move_branch_to(4,6,c1,0);
    // sol->update();
    // cout << "Restauration par c2->move_branch_to(4,6,c1,0) : \n"
    //      << "  " << inst->to_s(c1->stations) << endl
    //      << "  " << inst->to_s(c2->stations) << endl
    //      << "  => coût : " << sol->get_cost_string() << endl;

    // c1->move_branch_to(1,2,c2,3);
    // sol->update();
    // cout << "Solution après c1->move_branch_to(1,2,c2,3) : \n"
    //      << "  " << inst->to_s(c1->stations) << endl
    //      << "  " << inst->to_s(c2->stations) << endl
    //      << "  => coût : " << sol->get_cost_string() << endl;
    // c2->move_branch_to(3,4,c1,1);
    // sol->update();
    // cout << "Restauration par c2->move_branch_to(3,4,c1,1) : \n"
    //      << "  " << inst->to_s(c1->stations) << endl
    //      << "  " << inst->to_s(c2->stations) << endl
    //      << "  => coût : " << sol->get_cost_string() << endl;

    // RESTAURATION
    cout << endl;
    sol->copy(sol_init);
    cout << "Réinit : \n"
         << "  " << inst->to_s(c1->stations) << endl
         << "  " << inst->to_s(c2->stations) << endl
         << "  => coût : " << sol->get_cost_string() << endl;

    cout << endl;

    // DEPLACEMENETS VERS LE CIRCUIT DE DESTINATION AVEC INVERSION
    // c1->move_branch_to(0,2,c2,0,true);
    // sol->update();
    // cout << "Solution après c1->move_branch_to(0,2,c2,0,true) : \n"
    //      << "  " << inst->to_s(c1->stations) << endl
    //      << "  " << inst->to_s(c2->stations) << endl
    //      << "  => coût : " << sol->get_cost_string() << endl;

    // sol->copy(sol_init);
    // c1->move_branch_to(1,3,c2,3,true);
    // sol->update();
    // cout << "Solution après c1->move_branch_to(1,3,c2,3,true) : \n"
    //      << "  " << inst->to_s(c1->stations) << endl
    //      << "  " << inst->to_s(c2->stations) << endl
    //      << "  => coût : " << sol->get_cost_string() << endl;

    // sol->copy(sol_init);
    // c1->move_branch_to(0,2,c2,4,true);
    // sol->update();
    // cout << "Solution après c1->move_branch_to(0,2,c2,4,true) : \n"
    //      << "  " << inst->to_s(c1->stations) << endl
    //      << "  " << inst->to_s(c2->stations) << endl
    //      << "  => coût : " << sol->get_cost_string() << endl;


    // sol->copy(sol_init);
    // c1->move_branch_to(1,2,c2,3,true);
    // sol->update();
    // cout << "Solution après c1->move_branch_to(1,2,c2,3,true) : \n"
    //      << "  " << inst->to_s(c1->stations) << endl
    //      << "  " << inst->to_s(c2->stations) << endl
    //      << "  => coût : " << sol->get_cost_string() << endl;

    // sol->copy(sol_init);
    // c1->move_branch_to(0,4,c2,0,true);
    // sol->update();
    // cout << "Solution après c1->move_branch_to(0,4,c2,0,true) : \n"
    //      << "  " << inst->to_s(c1->stations) << endl
    //      << "  " << inst->to_s(c2->stations) << endl
    //      << "  => coût : " << sol->get_cost_string() << endl;
    // c2->move_branch_to(4,8,c1,0,true);
    // sol->update();
    // cout << "Solution après c2->move_branch_to(4,8,c1,0,true) (SANS REINIT) : \n"
    //      << "  " << inst->to_s(c1->stations) << endl
    //      << "  " << inst->to_s(c2->stations) << endl
    //      << "  => coût : " << sol->get_cost_string() << endl;

    // sol->copy(sol_init);
    // cout << "Réinit : \n"
    //      << "  " << inst->to_s(c1->stations) << endl
    //      << "  " << inst->to_s(c2->stations) << endl
    //      << "  => coût : " << sol->get_cost_string() << endl;

    cout << "Test test_circuit_move_branch_to : FIN \n\n";

}

void test_circuit_move() {
    logn1("Test test_circuit_move\n");
    Instance* inst = Instance::new_velib_mini_mono_remorque();

    // Création d'une solution initiale
    Solution* sol_init = new Solution(inst);
    sol_init->solve_stupid();
    cout << "Solution initiale : \n"
         << sol_init->to_s_lite() << endl;

    // Solution* sol = new Solution(sol_init);
    // cout << sol->to_s_lite();

    // Circuit* c1 = sol->circuits->at(0);
    // Circuit* c2 = sol->circuits->at(1);

    // c1->move(2,4);
    // sol->update();
    // cout << "Solution après move(2,4) :\n"
    //      << sol->to_s_lite() << endl;

    // c1->move(4,2);
    // sol->update();
    // cout << "Solution après move(4,2) :\n"
    //      << sol->to_s_lite() << endl;


    // c1->move(1,1);
    // sol->update();
    // cout << "Solution après move(1,1) :\n"
    //      << sol->to_s_lite() << endl;


    // c1->move(0,c1->stations->size()-1);
    // sol->update();
    // cout << "Solution après move(0,c1->stations->size()-1 :\n"
    //      << sol->to_s_lite() << endl;


    cout << "Test test_circuit_move : FIN \n\n";
}


void test_circuit_move_to() {
    logn1("Test test_circuit_move_to\n");
    Instance* inst = Instance::new_velib_mini();

    // Création d'une solution initiale
    Solution* sol_init = new Solution(inst);
    sol_init->solve_stupid();
    cout << "Solution initiale : \n"
         << sol_init->to_s_lite() << endl;

    // Solution* sol = new Solution(sol_init);
    // cout << sol->to_s_lite();

    // Circuit* c1 = sol->circuits->at(0);
    // Circuit* c2 = sol->circuits->at(1);

    // c1->move_to(2,c2,3);
    // sol->update();
    // cout << "Solution après c1->move_to(2,c2,3) :\n"
    //      << sol->to_s_lite() << endl;

    // c2->move_to(3,c1,2);
    // sol->update();
    // cout << "Solution après c2->move_to(3,c1,2) :\n"
    //      << sol->to_s_lite() << endl;

    // // déplacement de la première station de c1 en fin de c2 puis remise en état
    // c1->move_to(0, c2, c2->stations->size());
    // sol->update();
    // cout << "Solution après c1->move_to(0, c2, c2->stations->size()) :\n"
    //      << sol->to_s_lite() << endl;

    // c2->move_to(c2->stations->size()-1, c1, 0);
    // sol->update();
    // cout << "Solution après c2->move_to(c2->stations->size()-1, c1, 0) :\n"
    //      << sol->to_s_lite() << endl;


    cout << "Test test_circuit_move_to : FIN \n\n";
}


void test_util_rand() {
    logn1("Test test_util_rand\n");

    cout << "test 40 appels randint [0,1]\n";
    for (unsigned i = 0; i < 40; i++) {
        cout << U::randint(0,1) << " ";
    }
    cout << endl;

    cout << "test 40 appels randint [0,9]\n  ";
    for (unsigned i = 0; i < 40; i++) {
        cout << U::randint(0,9) << " ";
    }
    cout << endl;

    cout << "test 15 appels randdouble [0,2)\n  ";
    for (unsigned i = 0; i < 15; i++) {
        cout << U::randdouble(0,2) << " ";
    }
    cout << endl;

    cout << "test 15 appels randdouble [0,10)\n  ";
    for (unsigned i = 0; i < 15; i++) {
        cout << U::randdouble(0,10) << " ";
    }
    cout << endl;

    cout << "Test proto : FIN \n\n";
}

void test_proto() {
    logn1("Test test_util_rand\n");
    Instance* inst = Instance::new_velib_mini_mono_remorque();
    // cout << inst->to_s();

    // Création d'une solution initiale
    Solution* sol = new Solution(inst);
    sol->solve_stupid();


    list<Station*>* stations = sol->circuits->at(0)->stations;

    // cout << "Solution initiale : \n"<< sol->to_s();
    cout << "Solution initiale : \n  "
         << inst->to_s(stations) << endl
         << "  => coût : " << sol->get_cost_string() << endl;

    cout << "Test proto : FIN \n\n";
}
