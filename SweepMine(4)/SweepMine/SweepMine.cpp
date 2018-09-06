// SweepMine.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SweepMine.h"
#include <time.h>

#pragma comment(lib,"Msimg32.lib")
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

#define GRID_WIDTH 35
#define GRID_HEIGHT 35

POINT SelectedMineGridPos;
POINT SelectedSudokuPos;

BYTE MineGrid[GRID_WIDTH][GRID_HEIGHT];
BYTE SudokuMap[9][9];
int SpotWidth = 26, SpotHeight = 26;
int ClientWidth,ClientHeight;


int SudokuPlayMap[9][9];


int SudokuAnswerMap[9][9] = {
	{2,9,4,7,8,6,5,1,3},
	{8,5,6,4,1,3,9,7,2},
	{3,7,1,2,9,5,8,4,6},
	{5,1,8,6,2,7,4,3,9},
	{7,6,3,9,4,1,2,8,5},
	{4,2,9,5,3,8,7,6,1},
	{6,8,2,1,7,9,3,5,4},
	{1,4,7,3,5,2,6,9,8},
	{9,3,5,8,6,4,1,2,7} };

namespace GDIres
{
	HDC hdcSudoku;
	HBITMAP hBmpSudoku;
	HPEN hPen;
	HPEN hGridPen;
	HBRUSH hBkBrush;
	HDC hBmpDC;
	HDC hdcBuffer;
	HINSTANCE hInstance;
	HBITMAP hBmpBuffer;
	HFONT hFont;
	HBITMAP hSpotBmp,hSpotBmp_0,hSpotBmp_1,hSpotBmp_2,hSpotBmp_3,
		hSpotBmp_4,hSpotBmp_5,hSpotBmp_6,hSpotBmp_7,hSpotBmp_8,hSpotBmp_flag,hSpotHoverBmp,hMineBmp,hSpotBmpHover_flag;

	void CreateGDIresources()
	{
		HDC hdc = GetDC(NULL);
		hdcSudoku = CreateCompatibleDC(hdc);
		hBmpSudoku = CreateCompatibleBitmap(hdc,ClientWidth,ClientHeight);
		SelectObject(hdcSudoku,hBmpSudoku);
		hPen = CreatePen(PS_SOLID,26,0xFF00);
		hGridPen = CreatePen(PS_SOLID,26,0xFF);
		hBkBrush = CreateSolidBrush(0xFFFFFF);
		SelectObject(hdcSudoku,hBkBrush);
		hBmpDC = CreateCompatibleDC(hdc);
		hdcBuffer = CreateCompatibleDC(hdc);
		hBmpBuffer = CreateCompatibleBitmap(hdc,ClientWidth,ClientHeight);
		SelectObject(hdcBuffer,hBmpBuffer);
		hFont = CreateFont(36,36,0,0,FW_THIN,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,_T("微软雅黑")); 
		SelectObject(hdcSudoku,hFont);
		
		SetBkColor(hdcSudoku,0xFFFFFF);
		hSpotBmp = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP1));
		hSpotHoverBmp = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP2));
		hSpotBmp_0 = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP3));
		hSpotBmp_1 = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP4));
		hSpotBmp_2 = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP5));
		hSpotBmp_3 = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP6));
		hSpotBmp_4 = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP7));
		hSpotBmp_5 = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP11));
		hSpotBmp_6 = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP12));
		hSpotBmp_7 = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP13));
		hSpotBmp_8 = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP14));
		hSpotBmp_flag = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP8));
		hMineBmp = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP9));
		hSpotBmpHover_flag = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP10));

	}

	void DestroyGDIresources()
	{
		DeleteObject(hBkBrush);
		DeleteObject(hPen);
		DeleteObject(hGridPen);
		DeleteObject(hBmpSudoku);
		DeleteDC(hdcSudoku);	
		DeleteObject(hBmpBuffer);
		DeleteObject(hdcBuffer);
		DeleteObject(hFont);
	}

}


// 不重要
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;
	GDIres::hInstance = hInstance;
	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SWEEPMINE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SWEEPMINE));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

#define TEXT_COLOR 0x2222FF
#define NORMAL_SPOT_COLOR 0xAAAAAA
#define HOVER_SPOT_COLOR 0xBBCCBB



