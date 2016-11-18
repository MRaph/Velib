#include "remorque.hpp"

string Remorque::classname() {
    return "Remorque";
}

// // les "friend" en premier (si nécessaire)
// ostream& operator<<(ostream& os, const Remorque  &remorque) {
//     return os << remorque.name;
// }

Remorque::Remorque(int id, string name, int x, int y, int capa)
        : Site::Site(id, name, x, y) {

    // this->id = id;
    this->capa = capa;
}
Remorque::~Remorque() {
}

string Remorque::to_s() {
    // TODO : passer par super
    stringstream buf;
    buf << "remorque " << setw(4) << name
        << setw(5) << x << setw(4) << y
        << setw(5) << capa ;
    return buf.str();
}
string Remorque::to_s_long() {
    stringstream buf;
    buf << Site::to_s_long() << " id=" << id << " capa=" << capa;
    return buf.str();
}

int Remorque::getCapa() {
  return this->capa;
}

void Remorque::setCapa(int val){
  this->capa = val;
}

int Remorque::getLoad() {
  return this->capa;
}

void Remorque::setLoad(int val){
  this->load = val;
}
//./
