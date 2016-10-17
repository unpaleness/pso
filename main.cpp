#include <ctime>
#include <fstream>

#include "swarm.h"
#include "etc.h"

using namespace std;
using namespace BSO;

int main()
{
    srand(time(0)); /* Enabling random number generator update: Needed for
                       initial positions, velocities of bees and then to
                       randomize their behavior */
    ofstream log("bso.log"); // Create logging filestream
    log.setf(ios::left, ios::adjustfield);
    log.precision(4);
    log.setf(ios::fixed, ios::floatfield);
    Swarm swarm(&log);       // Initiate swarm
    swarm.start();           // Start it
}
