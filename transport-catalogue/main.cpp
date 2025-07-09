#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"
#include <iostream>
#include <string>

using namespace std;
using namespace TransportSystem;

int main() {
    try {
        // Создаем экземпляр каталога транспорта
        TransportCatalogue catalogue;
        
        // Читаем и обрабатываем входные данные
        Input::input_(catalogue);
        
        // Обрабатываем запросы (добавлено Stat::)
        Stat::output_(catalogue, cin, cout);
        
        return 0;
    } catch (const exception& e) {
        cerr << "Произошла ошибка: " << e.what() << endl;
        return 1;
    }
}
