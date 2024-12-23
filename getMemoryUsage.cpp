#include <iostream>
#include <mach/mach.h> // Для доступа к mach-системным вызовам
#include <iomanip>     // Для форматированного вывода

void getMemoryUsage() {
    // Получаем статистику виртуальной памяти
    mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;
    vm_statistics64_data_t vmStats;
    kern_return_t result = host_statistics64(mach_host_self(), HOST_VM_INFO64, (host_info64_t)&vmStats, &count);

    if (result == KERN_SUCCESS) {
        // Размер страницы в байтах
        unsigned long pageSize;
        host_page_size(mach_host_self(), &pageSize);

        // Вычисления на основе собранной статистики
        double freeMemory = round((double)vmStats.free_count * pageSize / 1'048'576 * 100) / 100;       // MB
        double activeMemory = round((double)vmStats.active_count * pageSize / 1'048'576 * 100) / 100;   // MB
        double inactiveMemory = round((double)vmStats.inactive_count * pageSize / 1'048'576 * 100) / 100; // MB
        double wiredMemory = round((double)vmStats.wire_count * pageSize / 1'048'576 * 100) / 100;       // MB
        double totalMemory = round((freeMemory + activeMemory + inactiveMemory + wiredMemory) * 100) / 100; // MB

        // Форматированный вывод
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Memory Usage:" << std::endl;
        std::cout << "- Total: " << totalMemory << " MB" << std::endl;
        std::cout << "- Free: " << freeMemory << " MB" << std::endl;
        std::cout << "- Active: " << activeMemory << " MB" << std::endl;
        std::cout << "- Inactive: " << inactiveMemory << " MB" << std::endl;
        std::cout << "- Wired: " << wiredMemory << " MB" << std::endl;
    } else {
        std::cerr << "Error: Unable to fetch memory usage." << std::endl;
    }
}
