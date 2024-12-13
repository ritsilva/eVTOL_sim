#include <iostream>
#include <vector>
#include <random>
#include <stdio.h>
#include <assert.h>
#include "../src/aircraft.h"
using namespace std;

#define IS_TRUE(x) {if (!(x)) cout << __FUNCTION__ << " failed on line " << __LINE__ << endl;}

void test_updateParameters() {
    string name = "mine";
    int cruiseSpeed = 100;      // mph
    int batteryCapacity = 300;  // kWh
    int timeToCharge = 0.1;     // hours
    int energyUsage = 1.5;      // kWh
    int passengerCount = 4;     // people
    int faultProb = 1;          // probabily/hour force fault

    Aircraft myAircraft(name, cruiseSpeed, batteryCapacity, timeToCharge, energyUsage, passengerCount, faultProb);

    float pre_remainingCharge = myAircraft.getRemainingCharge();
    Aircraft_states pre_current_state = myAircraft.getCurrent_state();
    int pre_flightTime = myAircraft.getFlightTime();
    float pre_distanceTraveled = myAircraft.getDistanceTraveled();
    int pre_timeCharging = myAircraft.getTimeCharging();
    int pre_numFaults = myAircraft.getNumFaults();

    // proceed 1 ms and check aircraft stats
    myAircraft.updateParameters(1);

    float post_remainingCharge = myAircraft.getRemainingCharge();
    Aircraft_states post_current_state = myAircraft.getCurrent_state();
    int post_flightTime = myAircraft.getFlightTime();
    float post_distanceTraveled = myAircraft.getDistanceTraveled();
    int post_timeCharging = myAircraft.getTimeCharging();
    int post_numFaults = myAircraft.getNumFaults();

    IS_TRUE(post_remainingCharge == pre_remainingCharge - energyUsage * cruiseSpeed * 1/HOUR_TO_MS);
    IS_TRUE(post_current_state == pre_current_state);
    IS_TRUE(post_flightTime == pre_flightTime + 1);
    IS_TRUE(post_distanceTraveled == pre_distanceTraveled + cruiseSpeed*1/HOUR_TO_MS);
    IS_TRUE(post_timeCharging == pre_timeCharging);
    IS_TRUE(post_numFaults == pre_numFaults + 1);
}

void test_charging() {
    string name = "mine";
    int cruiseSpeed = 100;      // mph
    int batteryCapacity = 10;   // kWh
    int timeToCharge = 0.1;     // hours
    int energyUsage = 1.5;      // kWh
    int passengerCount = 4;     // people
    int faultProb = 0.25;       // probabily/hour - force fault

    Aircraft myAircraft(name, cruiseSpeed, batteryCapacity, timeToCharge, energyUsage, passengerCount, faultProb);
    
    int pre_flightTime = myAircraft.getFlightTime();

    // deplete the battery
    myAircraft.updateParameters(1);
    // aircraft should gound, need to trigger charging process

    // charge
    myAircraft.charging(36000);

    // check that it starts flying after done charging
    int post_timeCharging = myAircraft.getTimeCharging();

    IS_TRUE(post_timeCharging == pre_flightTime + 36000)


}

int main() {

    test_updateParameters();
    return 0;
}