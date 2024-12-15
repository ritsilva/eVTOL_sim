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

string chargerStateToString(Charger_states state);

class Charger {
    private:
        Charger_states currentState;
        // Aircraft* dockedAircraft;
        vector<Aircraft*> dockedAircraft;
    
    public:
        Charger();
        void processTime(int step);
        void dockAircraft(Aircraft* aircraft);
        void undockAircraft();

        Charger_states getChargerState() {return currentState;}
        int getQueueSize() {return dockedAircraft.size();}
        string getChargingAircraftName() {return dockedAircraft[0]->getName();}
};

#endif // CHARGER_H