// 生成地图




int bombNum(int x, int y) {
	if (MineGrid[x][y] == 9) 
	{
		return 9;
	}
	else {
		int num = 0;
		for (int i = x - 1; i <= x + 1; i++) {
			for (int j = y - 1; j <= y + 1; j++) {
				if (i >= 0 && i < GRID_WIDTH && j >= 0 && j < GRID_HEIGHT) {
					if (MineGrid[i][j] == 9)num++;
				}
			}
		}
		return num;
	}
}

void CheckAndConfirmSudokuNum()
{
	POINT BaseSudokuPos;
	int thisSudokuOpenCount;
	

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if(SudokuPlayMap[i][j] >= 10)
				continue;
			
			bool AllOpen = true;
			BaseSudokuPos.x = i*4;
			BaseSudokuPos.y = j*4;
			for(int iDelta = 0 ;iDelta < 3 && AllOpen;iDelta++)
				for(int jDelta = 0 ;jDelta < 3 ;jDelta++)
				{
					int thisMine = MineGrid[BaseSudokuPos.x + iDelta][BaseSudokuPos.y + jDelta];
					if(thisMine == 29)
						continue;
					if(thisMine < 10 || thisMine >= 20)
					{
						AllOpen = false;
						break;
					}
				}

			if(AllOpen)
				SudokuPlayMap[i][j] = SudokuAnswerMap[i][j] + 10;
		}
	}	
}

void GenerateGrid()
{
	for (int i = 0; i < 35; i++) {
		for (int j = 0; j < 35; j++) {
			MineGrid[i][j] = 0;
		}
	}
	srand((int)time(0));
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			for (int k = 0; k < SudokuAnswerMap[i][j]; k++) {
				int randX = rand() % 3;
				int randY = rand() % 3;
				if (MineGrid[4*i+randX][4*j+randY] != 9) {
					MineGrid[4*i+randX][4*j+randY] = 9;
				}
				else k--;
			}
		}
	}
	for (int i = 0; i < 35; i++) {
		for (int j = 0; j < 35; j++) {
			MineGrid[i][j] = bombNum(i,j);
		}
	}
}


void StartNewGame()
{
	for(int i = 0 ;i < 9 ;i++)
		for(int j = 0 ;j < 9 ;j++)
			SudokuPlayMap[i][j] = -1;

	GenerateGrid();

}

// 游戏结束
void MakeGameOver()
{
	for (int i = 0; i < GRID_WIDTH; i++) 
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			if(MineGrid[i][j] < 10)
				MineGrid[i][j] += 10;
		}
	for(int i = 0 ;i < 9 ;i++)
		for(int j = 0 ;j < 9 ;j++)
			SudokuPlayMap[i][j] = SudokuAnswerMap[i][j] + 10;

	MessageBox(0,L"Game Over!!!",L"-_-",MB_OK);
	//GenerateGrid();
}


// 空白格的扩展
void ExpandFrom(int x, int y)
{
	
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			if (i >= 0 && i < GRID_WIDTH && j >= 0 && j < GRID_HEIGHT) {
				if (MineGrid[i][j] < 10) {
					MineGrid[i][j] += 10; 
					if (MineGrid[i][j] == 10) ExpandFrom(i, j);
				}
			}
		}
	}

}

