#include "./logging.h"
#include "./charger.h"
using namespace std;

Charger::Charger() {
    currentState = FREE;
    dockedAircraft = NULL;
}

void Charger::processTime(int step) {
    if(currentState == FREE) {
        return;
    } else {
        bool isDone = dockedAircraft->charge(step);
        if(isDone == true) {
            undockAircraft();
            if(queuedAircraft->size() > 0) {
                cout << "need to start charging the next aircraft" << endl;
                // dockAircraft(queuedAircraft[0]);
                // queuedAircraft->erase(0)
            }
        }
    }
}

void Charger::dockAircraft(Aircraft* aircraft) {
    currentState = OCCUPIED;
    dockedAircraft = aircraft;
    dockedAircraft->beginCharging();
}

void Charger::undockAircraft() {
    // if(queuedAircraft.size() > 0) {
    //     dockedAircraft = queuedAircraft[0];
    //     queuedAircraft.erase(queuedAircraft.begin());
    // } else {
    //     dockedAircraft = -1;
    //     currentState = FREE;
    // }
}