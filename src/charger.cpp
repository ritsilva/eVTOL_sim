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

void Charger::processTime(int step) {
    if(currentState == FREE) {
        return;
    } else {
        bool isDone = dockedAircraft[0]->charge(step);
        if(isDone == true) {
            undockAircraft();
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
        dockedAircraft.emplace_back(aircraft);
    }
}

void Charger::undockAircraft() {
    if(!dockedAircraft.empty()) {
        dockedAircraft.erase(dockedAircraft.begin());
        dockedAircraft[0]->beginCharging();
    } else {
        currentState = FREE;
        dockedAircraft.erase(dockedAircraft.begin());
    }

    currentState = FREE;
    
}