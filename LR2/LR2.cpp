#include <windows.h>
#include <fstream>
#include <math.h>

const wchar_t windowClass[] = L"win32app";
const wchar_t windowTitle[] = L"Лабораторная работа 2";
UINT_PTR timerID = 0;
HPEN pen, oldPen;
HBRUSH brush, oldBrush;
RECT r;
int x, y, xLast, yLast;
int Periods = 5;
int size_x = 0;
int size_y = 0;

//xLast и yLast нужны для того чтобы вписывать в них значения последнего местонахождения 
                          //объекта и при выходе за границы, возвращать объект туда

void Line(HDC hdc, int x1, int y1, int x2, int y2) //адекватная функция рисования линий
{
    MoveToEx(hdc, x1, y1, NULL); //сделать текущими координаты x1, y1
    LineTo(hdc, x2, y2);
}

void drawGrid(HDC hdc)
{
    pen = CreatePen(PS_USERSTYLE, 2, RGB(99, 97, 90));
    oldPen = (HPEN)SelectObject(hdc, pen);
    //LineTo(hdc, 1000, 0);


    for (int i = 0; i < 2000; i++)
    {
        if (i % 40 == 0)
        {
            Line(hdc, 0, i, 2000, i);
            Line(hdc, i, 0, i, 2000);
        }
    }


    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void drawTopology(HDC hdc)
{
    //обработчик чтобы не выйти за пределы поля
    if (x + 740 >= 1210 || y + 605 >= 765 || x + 570 < 570 || y + 605 >=765)//последнее условие нужно чтобы не залезть на график
    {
        x = xLast;
        y = yLast;
    }

    //рисуем
    pen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
    brush = CreateSolidBrush(RGB(0, 0, 0));
    oldPen = (HPEN)SelectObject(hdc, pen);
    oldBrush = (HBRUSH)SelectObject(hdc, brush);

    pen = CreatePen(PS_SOLID, 1, RGB(93, 194, 68));//рисуем все зеленые элементы
    brush = CreateSolidBrush(RGB(93, 194, 68));
    oldBrush = (HBRUSH)SelectObject(hdc, brush);
    oldPen = (HPEN)SelectObject(hdc, pen);
    Rectangle(hdc, x + 550, y + 75, x + 1045, y + 250);
    Rectangle(hdc, x + 870, y + 250, x + 1045, y + 380);
    Rectangle(hdc, x + 550, y + 520, x + 1045, y + 575);
    Rectangle(hdc, x + 870, y + 440, x + 1045, y + 575);

    pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));//рисуем все красные элементы
    brush = CreateSolidBrush(RGB(255, 0, 0));
    oldBrush = (HBRUSH)SelectObject(hdc, brush);
    oldPen = (HPEN)SelectObject(hdc, pen);
    Rectangle(hdc, x +900, y + 100, x + 930, y + 600);
    Rectangle(hdc, x +750, y + 100, x + 780, y + 520);
    Rectangle(hdc, x +600, y + 100, x + 630, y + 520);

    pen = CreatePen(PS_SOLID, 1, RGB(19, 89, 214));//рисуем все синие элементы
    brush = CreateSolidBrush(RGB(19, 89, 214));
    oldBrush = (HBRUSH)SelectObject(hdc, brush);
    oldPen = (HPEN)SelectObject(hdc, pen);
    Rectangle(hdc, x + 500, y + 35, x + 1070, y + 100);
    Rectangle(hdc, x + 1010, y + 130, x + 1045, y + 500);
    Rectangle(hdc, x + 550, y + 160, x + 580, y + 310);//толщина палки 30
    Rectangle(hdc, x + 540, y + 520, x + 570, y + 640);
    Rectangle(hdc, x + 500, y + 575, x + 1070, y + 640);
    Rectangle(hdc, x + 550, y + 310, x + 880, y + 340);
    Rectangle(hdc, x + 880, y + 310, x + 910, y + 480);
    Rectangle(hdc, x + 690, y + 450, x + 910, y + 480);
    Rectangle(hdc, x + 690, y + 450, x + 730, y + 550);
    Rectangle(hdc, x + 725, y + 360, x + 780, y + 420);
    Rectangle(hdc, x + 575, y + 360, x + 630, y + 420);

    SelectObject(hdc, oldPen);
    DeleteObject(pen);
    SelectObject(hdc, oldBrush);
    DeleteObject(brush);

    xLast = x;
    yLast = y;
}

void drawPlot(HDC hdc)
{
    //область для графика
    pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    brush = CreateSolidBrush(RGB(255, 255, 255));
    oldBrush = (HBRUSH)SelectObject(hdc, brush);
    oldPen = (HPEN)SelectObject(hdc, pen);
    Rectangle(hdc, 0, 2*size_y/3, size_x/3, size_y); //+-(1/9) от всей области



    oldBrush = (HBRUSH)SelectObject(hdc, brush);
    oldPen = (HPEN)SelectObject(hdc, pen);

    //Рисуем оси координат
    Line(hdc, 0, 2*size_y/3+size_y/6, size_x/3, 2*size_y/3+size_y/6);//ось X

    double period = 2.0 * 3.14159 * Periods; //Periods - число периодов, регулируется кнопками '+' и '-'(Periods == 5 умолчанию)
    double step = Periods * 3.14159 / (size_x/3);

    for (double i = 0; i <= size_x/3; i+=0.1)
    {
        double y = cos(i * step); // Вычисление косинуса угла
        int pixel = size_y / 6 - (y * size_y / 12) + 2 * size_y / 3;

        // Отображение точек на графике
        SetPixel(hdc, i, pixel, RGB(255, 0, 0));
    }
}

long __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{

    PAINTSTRUCT ps;
    HDC hdc;

    GetClientRect(hWnd, &r);

    size_x = r.right - r.left;
    size_y = r.bottom - r.top;

    std::ofstream file;
    switch (message) 
    {
    case WM_DESTROY:
        //при закрытии программы сохраняем данные в конфиг
        file.open("config.txt");
        file << Periods<<"\n";
        file << x<<"\n";
        file << y<<"\n";

        PostQuitMessage(0);
        break;

    case WM_LBUTTONDOWN:
            if (wParam & MK_CONTROL)
            {
                x = LOWORD(lParam) - 500;
                y = HIWORD(lParam) - 35;

                InvalidateRect(hWnd, nullptr, true);
            }
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        drawGrid(hdc);
        TextOut(hdc, 0, 0, L"Топология элемента OR2", 22);
        drawTopology(hdc);
        drawPlot(hdc);
       
        EndPaint(hWnd, &ps);
        break;

    case WM_KEYDOWN:
        switch (LOWORD(wParam))
        {
        case VK_OEM_PLUS:
            if (Periods < 30) {
                Periods += 1;
                InvalidateRect(hWnd, nullptr, true);
            }
            break;
        case VK_OEM_MINUS:
            if (Periods > 2) {
                Periods -= 1;
                InvalidateRect(hWnd, nullptr, true);
            }
            break;
        default:
            break;
        }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //открываем файл конфига и считываем его
    std::ifstream file;
    file.open("config.txt");
    file >> Periods;
    file >> x;
    file >> y;
    file.close();

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 3);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = windowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wcex)) 
    {
        MessageBox(NULL, L"Can’t register window class!", L"Win32 API Test", NULL);
        return 1;
    }

    HWND hWnd = CreateWindow(windowClass, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

    if (!hWnd) 
    {
        MessageBox(NULL, L"Can’t create window!", L"Win32 API Test", NULL);
        return 1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
