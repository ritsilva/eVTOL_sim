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

class Charger {
    private:
        Charger_states currentState;
        Aircraft* dockedAircraft;
        vector<Aircraft>* queuedAircraft;
    
    public:
        Charger();
        void processTime(int step);
        void dockAircraft(Aircraft* aircraft);
        void undockAircraft();

        Charger_states getChargerState() {return currentState;}
        int getQueueSize() {return queuedAircraft->size();}
        string getChargingAircraftName() {return dockedAircraft->getName();}
};

#endif // CHARGER_H