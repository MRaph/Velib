#ifndef STATION_H
#define STATION_H

#include "common.hpp"
#include "site.hpp"

class Station : public Site {

public:

    // int id; // commencera en 0 pour la première station

    int capa; // capacité maxi de la station
    int ideal; // nombre idéla de vélo dans la station
    int nbvp;   // nombre de vélo présent

    Station(int id, string name, int x, int y, int capa, int ideal, int nbvp);
    virtual ~Station();
    virtual string classname();

    virtual int margin();
    virtual int deficit();

    virtual string to_s();
    virtual string to_s_long();

    int getNbvp() {
        return this->nbvp;
    }

};
#endif
