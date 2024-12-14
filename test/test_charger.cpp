#include <iostream>
#include <stdio.h>
#include "../src/aircraft.h"
#include "../src/charger.h"
#include "../src/logging.h"
using namespace std;

void test_dockAircraft() {
    Logger logger("./test_dockAircraft.log");
    logger.log(INFO, "running test_dockAircraft()");

    // aircraft 1
    string name = "aircraft_1";
    float cruiseSpeed = 500;     // mph
    float batteryCapacity = 1;   // kWh
    float timeToCharge = 0.01;   // 0.01 hours -> 36000 ms
    float energyUsage = 10;      // kWh
    int passengerCount = 4;      // people
    float faultProb = 0.25;      // probabily/hour
    Aircraft myAircraft(name, cruiseSpeed, batteryCapacity, timeToCharge, energyUsage, passengerCount, faultProb);

    // charger 1
    Charger myCharger;
    IS_TRUE(myCharger.getChargerState() == FREE);


    // deplete the battery
    // aircraft should gound and have no remaining charge
    myAircraft.processTime(1000);

    Aircraft_states pre_current_state = myAircraft.getCurrentState();

    myCharger.dockAircraft(&myAircraft);

    Aircraft_states post_current_state = myAircraft.getCurrentState();

    // logging outcome of docking aircraft
    IS_TRUE(myCharger.getChargerState() == OCCUPIED);

    IS_TRUE(myAircraft.getCurrentState() == CHARGING);
    logger.log(DEBUG, "aircraft state: " + to_string(pre_current_state) + " -> " + to_string(post_current_state));
    
    IS_TRUE(myAircraft.getName() == myCharger.getChargingAircraftName());
    logger.log(DEBUG, "aircraft name: " + myAircraft.getName());
    logger.log(DEBUG, "charger docked aircraft name: " + myCharger.getChargingAircraftName());
}

void test_chargerProcessTime() {
    Logger logger("./test_chargerProcessTime.log");
    logger.log(INFO, "running test_chargerProcessTime()");

    // aircraft 1
    string name = "aircraft_1";
    float cruiseSpeed = 500;     // mph
    float batteryCapacity = 1;   // kWh
    float timeToCharge = 0.01;   // 0.01 hours -> 36000 ms
    float energyUsage = 10;      // kWh
    int passengerCount = 4;      // people
    float faultProb = 0.25;      // probabily/hour
    Aircraft myAircraft(name, cruiseSpeed, batteryCapacity, timeToCharge, energyUsage, passengerCount, faultProb);

    // charger 1
    Charger myCharger;

}

int main() {

    test_dockAircraft();
    test_chargerProcessTime();
    return 0;
}