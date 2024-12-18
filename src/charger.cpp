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
    if(dockedAircraftQueue.size() != 0) {
        return dockedAircraftQueue[0]->getName();
    } else {
        return "NULL";
    }
}

void Charger::processTime(int step) {
    // if no aircraft charing do nothing
    if(currentState == FREE) {
        return;
    } else {
        // charge aircraft 
        bool isDone = dockedAircraftQueue[0]->charge(step);
        
        // done charging current aircraft
        if(isDone == true) {
            undockAircraft();
        }

        // record how long each of the other aircrafts
        // are waiting in line for
        for(int i = 1; i < dockedAircraftQueue.size(); i++) {
            dockedAircraftQueue[i]->processTime(step);
        }
    }
}

void Charger::dockAircraft(Aircraft* aircraft) {
    // charger is free, begin charging
    if(currentState == FREE) {
        currentState = OCCUPIED;
        dockedAircraftQueue.emplace_back(aircraft);
        dockedAircraftQueue[0]->beginCharging();
    } else {
        // need to wait in line
        aircraft->dockIntoCharger();
        dockedAircraftQueue.emplace_back(aircraft);
    }
}

void Charger::undockAircraft() {
    // remove aircraft that is done charging
    dockedAircraftQueue.erase(dockedAircraftQueue.begin());
    
    // beging charging the next aircraft if there is any
    if(!dockedAircraftQueue.empty()) {
        dockedAircraftQueue[0]->beginCharging();
    } else {
        currentState = FREE;
    }    
}

int Charger::getQueueWaitTime() {
    float waitTime = dockedAircraftQueue[0]->getChargeTime() - dockedAircraftQueue[0]->getCurrentChargingTime();
    for(int i = 1; i < dockedAircraftQueue.size(); i++) {
        waitTime += dockedAircraftQueue[i]->getChargeTime();
    }
    return waitTime;
}