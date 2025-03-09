# Calculator_qt_cpp 🖩

Калькулятор с графическим интерфейсом, реализованный на C++ и Qt.  
Проект создан в Qt Creator и демонстрирует:
- Работу с виджетами и слотами/сигналами
- Обработку математических выражений
- Динамическое обновление интерфейса

[[https://www.figma.com/file/gJIvIUNBfA1OqMXshPFocu/Calculator_Android?node-id=0%3A1]]

## ✨ Особенности
- Базовые операции: `+`, `-`, `*`, `/`
- Поддержка десятичных чисел и отрицательных значений
- Обработка ошибок (деление на ноль, неверный ввод)
- История последних вычислений
- Интерфейс, созданный в Qt Designer

## 🛠 Технические детали
### Структура проекта
├── CMakeLists.txt # Конфигурация сборки CMake

├── calculator.cpp # Логика вычислений (парсинг выражений)

├── calculator.h # Заголовочный файл класса Calculator

├── main.cpp # Точка входа в приложение

├── mainwindow.cpp # Реализация GUI (обработка кнопок)

├── mainwindow.h # Заголовочный файл MainWindow

└── mainwindow.ui # XML-описание интерфейса (Qt Designer)


## ⚙️ Установка
### Требования
- Qt 5.x/6.x
- Компилятор C++17 (GCC, Clang, MSVC)
- CMake ≥ 3.16
- Qt Creator (опционально)

### Сборка
```bash
git clone https://github.com/yourusername/Calculator_qt_cpp.git
cd Calculator_qt_cpp
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH="путь_к_вашей_установке_Qt"
make
./Calculator_qt_cpp
```

## 🖱 Использование
Вводите числа и операторы с помощью кнопок

Для вычисления нажмите =

C — сброс текущего ввода

← — удаление последнего символа

При ошибке появится сообщение "Error"
