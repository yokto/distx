#include <zwolf.h>
#include "xgui_windows.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef uintptr_t WPARAM;
typedef intptr_t LPARAM;
typedef void* HWND;
typedef void* HINSTANCE;
typedef void* HICON;
typedef void* HCURSOR;
typedef void* HMENU;
typedef void* HBRUSH;
typedef void* HDC;
typedef char* LPCSTR;
typedef uint32_t UINT;
typedef int32_t LONG;
typedef int32_t BOOL;
typedef uint8_t BYTE;
typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef void VOID;
typedef void* LPVOID;
typedef int LRESULT;
typedef uint16_t ATOM;
#define WS_OVERLAPPEDWINDOW 0xcf0000
#define CW_USEDEFAULT 0x80000000
#define DIB_RGB_COLORS 0

#define WM_PAINT 0xf
#define WM_DESTROY 0x2

#if defined __x86_64__
#define CALLBACK __attribute((ms_abi))
#elif defined __x86_32__
#define CALLBACK __attribute((stdcall))
#else
#define CALLBACK /* empty */
#endif

typedef int (*WNDPROC)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) CALLBACK;
typedef struct tagWNDCLASSA {
  uint32_t      style;
  WNDPROC   lpfnWndProc;
  int       cbClsExtra;
  int       cbWndExtra;
  HINSTANCE hInstance;
  HICON     hIcon;
  HCURSOR   hCursor;
  HBRUSH    hbrBackground;
  LPCSTR    lpszMenuName;
  LPCSTR    lpszClassName;
} WNDCLASSA, *PWNDCLASSA, *NPWNDCLASSA, *LPWNDCLASSA;

typedef struct tagPOINT {
  LONG x;
  LONG y;
} POINT,*PPOINT,*NPPOINT,*LPPOINT;
typedef struct tagRECT {
  LONG left;
  LONG top;
  LONG right;
  LONG bottom;
} RECT, *PRECT, *NPRECT, *LPRECT;

typedef struct tagPAINTSTRUCT {
  HDC  hdc;
  BOOL fErase;
  RECT rcPaint;
  BOOL fRestore;
  BOOL fIncUpdate;
  BYTE rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT, *NPPAINTSTRUCT, *LPPAINTSTRUCT;

typedef struct tagBITMAPINFOHEADER {
  DWORD biSize;
  LONG  biWidth;
  LONG  biHeight;
  WORD  biPlanes;
  WORD  biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG  biXPelsPerMeter;
  LONG  biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagRGBQUAD {
  BYTE rgbBlue;
  BYTE rgbGreen;
  BYTE rgbRed;
  BYTE rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO {
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD          bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO, *PBITMAPINFO;

typedef struct tagMSG {
  HWND hwnd;
  UINT message;
  WPARAM wParam;
  LPARAM lParam;
  DWORD time;
  POINT pt;
} MSG,*PMSG,*NPMSG,*LPMSG;


void* (*GetModuleHandleA)(char* moduleName) __attribute((ms_abi));
HWND (*CreateWindowExA)(
  DWORD     dwExStyle,
  LPCSTR    lpClassName,
  LPCSTR    lpWindowName,
  DWORD     dwStyle,
  int       X,
  int       Y,
  int       nWidth,
  int       nHeight,
  HWND      hWndParent,
  HMENU     hMenu,
  HINSTANCE hInstance,
  LPVOID    lpParam
) __attribute((ms_abi));
BOOL (*ShowWindow)(
  HWND hWnd,
  int  nCmdShow
) __attribute((ms_abi)) = 0;
BOOL (*UpdateWindow)(
  HWND hWnd
) __attribute((ms_abi)) = 0;

HDC (*BeginPaint)(HWND hWnd, LPPAINTSTRUCT lpPaint) __attribute((ms_abi)) __attribute((ms_abi)) = 0;
BOOL (*EndPaint)(HWND hWnd, const PAINTSTRUCT *lpPaint) __attribute((ms_abi)) __attribute((ms_abi)) = 0;
int (*SetDIBitsToDevice)(
  HDC              hdc,
  int              xDest,
  int              yDest,
  DWORD            w,
  DWORD            h,
  int              xSrc,
  int              ySrc,
  UINT             StartScan,
  UINT             cLines,
  const VOID       *lpvBits,
  const BITMAPINFO *lpbmi,
  UINT             ColorUse
) __attribute((ms_abi)) = 0;
LRESULT (*DefWindowProcA)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) __attribute((ms_abi)) = 0;
BOOL (*InvalidateRect)(HWND hWnd, const RECT *lpRect, BOOL bErase) __attribute((ms_abi)) = 0;
ATOM (*RegisterClassA)(const WNDCLASSA *lpWndClass) __attribute((ms_abi)) = 0;
BOOL (*GetMessageA)(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax) __attribute((ms_abi)) = 0;
BOOL (*TranslateMessage)(const MSG *lpMsg) __attribute((ms_abi)) = 0;
LRESULT (*DispatchMessageA)(const MSG *lpMsg) __attribute((ms_abi)) = 0;

// Global variables
HINSTANCE hInst = 0;
void* hWnd = 0;
int width = 800;
int height = 600;
unsigned int* pixels = 0;

// Function declarations
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void UpdatePixels();

// Window procedure
int CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    printf("message %x\n", message);
    switch (message) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Draw pixels from the buffer
            BITMAPINFO bmi;
            memset(&bmi, 0, sizeof(BITMAPINFO));
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = width;
            bmi.bmiHeader.biHeight = -height;  // Negative height for top-down DIB
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;     // 32 bits per pixel (ARGB)

	    printf("f %p, hdc %p, p0 %x, s1 = %x, s2 = %x, s3 = %x\n", SetDIBitsToDevice, hdc, pixels[0], sizeof(BITMAPINFO), sizeof(BITMAPINFOHEADER), sizeof(WNDCLASSA));
            SetDIBitsToDevice(hdc, 0, 0, width, height, 0, 0, 0, height, pixels, &bmi, DIB_RGB_COLORS);

            EndPaint(hwnd, &ps);
            break;
        }

        case WM_DESTROY:
            exit(0);
            break;

        default:
            return DefWindowProcA(hwnd, message, wParam, lParam);
    }

    return 0;
}

