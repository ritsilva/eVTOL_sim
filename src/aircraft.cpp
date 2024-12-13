#include <random>
#include "./aircraft.h"
#include "./logging.h"
using namespace std;
default_random_engine rand_engine;


Aircraft::Aircraft(string name,
                   float _cruiseSpeed,
                   float _batteryCapacity,
                   float _timeToCharge,
                   float _energyUsage,
                   int _passengerCount,
                   float _faultProb) {

                    // static variables that shouldn't change
                    name = name;
                    cruiseSpeed = _cruiseSpeed;
                    batteryCapacity = _batteryCapacity;
                    timeToCharge = _timeToCharge;
                    energyUsage = _energyUsage;
                    passengerCount = _passengerCount;
                    faultProb = _faultProb;
                    
                    flightTime = 0;
                    distanceTraveled = 0;
                    timeCharging = 0;
                    numFaults = 0;
                    remainingCharge = batteryCapacity;

                    currentState = GROUNDED; // should the aircraft start in FLYING for simplicity?
                    currentChargingTime = 0;
                 }

bool Aircraft::doesFaultOccur(float prob) {
    bernoulli_distribution d(prob);
    return d(rand_engine);
}

void Aircraft::updateParameters(int step) {
    flightTime += step;
    distanceTraveled += cruiseSpeed * step / HOUR_TO_MS;
    numFaults += doesFaultOccur(faultProb);
    remainingCharge -= energyUsage * cruiseSpeed * step / HOUR_TO_MS;
    if (remainingCharge < 0) {
        currentState = GROUNDED;
    }
}

bool Aircraft::charging(int step) {
    timeCharging += step;
    currentChargingTime += step;
    if(currentChargingTime >= timeToCharge*HOUR_TO_MS) {
        remainingCharge = batteryCapacity;
        currentState = FLYING;
        currentChargingTime = 0;
        return true;
    }
    return false;
}
