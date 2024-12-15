#include <iostream>
#include <stdio.h>
#include "../src/aircraft.h"
#include "../src/charger.h"
#include "../src/logging.h"
using namespace std;

void test_simloop() {
    Logger logger("./test_simloop.log");
    logger.log(INFO, "running test_simloop()");

    logger.log(INFO, "create 3 aircraft");
    int numAircraft = 3;
    string name_1 = "aircraft_1";
    string name_2 = "aircraft_2";
    string name_3 = "aircraft_3";
    vector<string> names = {name_1, name_2, name_3};
    float cruiseSpeed = 100;     // mph
    float batteryCapacity = 100; // kWh
    float timeToCharge = 0.2;    // hours
    float energyUsage = 1.5;     // kWh
    int passengerCount = 5;      // people
    float faultProb = 0.10;      // probabily/hour
    
    vector<Aircraft> aircraftGroup;
    for(int i = 0; i < numAircraft; i++) {
        Aircraft newAircraft(names[i], cruiseSpeed, batteryCapacity, timeToCharge, energyUsage, passengerCount, faultProb);
        aircraftGroup.push_back(newAircraft);
    }

    logger.log(INFO, "create 2 chargers");
    int numChargers = 2;
    vector<Charger> chargerGroup;
    for(int i = 0; i < numChargers; i++) {
        Charger newCharger;
        chargerGroup.push_back(newCharger);
    }

    int totalTime = 2 * HOUR_TO_MS; // 1 hour
    int timeStep = 1; // 1 ms

    logger.log(INFO, "running simulation for " + to_string(totalTime) + " miliseconds");
    // start flying aircraft
    for(int i = 0; i < aircraftGroup.size(); i++) {
        aircraftGroup[i].beginFlying();
    }

    for(int i = 0; i < totalTime; i += timeStep) {
        vector<int> aircraftNeedingCharge;
        // process flying aircraft
        for(int j = 0; j < aircraftGroup.size(); j++) {
            if(aircraftGroup[j].getCurrentState() == FLYING) {
                aircraftGroup[j].processTime(timeStep);
            } else if(aircraftGroup[j].getCurrentState() == GROUNDED) {
                logger.log(DEBUG, aircraftGroup[j].getName() + " grounded for charging at " + to_string(i));
                aircraftNeedingCharge.push_back(j);
            }
        }

        // process charing aircraft
        for(int j = 0; j < chargerGroup.size(); j++) {
            chargerGroup[j].processTime(timeStep);
        }

        // check if there are any aircraft that need to be charged
        // does not make sense to check before processing 
        // the aircraft that are already charging
        for(const int &index : aircraftNeedingCharge) {
            int shortestQueue = 0;
            int lenOfShortestQueue = chargerGroup[0].getQueueSize();
            for(int j = 0; j < chargerGroup.size(); j++) {
                if(chargerGroup[j].getQueueSize() < lenOfShortestQueue) shortestQueue = j;
            }
            logger.log(DEBUG, "docking " + aircraftGroup[index].getName() + " into charger " + to_string(shortestQueue));
            chargerGroup[shortestQueue].dockAircraft(&aircraftGroup[index]);
        }
    }

}

int main() {

    test_simloop();

    return 0;
}