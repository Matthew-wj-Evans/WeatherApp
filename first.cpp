#ifndef UNICODE
#define UNICODE
#endif

#define BUFFER_WEATHER_API 2048

#define CONST_NORMAL 1
#define CONST_DOUBLE 2
#define CONST_TRIPLE 3

#define WINDOW_CLASS_STATIC L"STATIC"
#define WINDOW_CLASS_EDIT L"EDIT"

#define WIN_TYPE_LABEL 0
#define WIN_TYPE_ERROR 1
#define WIN_TYPE_HEADER 2
#define WIN_TYPE_EDIT 3

#define MAIN_WINDOW_WIDTH 500
#define MAIN_WINDOW_HEIGHT 600 

#define CONTROL_POSITION_ROW_COUNT 3
#define CONTROL_POSITION_X_DEFAULT 50
#define CONTROL_POSITION_Y_DEFAULT 50
#define CONTROL_GAP_XY 5

#define CONTROL_WIDTH_NORMAL 100
#define CONTROL_WIDTH_DOUBLE (CONTROL_WIDTH_NORMAL * 2) + CONTROL_GAP_XY
#define CONTROL_WIDTH_TRIPLE (CONTROL_WIDTH_NORMAL * 3) + CONTROL_GAP_XY 

#define CONTROL_HEIGHT_NORMAL 20
#define CONTROL_HEIGHT_DOUBLE (CONTROL_HEIGHT_NORMAL * 2) + CONTROL_GAP_XY
#define CONTROL_HEIGHT_TRIPLE (CONTROL_HEIGHT_NORMAL * 3) + CONTROL_GAP_XY

// META Id Info
#define RANGE_IDS_GAP 99
#define RANGE_IDS_STATIC_LABEL 100
#define RANGE_IDS_STATIC_ERROR 200
#define RANGE_IDS_STATIC_HEADER 300
#define RANGE_IDS_EDIT 400
#define RANGE_IDS_BUTTON 500
// Control Ids
#define ID_STATIC_LABEL_ONE 101
#define ID_STATIC_LABEL_TWO 102
#define ID_STATIC_ERROR_ONE 201
#define ID_STATIC_ERROR_TWO 202
#define ID_STATIC_HEADER_ONE 301
#define ID_STATIC_HEADER_TWO 302

#include<iostream>
#include<string>
#include<windows.h>
#include<wingdi.h>
#include "strsafe.h"

using namespace std;

struct WINDOW_SIZE {
    int xStart;
    int yStart;
    int width;
    int height;
};

struct WINDOW {
    int id;
    WINDOW_SIZE SIZE;
};



HWND PlaceWindow(WINDOW window, HWND hwnd, wchar_t* name, wchar_t* className);
WINDOW_SIZE GetCoordinates(int* xCount, int* yCount, int* row, int widthSpan = 1, int heightSpan = 1, bool isRowEnd = 1);

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            
            int xCount = 0, yCount = 0, row = xCount % CONTROL_POSITION_ROW_COUNT;
            /* Static Labels */

            // HWND staticLabelOne = CreateWindowW(WINDOW_CLASS_STATIC, L"Label One",
            //     WS_VISIBLE | WS_CHILD,
            //         CONTROL_POSITION_X_DEFAULT + ((CONTROL_WIDTH_NORMAL + CONTROL_GAP_XY) * row ), 
            //         CONTROL_POSITION_Y_DEFAULT + ( (CONTROL_HEIGHT_NORMAL + CONTROL_GAP_XY) * yCount), 
            //         CONTROL_WIDTH_NORMAL, CONTROL_HEIGHT_NORMAL,
            //     hwnd, (HMENU) ID_STATIC_LABEL_ONE, NULL, NULL
            // );
            // ...better?
            WINDOW labelOne;
            labelOne.SIZE = GetCoordinates(&xCount, &yCount, &row, CONST_DOUBLE, CONST_NORMAL, 0);
            labelOne.id = ID_STATIC_LABEL_ONE;
            HWND staticLabelOne = PlaceWindow(labelOne, hwnd, new wchar_t[]{L"Label One"}, new wchar_t[]{L"STATIC"});

            if (staticLabelOne == NULL)
            {
                MessageBox(NULL, L"Static Label One creation failed.", L"Error", MB_ICONEXCLAMATION);
                exit(EXIT_FAILURE);
            }
        }
        break;
        case WM_COMMAND:
        {}
        break;
        case WM_NOTIFY:
        {}
        break;
        case WM_CTLCOLORSTATIC:
        {
            /*
            If a handle to a brush is not returned, then DefWindowProc will be returned instead, overwritting 
            the colours
            */
            SetBkMode((HDC)wParam, TRANSPARENT);
            int id = GetDlgCtrlID((HWND) lParam);
            /*
                HOW TO:
                    Get the Id to the evenWindow
            */

            if (id > RANGE_IDS_STATIC_LABEL && id <= (RANGE_IDS_STATIC_LABEL + RANGE_IDS_GAP))
            {
                SetTextColor((HDC) wParam, RGB(0,0,0));
                return (INT_PTR)CreateSolidBrush(RGB(255,255,255));
            } 
            else if (id > RANGE_IDS_STATIC_ERROR && id <= (RANGE_IDS_STATIC_ERROR + RANGE_IDS_GAP))
            {
                SetTextColor((HDC) wParam, RGB(128,0,0));
                return (INT_PTR)CreateSolidBrush(RGB(192,192,192));
            }
            else if (id > RANGE_IDS_STATIC_HEADER && id <= (RANGE_IDS_STATIC_HEADER + RANGE_IDS_GAP)) 
            {
                SetTextColor((HDC) wParam, RGB(0,0,0));
                return (INT_PTR)CreateSolidBrush(RGB(128,128,128));
            }
            SetTextColor((HDC) wParam, RGB(128,0,255));
            return (INT_PTR)CreateSolidBrush(RGB(64, 64, 64));
        }
        break;
        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
            return 0;
        }
        break;
        case WM_DESTROY:
        {
            
            return 0;
        }
        break;
        
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, INT nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;
    const wchar_t ClassName[] = L"Main_Window";

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = MainWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = ClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed", L"Error", MB_ICONEXCLAMATION | MB_OK);
        exit(EXIT_FAILURE);
    }

    hwnd = CreateWindowEx(WS_EX_APPWINDOW, ClassName, L"Window", 
        WS_EX_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, 
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed.", L"Error", MB_ICONEXCLAMATION | MB_OK);
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.message;
}

