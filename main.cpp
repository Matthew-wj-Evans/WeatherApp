#ifndef UNICODE
#define UNICODE
#endif

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <wingdi.h>
#include "strsafe.h"

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>


#include "./libraries/NetworkAPI.cpp"
#include "./libraries/ProjectConstants.cpp"
#include "./libraries/MapJsonToWeather.cpp"
#include "./libraries/Response.cpp"
#include "./classes/Weather.hpp"
#include "./libraries/JsonProcessor.cpp"


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

// Windows API Function headers
LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, INT nCmdShow);

// Support function headers
HWND PlaceWindow(WINDOW window, HWND hwnd, wchar_t* name, wchar_t* className);
WINDOW_SIZE GetCoordinates(int* xCount, int* yCount, int* row, int widthSpan = 1, int heightSpan = 1, bool isRowEnd = 1);
std::string GetApiKey();
bool PrintJsonToFile(std::string JsonResponse);
bool ParseJsonToWeatherObject(vector<char> jsonResponseString);

// Global variables
asio::error_code ec;
asio::io_context context;

// Window API methods
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

    // BAD NETWORKING CODE
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("37.139.20.5", ec), 80);
    asio::ip::tcp::socket socket(context);
    socket.connect(endpoint, ec);

    if (socket.is_open())
    {
        std::string requestString = "GET /data/2.5/weather?lat=37.39&lon=-122.08&appid=" + GetApiKey() +  " HTTP/1.1\r\n"
                                "Host: api.openweathermap.org\r\n"
                                "Connection: close\r\n\r\n";
        socket.write_some(asio::buffer(requestString.data(), requestString.size()), ec);

        socket.wait(socket.wait_read);

        size_t bytes = socket.available();

        if (bytes > 0)
        {
            std::vector<char> vBuffer(bytes);
            socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);

            const size_t cSize = strlen(vBuffer.data()) + 1;
            wchar_t* wcConverted = new wchar_t[cSize];
            mbstowcs(wcConverted, vBuffer.data(), cSize);

            //MessageBox(NULL, wcConverted, L"Weather API data", MB_OK);
            ParseJsonToWeatherObject(vBuffer);
        }
    }
    // END BAD NETWORKING CODE

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.message;
}

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
            int a = 2, b = 5;
            int r = a + b;
            wchar_t resultString;
            
            WINDOW labelOne;
            labelOne.SIZE = GetCoordinates(&xCount, &yCount, &row, CONST_DOUBLE, CONST_NORMAL, 0);
            labelOne.id = ID_STATIC_LABEL_ONE;
            
            if (!ec) {
                HWND staticLabelOne = PlaceWindow(labelOne, hwnd, new wchar_t[]{L"Connected!"}, new wchar_t[]{L"STATIC"});
            } else {
                HWND staticLabelOne = PlaceWindow(labelOne, hwnd, new wchar_t[]{L"Failed!"}, new wchar_t[]{L"STATIC"});
            }
        }
        break;
        case WM_COMMAND:
        {

        }
        break;
        case WM_NOTIFY:
        {

        }
        break;
        case WM_CTLCOLORSTATIC:
        {
            /*
            If a handle to a brush is not returned, then DefWindowProc will be returned instead, overwritting 
            the colours for static controls
            */
            int id = GetDlgCtrlID((HWND) lParam);

            if ( id > RANGE_IDS_STATIC_LABEL && id <= (RANGE_IDS_STATIC_LABEL + RANGE_IDS_GAP))
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
                // Can font 
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
            PostQuitMessage(0);
            return 0;
        }
        break;
        
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Support methods
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
    int controlWidth = (widthSpan == CONST_NORMAL ) ? CONTROL_WIDTH_NORMAL :
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

std::string GetApiKey()
{
    std::string filePath = "./donotpush/apikey.txt";
    std::string keyString;
    std::ifstream keyFile;
    keyFile.open(filePath);

    if (keyFile.is_open())
    {
        keyFile >> keyString;
    }
    keyFile.close();

    return keyString;
}

bool PrintJsonToFile(std::string JsonResponse)
{
    std::string JsonOutputfilePath = "./donotpush/jsonoutput.txt";
    std::ofstream file;
    file.open(JsonOutputfilePath);
    bool operationResult = false;

    if (!file.is_open()) {
        std::cerr << "Could not open file" << endl;
    } else {
        file << JsonResponse << endl;
        operationResult = true;
    }
    file.close();
    return operationResult;
}

/*
    JSON Parsing
 */
bool ParseJsonToWeatherObject(vector<char> jsonResponseChar) {
    Response response = GetResponseObject(jsonResponseChar);

    Weather weatherData = MapJsonToWeather::MapWeatherObject(response.JSON);
    
    return false;
}