// Function to update pixel buffer
void UpdatePixels() {
    // Set all pixels to red (ARGB format)
    for (int i = 0; i < width * height; ++i) {
        pixels[i] = 0xFFFF0000;  // Red
	if (i < 12000) {
        pixels[i] = 0xFF00FF00;  // Red
	}
    }

    // Force window to repaint
    InvalidateRect(hWnd, NULL, true);
}

void init_win(void* libc, void* kernel32, void* user32, void* gdi32) {
    zwolf_write("init windows");
    GetModuleHandleA = zwolf_sym(kernel32, "GetModuleHandleA");
    CreateWindowExA = zwolf_sym(user32, "CreateWindowExA");
    ShowWindow = zwolf_sym(user32, "ShowWindow");
    UpdateWindow = zwolf_sym(user32, "UpdateWindow");
    BeginPaint = zwolf_sym(user32, "BeginPaint");
    EndPaint = zwolf_sym(user32, "EndPaint");
    SetDIBitsToDevice = zwolf_sym(gdi32, "SetDIBitsToDevice");
    DefWindowProcA = zwolf_sym(user32, "DefWindowProcA");
    InvalidateRect = zwolf_sym(user32, "InvalidateRect");
    RegisterClassA = zwolf_sym(user32, "RegisterClassA");
    GetMessageA = zwolf_sym(user32, "GetMessageA");
    TranslateMessage = zwolf_sym(user32, "TranslateMessage");
    DispatchMessageA = zwolf_sym(user32, "DispatchMessageA");
    hInst = GetModuleHandleA(0);
    printf("hInst %p\n", hInst);

    // Allocate memory for pixel buffer
    pixels = (unsigned int*)malloc(width * height * sizeof(unsigned int));
    UpdatePixels();

    // Initialize the window class
    char* CLASS_NAME = "DirectRenderingClass";
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;
    //uintptr_t COLOR_BACKGROUND=1;
    //wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

    if (!RegisterClassA(&wc)) {
	fprintf(stderr, "Window Registration Failed!");
	exit(0);
    }

    printf("reg %p, create %p, ws_overlap %x, cw_default %x\n", &RegisterClassA, &CreateWindowExA, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT);

    // Create the window
    hWnd = CreateWindowExA(
	0,
        CLASS_NAME,
        "Direct Rendering Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, hInst, NULL
    );

    if (hWnd == NULL) {
	fprintf(stderr, "Window Creation Failed!");
	exit(0);
    }

    // Show and update the window
    ShowWindow(hWnd, 1);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    // Release allocated memory
    free(pixels);

    return; //msg.wParam;
}

