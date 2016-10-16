#include <cstdlib>

#include "bee.h"

/******************\
|* PUBLIC METHODS *|
\******************/

Bee::Bee() {}

Bee::Bee(Swarm *s)
{
    this->swarm = s;
    p = new double [this->swarm->getN()];
    v = new double [this->swarm->getN()];
}

Bee::~Bee()
{
    delete [] p;
    delete [] v;
}

double Bee::getV(unsigned int i)
{
    return v[i];
}

/*******************\
|* PRIVATE METHODS *|
\*******************/
