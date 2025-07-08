#pragma once

#include <cmath>

// Географические координаты (широта и долгота)
struct Coordinates {
    double latitude;  // Широта в градусах
    double longitude; // Долгота в градусах
    
    // Проверка на равенство координат
    bool operator==(const Coordinates& other) const {
        return latitude == other.latitude && longitude == other.longitude;
    }
};

// Радиус Земли в метрах
constexpr double EARTH_RADIUS = 6371000;

// Вычисление расстояния между двумя точками на сфере (Земле)
inline double ComputeDistance(Coordinates start, Coordinates end) {
    using namespace std;
    
    // Если точки совпадают, возвращаем 0
    if (start == end) {
        return 0.0;
    }

    // Коэффициент перевода градусов в радианы
    const double dr = 3.1415926535 / 180.;
    
    // Вычисление расстояния по формуле гаверсинусов
    return acos(sin(start.latitude * dr) * sin(end.latitude * dr) +
               cos(start.latitude * dr) * cos(end.latitude * dr) *
               cos(abs(start.longitude - end.longitude) * dr)) *
               EARTH_RADIUS;
}
