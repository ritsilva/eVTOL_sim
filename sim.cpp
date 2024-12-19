#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "./src/aircraft.h"
#include "./src/charger.h"
using namespace std;

int main() {
    Logger logger("./sim.log");
    srand(time(0));

    // simulation parameters
    int numAircraft = 20;
    int numChargers = 3;
    int totalTime = 3 * HOUR_TO_MS; // 3 hours in miliseconds
    int timeStep = 1;
    // NOTE: implement something to allow controlled sim speeds
    // and update rate in case you want to fastforward or jump 
    // to predetermined amounts of time

    logger.log(INFO, "Running simulation with:");
    logger.log(INFO, "numAircraft: " + to_string(numAircraft));
    logger.log(INFO, "numChargers: " + to_string(numChargers));
    logger.log(INFO, "totalTime(hrs): " + to_string(totalTime/HOUR_TO_MS));

    // randomize how many of each aircraft there are.
    vector<Aircraft> aircrafts;
    vector<Charger> chargers;
    for (int i =0; i < numAircraft; i++) {
        int type = rand()%(5);
        string name;
        float cruiseSpeed;
        float batteryCapacity;
        float timeToCharge;
        float energyUsage;
        int passengerCount;
        float faultProb;

        switch(type) {
        case 0:
            name = "Alpha Company";
            cruiseSpeed = 120;
            batteryCapacity = 320;
            timeToCharge = 0.6;
            energyUsage = 1.6;
            passengerCount = 4;
            faultProb = 0.25;
            break;
        case 1:
            name = "Bravo Company";
            cruiseSpeed = 100;
            batteryCapacity = 100;
            timeToCharge = 0.2;
            energyUsage = 1.5;
            passengerCount = 5;
            faultProb = 0.10;
            break;
        case 2:
            name = "Charlie Company";
            cruiseSpeed = 160;
            batteryCapacity = 220;
            timeToCharge = 0.8;
            energyUsage = 2.2;
            passengerCount = 3;
            faultProb = 0.05;
            break;
        case 3:
            name = "Delta Company";
            cruiseSpeed = 90;
            batteryCapacity = 120;
            timeToCharge = 0.62;
            energyUsage = 0.8;
            passengerCount = 2;
            faultProb = 0.22;
            break;
        case 4:
            name = "Echo Company";
            cruiseSpeed = 150;
            batteryCapacity = 0.3;
            timeToCharge = 5.8;
            energyUsage = 2;
            passengerCount = 2;
            faultProb = 0.61;
            break;

        default:
            logger.log(ERROR, "Could not generate aircraft");
            break;
        }
        Aircraft newAircraft(name, cruiseSpeed, batteryCapacity, timeToCharge, energyUsage, passengerCount, faultProb);
        aircrafts.push_back(newAircraft);
    }

    for(int i = 0; i < numChargers; i++) {
        Charger newCharger;
        chargers.push_back(newCharger);
    }

    // start flying aircraft
    for(int i = 0; i < aircrafts.size(); i++) {
        aircrafts[i].beginFlying();
    }

    // simulation loop
    for (int time = 0; time < totalTime; time += timeStep) {
        vector<int> aircraftNeedingCharge;
        // process flying aircraft
        for(int j = 0; j < aircrafts.size(); j++) {
            if(aircrafts[j].getCurrentState() == FLYING) {
                aircrafts[j].processTime(timeStep);
            } else if(aircrafts[j].getCurrentState() == GROUNDED) {
                logger.log(DEBUG, aircrafts[j].getName() + " grounded for charging at " + to_string(time));
                aircraftNeedingCharge.push_back(j);
            }
        }

        // process charing aircraft
        for(int j = 0; j < chargers.size(); j++) {
            chargers[j].processTime(timeStep);
        }

        // check if there are any aircraft that need to be charged
        // does not make sense to check before processing 
        // the aircraft that are already charging
        // NOTE: you could potentially get a better estimate for the 
        // shortest queue time by taking the sum of all the aircraft  
        // capacities that are in a charger's queue.
        // alternativly you could implement a shared queue for all chargers. 
        // that way once an aircraft finishes charging which ever aircraft 
        // is next in line, reguardless of what station it landed at, could
        // begin charging immediately
        for(const int &index : aircraftNeedingCharge) {
            int shortestQueue = 0;
            int lenOfShortestQueue = chargers[0].getQueueSize();
            for(int j = 0; j < chargers.size(); j++) {
                if(chargers[j].getQueueSize() < lenOfShortestQueue) shortestQueue = j;
            }
            logger.log(DEBUG, "docking " + aircrafts[index].getName() + " into charger " + to_string(shortestQueue));
            chargers[shortestQueue].dockAircraft(&aircrafts[index]);
        }
    }

    // gather stats for each aircraft type
    vector<string> names;
    for(int i = 0; i < aircrafts.size(); i++) {
        // get unique aircraft names
        if(!(find(names.begin(), names.end(), aircrafts[i].getName()) != names.end())) {
            names.push_back(aircrafts[i].getName());
        }
    }

    for(const string &myName : names) {
        float avgFlightTime = 0;
        float avgDistanceTraveled = 0;
        float avgChargingTime = 0;
        float passengerMiles = 0;
        int totalFaults = 0;
        int numAircraft = 0;
        for(int i = 0; i < aircrafts.size(); i++) {
            if(aircrafts[i].getName() == myName) {
                avgFlightTime += aircrafts[i].getAvgFlightTime();
                avgDistanceTraveled += aircrafts[i].getAvgDistanceTraveled();
                avgChargingTime += aircrafts[i].getAvgChargingTimes();
                passengerMiles += aircrafts[i].getPassengerMiles();
                totalFaults += aircrafts[i].getNumFaults();
                numAircraft ++;
            }
        }
        avgFlightTime /= numAircraft;
        avgDistanceTraveled /= numAircraft;
        avgChargingTime /= numAircraft;

        logger.log(INFO, "---- Results for " + myName + " ----");
        logger.log(INFO, "numAircraft: " + to_string(numAircraft));
        logger.log(INFO, "avgFlightTime(hours): " + to_string(avgFlightTime/HOUR_TO_MS));
        logger.log(INFO, "avgDistanceTraveled(miles): " + to_string(avgDistanceTraveled));
        logger.log(INFO, "avgChargingTime(hours): " + to_string(avgChargingTime/HOUR_TO_MS));
        logger.log(INFO, "passengerMiles(miles): " + to_string(passengerMiles));
        logger.log(INFO, "totalFaults: " + to_string(totalFaults));
    }

    return 0;
}