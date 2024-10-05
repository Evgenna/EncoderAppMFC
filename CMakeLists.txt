cmake_minimum_required(VERSION 3.10)

# Задаем название проекта
project(EncoderApp)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 14)

# Определяем макрос для поддержки MFC
add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 1)

# Укажите исходные файлы
set(SOURCES
    ../EncoderApp/EncoderController.cpp
    ../EncoderApp/EncoderController.h
    ../EncoderApp/EncoderModel.cpp
    ../EncoderApp/EncoderModel.h
    ../EncoderApp/EncoderApp.cpp
    ../EncoderApp/EncoderAppDlg.cpp
    ../EncoderApp/pch.cpp
    ../EncoderApp/EncoderApp.h
    ../EncoderApp/EncoderAppDlg.h
    ../EncoderApp/pch.h
    ../EncoderApp/Resource.h
    ../EncoderApp/targetver.h
    ../EncoderApp/EncoderApp.rc
)

# Укажите исполняемый файл
add_executable(EncoderApp WIN32 ${SOURCES})

# Указываем, что это Windows-приложение и, при необходимости, используем статическую библиотеку MFC
set_target_properties(EncoderApp PROPERTIES
    WIN32_EXECUTABLE TRUE
    MFC_USE_STATIC TRUE  # Использовать статическую библиотеку MFC (по желанию)
)

# Присоедините MFC (здесь можно использовать target_link_libraries, если необходимо)
target_link_libraries(EncoderApp PRIVATE ${MFC_LIBRARIES})

# Если вы используете ресурсы, убедитесь, что CMake знает о файле ресурсов
set_source_files_properties(../EncoderApp.rc PROPERTIES LANGUAGE RC)
