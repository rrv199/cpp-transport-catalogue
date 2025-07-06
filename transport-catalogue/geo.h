#pragma once 
#include <cmath>  

// Структура для хранения географических координат
struct Coordinates {
    double latitude;  // Широта в градусах
    double longitude;  // Долгота в градусах
    
    // Оператор сравнения на равенство для координат
    bool operator==(const Coordinates& other) const {
        return latitude == other.latitude  // Сравнение широт
            && longitude == other.longitude;  // Сравнение долгот
    }
};

// Функция для вычисления расстояния между двумя точками на поверхности Земли
inline double ComputeDistance(Coordinates start, Coordinates end) {
    using namespace std; 
    
    // Проверка на совпадение начальной и конечной точек
    if (!(start == end)) {
        const double dr = 3.1415926535 / 180.;  // Константа для перевода градусов в радианы
        
        // Формула гаверсинуса для вычисления расстояния между двумя точками на сфере
        return acos(sin(start.latitude * dr) * sin(end.latitude * dr) 
                    + cos(start.latitude * dr) * cos(end.latitude * dr) 
                    * cos(abs(start.longitude - end.longitude) * dr))  
                    * 6371000;  
    } else {
        return 0.0;  
    }
}