HWND PlaceWindow(WINDOW window, HWND hwnd, wchar_t name[], wchar_t className[])
{
    return CreateWindowW(
            className, name, WS_VISIBLE | WS_CHILD,
            window.SIZE.xStart, window.SIZE.yStart, window.SIZE.width, window.SIZE.height, 
            hwnd, (HMENU) ((INT_PTR)window.id), NULL, NULL
        );
}

/*
    yCount = used to derive the y-start value
    row = result of a modulus operation, used to derive the x-start value. repeating values (0, 1, 2, ...N, 0, 1, 2, ...N, ... )
    widthSpan = used to modify a control's width by either double or triple the standard; DEFAULTED TO 1
    heightSpan = used to modfy a control's height by either double or triple the standard; DEFAULTED TO 1
    isRowEnd = used to 'peek ahead', allows side-by-side placement by not incrementing yCount when FALSE; DEFAULTED TO TRUE
    [REMOVED] xCount = pointless? Row replaces it
    [REMOVED] controlType = TODO: Headers will natively want a larger window size than a static-label type
*/
WINDOW_SIZE GetCoordinates(int* xCount, int* yCount, int* row, int widthSpan, int heightSpan, bool isRowEnd)
{
    WINDOW_SIZE windowSizing;

    
    // If widthSpan == 1 -> Normal; Else/If heightSpan == 2 -> Double; Else heightspan == 3 -> Triple
    int controlWidth = (widthSpan == CONST_NORMAL) ? CONTROL_WIDTH_NORMAL :
        (widthSpan == CONST_DOUBLE) ? CONTROL_WIDTH_DOUBLE : CONTROL_HEIGHT_TRIPLE;

    // If heightSpan == 1 -> Normal; Else/If heightSpan == 2 -> Double; Else heightspan == 3 -> Triple
    int controlHeight = (heightSpan == CONST_NORMAL) ? CONTROL_HEIGHT_NORMAL :
        (heightSpan == CONST_DOUBLE) ? CONTROL_HEIGHT_DOUBLE : CONTROL_HEIGHT_TRIPLE;
    
    
    int xLocation = CONTROL_POSITION_X_DEFAULT + ( (*row) * (CONTROL_WIDTH_NORMAL + CONTROL_GAP_XY));
    int yLocation = CONTROL_POSITION_Y_DEFAULT + ( (*yCount) * (CONTROL_HEIGHT_NORMAL + CONTROL_GAP_XY));

    *xCount = *xCount + widthSpan;
    *row = (*xCount) % CONTROL_POSITION_ROW_COUNT;
    if (isRowEnd) 
    {
        *yCount += heightSpan;
    }

    windowSizing.xStart = xLocation;
    windowSizing.yStart = yLocation;
    windowSizing.width = controlWidth;
    windowSizing.height = controlHeight;

    return windowSizing;
}