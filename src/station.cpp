#include "station.hpp"

string Station::classname() {
    return "Station";
}

Station::Station(int id, string name, int x, int y,
                  int capa, int ideal, int nbvp)
        : Site::Site(id, name, x, y) {

    // this->id = id;
    this->capa = capa;
    this->ideal = ideal;
    this->nbvp = nbvp;
}
Station::~Station() {
}

string Station::to_s() {
    ostringstream buf;
    buf << "station " << setw(5) << name
        << setw(5) << x << setw(4) << y
        << setw(8) << capa << setw(6) << ideal
        << setw(5) << nbvp ;
    return buf.str();
}

string Station::to_s_long() {
    ostringstream buf;
    buf << Site::to_s_long() << " id=" << id
                             << " capa=" << capa
                             << " ideal=" << ideal
                             << " nbvp="   << nbvp
                             ;
    return buf.str();
}
int Station::margin() {
    return this->capa - this->nbvp;
}
int Station::deficit() {
    return this->ideal - this->nbvp;
}

void Station::addBikes(int val) {
    this->nbvp += val;
}

void Station::removeBikes(int val) {
    this->nbvp -= val;
}
//./
