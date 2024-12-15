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
    logger.log(INFO, "deplete the battery, aircraft should gound and have no remaining charge");
    myAircraft.beginFlying();
    myAircraft.processTime(1000);

    Aircraft_states pre_current_state = myAircraft.getCurrentState();

    myCharger.dockAircraft(&myAircraft);

    Aircraft_states post_current_state = myAircraft.getCurrentState();

    // logging outcome of docking aircraft
    IS_TRUE(myCharger.getChargerState() == OCCUPIED);

    IS_TRUE(myAircraft.getCurrentState() == CHARGING);
    logger.log(DEBUG, "aircraft state: " + aircraftStateToString(pre_current_state) + " -> " + aircraftStateToString(post_current_state));
    
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

    // deplete the battery and dock into charging station
    logger.log(INFO, "deplete the battery and dock into charging station");
    myAircraft.beginFlying();
    myAircraft.processTime(1000);
    myCharger.dockAircraft(&myAircraft);

    // charge aicraft via the charging station
    // aircraft will finish charing in 36000ms
    myCharger.processTime(18000);

    IS_TRUE(myCharger.getChargerState() == OCCUPIED);
    logger.log(DEBUG, "station state after charging a little: " + chargerStateToString(myCharger.getChargerState()));

    IS_TRUE(myAircraft.getCurrentState() == CHARGING);
    logger.log(DEBUG, "aircraft state after charging a little: " + aircraftStateToString(myAircraft.getCurrentState()));
}

void test_finishCharging() {
    Logger logger("./test_finishCharging.log");
    logger.log(INFO, "running test_finishCharging()");

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

    // deplete the battery and dock into charging station
    myAircraft.beginFlying();
    myAircraft.processTime(1000);
    myCharger.dockAircraft(&myAircraft);

    // charge aicraft via the charging station
    // aircraft will finish charing in 36000ms
    myCharger.processTime(36000);

    IS_TRUE(myCharger.getChargerState() == FREE);
    logger.log(DEBUG, "charging station state after aircraft finishes charging: " + chargerStateToString(myCharger.getChargerState()));

    IS_TRUE(myAircraft.getCurrentState() == FLYING);
    logger.log(DEBUG, "aircraft state after finish charging: " + aircraftStateToString(myAircraft.getCurrentState()));
}

void test_chargeMultAircraft() {
    Logger logger("./test_chargeMultAircraft.log");
    logger.log(INFO, "running test_chargeMultAircraft()");

    // aircrafts
    string name_1 = "aircraft_1";
    string name_2 = "aircraft_2";
    float cruiseSpeed = 500;     // mph
    float batteryCapacity = 1;   // kWh
    float timeToCharge = 0.01;   // 0.01 hours -> 36000 ms
    float energyUsage = 10;      // kWh
    int passengerCount = 4;      // people
    float faultProb = 0.25;      // probabily/hour
    Aircraft aircraft_1(name_1, cruiseSpeed, batteryCapacity, timeToCharge, energyUsage, passengerCount, faultProb);
    Aircraft aircraft_2(name_2, cruiseSpeed, batteryCapacity, timeToCharge, energyUsage, passengerCount, faultProb);

    Charger myCharger;
    myCharger.dockAircraft(&aircraft_1);

    IS_TRUE(myCharger.getQueueSize() == 1);
    logger.log(DEBUG, "docked 1 aircraft, aircraft in queue: " + to_string(myCharger.getQueueSize()));

    myCharger.processTime(18000);

    myCharger.dockAircraft(&aircraft_2);
    IS_TRUE(myCharger.getQueueSize() == 2);
    logger.log(DEBUG, "docked 2nd aircraft, aircraft in queue: " + to_string(myCharger.getQueueSize()));

    // finish charging first aircraft
    logger.log(INFO, "finish charging first aircraft");
    myCharger.processTime(18000);

    IS_TRUE(myCharger.getQueueSize() == 1);
    IS_TRUE(aircraft_1.getCurrentState() == FLYING);
    IS_TRUE(aircraft_2.getCurrentState() == CHARGING);
    logger.log(DEBUG, "first aircraft starts flying, aircraft in queue: " + to_string(myCharger.getQueueSize()));
    
    IS_TRUE(myCharger.getChargingAircraftName() == "aircraft_2");
    logger.log(DEBUG, "aircraft that is now charging: " + myCharger.getChargingAircraftName());

    // finish charging second aircraft
    logger.log(INFO, "finish charging second aircraft");
    myCharger.processTime(36000);

    IS_TRUE(myCharger.getQueueSize() == 0);
    logger.log(DEBUG, "second aircraft starts flying, aircraft in queue: " + to_string(myCharger.getQueueSize()));
    
    IS_TRUE(myCharger.getChargingAircraftName() == "NULL");
    logger.log(DEBUG, "no aircraft should be left in queue: " + myCharger.getChargingAircraftName());

    IS_TRUE(aircraft_2.getCurrentState() == FLYING);
    logger.log(DEBUG, "aircraft_2 should now be flying, current state: " + aircraftStateToString(aircraft_2.getCurrentState()));
}

int main() {

    test_dockAircraft();
    test_chargerProcessTime();
    test_finishCharging();
    test_chargeMultAircraft();
    return 0;
}