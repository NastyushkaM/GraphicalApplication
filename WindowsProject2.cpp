#include <windows.h>

//типы фигур
enum FigureType {
    RECTANGLE, // Прямоугольник
    CIRCLE,    // Круг
    SQUARE,    // Квадрат
    TRIANGLE   // Треугольник
};

HINSTANCE hInst; // Дескриптор текущего экземпляра приложения
const wchar_t ClassName[] = L"MyWindowClass"; // Имя класса окна
HWND hwndSelect, hwndDraw; // Дескрипторы окон выбора и рисования
FigureType selectedFigure = RECTANGLE; // Выбранная фигура по умолчанию - прямоугольник

// Обработчик сообщений для окна выбора
LRESULT CALLBACK SelectWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: { // Обработка создания окна
        // Создание кнопки для выбора прямоугольника
        CreateWindow(L"BUTTON", L"Прямоугольник", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            10, 10, 300, 30, hwnd, (HMENU)RECTANGLE, hInst, NULL);
        // Создание кнопки для выбора круга
        CreateWindow(L"BUTTON", L"Круг", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            10, 40, 100, 30, hwnd, (HMENU)CIRCLE, hInst, NULL);
        // Создание кнопки для выбора квадрата
        CreateWindow(L"BUTTON", L"Квадрат", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            10, 70, 100, 30, hwnd, (HMENU)SQUARE, hInst, NULL);
        // Создание кнопки для выбора треугольника
        CreateWindow(L"BUTTON", L"Треугольник", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            10, 100, 300, 30, hwnd, (HMENU)TRIANGLE, hInst, NULL);
        break;
    }
    case WM_COMMAND: { // Обработка команды
        // Установка выбранной фигуры в зависимости от нажатой кнопки
        selectedFigure = (FigureType)LOWORD(wParam);
        // Перерисовка окна рисования
        InvalidateRect(hwndDraw, NULL, TRUE);
        break;
    }
    case WM_DESTROY: { // Обработка уничтожения окна
        // Завершение работы приложения
        PostQuitMessage(0);
        break;
    }
    default: {
        // Обработка всех остальных сообщений по умолчанию
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    }
    return 0;
}

// Обработчик сообщений для окна рисования
LRESULT CALLBACK DrawWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: { // Обработка перерисовки окна
        PAINTSTRUCT ps; // Структура для рисования
        HDC hdc = BeginPaint(hwnd, &ps); // Начало рисования

        HBRUSH hBrush = NULL; // Кисть для рисования
        POINT points[3]; // Массив точек для треугольника

        switch (selectedFigure) {
        case RECTANGLE:
            // Создание красной кисти для прямоугольника
            hBrush = CreateSolidBrush(RGB(255, 0, 0));
            SelectObject(hdc, hBrush); // Выбор кисти
            // Рисование прямоугольника
            Rectangle(hdc, 50, 50, 200, 150);
            break;
        case CIRCLE:
            // Создание синей кисти для круга
            hBrush = CreateSolidBrush(RGB(0, 0, 255));
            SelectObject(hdc, hBrush); // Выбор кисти
            // Рисование круга
            Ellipse(hdc, 50, 50, 200, 200);
            break;
        case SQUARE:
            // Создание зеленой кисти для квадрата
            hBrush = CreateSolidBrush(RGB(0, 255, 0));
            SelectObject(hdc, hBrush); // Выбор кисти
            // Рисование квадрата
            Rectangle(hdc, 50, 50, 150, 150);
            break;
        case TRIANGLE:
            // Создание желтой кисти для треугольника
            hBrush = CreateSolidBrush(RGB(255, 255, 0));
            SelectObject(hdc, hBrush); // Выбор кисти
            // Установка точек для треугольника
            points[0] = { 125, 50 };
            points[1] = { 50, 200 };
            points[2] = { 200, 200 };
            // Рисование треугольника
            Polygon(hdc, points, 3);
            break;
        default:
            break;
        }

        if (hBrush) {
            // Удаление кисти после использования
            DeleteObject(hBrush);
        }


        EndPaint(hwnd, &ps); // Завершение рисования
        break;
    }
    case WM_DESTROY: { // Обработка уничтожения окна
        // Завершение работы приложения
        PostQuitMessage(0);
        break;
    }
    default: {
        // Обработка всех остальных сообщений по умолчанию
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    }
    return 0;
}

// Регистрация классов окон
void RegisterWindowClass() {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, SelectWndProc, 0, 0,
                      hInst, LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
                      (HBRUSH)COLOR_WINDOW, NULL, ClassName, NULL };
    RegisterClassEx(&wc);

    wc.lpfnWndProc = DrawWndProc; // Установка обработчика для окна рисования
    wc.lpszClassName = L"DrawWindowClass"; // Имя класса для окна рисования
    RegisterClassEx(&wc); // Регистрация класса
}

// Создание окна с указанным заголовком и классом
HWND CreateWindowWithTitle(LPCWSTR title, LPCWSTR className) {
    return CreateWindow(className, title, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, hInst, NULL);
}

// Точка входа в приложение
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance; // Сохранение дескриптора экземпляра

    RegisterWindowClass(); // Регистрация классов окон

    // Создание окна выбора фигуры
    hwndSelect = CreateWindowWithTitle(L"Выбор фигуры", L"MyWindowClass");
    // Создание окна рисования фигуры
    hwndDraw = CreateWindowWithTitle(L"Рисование фигуры", L"DrawWindowClass");

    // Отображение окон
    ShowWindow(hwndSelect, nCmdShow);
    ShowWindow(hwndDraw, nCmdShow);

    // Обновление окон
    UpdateWindow(hwndSelect);
    UpdateWindow(hwndDraw);

    MSG msg; // Переменная для хранения сообщения
    // Цикл обработки сообщений
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam; // Возврат кода завершения
}