//鼠标左键
bool UI_MouseLeftClick()
{
	if(SelectedMineGridPos.x == -1 || SelectedMineGridPos.y == -1)
		return false;
	if (SelectedMineGridPos.x % 4 == 3) {
		for (int i = 0; i < GRID_WIDTH; i++) {
			if (MineGrid[SelectedMineGridPos.x][i] < 10)
				MineGrid[SelectedMineGridPos.x][i] += 10;
			else if (MineGrid[SelectedMineGridPos.x][i] >= 20)
				MineGrid[SelectedMineGridPos.x][i] -= 10;
		}
	}
	if (SelectedMineGridPos.y % 4 == 3) {
		for (int i = 0; i < GRID_WIDTH; i++) {
			if (MineGrid[i][SelectedMineGridPos.y] < 10)
				MineGrid[i][SelectedMineGridPos.y] += 10;
			else if (MineGrid[i][SelectedMineGridPos.y] >= 20)
				MineGrid[i][SelectedMineGridPos.y] -= 10;
		}
	}
	BYTE thisSpot = MineGrid[SelectedMineGridPos.x][SelectedMineGridPos.y];
	switch(thisSpot)
	{
	case 9:
		MineGrid[SelectedMineGridPos.x][SelectedMineGridPos.y] += 10;
		MakeGameOver();
		break;
	case 0:
		MineGrid[SelectedMineGridPos.x][SelectedMineGridPos.y] += 10;
		ExpandFrom(SelectedMineGridPos.x,SelectedMineGridPos.y);
		CheckAndConfirmSudokuNum();
		break;
	default:
		if(MineGrid[SelectedMineGridPos.x][SelectedMineGridPos.y] < 10)
			MineGrid[SelectedMineGridPos.x][SelectedMineGridPos.y] += 10;
		else if(MineGrid[SelectedMineGridPos.x][SelectedMineGridPos.y] >= 20)
			MineGrid[SelectedMineGridPos.x][SelectedMineGridPos.y] -= 10;
		CheckAndConfirmSudokuNum();
	}

	return true;
}


//鼠标右键
bool UI_MouseRightClick()
{
	if(	SelectedMineGridPos.x == -1 || SelectedMineGridPos.y == -1 || 
		(MineGrid[SelectedMineGridPos.x][SelectedMineGridPos.y] >= 10 && 
		 MineGrid[SelectedMineGridPos.x][SelectedMineGridPos.y] < 20))
		return false;

	if(MineGrid[SelectedMineGridPos.x][SelectedMineGridPos.y] >= 20)
		MineGrid[SelectedMineGridPos.x][SelectedMineGridPos.y] = MineGrid[SelectedMineGridPos.x][SelectedMineGridPos.y] % 10;
	else
		MineGrid[SelectedMineGridPos.x][SelectedMineGridPos.y] += 20;
}


void UI_DrawSudoku(HDC hdc,int ClientWidth,int ClientHeight)
{

	BLENDFUNCTION BlendFunction;
	BlendFunction.BlendOp = AC_SRC_OVER;
	BlendFunction.BlendFlags = 0;
	BlendFunction.SourceConstantAlpha = 155;
	BlendFunction.AlphaFormat = 0;
	POINT NON;

	Rectangle(GDIres::hdcSudoku,0,0,ClientWidth,ClientHeight);
	WCHAR TextBuffer[10];


	SelectObject(hdc,GDIres::hPen);
	int j = 1;
	for(int i = 3 ;i <= GRID_WIDTH ;i+=4,j++)
	{
		if(j%3 == 0)
			continue;
		MoveToEx(hdc,0,SpotWidth*i+13,&NON);
		LineTo(hdc,SpotWidth*GRID_WIDTH,SpotWidth*i+13);	
		MoveToEx(hdc,SpotWidth*i+13,0,&NON);
		LineTo(hdc,SpotWidth*i+13,SpotWidth*GRID_WIDTH);	
		
	}
	j = 1;
	for(int i = 3 ;i <= GRID_WIDTH ;i+=4,j++)
	{
		if(j%3 == 0)
			SelectObject(hdc,GDIres::hGridPen);
		else 
			continue;
		MoveToEx(hdc,0,SpotWidth*i+13,&NON);
		LineTo(hdc,SpotWidth*GRID_WIDTH,SpotWidth*i+13);	
		MoveToEx(hdc,SpotWidth*i+13,0,&NON);
		LineTo(hdc,SpotWidth*i+13,SpotWidth*GRID_WIDTH);	
	}

	for(int i = 0 ;i < 9 ;i++)
		for(int j = 0 ; j < 9 ;j++)
		{
			if(SudokuPlayMap[i][j] == -1)
				continue;
			
			RECT Rect;
			wsprintf(TextBuffer,L"%d",SudokuPlayMap[i][j] % 10);
			Rect.left = i * SpotWidth * 4;
			Rect.top = j * SpotHeight * 4;
			Rect.bottom = Rect.top + SpotHeight * 3;
			Rect.right = Rect.left + SpotWidth * 3;
			if(SudokuPlayMap[i][j] >= 10)
				SetTextColor(GDIres::hdcSudoku,0xCC);
			else
				SetTextColor(GDIres::hdcSudoku,0xCC0000);

			DrawText(GDIres::hdcSudoku,TextBuffer,lstrlen(TextBuffer),&Rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);    
		}
	

	AlphaBlend(hdc,0,0,ClientWidth,ClientHeight,GDIres::hdcSudoku,0,0,ClientWidth,ClientHeight,BlendFunction);	
}


