#ifndef _BEE_H_
#define _BEE_H_

#include "swarm.h"

class Bee
{
public:
    Bee();
    Bee(Swarm *s);
    ~Bee();

    double getV(undigned int i); // Getter for velocity

private:
    Swarm *swarm; // Each bee should store pointer to its own swarm
    double *x;    // Bee's current position
    double *p;    // Vector with bee's best known position
    double *v;    // Vector with bee's velocity vector

    void init();  // Initiate bee, its position and velocity
};

#endif // _BEE_H_
