#include "./logging.h"
#include "./charger.h"
using namespace std;

string chargerStateToString(Charger_states state) {
    switch(state) {
        case FREE: return "FREE";
        case OCCUPIED: return "OCCUPIED";
        case MAX_CHARGER_STATE: return "MAX_CHARGER_STATE";
        default: return "UNKNOWN";
    }
}

Charger::Charger() {
    currentState = FREE;
}

string Charger::getChargingAircraftName() {
    if(dockedAircraft.size() != 0) {
        return dockedAircraft[0]->getName();
    } else {
        return "NULL";
    }
}

void Charger::processTime(int step) {
    if(currentState == FREE) {
        return;
    } else {
        bool isDone = dockedAircraft[0]->charge(step);
        if(isDone == true) {
            undockAircraft();
        }

        for(int i = 1; i < dockedAircraft.size(); i++) {
            dockedAircraft[i]->processTime(step);
        }
    }
}

void Charger::dockAircraft(Aircraft* aircraft) {
    // charger is free, begin charging
    if(currentState == FREE) {
        currentState = OCCUPIED;
        dockedAircraft.emplace_back(aircraft);
        dockedAircraft[0]->beginCharging();
    } else {
        // need to wait in line
        aircraft->dockIntoCharger();
        dockedAircraft.emplace_back(aircraft);
    }
}

void Charger::undockAircraft() {
    // remove aircraft that is done charging
    dockedAircraft.erase(dockedAircraft.begin());
    if(!dockedAircraft.empty()) {
        dockedAircraft[0]->beginCharging();
    } else {
        currentState = FREE;
    }    
}