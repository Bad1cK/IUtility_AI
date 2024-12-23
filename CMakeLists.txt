# Минимальная версия CMake
cmake_minimum_required(VERSION 3.28)

# Название проекта
project(iUtile)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Добавляем флаги компилятора для macOS (если нужно)
if(APPLE)
    message(STATUS "Configuring for macOS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
    find_library(IOKIT_FRAMEWORK IOKit)
    list(APPEND EXTRA_LIBS ${COREFOUNDATION_FRAMEWORK} ${IOKIT_FRAMEWORK})
endif()

# Указываем исходные файлы
add_executable(iUtile
        main.cpp
        CPUUsage.cpp
        CPUUsage.h
        getDiskUsage.cpp
        getDiskUsage.h
        getMemoryUsage.cpp
        getMemoryUsage.h
        getTopCPUProcessesWithImpact.cpp
        getTopCPUProcessesWithImpact.h
        Battery.cpp
        Battery.h
)

# Подключение дополнительных библиотек для macOS
if(APPLE)
    target_link_libraries(iUtile ${EXTRA_LIBS})
endif()

# Дополнительные настройки
message(STATUS "CMake configuration completed for project: ${PROJECT_NAME}")
