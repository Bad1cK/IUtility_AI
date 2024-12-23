#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <cstdio>
#include <unistd.h>
#include <sys/sysctl.h>

void getTopCPUProcessesWithImpact() {
    // Получаем количество ядер CPU
    int coreCount = sysconf(_SC_NPROCESSORS_ONLN);

    if (coreCount <= 0) {
        std::cerr << "Error: Unable to determine the number of CPU cores." << std::endl;
        return;
    }

    // Команда для получения данных о процессах
    const std::string command = "ps axo pid,pcpu,comm | sort -k 2 -nr | head -n 10";

    // Открываем процесс через popen
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error: Unable to execute command." << std::endl;
        return;
    }

    char buffer[256];
    std::ostringstream result;

    // Считываем вывод команды
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result << buffer;
    }
    pclose(pipe);

    // Выводим заголовки
    std::cout << "Top 10 apps for CPU:" << std::endl;
    std::cout << "CPU Cores: " << coreCount << std::endl;

    // Обрабатываем результат команды
    std::istringstream output(result.str());
    std::string line;

    // Пропускаем первую строку, если она содержит заголовки
    while (std::getline(output, line)) {
        std::istringstream lineStream(line);
        std::string pid, cpuUsageStr, command;

        // Разбиваем строку на PID, CPU% и команду
        if (lineStream >> pid >> cpuUsageStr) {
            std::getline(lineStream, command);
            try {
                double cpuUsage = std::stod(cpuUsageStr);
                double impact = cpuUsage / coreCount;

                // Форматированный вывод
                std::cout << "PID: " << pid
                          << ", CPU: " << std::fixed << std::setprecision(2) << cpuUsage << "%"
                          << ", Command: " << command << std::endl;
                std::cout << "CPU usage: ~" << std::fixed << std::setprecision(2) << impact * 10
                          << "% core usage\n"
                          << std::endl;
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Invalid CPU usage value in output." << std::endl;
            }
        }
    }
}
