#include <iostream>
#include <vector>
#include <random>
#include "./src/aircraft.h"
using namespace std;

int main() {
    // simulation parameters
    int numAircraft = 1;
    int numChargers = 3;
    int totalTime = 3*60*60*1000; // 3 hours in miliseconds
    // implement something to allow controlled sim speeds and update rate

    // randomize how many of each aircraft are flying.
    vector<Aircraft> flyingAircrafts(numAircraft);
    vector<Aircraft> chargingAircraft(numChargers);
    vector<Aircraft> waitingToChargeAircraft(numAircraft);
    for (int i =0; i < numAircraft; i++) {

    }

    // simulation loop
    for (int time = 0; time < totalTime; time++) {
        // TODO: run class functions to update stats
        for (int i = 0; i < numAircraft; i++) {
            // TODO
            // if an aircraft runs out of power it must land at a charging station
            // if there are no open charging stations then it waits until 1 is avalible
            continue;
        }

        // TODO handle states of charging stations
        for (int i = 0; i < numChargers; i++) {
            // TODO
        }
    }

    return 0;
}