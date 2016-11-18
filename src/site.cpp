#include "site.hpp"

// initialisation des variables statiques a l'exterieur de toute methode
int Site::last_sid = 0;

string Site::classname() {
    return "Site";
}

// les "friend" en premier
ostream& operator<<(ostream& os, const Site  &site) {
    return os << site.name;
}


Site::Site(int id, string name, int x, int y) {
    this->id = id;
    this->name = name;
    this->x = x;
    this->y = y;
    this->sid = Site::last_sid++;
}
Site::~Site() {
}

string Site::to_s() {
    ostringstream stm;
    stm << name << ": (" << x << " " << y << ")" ;
    return stm.str();
}
string Site::to_s_long() {
    // pourra être plus verbeux à l'avenir
    ostringstream stm;
    stm << "site " << name << ": (" << x << "," << y << ")" ;
    return stm.str();
}
string Site::to_s(vector<Site*>* sites) {
    stringstream buf;
    string sep = "";
    buf << "[";
    for (unsigned i=0; i < sites->size(); i++) {
        buf << sep << sites->at(i)->to_s();
        sep = ",";
    }
    buf << "]";
    return buf.str();
}

//./
