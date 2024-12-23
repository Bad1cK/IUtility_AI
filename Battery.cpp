#include <iostream>
#include <iomanip>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/ps/IOPowerSources.h>
#include <IOKit/ps/IOPSKeys.h>
#include <cstdlib>
#include <string>
#include <sstream>

// Цвета текста
const std::string GREEN_TEXT = "\033[0;32m";
const std::string RED_TEXT = "\033[0;31m";
const std::string RESET_TEXT = "\033[0m";

// Получение статуса батареи
void getBatteryStatus() {
    CFTypeRef info = IOPSCopyPowerSourcesInfo();
    CFArrayRef sources = IOPSCopyPowerSourcesList(info);

    if (!info || !sources) {
        std::cerr << RED_TEXT << "Error: Unable to fetch battery information" << RESET_TEXT << std::endl;
        return;
    }

    for (CFIndex i = 0; i < CFArrayGetCount(sources); i++) {
        CFDictionaryRef batteryInfo = IOPSGetPowerSourceDescription(info, CFArrayGetValueAtIndex(sources, i));
        if (batteryInfo) {
            CFStringRef nameRef = (CFStringRef)CFDictionaryGetValue(batteryInfo, CFSTR(kIOPSNameKey));
            CFNumberRef currentCapacityRef = (CFNumberRef)CFDictionaryGetValue(batteryInfo, CFSTR(kIOPSCurrentCapacityKey));
            CFNumberRef maxCapacityRef = (CFNumberRef)CFDictionaryGetValue(batteryInfo, CFSTR(kIOPSMaxCapacityKey));
            CFBooleanRef isChargingRef = (CFBooleanRef)CFDictionaryGetValue(batteryInfo, CFSTR(kIOPSIsChargingKey));

            if (nameRef && currentCapacityRef && maxCapacityRef && isChargingRef) {
                char name[256];
                CFStringGetCString(nameRef, name, sizeof(name), kCFStringEncodingUTF8);

                int currentCapacity, maxCapacity;
                CFNumberGetValue(currentCapacityRef, kCFNumberIntType, &currentCapacity);
                CFNumberGetValue(maxCapacityRef, kCFNumberIntType, &maxCapacity);

                bool isCharging = CFBooleanGetValue(isChargingRef);
                double chargePercentage = (double)currentCapacity / maxCapacity * 100.0;

                std::cout << GREEN_TEXT << "Battery Status:" << RESET_TEXT << std::endl;
                std::cout << "Name: " << name << std::endl;
                std::cout << "Charge: " << std::fixed << std::setprecision(2) << chargePercentage << "%" << std::endl;
                std::cout << "Status: " << (isCharging ? "Charging" : "Not Charging") << std::endl;
            }
        }
    }

    CFRelease(sources);
    CFRelease(info);
}

// Включение энергосбережения
void enablePowerSavingMode() {
    int result = system("sudo pmset lowpowermode 1");
    if (result == 0) {
        std::cout << "Power-saving mode enabled" << std::endl;
    } else {
        std::cerr << "Error enabling power-saving mode" << std::endl;
    }
}

// Выключение энергосбережения
void disablePowerSavingMode() {
    int result = system("sudo pmset lowpowermode 0");
    if (result == 0) {
        std::cout << "Power-saving mode disabled" << std::endl;
    } else {
        std::cerr << "Error disabling power-saving mode" << std::endl;
    }
}

// Ручное чтение информации о батарее
void readBatteryInfoManually() {
    io_registry_entry_t service = IOServiceGetMatchingService(kIOMainPortDefault, IOServiceMatching("AppleSmartBattery"));

    if (!service) {
        std::cerr << "Error: Cannot find service AppleSmartBattery" << std::endl;
        return;
    }

    CFMutableDictionaryRef properties = nullptr;
    kern_return_t result = IORegistryEntryCreateCFProperties(service, &properties, kCFAllocatorDefault, 0);

    if (result == KERN_SUCCESS && properties) {
        CFNumberRef currentCapacityRef = (CFNumberRef)CFDictionaryGetValue(properties, CFSTR("CurrentCapacity"));
        CFNumberRef maxCapacityRef = (CFNumberRef)CFDictionaryGetValue(properties, CFSTR("MaxCapacity"));
        CFBooleanRef isChargingRef = (CFBooleanRef)CFDictionaryGetValue(properties, CFSTR("IsCharging"));

        if (currentCapacityRef && maxCapacityRef && isChargingRef) {
            int currentCapacity, maxCapacity;
            CFNumberGetValue(currentCapacityRef, kCFNumberIntType, &currentCapacity);
            CFNumberGetValue(maxCapacityRef, kCFNumberIntType, &maxCapacity);
            bool isCharging = CFBooleanGetValue(isChargingRef);

            double chargePercentage = (double)currentCapacity / maxCapacity * 100.0;
            std::cout << "Capacity: " << currentCapacity << " mAh" << std::endl;
            std::cout << "Max Capacity: " << maxCapacity << " mAh" << std::endl;
            std::cout << "Charge: " << std::fixed << std::setprecision(2) << chargePercentage << "%" << std::endl;
            std::cout << "Status: " << (isCharging ? "Charging" : "Not Charging") << std::endl;
        } else {
            std::cerr << "Error: Cannot fetch battery properties" << std::endl;
        }
    } else {
        std::cerr << "Error: Unable to access battery properties" << std::endl;
    }

    if (properties) {
        CFRelease(properties);
    }
    IOObjectRelease(service);
}
