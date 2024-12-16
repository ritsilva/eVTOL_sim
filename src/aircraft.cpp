#include <random>
#include "./aircraft.h"
#include "./logging.h"
using namespace std;
default_random_engine rand_engine;


string aircraftStateToString(Aircraft_states state) {
    switch(state) {
        case GROUNDED: return "GROUNDED";
        case FLYING: return "FLYING";
        case CHARGING: return "CHARGING";
        case MAX_AIRCRAFT_STATES: return "MAX_AIRCRAFT_STATES";
        default: return "UNKNOWN";
    }
}

Aircraft::Aircraft(string _name,
                   float _cruiseSpeed,
                   float _batteryCapacity,
                   float _timeToCharge,
                   float _energyUsage,
                   int _passengerCount,
                   float _faultProb) {

                    // static variables that shouldn't change
                    name = _name;
                    cruiseSpeed = _cruiseSpeed;
                    batteryCapacity = _batteryCapacity;
                    timeToCharge = _timeToCharge;
                    energyUsage = _energyUsage;
                    passengerCount = _passengerCount;
                    faultProb = _faultProb;
                    
                    flightTime = 0;
                    distanceTraveled = 0;
                    timeWaiting = 0;
                    numFaults = 0;
                    remainingCharge = batteryCapacity;

                    currentState = GROUNDED; // should the aircraft start in FLYING for simplicity?
                    currentChargingTime = 0;
                 }

bool Aircraft::doesFaultOccur(float prob) {
    bernoulli_distribution d(prob);
    return d(rand_engine);
}

void Aircraft::processTime(int step) {
    // dont want to make the aircraft fly if it is charging
    if(currentState == FLYING) {
        flightTime += step;
        distanceTraveled += cruiseSpeed * step / HOUR_TO_MS;
        numFaults += doesFaultOccur(faultProb/HOUR_TO_MS * step);
        remainingCharge -= energyUsage * cruiseSpeed * step / HOUR_TO_MS;
        if(remainingCharge < 0) {
            currentState = GROUNDED;
            pastFlightTimes.push_back(flightTime);
            pastFlightDistances.push_back(distanceTraveled);
            distanceTraveled = 0;
            flightTime = 0;
        }
    } else if(currentState == GROUNDED) {
        timeWaiting += step;
    }

}

void Aircraft::beginFlying() {
    currentState = FLYING;
}

void Aircraft::dockIntoCharger() {
    currentState = DOCKED;
}

void Aircraft::beginCharging() {
    currentState = CHARGING;
}

bool Aircraft::charge(int step) {
    currentChargingTime += step;
    if(currentChargingTime >= timeToCharge*HOUR_TO_MS) {
        remainingCharge = batteryCapacity;
        currentState = FLYING;
        pastChargingTimes.push_back(currentChargingTime + timeWaiting);
        timeWaiting = 0;
        currentChargingTime = 0;
        return true;
    }
    return false;
}

float Aircraft::getPassengerMiles() {
    float avgFlightTime = 0;
    for(const int &time : pastFlightTimes) {
        avgFlightTime += time;
    }

    return passengerCount*avgFlightTime/HOUR_TO_MS*cruiseSpeed;
}

float Aircraft::getAvgFlightTime() {
    // assuming we want to include the current flight in our avg calc
    float avgFlightTime = flightTime;
    for(const int &time : pastFlightTimes) {
        avgFlightTime += time;
    }

    if(flightTime != 0) {
        avgFlightTime = avgFlightTime/(pastFlightTimes.size() + 1); // plus 1 to acocunt for current trip
    } else {
        avgFlightTime = avgFlightTime/pastFlightTimes.size();
    }

    return avgFlightTime;
}

float Aircraft::getAvgDistanceTraveled() {
    // assuming we want to include the current trip in our avg calc
    float avgDistance = distanceTraveled;
    for(const int &time : pastFlightDistances) {
        avgDistance += time;
    }

    if(distanceTraveled != 0) {
        avgDistance = avgDistance/(pastFlightDistances.size() + 1); // plus 1 to acocunt for current trip
    } else {
        avgDistance = avgDistance/pastFlightTimes.size();
    }

    return avgDistance;
}

float Aircraft::getAvgChargingTimes() {
    float avgChargingTime = currentChargingTime;
    for(const int &time : pastChargingTimes) {
        avgChargingTime += time;
    }

    if(currentChargingTime != 0) {
        avgChargingTime = avgChargingTime/(pastChargingTimes.size() + 1); // plus 1 to acocunt for current trip
    } else {
        avgChargingTime = avgChargingTime/pastChargingTimes.size();
    }

    return avgChargingTime;
}

