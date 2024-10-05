cmake_minimum_required(VERSION 3.10)

# Укажите имя проекта
project(EncoderApp)

# Укажите версию C++
set(CMAKE_CXX_STANDARD 14)

# Включите поддержку MFC
set(CMAKE_MFC_FLAG 2)  # Используйте MFC в проекте

# Укажите исходные файлы
set(SOURCES
    ../EncoderApp/EncoderApp.cpp
    ../EncoderApp/EncoderAppDlg.cpp
    ../EncoderApp/EncoderModel.cpp
    ../EncoderApp/EncoderController.cpp
)

# Укажите исполняемый файл
add_executable(EncoderApp ${SOURCES})


# Для MFC вам может понадобиться указать конкретный тип приложения
# Например, если это приложение с диалогами:
set_target_properties(EncoderApp PROPERTIES
    WIN32_EXECUTABLE TRUE
    MFC_USE_STATIC TRUE  # Использовать статическую библиотеку MFC (по желанию)
    _AFXDLL TRUE
)

# Присоедините MFC
target_link_libraries(EncoderApp PRIVATE ${MFC_LIBRARIES})