// Saper.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Saper.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>




// zamiast niepoprawnych:

#define MAX_LOADSTRING 100
#define WIN_SIZE 25

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))
// Global Variables:
int BOMB_COUNTER = 10;
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HANDLE isRead;
const int maxX = 80;
const int maxY = 40;
int x = 10;
int y = 10;
int color[maxX*maxY]; // 0 - gray 1 - mine 2 - neighbpur 3 - flag
int arr[maxX*maxY];
int indexArr[maxX*maxY];
int howManyMines[maxX*maxY];
int mines;// = BOMB_COUNTER;
int mineCounter;
int mineAndFlag = 0;
HWND hWnd2[maxX*maxY];
HWND hGray[maxX*maxY];
int flagPaintFlag = 0;
int timeFlag = 0;
HWND h;
HWND hWndP;
int Parent = 1;
int gameTime = 0;
int cmd;
int debug = 0;
struct data
{
	RECT r;
	int cmd;
};
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void fillRandomBombs(/*int (&arr)[x*y]*/);
void fillNeighbours(/*int(&howMany)[x*y]*/);
void clickOnEmptyField(int i);
void disabledChildren();
void newGame();
BOOL CALLBACK DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SAPER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SAPER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SAPER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SAPER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
void initGame()
{	
	fillRandomBombs();
	fillNeighbours();
	mines = BOMB_COUNTER;
	RECT wr = { 0, 0, WIN_SIZE*x, WIN_SIZE * (y+1) };  //zmiana
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, true);
	data *dat = new data{ wr,cmd };
	mineCounter = BOMB_COUNTER;
	hWndP = CreateWindow(szWindowClass, szTitle, (WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_CLIPSIBLINGS) & (~WS_THICKFRAME) & (~WS_MAXIMIZEBOX),
		CW_USEDEFAULT, 0, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, hInst, (LPVOID)dat);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   
   cmd = nCmdShow;
   /*
   fillRandomBombs(arr);
   fillNeighbours(howManyMines);
   RECT wr = { 0, 0, WIN_SIZE*x, WIN_SIZE * y };
   AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, true);
   data *dat = new data{ wr,nCmdShow };*/
   //hWndP = CreateWindow(szWindowClass, szTitle, (WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_CLIPSIBLINGS) /*& (~WS_THICKFRAME)*/,
	//   CW_USEDEFAULT, 0, wr.right-wr.left, wr.bottom - wr.top, nullptr, nullptr, hInstance, (LPVOID)dat);
   initGame();

   //auto v = CreateEllipticRgn(2, 2, 10, 10);
   //howWindow(hWnd, 0);
   //h = CreateWindow(szWindowClass, szTitle, (~WS_THICKFRAME) & (WS_VISIBLE | WS_CHILD | WS_BORDER | WS_DISABLED),
	  // /*CW_USEDEFAULT-*/0, 0, wr.right - wr.left, WIN_SIZE, hWnd, nullptr, hInstance, NULL);
   ////EnableWindow(h, FALSE);
   //for (int i = 0; i < x; i++) {
	  // for (int j = 0; j < y; j++) {	   
		 //  hWnd2[10*j+i] = CreateWindow(szWindowClass, nullptr, (~WS_THICKFRAME) & (WS_VISIBLE | WS_CHILD | WS_BORDER),
			//   i * WIN_SIZE, (j+1) * WIN_SIZE, WIN_SIZE, WIN_SIZE, hWnd, NULL, hInstance, (LPVOID)arr[10 * j + i]);
		 //  ShowWindow(hWnd2[10 * j + i], nCmdShow);
		 //  //UpdateWindow(hWnd);
	  // } 
   //}
   
   
   if (!hWndP)
   {
      return FALSE;
   }

   ShowWindow(hWndP, nCmdShow);
   UpdateWindow(hWndP);
   
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER:
	{
		gameTime++;
		//timeFlag = 1;
		InvalidateRect(h, NULL, TRUE);
	}
	break;
	case WM_CREATE:
		if (Parent == 1)
		{
			Parent = 0;
			//SetTimer(hWnd, 7, 250, NULL);
			data* d = (data*)lParam;
			RECT wr = d->r;

			int cmd = d->cmd;

			h = CreateWindow(szWindowClass, szTitle, (~WS_THICKFRAME) & (WS_VISIBLE | WS_CHILD | WS_BORDER),
				0, 0, WIN_SIZE*x, WIN_SIZE, hWnd, nullptr, hInst, NULL);

			ShowWindow(h, cmd);
			//EnableWindow(h, FALSE);
			for (int j = 0; j < y; j++)//for (int i = 0; i < x; i++)
			{
				for (int i = 0; i < x; i++)//for (int j = 0; j < y; j++)
				{
					hWnd2[x * j + i] = CreateWindow(szWindowClass, nullptr, (~WS_THICKFRAME) & (WS_VISIBLE | WS_CHILD | WS_BORDER),
						i * WIN_SIZE, (j + 1) * WIN_SIZE, WIN_SIZE, WIN_SIZE, hWnd, NULL, hInst, NULL);
					indexArr[x * j + i] = x * j + i;
					color[x * j + i] = 0;
					SetProp(hWnd2[x * j + i], L"color", &color[x * j + i]);
					SetProp(hWnd2[x * j + i], L"isMine", &arr[x * j + i]);
					SetProp(hWnd2[x * j + i], L"Neighbour", &howManyMines[x * j + i]);
					SetProp(hWnd2[x * j + i], L"idx", &indexArr[x * j + i]);

					//InvalidateRect(hWnd2[10 * j + i], NULL, TRUE);
					/*hWnd2[y * i + j] = CreateWindow(szWindowClass, nullptr, (~WS_THICKFRAME) & (WS_VISIBLE | WS_CHILD | WS_BORDER),
						i * WIN_SIZE, (j + 1) * WIN_SIZE, WIN_SIZE, WIN_SIZE, hWnd, NULL, hInst, NULL);
					indexArr[y * i + j] = y * i + j;
					color[y * i + j] = 0;
					SetProp(hWnd2[y * i + j], L"color", &color[y * i + j]);
					SetProp(hWnd2[y * i + j], L"isMine", &arr[y * i + j]);
					SetProp(hWnd2[y * i + j], L"Neighbour", &howManyMines[y * i + j]);
					SetProp(hWnd2[y * i + j], L"idx", &indexArr[y * i + j]);*/
				}
			}
		}

		break;
	case WM_RBUTTONDOWN:
		/*CloseWindow(hWnd);
		DeleteObject(hWnd);*/
		//{
		if(gameTime == 0) SetTimer(hWnd, 7, 1000, NULL);
		if (hWnd != h && hWnd != hWndP)   // 0 - gray   1 - mine   2 - neighbpur   3 - flag  4-white
		{
			int* isMine = (int*)GetProp(hWnd, L"isMine"); //
			int* color = (int*)GetProp(hWnd, L"color"); //
			if (*color == 0) 
			{ 
				int *v1=new int; 
				*v1 = 3; 
				SetProp(hWnd, L"color", v1); 
				if (*isMine == 1) mineAndFlag++;
				mines--;
				if(mineAndFlag == BOMB_COUNTER) MessageBox(hWndP, L"SUCCESS!!", L"Game", MB_SYSTEMMODAL);
				UpdateWindow(h);
			}// from gray to flag
			if (*color == 3)
			{
				int *v = new int;
				*v = 0; 
				SetProp(hWnd, L"color", v);
				if (*isMine == 1) mineAndFlag--;
				mines++;
				UpdateWindow(h);
			}// from flag to gray
				//flagPaintFlag = 1;
				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
			
		}		
		break;
	case WM_LBUTTONDOWN:
	{
		if (gameTime == 0) SetTimer(h, 7, 1000, NULL);
		if (hWnd != h && hWnd != hWndP)
		{
			int* color = (int*)GetProp(hWnd, L"color"); 
			int* neigh = (int*)GetProp(hWnd, L"Neighbour");
			auto b = GetProp(hWnd, (LPCTSTR)"isRed");
			if (*neigh == -1 && *color != 3)
			{
				int *v=new int;
				*v = 1;
				SetProp(hWnd, L"color", v);
				InvalidateRect(hWnd, NULL, TRUE);
				MessageBox(hWnd, L"BOOM!!", L"Mine", MB_ICONERROR /*| MB_SYSTEMMODAL*/);
				disabledChildren();
			}
			else if (*neigh == 0)
			{
				int *v = new int; //white
				*v = 4;
				int *neig = new int;
				*neig = 2;				
				SetProp(hWnd, L"color", v);
				int *index = (int*)GetProp(hWnd, L"idx");
				int i = *index;
				clickOnEmptyField(i);
				
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if (*neigh != 0 && *neigh != -1)
			{
				int *v = new int;
				*v = 2;
				SetProp(hWnd, L"color", v);
				
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
	}
	break;
	case WM_ERASEBKGND:
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_HELP_DEBUG:
			debug = debug == 0 ? 1 : 0;
			break;
		case IDM_NEW:
			//DestroyWindow(hWnd);
			newGame();
			break;
		case ID_CS:
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogProc);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		if(hWnd == h )
		{

			RECT rc;
			RECT rc2;
			GetClientRect(hWnd, &rc);
			FillRect(hdc, &rc, CreateSolidBrush(RGB(255, 255, 255)));
			rc.right /= 2;
			SetTextColor(hdc, RGB(0, 0, 0));
			SetBkMode(hdc, TRANSPARENT);				
			wchar_t  min[5];
			wchar_t  time[5];
			int tmp = mines > 0 ? mines : 0;
			_snwprintf_s(min, 5, 5, L"%04d", tmp);
			_snwprintf_s(time, 5, 5, L"%04d", gameTime);
			DrawTextEx(hdc, (LPWSTR)min, 4, &rc, DT_CENTER|DT_SINGLELINE|DT_VCENTER, NULL);
			GetClientRect(hWnd, &rc2);
			rc2.left = 0.5*(rc2.right - rc2.left);
			DrawTextEx(hdc, (LPWSTR)time, 4, &rc2, DT_CENTER | DT_SINGLELINE | DT_VCENTER, NULL);
		}
		//if (timeFlag == 1)
		//{
		//	timeFlag = 0;
		//	RECT rc;
		//	GetClientRect(h, &rc);
		//	//DrawText(hdc, L"7", (int)_tcslen(L"7"), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//}
		if (hWnd != h && hWnd != hWndP) // male okienka
		{
			RECT rc;
			GetClientRect(hWnd, &rc);
			int* mine = (int*)GetProp(hWnd, L"isMine");
			int* color = (int*)GetProp(hWnd, L"color");  // 0 - gray   1 - mine   2 - neighbpur   3 - flag  4-white
			int c = *color;
			switch (c)
			{
			case 0: // gray
			{
				FillRect(hdc, &rc, CreateSolidBrush(RGB(169, 169, 169)));
			}
			break;
			case 1:  // mine
			{
				HBRUSH brushR = CreateSolidBrush(RGB(0, 0, 0));
				HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brushR);
				Ellipse(hdc, 0.15*WIN_SIZE, 0.15*WIN_SIZE, 0.85*WIN_SIZE, 0.85*WIN_SIZE);

				SelectObject(hdc, oldBrush);
				DeleteObject(brushR);
			}
			break;
			case 2: // neighbour
			{
				int* nieghbour = (int*)GetProp(hWnd, L"Neighbour");
				int n = *nieghbour;
				FillRect(hdc, &rc, CreateSolidBrush(RGB(255, 255, 255)));
				switch (n)
				{
				case 1:
					SetTextColor(hdc, RGB(153, 153, 255));
					SetBkMode(hdc, TRANSPARENT);
					DrawText(hdc, L"1", (int)_tcslen(L"1"), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					break;
				case 2:
					SetTextColor(hdc, RGB(0, 0, 255));
					SetBkMode(hdc, TRANSPARENT);
					DrawText(hdc, L"2", (int)_tcslen(L"2"), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					break;
				case 3:
					SetTextColor(hdc, RGB(0, 0, 102));
					SetBkMode(hdc, TRANSPARENT);
					DrawText(hdc, L"3", (int)_tcslen(L"3"), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					break;
				case 4:
					SetTextColor(hdc, RGB(255, 0, 0));
					SetBkMode(hdc, TRANSPARENT);
					DrawText(hdc, L"4", (int)_tcslen(L"4"), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					break;
				case 5:
					SetTextColor(hdc, RGB(153, 0, 0));
					SetBkMode(hdc, TRANSPARENT);
					DrawText(hdc, L"5", (int)_tcslen(L"5"), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					break;
				case 6:
					SetTextColor(hdc, RGB(255, 0, 0));
					SetBkMode(hdc, TRANSPARENT);
					DrawText(hdc, L"6", (int)_tcslen(L"6"), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					break;
				case 7:
					SetTextColor(hdc, RGB(255, 0, 0));
					SetBkMode(hdc, TRANSPARENT);
					DrawText(hdc, L"7", (int)_tcslen(L"7"), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					break;
				case 8:
					SetTextColor(hdc, RGB(255, 0, 0));
					SetBkMode(hdc, TRANSPARENT);
					DrawText(hdc, L"8", (int)_tcslen(L"8"), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					break;
				default:

					break;
				}

			}
			break;
			case 3: // flag
			{
				auto bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
				HDC hdcNowy = CreateCompatibleDC(hdc);
				HBITMAP hbmOld = (HBITMAP)SelectObject(hdcNowy, bitmap);
				BitBlt(hdc, WIN_SIZE, WIN_SIZE, 48, 48, hdcNowy, 0, 0, SRCCOPY);
				StretchBlt(hdc, 0, 0, WIN_SIZE, WIN_SIZE, hdcNowy, 0, 0, 48, 48, SRCCOPY);
				SelectObject(hdcNowy, hbmOld);
				DeleteObject(bitmap);
				DeleteDC(hdcNowy);
			}
			break;
			case 4:  // white
			{
				FillRect(hdc, &rc, CreateSolidBrush(RGB(255, 255, 255)));
			}
			break;
			default:
				break;
			}
		}
		/*if(flagPaintFlag == 1 && hWnd != h && hWnd != hWndP)
		{
			auto b = GetProp(hWnd, (LPCTSTR)"isRed");
			if (b == (HANDLE)1) // jest flaga
			{
				RECT rc = { 0,0,WIN_SIZE ,WIN_SIZE };
				HBRUSH brushW = CreateSolidBrush(RGB(255, 255, 255));
				FillRect(hdc, &rc, brushW);
				//SetBkMode(hdc, TRANSPARENT);
				SetProp(hWnd, (LPCTSTR)"isRed", (HANDLE)0);
			}
			else
			{
				auto bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
				HDC hdcNowy = CreateCompatibleDC(hdc);
				HBITMAP hbmOld = (HBITMAP)SelectObject(hdcNowy, bitmap);
				BitBlt(hdc, 25, 25, 48, 48, hdcNowy, 0, 0, SRCCOPY);
				StretchBlt(hdc, 0, 0, 25, 25, hdcNowy, 0, 0, 48, 48, SRCCOPY);
				SelectObject(hdcNowy, hbmOld);
				DeleteObject(bitmap);
				DeleteDC(hdcNowy);
				flagPaintFlag = 0;
				//SetBkMode(hdc, TRANSPARENT);
				SetProp(hWnd, (LPCTSTR)"isRed", (HANDLE)1);
			}
		}*/
		///
		EndPaint(hWnd, &ps);
		UpdateWindow(hWnd);
	}
	break;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void fillRandomBombs(/*int (&arr)[x*y]*/)
{
	srand(time(NULL));
	int length = x*y;
	for (int i = 0; i < BOMB_COUNTER; i++)
	{		
		int r = rand() % length;
		if (arr[r] != 1) arr[r] = 1;
		else i--;
	}
}

void fillNeighbours(/*int(&howMany)[x*y]*/)
{
	for (int i = 0; i < x*y; i++)
	{
		if (arr[i] != 1)
		{
			int counter = 0;
			if (i%x != 0 && i >= x && arr[i - x - 1] == 1) counter++; // lewy-gorny
			if (i >= x && arr[i - x] == 1) counter++; // gorny
			if (i % (x) != x - 1 && i >= x && arr[i - x + 1] == 1) counter++; // prawygorny
			if (i%x != 0 && arr[i - 1] == 1) counter++; // left
			if ((i % (x) != x - 1 || i == 0) && arr[i + 1] == 1) counter++; // right
			if (i%x != 0 && i < (x - 1)*y && arr[i + x - 1] == 1) counter++; // lewy-dolny
			if (i < (x - 1)*y && arr[i + x] == 1) counter++; // down
			if (i % (x) != x - 1 && i < (x - 1)*y && arr[i + x + 1] == 1) counter++; // prawy-dolny
			howManyMines[i] = counter; 
		}
		else howManyMines[i] = -1;
	}
}

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hEditBox = 0;
	static HWND wEditBox = 0;
	static HWND mEditBox = 0;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hwndDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDOK)
		{		
			//newGame();
			char h1[8];
			char w[8];
			char m[8];
			hEditBox = GetDlgItem(hwndDlg, IDC_EDIT1);
			wEditBox = GetDlgItem(hwndDlg, IDC_EDIT2);
			mEditBox = GetDlgItem(hwndDlg, IDC_EDIT3);
			GetWindowText(hEditBox, (LPWSTR)h1, 4);
			GetWindowText(wEditBox, (LPWSTR)w, 4);
			GetWindowText(mEditBox, (LPWSTR)m, 4);
			/*y = ((int)h[0] - 48)*10+ (int)h[2] - 48;
			x = ((int)w[0] - 48) * 10 + (int)w[2] - 48;*/
			mines = ((int)m[0] - 48) * 10 + (int)m[2] - 48;
			x = ((int)w[0] - 48) * 10 + (int)w[2] - 48;
			y = ((int)h1[0] - 48) * 10 + (int)h1[2] - 48;
			//KillTimer(h,7);
			RECT wr = { 0, 0, WIN_SIZE*x, WIN_SIZE * (y + 1) };  //zmiana
			AdjustWindowRect(&wr, /*WS_OVERLAPPEDWINDOW*/(WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_CLIPSIBLINGS) & (~WS_THICKFRAME) & (~WS_MAXIMIZEBOX), true);
			data *dat = new data{ wr,cmd };
			mineCounter = BOMB_COUNTER;
			/*h = CreateWindow(szWindowClass, szTitle, (~WS_THICKFRAME) & (WS_VISIBLE | WS_CHILD | WS_BORDER),
				0, 0, WIN_SIZE*x, WIN_SIZE, hWndP, nullptr, hInst, NULL);*/

			//Parent = 1;
			//hWndP = CreateWindow(szWindowClass, szTitle, (WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_CLIPSIBLINGS) & (~WS_THICKFRAME) & (~WS_MAXIMIZEBOX),
				//CW_USEDEFAULT, 0, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, hInst, (LPVOID)dat);
			MoveWindow(hWndP, 0, 0, wr.right - wr.left, wr.bottom - wr.top, true);
			//UpdateWindow(hWndP);
			newGame();
			
			EndDialog(hwndDlg, LOWORD(wParam));
			// Restart game
			return (INT_PTR)TRUE;
		}
		break;
		
	}
	return (INT_PTR)FALSE;
}
void clickOnEmptyField(int i)
{
	int *v = new int; //white
	*v = 4;
	int *neig = new int;
	*neig = 2;
	if (i%x != 0 && i >= x && howManyMines[i - x - 1] != 10 && *(int*)GetProp(hWnd2[i - x - 1], L"color") == 0)
	{
		if (howManyMines[i - x - 1] == 0) { SetProp(hWnd2[i - x - 1], L"color", v); clickOnEmptyField(i - x - 1); }
		else SetProp(hWnd2[i - x - 1], L"color", neig);
		InvalidateRect(hWnd2[i - x - 1], NULL, TRUE);
		
	}
	if (i >= x && howManyMines[i - x] != 10 && *(int*)GetProp(hWnd2[i - x], L"color") == 0)
	{
		if (howManyMines[i - x] == 0) {SetProp(hWnd2[i - x], L"color", v); clickOnEmptyField(i - x);}
		else SetProp(hWnd2[i - x], L"color", neig);
		InvalidateRect(hWnd2[i - x], NULL, TRUE);
		
	}
	if (i % (x) != x - 1 && i >= x && howManyMines[i - x + 1] != 10 && *(int*)GetProp(hWnd2[i - x + 1], L"color") == 0)
	{
		if (howManyMines[i - x + 1] == 0) { SetProp(hWnd2[i - x + 1], L"color", v); clickOnEmptyField(i - x + 1); }
		else SetProp(hWnd2[i - x + 1], L"color", neig);
		InvalidateRect(hWnd2[i - x + 1], NULL, TRUE);
		
	}
	if (i%x != 0 && howManyMines[i - 1] != 10 && *(int*)GetProp(hWnd2[i - 1], L"color") == 0)  // left
	{
		if (howManyMines[i - 1] == 0) {SetProp(hWnd2[i - 1], L"color", v); clickOnEmptyField(i - 1);}
		else SetProp(hWnd2[i - 1], L"color", neig);
		InvalidateRect(hWnd2[i - 1], NULL, TRUE);
	}
	if ((i % (x) != x - 1 || i == 0) && howManyMines[i + 1] != 10 && *(int*)GetProp(hWnd2[i + 1], L"color") == 0)  // right
	{
		if (howManyMines[i + 1] == 0) {SetProp(hWnd2[i + 1], L"color", v); clickOnEmptyField(i + 1);}
		else SetProp(hWnd2[i + 1], L"color", neig);
		InvalidateRect(hWnd2[i + 1], NULL, TRUE);
	}
	if (i%x != 0 && i < (x - 1)*y && howManyMines[i + x - 1] != 10 && *(int*)GetProp(hWnd2[i + x - 1], L"color") == 0)
	{
		if (howManyMines[i + x - 1] == 0) { SetProp(hWnd2[i + x - 1], L"color", v);  clickOnEmptyField(i + x - 1);}
		else SetProp(hWnd2[i + x - 1], L"color", neig);
		InvalidateRect(hWnd2[i + x - 1], NULL, TRUE);
	}
	if (i < (x - 1)*y && howManyMines[i + x] != 10 && *(int*)GetProp(hWnd2[i + x], L"color") == 0)  // down
	{
		if (howManyMines[i + x] == 0) { SetProp(hWnd2[i + x], L"color", v); clickOnEmptyField(i + x);}
		else SetProp(hWnd2[i + x], L"color", neig);
		InvalidateRect(hWnd2[i + x], NULL, TRUE);
	}
	if (i % (x) != x - 1 && i < (x - 1)*y && howManyMines[i + x + 1] != 10 && *(int*)GetProp(hWnd2[i + x + 1], L"color") == 0)
	{
		if (howManyMines[i + x + 1] == 0) { SetProp(hWnd2[i + x + 1], L"color", v);  clickOnEmptyField(i + x + 1);}
		else SetProp(hWnd2[i + x + 1], L"color", neig);
		InvalidateRect(hWnd2[i + x + 1], NULL, TRUE);
	}
}
void disabledChildren()
{
	for (int i = 0; i < x*y; i++)
	{
		EnableWindow(hWnd2[i], FALSE);
	}
}
void newGame()
{
	mines = BOMB_COUNTER;//
	mineCounter = BOMB_COUNTER;
	mineAndFlag = 0;
	KillTimer(h, 7);
	gameTime = 0;
	fillRandomBombs();
	fillNeighbours();
	/*h = CreateWindow(szWindowClass, szTitle, (~WS_THICKFRAME) & (WS_VISIBLE | WS_CHILD | WS_BORDER),
		0, 0, WIN_SIZE*x, WIN_SIZE, hWndP, nullptr, hInst, NULL);*/

	for (int j = 0; j < y; j++)//for (int i = 0; i < x; i++)
	{
		for (int i = 0; i < x; i++)//for (int j = 0; j < y; j++)
		{
			hWnd2[x * j + i] = CreateWindow(szWindowClass, nullptr, (~WS_THICKFRAME) & (WS_VISIBLE | WS_CHILD | WS_BORDER),
				i * WIN_SIZE, (j + 1) * WIN_SIZE, WIN_SIZE, WIN_SIZE, hWndP, NULL, hInst, NULL);
			indexArr[x * j + i] = x * j + i;
			color[x * j + i] = 0;
			SetProp(hWnd2[x * j + i], L"color", &color[x * j + i]);
			SetProp(hWnd2[x * j + i], L"isMine", &arr[x * j + i]);
			SetProp(hWnd2[x * j + i], L"Neighbour", &howManyMines[x * j + i]);
			SetProp(hWnd2[x * j + i], L"idx", &indexArr[x * j + i]);
			//InvalidateRect(hWnd2[10 * j + i], NULL, TRUE);
			/*hWnd2[y * i + j] = CreateWindow(szWindowClass, nullptr, (~WS_THICKFRAME) & (WS_VISIBLE | WS_CHILD | WS_BORDER),
			i * WIN_SIZE, (j + 1) * WIN_SIZE, WIN_SIZE, WIN_SIZE, hWnd, NULL, hInst, NULL);
			indexArr[y * i + j] = y * i + j;
			color[y * i + j] = 0;
			SetProp(hWnd2[y * i + j], L"color", &color[y * i + j]);
			SetProp(hWnd2[y * i + j], L"isMine", &arr[y * i + j]);
			SetProp(hWnd2[y * i + j], L"Neighbour", &howManyMines[y * i + j]);
			SetProp(hWnd2[y * i + j], L"idx", &indexArr[y * i + j]);*/
		}
	}
	

}
