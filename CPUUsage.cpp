#include <iostream>
#include <mach/mach_host.h>
#include <mach/host_info.h>
#include <cmath>

void getCPUUsage() {
    host_cpu_load_info_data_t cpuLoad;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    kern_return_t result = host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuLoad, &count);

    if (result == KERN_SUCCESS) {
        double user = static_cast<double>(cpuLoad.cpu_ticks[CPU_STATE_USER]);
        double system = static_cast<double>(cpuLoad.cpu_ticks[CPU_STATE_SYSTEM]);
        double idle = static_cast<double>(cpuLoad.cpu_ticks[CPU_STATE_IDLE]);
        double total = user + system + idle;

        double userUsage = std::round((user / total) * 100 * 100) / 100;
        double systemUsage = std::round((system / total) * 100 * 100) / 100;
        double idleUsage = std::round((idle / total) * 100 * 100) / 100;

        std::cout << "CPU Usage: \n"
                  << " User: " << userUsage << "%\n"
                  << " System: " << systemUsage << "%\n"
                  << " Idle: " << idleUsage << "%\n";
    } else {
        std::cerr << "Error: Unable to fetch CPU usage" << std::endl;
    }
}
