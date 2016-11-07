#include <cstdlib>

#include "particle.h"
#include "etc.h"

using namespace PSO;

/******************\
|* PUBLIC METHODS *|
\******************/

Particle::Particle() {}

Particle::Particle(ofstream *log, Swarm *s)
{
    this->log = log;
    swarm = s;
    x = new double [this->swarm->getN()];
    p = new double [this->swarm->getN()];
    v = new double [this->swarm->getN()];
    isMemoryAlloc = true;
    init();
}

Particle::~Particle()
{
    if (isMemoryAlloc)
    {
        delete [] x;
        delete [] p;
        delete [] v;
    }
}

int Particle::update()
{
    int status { 1 };   /* 0 - outside, 1 - inside boundaries, 2 - found new
                           particle's best position, 3 - found new swarm's best
                           position */
    status = updateX(); // update position
    updateV();          // update velocity
    if (swarm->getEnableParticleLog()) logInfo();          // write to log
    if (status == 1)    // If particle is inside boundaries, then check for best pos
    {
        /* We make MINIMIZATION so we check wheather function value in current
           position is better then in previous */
        if (swarm->f(x) < swarm->f(p)) // If so, then update particle's best position
        {
            status = 2;
            if (swarm->getEnableParticleLog()) *log << "  Good news: particle found new \
particle's best position, updating..." << endl;
            for (int i = 0; i < swarm->getN(); ++i)
                p[i] = x[i];
        }
        if (swarm->f(x) < swarm->f(swarm->getG())) /* If so, then update
                                                      swarm's best position */
        {
            status = 3;
            if (swarm->getEnableParticleLog()) *log << "  Good news: particle found new \
swarm's best position, updating..." << endl;
            for (int i = 0; i < swarm->getN(); ++i)
                swarm->setG(i, x[i]);
        }
    }
    else
        if (swarm->getEnableParticleLog()) *log << "  Warning: particle is outside \
boundaries, skipping her results..." << endl;
    return status;
}

/*******************\
|* PRIVATE METHODS *|
\*******************/

void Particle::init()
{
    for (int i = 0; i < swarm->getN(); ++i)
    {
        x[i] = dRand(swarm->getMin(i), swarm->getMax(i));
        p[i] = x[i];
        v[i] = dRand(swarm->getMin(i) - swarm->getMax(i),
                     swarm->getMax(i) - swarm->getMin(i));
    }
    if (swarm->getEnableParticleLog()) logInfo();
    if (swarm->f(x) < swarm->f(swarm->getG())) /* If so, then update swarm's
                                                  best position */
    {
            for (int i = 0; i < swarm->getN(); ++i)
                swarm->setG(i, x[i]);
    }
}

double Particle::dRand(double min, double max)
{
    double m = (double)rand() / RAND_MAX;
    return min + m * (max - min);
}

void Particle::updateV()
{
    for(int i = 0; i < swarm->getN(); ++i)
    {
        v[i] = swarm->getVCoef() * v[i]
             + swarm->getPCoef() * dRand(-1.0, 1.0) * (p[i] - x[i])
             + swarm->getGCoef() * dRand(-1.0, 1.0) * (swarm->getG(i) - x[i]);
    }
}

int Particle::updateX()
{
    int returnValue { 1 };
    for (int i = 0; i < swarm->getN(); ++i)
    {
        x[i] += v[i];
        if (x[i] > swarm->getMax(i) || x[i] < swarm->getMin(i))
            returnValue = 0;
    }
    return returnValue;
}

void Particle::logInfo()
{
    *log << " Particle #" << this << ":" << endl << "  ";
    log->width(PSO::W);
    *log << "dimention:";
    for (int i = 0; i < swarm->getN(); ++i)
    {
        log->width(PSO::W);
        *log << i + 1;
    }
    *log << endl << "  ";
    log->width(PSO::W);
    *log << "x:";
    for (int i = 0; i < swarm->getN(); ++i)
    {
        log->width(PSO::W);
        *log << x[i];
    }
    *log << endl << "  ";
    log->width(PSO::W);
    *log << "p:";
    for (int i = 0; i < swarm->getN(); ++i)
    {
        log->width(PSO::W);
        *log << p[i];
    }
    *log << endl << "  ";
    log->width(PSO::W);
    *log << "v:";
    for (int i = 0; i < swarm->getN(); ++i)
    {
        log->width(PSO::W);
        *log << v[i];
    }
    *log << endl;
}