void UI_DrawMineGrid(HDC hdc,bool isSudoku)
{
	for(int i = 0 ;i < GRID_WIDTH ;i ++)
		for (int j = 0 ; j < GRID_HEIGHT ;j++)
		{
			//MineGrid[i][j] = 11;
			bool isHover = FALSE;
			if(SelectedMineGridPos.x == i && SelectedMineGridPos.y == j)
				isHover = TRUE;
			
			if(isSudoku)
			{
				if(	i / 4 == SelectedSudokuPos.x && 
					j / 4 == SelectedSudokuPos.y)
					SelectObject(GDIres::hBmpDC,GDIres::hSpotHoverBmp);
				else
					SelectObject(GDIres::hBmpDC,GDIres::hSpotBmp);
			}
			else if(MineGrid[i][j] < 10)
			{
				if(isHover)
					SelectObject(GDIres::hBmpDC,GDIres::hSpotHoverBmp);
				else
					SelectObject(GDIres::hBmpDC,GDIres::hSpotBmp);
			}
			else
				switch(MineGrid[i][j])
			{
				case 10:
					SelectObject(GDIres::hBmpDC,GDIres::hSpotBmp_0);
					break;
				case 11:
					SelectObject(GDIres::hBmpDC,GDIres::hSpotBmp_1);
					break;
				case 12:
					SelectObject(GDIres::hBmpDC,GDIres::hSpotBmp_2);
					break;
				case 13:
					SelectObject(GDIres::hBmpDC,GDIres::hSpotBmp_3);
					break;
				case 14:
					SelectObject(GDIres::hBmpDC,GDIres::hSpotBmp_4);
					break;
				case 15:
					SelectObject(GDIres::hBmpDC,GDIres::hSpotBmp_5);
					break;
				case 16:
					SelectObject(GDIres::hBmpDC,GDIres::hSpotBmp_6);
					break;
				case 17:
					SelectObject(GDIres::hBmpDC,GDIres::hSpotBmp_7);
					break;
				case 18:
					SelectObject(GDIres::hBmpDC,GDIres::hSpotBmp_8);
					break;
				case 19:
					SelectObject(GDIres::hBmpDC,GDIres::hMineBmp);
					break;
				default:
					if(MineGrid[i][j] >= 20)
					{
						if(isHover)
							SelectObject(GDIres::hBmpDC,GDIres::hSpotBmpHover_flag);
						else
							SelectObject(GDIres::hBmpDC,GDIres::hSpotBmp_flag);
					}
					else
						SelectObject(GDIres::hBmpDC,GDIres::hSpotBmp);
			}
			BitBlt(hdc,i * SpotWidth ,j * SpotHeight, SpotWidth, SpotHeight, GDIres::hBmpDC,0,0,SRCCOPY);
		}
}

//绘制UI
void UI_Draw(HDC hdc,int ClientWidth, int ClientHeight,bool isDrawSudoku)
{
	
	UI_DrawMineGrid(GDIres::hdcBuffer,isDrawSudoku);
	if(isDrawSudoku)
		UI_DrawSudoku(GDIres::hdcBuffer,ClientWidth,ClientHeight);

	BitBlt(hdc,0,0,ClientWidth,ClientHeight,GDIres::hdcBuffer,0,0,SRCCOPY);

}


// 不重要
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SWEEPMINE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SWEEPMINE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

// 不重要
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd,hWnd2;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);


   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);


   return TRUE;
}


