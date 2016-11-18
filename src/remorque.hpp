#ifndef REMORQUE_H
class Remorque;
#define REMORQUE_H

#include "common.hpp"
#include "site.hpp"

class Remorque : public Site {

public:
    // int id; // commencera en 0 pour la première remorque

    //Chargement de la remorque
    int load;
    //Capacité maximale de la remorque
    int capa;

    Remorque(int id, string name, int x, int y, int capa);
    virtual ~Remorque();
    virtual string classname();

    virtual string to_s();
    virtual string to_s_long();

    int getCapa();
    void setCapa(int val);
    int getLoad();
    void setLoad(int val);

};
#endif
