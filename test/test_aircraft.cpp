#include <iostream>
#include <stdio.h>
#include "../src/aircraft.h"
#include "../src/logging.h"
using namespace std;

void test_processTime() {
    Logger logger("./test_processTime.log");
    logger.log(INFO, "running test_processTime()");
    string name = "mine";
    float cruiseSpeed = 100;      // mph
    float batteryCapacity = 300;  // kWh
    float timeToCharge = 0.1;     // hours
    float energyUsage = 1.5;      // kWh
    int passengerCount = 4;       // people
    float faultProb = 1;          // probabily/hour -> set to 1 to force fault

    Aircraft myAircraft(name, cruiseSpeed, batteryCapacity, timeToCharge, energyUsage, passengerCount, faultProb);

    float pre_remainingCharge = myAircraft.getRemainingCharge();
    Aircraft_states pre_current_state = myAircraft.getCurrentState();
    int pre_flightTime = myAircraft.getFlightTime();
    float pre_distanceTraveled = myAircraft.getDistanceTraveled();
    int pre_timeCharging = myAircraft.getCurrentChargingTime();
    int pre_numFaults = myAircraft.getNumFaults();

    // proceed 1 ms and check aircraft stats
    logger.log(INFO, "proceed 1 ms and check aircraft stats");
    myAircraft.beginFlying();
    myAircraft.processTime(1);

    float post_remainingCharge = myAircraft.getRemainingCharge();
    Aircraft_states post_current_state = myAircraft.getCurrentState();
    int post_flightTime = myAircraft.getFlightTime();
    float post_distanceTraveled = myAircraft.getDistanceTraveled();
    int post_timeCharging = myAircraft.getCurrentChargingTime();
    int post_numFaults = myAircraft.getNumFaults();


    IS_TRUE(post_remainingCharge == pre_remainingCharge - energyUsage * cruiseSpeed * 1/HOUR_TO_MS);
    logger.log(DEBUG, "remaingCharge: " + to_string(pre_remainingCharge) + " -> " + to_string(post_remainingCharge));
    
    IS_TRUE(pre_current_state == GROUNDED)
    IS_TRUE(post_current_state == FLYING);
    logger.log(DEBUG, "currentState: " + aircraftStateToString(pre_current_state) + " -> " + aircraftStateToString(post_current_state));
    
    IS_TRUE(post_flightTime == pre_flightTime + 1);
    logger.log(DEBUG, "flightTime: " + to_string(pre_flightTime) + " -> " + to_string(post_flightTime));
    
    IS_TRUE(post_distanceTraveled == pre_distanceTraveled + cruiseSpeed*1/HOUR_TO_MS);
    logger.log(DEBUG, "distanceTraveled: " + to_string(pre_distanceTraveled) + " -> " + to_string(post_distanceTraveled));
    
    IS_TRUE(post_timeCharging == pre_timeCharging);
    logger.log(DEBUG, "timeCharging: " + to_string(pre_timeCharging) + " -> " + to_string(post_timeCharging));
    
    IS_TRUE(post_numFaults == pre_numFaults + 1);
    logger.log(DEBUG, "numFaults: " + to_string(pre_numFaults) + " -> " + to_string(post_numFaults));
}

void test_charging() {
    Logger logger("./test_charging.log");
    logger.log(INFO, "running test_charging()");

    string name = "mine";
    float cruiseSpeed = 500;     // mph
    float batteryCapacity = 1;   // kWh
    float timeToCharge = 0.01;   // 0.01 hours -> 36000 ms
    float energyUsage = 10;      // kWh
    int passengerCount = 4;      // people
    float faultProb = 0.25;      // probabily/hour

    Aircraft myAircraft(name, cruiseSpeed, batteryCapacity, timeToCharge, energyUsage, passengerCount, faultProb);
    
    int pre_flightTime = myAircraft.getFlightTime();
    float pre_remainingCharge = myAircraft.getRemainingCharge();
    Aircraft_states pre_current_state = myAircraft.getCurrentState();

    // deplete the battery
    // aircraft should gound and have no remaining charge
    logger.log(INFO, "deplete the battery, aircraft should gound and have no remaining charge");
    myAircraft.beginFlying();
    myAircraft.processTime(1000);

    int post_flightTime = myAircraft.getPastFlightTimes()[0];
    float post_remainingCharge = myAircraft.getRemainingCharge();
    Aircraft_states post_current_state = myAircraft.getCurrentState();

    // logging outcome of processTime
    IS_TRUE(post_flightTime == pre_flightTime + 1000);
    logger.log(DEBUG, "flightTime: " + to_string(pre_flightTime) + " -> " + to_string(post_flightTime));

    IS_TRUE(post_remainingCharge < 0);
    logger.log(DEBUG, "remainingCharge: " + to_string(pre_flightTime) + " -> " + to_string(post_remainingCharge));

    IS_TRUE(post_current_state == pre_current_state);
    logger.log(DEBUG, "currentState: " + aircraftStateToString(pre_current_state) + " -> " + aircraftStateToString(post_current_state));


    // charge the battery
    // aircraft should being flying and battery should refill
    logger.log(INFO, "aircraft should being flying and battery should refill");
    int pre_timeCharging = myAircraft.getCurrentChargingTime();

    myAircraft.charge(36000);

    int post_timeCharging = myAircraft.getPastChargingTimes()[0];

    // logging outcome of charging
    IS_TRUE(post_timeCharging == pre_timeCharging + 36000);
    logger.log(DEBUG, "timeCharging: " + to_string(pre_timeCharging) + " -> " + to_string(post_timeCharging));

    IS_TRUE(myAircraft.getRemainingCharge() == batteryCapacity);
    logger.log(DEBUG, "remainingCharge: " + to_string(post_remainingCharge) + " -> " + to_string(myAircraft.getRemainingCharge()));

    IS_TRUE(myAircraft.getCurrentState() == FLYING);
    logger.log(DEBUG, "currentState: " + aircraftStateToString(post_current_state) + " -> " + aircraftStateToString(myAircraft.getCurrentState()));

}

int main() {

    test_processTime();
    test_charging();

    return 0;
}