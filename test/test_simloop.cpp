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

    int totalTime = 2 * HOUR_TO_MS; // 2 hours
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

void test_simGatherData() {
    Logger logger("./test_simloop.log");
    logger.log(INFO, "running test_simloop()");

    logger.log(INFO, "create 3 aircraft");
    int numAircraft = 3;
    string name_1 = "Bravo Company";
    string name_2 = "Bravo Company";
    string name_3 = "Alpha Company";
    vector<string> names = {name_1, name_2, name_3};
    vector<float> cruiseSpeed = {100, 100, 120};     // mph
    vector<float> batteryCapacity = {100, 100, 320}; // kWh
    vector<float> timeToCharge = {0.2, 0.2, 0.6};    // hours
    vector<float> energyUsage = {1.5, 1.5, 1.6};     // kWh
    vector<int> passengerCount = {5, 5, 4};          // people
    vector<float> faultProb = {0.10, 0.10, 0.25};    // probabily/hour
    
    vector<Aircraft> aircraftGroup;
    for(int i = 0; i < numAircraft; i++) {
        Aircraft newAircraft(names[i], cruiseSpeed[i], batteryCapacity[i], timeToCharge[i], energyUsage[i], passengerCount[i], faultProb[i]);
        aircraftGroup.push_back(newAircraft);
    }

    logger.log(INFO, "create 2 chargers");
    int numChargers = 2;
    vector<Charger> chargerGroup;
    for(int i = 0; i < numChargers; i++) {
        Charger newCharger;
        chargerGroup.push_back(newCharger);
    }

    int totalTime = 3 * HOUR_TO_MS; // 1 hour
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

        // gather stats for each aircraft type per cycle?        
    }

    // gather each aircraft stat after they are done simulating
    // aircraft[0:1] belong to bravo
    // aircraft[2] belongs to alpha
    float bravoAvgFlightTime = (aircraftGroup[0].getAvgFlightTime() + aircraftGroup[1].getAvgFlightTime())/2;
    float bravoAvgDistanceTraveled = (aircraftGroup[0].getAvgDistanceTraveled() + aircraftGroup[1].getAvgDistanceTraveled())/2;
    float braveAvgChargingTime = (aircraftGroup[0].getAvgChargingTimes() + aircraftGroup[1].getAvgChargingTimes())/2;
    int bravoTotalFaults = aircraftGroup[0].getNumFaults() + aircraftGroup[1].getNumFaults();
    float bravoPassengerMiles = aircraftGroup[0].getPassengerMiles() + aircraftGroup[1].getPassengerMiles();
    logger.log(DEBUG, "bravo avg flight time: " + to_string(bravoAvgFlightTime));
    logger.log(DEBUG, "bravo avg distance traveled: " + to_string(bravoAvgDistanceTraveled));
    logger.log(DEBUG, "bravo avg charging time: " + to_string(braveAvgChargingTime));
    logger.log(DEBUG, "bravo total faults: " + to_string(bravoTotalFaults));
    logger.log(DEBUG, "bravo passenger miles: " + to_string(bravoPassengerMiles));

    float alphaAvgFlightTime = aircraftGroup[2].getAvgFlightTime();
    float alphaAvgDistanceTraveled = aircraftGroup[2].getAvgDistanceTraveled();
    float alphaAvgChargingTime = aircraftGroup[2].getAvgChargingTimes();
    int alphaTotalFaults = aircraftGroup[2].getNumFaults();
    float alphaPassengerMiles = aircraftGroup[2].getPassengerMiles();
    logger.log(DEBUG, "alpha avg flight time: " + to_string(alphaAvgFlightTime));
    logger.log(DEBUG, "alpha avg distance traveled: " + to_string(alphaAvgDistanceTraveled));
    logger.log(DEBUG, "alpha avg charging time: " + to_string(alphaAvgChargingTime));
    logger.log(DEBUG, "alpha total faults: " + to_string(alphaTotalFaults));
    logger.log(DEBUG, "alpha passenger miles: " + to_string(alphaPassengerMiles));
}

int main() {

    // test_simloop();
    test_simGatherData();

    return 0;
}