// ScreenSaverRT.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://www.wischik.com/scr/holsavers.html
// https://learn.microsoft.com/en-US/troubleshoot/windows/win32/screen-saver-command-line

#pragma message("if you edit the vertex shader or pixel shader you GYATT to update the resource file unfortunately (i also could've just used Direct2D but i think D3D gives me the BEST performance POSSIBLE (pure speculation))")

#include <iostream>
#include <windows.h>
#include <string>

//__declspec(align(16)) //well i'll be (For ConstantBuffers, marked with the D3D11_BIND_CONSTANT_BUFFER BindFlag, the ByteWidth (value = 16004) must be a multiple of 16.)

//constexpr int circnumb = 1000;
#define MAX_CIRCLES 1000 //NO MATTER WHAT THEY TELL YOU 1 MILLION IS POSSIBLE!!!!

struct ConstantBuffer2 {
    float colors = 0;
    float x = 0;
    float y = 0;
    float z = 0;
    RECT circles[MAX_CIRCLES]{};
};

ConstantBuffer2 cb2data;

#include "Direct3D.h"

using namespace std;

#define print(msg) cout << msg << endl

//int refcount = 0;
//bool bigboobs = false;
//
//void Init() {
//    refcount++;
//    //bigboobs = RegLoadKey(HKEY_LOCAL_MACHINE, );
//    print("enter");
//}
//
//void Exit() {
//    refcount--; if (refcount != 0) return;
//    print("exit");
//}

HDC dc = 0;

float colors = 0;

//RECT circles[1000]{};

int ticks = 0;

Direct3D* direct3d = nullptr;

LRESULT SaverWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_CREATE) {
        SetTimer(hwnd, 1, 16, NULL);
        dc = GetDC(hwnd);
    }
    else if (msg == WM_TIMER) {
        if (GetAsyncKeyState(VK_ESCAPE)) {
            PostQuitMessage(0);
            //DestroyWindow(hwnd);
        }

        if (direct3d) {
            //direct3d->Clear(.5, 1, .5);
            POINT p; GetCursorPos(&p);
            RECT r; GetWindowRect(hwnd, &r);
            p = { p.x - r.left, p.y - r.top };
            direct3d->DrawShit();
            direct3d->Present();

            //    HBRUSH shit = (HBRUSH)SelectObject(dc, GetStockObject(DC_BRUSH));
            //    //POINT p{ 0 }; GetCursorPos(&p);
            //    //RECT r{p.x-128,p.y-128,p.x+128, p.y+128};
            //    //FillRect(dc, &r, NULL);
            //    //SelectObject(dc, shit);
            //    //HDC memDC = CreateCompatibleDC(dc);
            //    //HBRUSH last = (HBRUSH)SelectObject(memDC, GetStockObject(DC_BRUSH));
            for (RECT& circle : cb2data.circles) {
                circle.left += circle.right;
                circle.top += circle.bottom;
                if (circle.left > direct3d->width) {
                    circle.left = direct3d->width;
                    circle.right *= -1;
                }
                else if (circle.left < 0) {
                    circle.left = 0;
                    circle.right *= -1;
                }
                if (circle.top > direct3d->height) {
                    circle.top = direct3d->height;
                    circle.bottom *= -1;
                }
                else if (circle.top < 0) {
                    circle.top = 0;
                    circle.bottom *= -1;
                }
                //SetDCBrushColor(dc, RGB(colors, circle.top/4, circle.left/6));
                //RECT r{circle.left, circle.top, circle.left+30, circle.top+30};
                //FillRect(dc, &r, NULL);
            }
            //    //BitBlt(dc, 0, 0, 1920, 1080, memDC, 0, 0, SRCCOPY);
            //    //SelectObject(memDC, last);
            //    //DeleteDC(memDC);
            //    SelectObject(dc, shit);
            //
            cb2data.colors = (sin(ticks / 100.f) + 1) / 2;//*(255.0f/2);
            ticks++;
        }
    }else if(msg == WM_SETCURSOR) {
        SetCursor(NULL);
        return 0;
    }
    else if (msg == WM_DESTROY || msg == WM_KEYDOWN || msg == WM_CLOSE || msg == WM_SYSCOMMAND || msg == WM_KILLFOCUS || msg == WM_LBUTTONDOWN) {
        PostQuitMessage(0);
    }

    return DefWindowProc(hwnd, msg, wp, lp);
}

void SaverPrereq(HWND hwnd, bool realdeal) {
    //RUSSIAN_CHARSET
    for (int i = 0; i < MAX_CIRCLES; i++) {
        //cb2data.circles[i] = RECT{ rand() % 1250 + 50,rand() % 600 + 50, rand() % 2 == 0 ? 2 : -2, rand() % 2 == 0 ? 2 : -2 };
        cb2data.circles[i] = RECT{ rand() % 1920 ,rand() % 1080 , rand() % 2 == 0 ? 2 : -2, rand() % 2 == 0 ? 2 : -2 };
    }
    //bool worked = true;
    direct3d = new Direct3D(hwnd);//, worked);
    //if (!worked) {
    //    DestroyWindow(hwnd);
    //    return;
    //}
    UINT oldval;
    if(realdeal) SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, 1, &oldval, 0);
    //run screen saver some how
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    if (realdeal) SystemParametersInfo(SPI_SETSCREENSAVERRUNNING, 0, &oldval, 0);
}

//@template<class T>;
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    //Init();
    //MessageBoxA(NULL, lpCmdLine, "HELP", MB_OK); //https://learn.microsoft.com/en-US/troubleshoot/windows/win32/screen-saver-command-line
    WNDCLASSEX wc{ 0 };
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpfnWndProc = SaverWndProc;
    wc.cbWndExtra = 8; //what?
    wc.lpszClassName = TEXT("ScrClass");
    wc.cbSize = sizeof(WNDCLASSEX);
    RegisterClassEx(&wc);

    if (strlen(lpCmdLine) == 0) {
        //DialogBoxA(hInstance, );
        MessageBoxA(NULL, "ok you would normally do settings and shit but i don'\t got a dialog", lpCmdLine, MB_OK);
    }
    else if (strncmp("/p", lpCmdLine, 2) == 0) {
        HWND hparwnd = (HWND)stoi(string(lpCmdLine).substr(3));
        MessageBoxA(NULL, &(string("patching through ") + std::to_string((LONG_PTR)hparwnd))[0], "NIGGER", MB_OK);
        RECT rc; GetWindowRect(hparwnd, &rc);
        int w = rc.right - rc.left;
        int h = rc.bottom - rc.top;
        //int id = -1;
        
        SaverPrereq(CreateWindowEx(0, TEXT("ScrClass"), TEXT(""), WS_CHILD | WS_VISIBLE, 0, 0, w, h, hparwnd, NULL, hInstance, NULL)/*;&id); */ , false);
    }else {
        //SetCursor(NULL);
        /*HWND hthis = */SaverPrereq(CreateWindowEx(0, TEXT("ScrClass"), TEXT(""), WS_POPUP | WS_VISIBLE, 0, 0, 1920, 1080, NULL, NULL, hInstance, NULL), true);

        //for (int i = 0; i < 1000; i++) {
        //    circles[i] = RECT{ rand() % 1250 + 50,rand() % 600 + 50, rand() % 2 == 0 ? 2 : -2, rand() % 2 == 0 ? 2 : -2 };
        //}
        //
        //MSG msg;
        //while (GetMessage(&msg, NULL, 0, 0))
        //{
        //    TranslateMessage(&msg);
        //    DispatchMessage(&msg);
        //}
    }
    //Exit();
}