#include <iostream>
#include <sys/statvfs.h>
#include <string>
#include <iomanip> // для форматирования вывода

void getDiskUsage(const std::string& path) {
    struct statvfs stats;

    // Проверяем существование пути
    if (statvfs(path.c_str(), &stats) != 0) {
        std::cerr << "Error: Path '" << path << "' does not exist or cannot be accessed." << std::endl;
        return;
    }

    // Получаем информацию о диске
    unsigned long long total = stats.f_blocks * stats.f_frsize; // Общий размер в байтах
    unsigned long long available = stats.f_bavail * stats.f_frsize; // Доступное пространство в байтах
    unsigned long long used = total - available; // Использованное пространство

    // Перевод в мегабайты
    double totalMB = static_cast<double>(total) / 1'048'576;
    double availableMB = static_cast<double>(available) / 1'048'576;
    double usedMB = static_cast<double>(used) / 1'048'576;

    // Форматированный вывод
    std::cout << "Disk Usage for path '" << path << "':" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "- Total: " << totalMB << " MB" << std::endl;
    std::cout << "- Used: " << usedMB << " MB" << std::endl;
    std::cout << "- Available: " << availableMB << " MB" << std::endl;
}
