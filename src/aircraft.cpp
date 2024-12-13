#include "./aircraft.h"
using namespace std;


Aircraft::Aircraft(string name,
                 int cruiseSpeed,
                 int batteryCapacity,
                 int timeToCharge,
                 int energyUsage,
                 int passengerCount,
                 int faultProb) {

                    // static variables that shouldn't change
                    name = name;
                    cruiseSpeed = cruiseSpeed;
                    batteryCapacity = batteryCapacity;
                    timeToCharge = timeToCharge;
                    energyUsage = energyUsage;
                    passengerCount = passengerCount;
                    faultProb = faultProb;
                    
                    flightTime = 0;
                    distanceTraveled = 0;
                    timeCharging = 0;
                    numFaults = 0;
                    remainingCharge = batteryCapacity;

                    current_state = GROUNDED; // should the aircraft start in FLYING for simplicity?
                 }

void Aircraft::updateParameters(int step) {
    flightTime += step;
    distanceTraveled += cruiseSpeed * step / HOUR_TO_MS;
    // numFaults += rand(faultProb);
    remainingCharge -= energyUsage * cruiseSpeed * step / HOUR_TO_MS;
    if (remainingCharge < 0) {
        current_state = GROUNDED;
    }
}

bool Aircraft::charging(int step) {
    timeCharging += step;
    if(timeCharging % timeToCharge == 0) {
        remainingCharge = batteryCapacity;
        current_state = FLYING;
        return true;
    }
    return false;
}
