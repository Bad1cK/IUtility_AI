#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "CPUUsage.h"
#include "getDiskUsage.h"
#include "getMemoryUsage.h"
#include "getTopCPUProcessesWithImpact.h"
#include "Battery.h"

// Функции должны быть объявлены в соответствующих заголовках
void getCPUUsage();
void getMemoryUsage();
void getBatteryStatus();
void enablePowerSavingMode();
void disablePowerSavingMode();
void readBatteryInfoManually();
void getTopCPUProcessesWithImpact();

int main(int argc, char* argv[]) {
    // Считываем аргументы командной строки
    std::vector<std::string> arguments(argv, argv + argc);

    if (arguments.size() > 1) {
        std::string command = arguments[1];

        if (command == "cpu-usage") {
            getCPUUsage();
        } else if (command == "memory-usage") {
            getMemoryUsage();
        } else if (command == "battery-status") {
            getBatteryStatus();
        } else if (command == "enable-powersave") {
            enablePowerSavingMode();
        } else if (command == "disable-powersave") {
            disablePowerSavingMode();
        } else if (command == "battery-plus") {
            readBatteryInfoManually();
        } else if (command == "list-process") {
            getTopCPUProcessesWithImpact();
        } else {
            std::cerr << "-- ERROR: Unknown command: " << command << std::endl;
        }
    } else {
        std::cout << "Usage: ./iUtile <command>" << std::endl;
        std::cout << "Available commands:" << std::endl;
        std::cout << "  cpu-usage" << std::endl;
        std::cout << "  memory-usage" << std::endl;
        std::cout << "  battery-status" << std::endl;
        std::cout << "  enable-powersave" << std::endl;
        std::cout << "  disable-powersave" << std::endl;
        std::cout << "  battery-plus" << std::endl;
        std::cout << "  list-process" << std::endl;
    }

    return 0;
}
