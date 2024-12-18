#ifndef CHARGER_H
#define CHARGER_H

#include <vector>
#include "./logging.h"
#include "aircraft.h"
using namespace std;

enum Charger_states {
    FREE,
    OCCUPIED,
    MAX_CHARGER_STATE
};

// function used to express Charger_states in testing
string chargerStateToString(Charger_states state);

class Charger {
    private:
        Charger_states currentState;
        vector<Aircraft*> dockedAircraftQueue;

    public:
        // constructor
        Charger();

        // procced 1 'step' of time (miliseconds)
        void processTime(int step);

        // add aircraft into queue of vehiciles waiting to charge
        // if the charge is FREE begin charging
        void dockAircraft(Aircraft* aircraft);

        // release currently done charging aircraft
        // and begin charging the next, if there is any
        // otherwise set status to FREE
        void undockAircraft();

        // accessor functions
        Charger_states getChargerState() {return currentState;}
        int getQueueSize() {return dockedAircraftQueue.size();}
        int getQueueWaitTime();
        string getChargingAircraftName();
};

#endif // CHARGER_H