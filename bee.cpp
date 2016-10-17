#include <cstdlib>

#include "bee.h"
#include "etc.h"

using namespace BSO;

/******************\
|* PUBLIC METHODS *|
\******************/

Bee::Bee() {}

Bee::Bee(ofstream *log, Swarm *s)
{
    this->log = log;
    swarm = s;
    x = new double [this->swarm->getN()];
    p = new double [this->swarm->getN()];
    v = new double [this->swarm->getN()];
    isMemoryAlloc = true;
    init();
}

Bee::~Bee()
{
    if (isMemoryAlloc)
    {
        delete [] x;
        delete [] p;
        delete [] v;
    }
}

int Bee::update()
{
    int status { 1 };   /* 0 - outside, 1 - inside boundaries, 2 - found new
                           bee's best position, 3 - found new swarm's best
                           position */
    status = updateX(); // update position
    updateV();          // update velocity
    if (swarm->getEnableBeeLog()) logInfo();          // write to log
    if (status == 1)    // If bee is inside boundaries, then check for best pos
    {
        /* We make MINIMIZATION so we check wheather function value in current
           position is better then in previous */
        if (swarm->f(x) < swarm->f(p)) // If so, then update bee's best position
        {
            status = 2;
            if (swarm->getEnableBeeLog()) *log << "  Good news: bee found new \
bee's best position, updating..." << endl;
            for (int i = 0; i < swarm->getN(); ++i)
                p[i] = x[i];
        }
        if (swarm->f(x) < swarm->f(swarm->getG())) /* If so, then update
                                                      swarm's best position */
        {
            status = 3;
            if (swarm->getEnableBeeLog()) *log << "  Good news: bee found new \
swarm's best position, updating..." << endl;
            for (int i = 0; i < swarm->getN(); ++i)
                swarm->setG(i, x[i]);
        }
    }
    else
        if (swarm->getEnableBeeLog()) *log << "  Warning: bee is outside \
boundaries, skipping her results..." << endl;
    return status;
}

/*******************\
|* PRIVATE METHODS *|
\*******************/

void Bee::init()
{
    for (int i = 0; i < swarm->getN(); ++i)
    {
        x[i] = dRand(swarm->getMin(i), swarm->getMax(i));
        p[i] = x[i];
        v[i] = dRand(swarm->getMin(i) - swarm->getMax(i),
                     swarm->getMax(i) - swarm->getMin(i));
    }
    if (swarm->getEnableBeeLog()) logInfo();
    if (swarm->f(x) < swarm->f(swarm->getG())) /* If so, then update swarm's
                                                  best position */
    {
            for (int i = 0; i < swarm->getN(); ++i)
                swarm->setG(i, x[i]);
    }
}

double Bee::dRand(double min, double max)
{
    double m = (double)rand() / RAND_MAX;
    return min + m * (max - min);
}

void Bee::updateV()
{
    for(int i = 0; i < swarm->getN(); ++i)
    {
        v[i] = swarm->getVCoef() * v[i]
             + swarm->getPCoef() * dRand(-1.0, 1.0) * (p[i] - x[i])
             + swarm->getGCoef() * dRand(-1.0, 1.0) * (swarm->getG(i) - x[i]);
    }
}

int Bee::updateX()
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

void Bee::logInfo()
{
    *log << " Bee #" << this << ":" << endl << "  ";
    log->width(BSO::W);
    *log << "dimention:";
    for (int i = 0; i < swarm->getN(); ++i)
    {
        log->width(BSO::W);
        *log << i + 1;
    }
    *log << endl << "  ";
    log->width(BSO::W);
    *log << "x:";
    for (int i = 0; i < swarm->getN(); ++i)
    {
        log->width(BSO::W);
        *log << x[i];
    }
    *log << endl << "  ";
    log->width(BSO::W);
    *log << "p:";
    for (int i = 0; i < swarm->getN(); ++i)
    {
        log->width(BSO::W);
        *log << p[i];
    }
    *log << endl << "  ";
    log->width(BSO::W);
    *log << "v:";
    for (int i = 0; i < swarm->getN(); ++i)
    {
        log->width(BSO::W);
        *log << v[i];
    }
    *log << endl;
}
