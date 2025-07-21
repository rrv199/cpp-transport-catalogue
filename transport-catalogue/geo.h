#pragma once

#include <cmath>

struct Coordinates {
    double latitude;
    double longitude;
    
    bool operator==(const Coordinates& other) const {
        return latitude == other.latitude && longitude == other.longitude;
    }
};

constexpr double EARTH_RADIUS = 6371000;

inline double ComputeDistance(Coordinates start, Coordinates end) {
    using namespace std;
    
    if (start == end) {
        return 0.0;
    }

    const double dr = 3.1415926535 / 180.;
    return acos(sin(start.latitude * dr) * sin(end.latitude * dr) +
               cos(start.latitude * dr) * cos(end.latitude * dr) *
               cos(abs(start.longitude - end.longitude) * dr)) *
               EARTH_RADIUS;
}