void ModifySelectedSudokuPos(bool UpThanDown)
{
	if(SelectedSudokuPos.x == -1 || SelectedSudokuPos.y == -1)
		return;
	if(SudokuPlayMap[SelectedSudokuPos.x][SelectedSudokuPos.y] >= 10)
		return;

	if(UpThanDown)
	{
		if(SudokuPlayMap[SelectedSudokuPos.x][SelectedSudokuPos.y]	== 8)
			SudokuPlayMap[SelectedSudokuPos.x][SelectedSudokuPos.y] = -1;
		else
			SudokuPlayMap[SelectedSudokuPos.x][SelectedSudokuPos.y]++;
	}
	else
	{
		if(SudokuPlayMap[SelectedSudokuPos.x][SelectedSudokuPos.y]	== -1)
			SudokuPlayMap[SelectedSudokuPos.x][SelectedSudokuPos.y]	= 8;
		else
			SudokuPlayMap[SelectedSudokuPos.x][SelectedSudokuPos.y]--;
	}

}

void DetermineSelected(HWND hWnd,bool isSudoku , int MouseX, int MouseY)
{
	//if(isSudoku)
	{
		if(	SelectedSudokuPos.x % (4 * SpotWidth) < (3 * SpotWidth) &&
				SelectedSudokuPos.y % (4 * SpotHeight) < (3 * SpotHeight))
		{
			SelectedSudokuPos.x = MouseX / (4 * SpotWidth);
			SelectedSudokuPos.y = MouseY / (4 * SpotHeight);
		}
		else
			SelectedSudokuPos.x = SelectedSudokuPos.y = -1;
	}
	//else
	{
		SelectedMineGridPos.x = MouseX / (SpotWidth);
		SelectedMineGridPos.y = MouseY / (SpotHeight);	
	}
	InvalidateRect(hWnd,NULL,NULL);	
}

void CorrectSudokuMap()
{
	for(int i = 0 ;i < 9 ;i++)
		for(int j = 0 ;j < 9 ;j++)
			SudokuAnswerMap[i][j]--;
}

//不重要
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	int zDelta;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT Rect;
	POINT MousePos;

	static bool isTabOn = false;

	switch (message)
	{
	case WM_CREATE:
		SetWindowPos(hWnd,0,0,0,SpotWidth*GRID_WIDTH+20,SpotHeight*GRID_HEIGHT+70,SWP_NOMOVE);
		GetClientRect(hWnd,&Rect);
		ClientWidth = Rect.right;
		ClientHeight = Rect.bottom;
		CorrectSudokuMap();
		StartNewGame();
		GDIres::CreateGDIresources();
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_32771:
			StartNewGame();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_MOUSEWHEEL:
		zDelta = (short)HIWORD(wParam);
		if(isTabOn && abs(zDelta) >  100)
			ModifySelectedSudokuPos(zDelta > 0);
		break;
	case WM_KEYUP:
		if(wParam == VK_TAB)
		{
			GetCursorPos(&MousePos);
			ScreenToClient(hWnd,&MousePos);
			DetermineSelected(hWnd,isTabOn,MousePos.x,MousePos.y);
			isTabOn = false;
			InvalidateRect(hWnd,NULL,NULL);
		}
			
		break;
	case WM_KEYDOWN:
		if(wParam == VK_TAB)
		{
			isTabOn = true;
			GetCursorPos(&MousePos);
			ScreenToClient(hWnd,&MousePos);
			DetermineSelected(hWnd,isTabOn,MousePos.x,MousePos.y);
			InvalidateRect(hWnd,NULL,NULL);
		}
		break;
	case WM_MOUSEMOVE:
		DetermineSelected(hWnd,isTabOn,LOWORD(lParam),HIWORD(lParam));
		break;
	case WM_LBUTTONDOWN:
		if(isTabOn)
			break;
		if(wParam & MK_RBUTTON)
			MessageBox(hWnd,L"OKOKOK",NULL,MB_OK);
		if(UI_MouseLeftClick())
			InvalidateRect(hWnd,NULL,NULL);
		break;
	case WM_RBUTTONDOWN:
		if(isTabOn)
			break;
		if(wParam & MK_LBUTTON)
			MessageBox(hWnd,L"OKOKOK",NULL,MB_OK);
		if(UI_MouseRightClick())
			InvalidateRect(hWnd,NULL,NULL);
		break;
	case WM_PAINT:
		GetClientRect(hWnd,&Rect);
		hdc = BeginPaint(hWnd, &ps);		
		UI_Draw(hdc,Rect.right,Rect.bottom,isTabOn);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		GDIres::DestroyGDIresources();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

