// I N C L U D E S
/////////////////////////////////////////////////////////////////////
#define _WIN32_WINNT	0x0601

#include "Zapisz_ikonê_v2.h"
#include "Zapisz_ikonê.h"
#include "Nag³ówek g³ówny.h"
#include "Icon.h"
#include "resource.h"







//D E F I N E S
/////////////////////////////////////////////////////////////////////
#define Show(Window)  RedrawWindow(Window,0,0,0);ShowWindow(Window,SW_SHOW);


#define ID_EDIT1	1
#define ID_LIST0    2
#define ID_EDIT3	3
#define ID_EDIT4	4
#define ID_EDIT5	5
#define ID_EDIT6	6
#define ID_EDIT7	7
#define ID_LIST1	8
#define ID_LIST2	9
#define ID_SAVE		10

#define ID_ZAPISZ_W				11
#define ID_LOKALIZACJA_ZAPISU	12

#define ID_EDIT8	13

#define ID_STATIC1	501 
#define ID_STATIC2	502
#define ID_STATIC3	503 
#define ID_STATIC4	504 
#define ID_STATIC5	505 
#define ID_STATIC6	506 
#define ID_STATIC7	507 
#define ID_STATIC8	508 
#define ID_STATIC9	509 
#define ID_STATIC10 510
#define ID_STATIC11 511

#define WM_TAB (WM_USER) 

//ID Przyciski2
#define ID_WCZYTAJ			601
#define ID_RESET			602
#define ID_PRZYCISK			603
#define ID_WczytajZPliku	604
#define ID_CZYSC			605
#define ID_PANORAMA			606
#define ID_TRADYCYJNY		607
#define ID_CALE_POLE		608
#define ID_WCZYTAJ_WIDEO	609
#define ID_WCZYTAJ_PONOWNIE 610
#define ID_KOMUNIKAT		611

#define ID_CZYSC_COMBOBOX	401
#define ID_ZAPISZ_OPCJE		402
#define ID_NASTEPNY_WPIS	403
#define ID_OTWORZ_KATALOG	404
//P R A G M A
/////////////////////////////////////////////////////////////////////
//#pragma comment( lib, "comctl32.lib" )
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


// D E K L A R A C J E
/////////////////////////////////////////////////////////////////////
ATOM RegisterWndClass(TCHAR* pszClassName, WNDPROC fpWndProc);
HWND CreateAppWindow(int iPosX, int iPosY, int iWidth, int iHeight,
	char* pszClassName, char* pszWindowName, bool bAdjust,
	unsigned long ulWndStyle, unsigned long ulExWndStyle);


LPWSTR WyborFolderu(HWND hWnd);
LPWSTR UtworzNowyfolder(HWND UchwytKontrolki1, HWND UchwytKontrolki2);
HANDLE OtworzNowyPlik(LPWSTR Lokalizacja, HWND Komputer, DWORD Distribution);
int ZapisDanychdoPlikuA(HWND UchwytKontrolki, LPSTR TekstDoSkopiowania, HANDLE UchwytdoPliku);

int WstawTekstDoKontrolki(HWND UchwytComboBox, LPWSTR TekstDoKontrolki);
INT ZamknijPlik(HANDLE hPlik);
LPWSTR TekstZKontrolki(HWND UchwytKontrolki);
HWND subclass_combo_box(HWND hWndCombo);

static HWND    MCIFrame;
static HANDLE  ghInst;

// D E F I N I C J E
/////////////////////////////////////////////////////////////////////

WNDPROC lpfnEditWndProc;
WNDPROC StaraKontrolka[9999];
INT idFocus = 0;
TEXTMETRIC tm1, tm2;
PAINTSTRUCT ps1, ps2;
HDC hdc1, hdc2;
TCHAR* const gc_pszClassName = TEXT("AppWindow");
TCHAR* const gc_pszWindowTitle = TEXT("Program do Katalogów");
HWND hwndMain;
BOOL Kontrola_Zapisu = 0;
BOOL Kontrola_Walidacji_K = 1;
BOOL Kontrola_Walidacji_N = 1;
static TCHAR Bufor_Sciezki[MAX_PATH];
HWND KOMUNIKAT, PODAJLOKALIZACJE, LOKALIZACJASCIEZKI;


//R E J E S T R A C J A  O K N A
//////////////////////////////////////////////////////////////////////
ATOM RegisterWndClass(TCHAR* pszClassName, WNDPROC fpWndProc)
{
	WNDCLASSEX WndClassEx = { 0 };

	WndClassEx.cbSize = sizeof(WNDCLASSEX);
	WndClassEx.hInstance = GetModuleHandle(NULL);
	WndClassEx.lpszClassName = pszClassName;
	WndClassEx.lpfnWndProc = fpWndProc;
	WndClassEx.style = CS_HREDRAW | CS_VREDRAW;
	WndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClassEx.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	WndClassEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClassEx.lpszMenuName = NULL;
	WndClassEx.cbClsExtra = 0;
	WndClassEx.cbWndExtra = 0;
	WndClassEx.hIconSm = GoFindAnIcon("%SYSTEMROOT%\\system32\\shell32.dll,115", NULL);

	return RegisterClassEx(&WndClassEx);
} // end RegisterWndClass

HWND CreateAppWindow(int iPosX, int iPosY, int iWidth, int iHeight,
	TCHAR* pszClassName, TCHAR* pszWindowName, bool bAdjust,
	unsigned long ulWndStyle, unsigned long ulExWndStyle)
{
	RECT WndRect = { 0, 0, iWidth, iHeight };
	if (bAdjust)
		AdjustWindowRectEx(&WndRect, ulWndStyle, false, ulExWndStyle);

	HWND hWnd = NULL;
	hWnd = CreateWindowEx(
		ulExWndStyle, pszClassName, pszWindowName,
		ulWndStyle, iPosX, iPosY,
		WndRect.right - WndRect.left, WndRect.bottom - WndRect.top,
		NULL, NULL, GetModuleHandle(NULL), NULL);
	hwndMain = hWnd;
	return hWnd;
} // end CreateAppWindow



// W I N M A I N
/////////////////////////////////////////////////////////////////////
int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR pCmdLine, int iCmdShow)
{

	RegisterWndClass(gc_pszClassName, WndProc);

	HWND hWnd = NULL;
	hWnd = CreateAppWindow(
		100, 100, 950, 576, gc_pszClassName,
		gc_pszWindowTitle, false, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, 0);

	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);

	HACCEL hAccel = NULL;

	HMENU hMenuMain;

	hAccel = LoadAccelerators(hInstance, TEXT("IDR_ACCELERATOR1"));

	hMenuMain = LoadMenu(hInstance, MAKEINTRESOURCE(111));

	SetMenu(hWnd, hMenuMain);

	// pompa wiadomoœci
	MSG Msg = { 0 };
	for (;;)
	{
		if (GetMessage(&Msg, NULL, 0, 0) != 0)
		{

			if (!TranslateAccelerator(hWnd, hAccel, &Msg))
			{
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}


		}
		else break;
	} // end for

	UnregisterClass(gc_pszClassName, hInstance);
	return (int)Msg.wParam;
} // end WinMain

// F U N K C J E
/////////////////////////////////////////////////////////////////////


LRESULT CALLBACK WndProc(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TCHAR s_achTxtBuffer[80] = { 0 };
	static TCHAR s_Podaj_Nazwe[50] = TEXT("WprowadŸ nowy katalog: ");
	static TCHAR s_Podaj_Komputer[] = TEXT("Podaj gdzie zapisany jest film: ");
	static TCHAR s_Podaj_Date[] = TEXT("Podaj datê wykonania: ");
	static TCHAR s_Lokalizacja[] = TEXT("Podaj lokalizacjê folderu z filmem: ");
	static TCHAR s_Podfoldery[] = TEXT("Zawierane podfoldery :");
	static TCHAR s_Lokalizacja_Sciezki[] = TEXT("Podaj lokalizacje katalogu: ");
	static TCHAR s_Podaj_Date_Wpisu[] = TEXT("Podaj datê pierwszego zapisu: ");
	static TCHAR s_Rozdzielczosc[] = TEXT("Rozdzieleczoœæ wideo: ");
	static TCHAR s_Format[] = TEXT("Format pliku: ");
	static int  cxChar = 1, cyChar = 1, wxChar, xPrawy = 1, xPrawy2 = 1, xStart = 1, MB1 = IDNO, MB2;

	//Kontrolki
	static HWND PodajNazwePliku, PodajNazweKomputera, PodajDate, PodajLokalizacje, PodajPodfoldery, LokalizacjaSciezki, ListaRozdzielczosc, ListaFormat
				, PodajDateWpisu;
	static HWND PodajNazwePlikuTEXT, PodajNazweKomputeraTEXT, PodajDateTEXT, PodajLokalizacjeTEXT, PodajPodFolderyTEXT, LokalizacjaSciezkiTEXT
		, ListaRozdzielczoscTEXT, ListaFormatTEXT, PodajDateWpisuTEXT, KomunikatyTEXT;
	static HWND Save, Zapisz_w, Sciezka_do, Okno_Odtwarzacza;
	
	static HWND ComboEdit1, ComboEdit2, ComboEdit3;

	static TCHAR sz_BufornaTekstZKontrolki[10000]; 
	static TCHAR sz_UstawieniaKonfiguracyjne[10000];
	static HANDLE Plik;
	static SIZE size;

	static char s_Wczytana_Data[] = "Data wykonania lub pierwszego zapisu: ";
	static char s_Wczytana_Lokalizacja[] = "Lokalizacja: ";
	static char s_Wczytane_Podfoldery[] = "Podfoldery: ";
	static char s_Wczytana_Rozdzielczosc[] = "Rozdzielczoœæ: ";
	static char s_Wczytany_Format[] = "Format: ";

	static TCHAR sz_Lokalizacja[MAX_PATH];
	static TCHAR sz_Sciezka[MAX_PATH];

	//Bufor na tekst pobrany z kontrolki
	static TCHAR s_Komputer[1000];
	static TCHAR sz_LokalizacjaZapisu[MAX_PATH];
	static TCHAR sz_LokalizacjaOdczytu[MAX_PATH] = TEXT("Plik_Konfiguracyjny.ini");

	static HICON hIcon;
	// for mouse wheel logic
	static int  iDeltaPerLine, iAccumDelta;    
	ULONG       ulScrollLines;
	

	//Paski przesuwu

	static SCROLLINFO  si;
	static SCROLLINFO  si2;
	static SCROLLINFO  si3;
	RECT okno;
	INT iVertPos = 0, iHorzPos = 0;

	//Kontrolki statyczne
	INT i;
	static HBRUSH hBrushStatic;

	//Bufor na "otwórz katalog"
	DWORD Dlugosc_Katalogu;
	LPWSTR Bufor_na_Katalog;	

	//Bufor na "Odtwórz Wideo"
	DWORD dwdlugosc;
	LPWSTR Bufor;

	//Obs³uga Okna dialogowego
	static HINSTANCE hInstance;

	HBRUSH hBrushEdit = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH hBrushEditBialy = CreateSolidBrush(RGB(255, 255, 255));
	HWND hCtl;
	HDC hDC;

	ZeroMemory(&si, sizeof(si));
	switch (message)
	{
		//kolory aplikacji 
	case WM_CTLCOLORSTATIC:
		i = GetWindowLong((HWND)lParam, GWL_ID);
		if (i >= ID_STATIC1 && i <= ID_STATIC9)
		{ 
			SetBkColor((HDC)wParam, GetSysColor(COLOR_BTNHIGHLIGHT));
			return (LRESULT)hBrushStatic;
		}

		hCtl = (HWND)lParam;
		hDC = (HDC)wParam;

		if (hCtl == KOMUNIKAT)
		{
			SetBkMode(hDC, TRANSPARENT);
			SetTextColor(hDC, RGB(255, 0, 0));

			return(LRESULT)hBrushEditBialy;
		}

		break;
	case WM_SYSCOLORCHANGE :
		DeleteObject(hBrushStatic);
		hBrushStatic = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));
		return 0;
	
	case WM_CTLCOLOREDIT:
	
		hCtl = (HWND)lParam;
		hDC = (HDC)wParam;

		
		if ((hCtl == PodajLokalizacje && Kontrola_Walidacji_K == 0) || (hCtl == PodajNazwePliku && Kontrola_Walidacji_N == 0))
			{
				SetBkMode(hDC, TRANSPARENT);
				SetTextColor(hDC, RGB(255, 255, 255));
				
				return(LRESULT)hBrushEdit;
			}
		else
			if ((hCtl == PodajLokalizacje && Kontrola_Walidacji_K == 1) || (hCtl == PodajNazwePliku && Kontrola_Walidacji_N == 1))
		{
			SetBkMode(hDC, TRANSPARENT);
			SetTextColor(hDC, RGB(0, 0, 0));
			SetBkMode(hDC, OPAQUE);
			return(LRESULT)hBrushEditBialy;
		}
		
		break;

	case WM_LBUTTONUP:
		SetFocus(hwndMain);
		return 0;
		//paski przewijania
	case WM_MOUSEWHEEL:
		if (iDeltaPerLine == 0)
			break;

		iAccumDelta += (short)HIWORD(wParam);     // 120 or -120

		while (iAccumDelta >= iDeltaPerLine)
		{
			SendMessage(hWnd, WM_VSCROLL, SB_LINEUP, 0);
			iAccumDelta -= (iDeltaPerLine * 2);
		}

		while (iAccumDelta <= -iDeltaPerLine)
		{
			SendMessage(hWnd, WM_VSCROLL, SB_LINEDOWN, 0);
			iAccumDelta += (iDeltaPerLine * 2);
		}

		return 0;

	case WM_SIZE:
		GetClientRect(hWnd, &okno);

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE ;
		si.nMin = 0;
		si.nMax = 1000;
		si.nPage = okno.bottom;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

		si2.cbSize = sizeof(si2);
		si2.fMask = SIF_RANGE | SIF_PAGE;
		si2.nMin = 0;
		si2.nMax = 10000;
		si2.nPage = okno.right;
		SetScrollInfo(hWnd, SB_HORZ, &si2, TRUE);


		return 0;

	case WM_VSCROLL:
		// Get all the vertical scroll bar information
		
		SCROLLINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hWnd, SB_VERT, &si);

		// Save the position for comparison later on

		iVertPos = si.nPos;

		switch (LOWORD(wParam))
		{
		case SB_TOP:
			si.nPos = si.nMin;
			break;

		case SB_BOTTOM:
			si.nPos = si.nMax;
			break;

		case SB_LINEUP:
			si.nPos -= 1;
			break;

		case SB_LINEDOWN:
			si.nPos += 1;
			break;

		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;

		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;

		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;

		default:
			break;
		}
		// Set the position and then retrieve it.  Due to adjustments
		//   by Windows it might not be the same as the value set.

		si.fMask = SIF_POS;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		GetScrollInfo(hWnd, SB_VERT, &si);

		// If the position has changed, scroll the window and update it

		if (si.nPos != iVertPos)
		{
			
			ScrollWindowEx(hWnd, 0, (iVertPos - si.nPos), (CONST RECT *) NULL, (CONST RECT *) NULL, (HRGN)NULL, (LPRECT)NULL,
				SW_SCROLLCHILDREN | SW_ERASE | SW_INVALIDATE);

			UpdateWindow(hWnd);

		}
		
		return 0;

	case WM_HSCROLL:
		// Get all the vertical scroll bar information

		si2.cbSize = sizeof(si2);
		si2.fMask = SIF_ALL;

		// Save the position for comparison later on

		GetScrollInfo(hWnd, SB_HORZ, &si2);
		iHorzPos = si2.nPos;

		switch (LOWORD(wParam))
		{
		case SB_LINELEFT:
			si2.nPos -= 1;
			break;

		case SB_LINERIGHT:
			si2.nPos += 1;
			break;

		case SB_PAGELEFT:
			si2.nPos -= si2.nPage;
			break;

		case SB_PAGERIGHT:
			si2.nPos += si2.nPage;
			break;

		case SB_THUMBPOSITION:
			si2.nPos = si2.nTrackPos;
			break;

		default:
			break;
		}
		// Set the position and then retrieve it.  Due to adjustments
		//   by Windows it might not be the same as the value set.

		si2.fMask = SIF_POS;
		SetScrollInfo(hWnd, SB_HORZ, &si2, TRUE);
		GetScrollInfo(hWnd, SB_HORZ, &si2);

		// If the position has changed, scroll the window 

		if (si2.nPos != iHorzPos)
		{
			ScrollWindowEx(hWnd, (iHorzPos - si2.nPos), 0, (CONST RECT *) NULL, (CONST RECT *) NULL, (HRGN)NULL, (LPRECT)NULL,
				SW_SCROLLCHILDREN | SW_ERASE | SW_INVALIDATE);

			UpdateWindow(hWnd);

		}
		
		return 0;
	case WM_CREATE:

		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;

		hdc1 = GetDC(hWnd);

		GetTextMetrics(hdc1, &tm1);
		cxChar = LOWORD(GetDialogBaseUnits());
		cyChar = tm1.tmHeight + tm1.tmExternalLeading;
		wxChar = tm1.tmAveCharWidth;

		//Tworzenie kontrolek
		PodajNazwePlikuTEXT = CreateWindowEx(0, TEXT("STATIC"), s_Podaj_Nazwe,
			WS_CHILD | WS_VISIBLE | SS_LEFT
			, 5, 5, 200, cyChar, hWnd, (HMENU) ID_STATIC1,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		PodajNazwePliku = CreateWindowEx(0, TEXT("Edit"), NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | WS_HSCROLL | WS_TABSTOP
			, 7, 25, (cxChar * 40), (40), hWnd, (HMENU)ID_EDIT1,
			GetModuleHandle(NULL), NULL);

		PodajNazweKomputeraTEXT = CreateWindowEx(0, TEXT("STATIC"), s_Podaj_Komputer,
			WS_CHILD | WS_VISIBLE | SS_LEFT
			, 5, 70, (wxChar * wcslen(s_Podaj_Komputer)), cyChar, hWnd, (HMENU)ID_STATIC2,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		
		PodajNazweKomputera = CreateWindow(TEXT("COMBOBOX"), NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN | WS_VSCROLL | WS_TABSTOP
			, 7, 90, (cxChar * 40), (100), hWnd, (HMENU)ID_LIST0,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		PodajDateTEXT = CreateWindowEx(0, TEXT("STATIC"), s_Podaj_Date,
			WS_CHILD | WS_VISIBLE | SS_LEFT
			, 5, 135, (wxChar * wcslen(s_Podaj_Date)), cyChar, hWnd, (HMENU)ID_STATIC3,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		PodajDate = CreateWindowEx(0, TEXT("Edit"), NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | WS_HSCROLL | WS_TABSTOP
			, 7, 155, (cxChar * 40), (40), hWnd, (HMENU)ID_EDIT3,
			GetModuleHandle(NULL), NULL);

		PodajLokalizacjeTEXT = CreateWindowEx(0, TEXT("STATIC"), s_Lokalizacja,
			WS_CHILD | WS_VISIBLE | SS_LEFT
			, 5, 200, (wxChar * wcslen(s_Lokalizacja)), cyChar, hWnd, (HMENU)ID_STATIC4,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		PodajLokalizacje = CreateWindowEx(0, TEXT("Edit"), NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | WS_HSCROLL | WS_TABSTOP
			, 7, 220, (cxChar * 40), (40), hWnd, (HMENU)ID_EDIT4,
			GetModuleHandle(NULL), NULL);

		PodajPodFolderyTEXT = CreateWindowEx(0, TEXT("STATIC"), s_Podfoldery,
			WS_CHILD | WS_VISIBLE | SS_LEFT
			, 5, 265, (wxChar * wcslen(s_Podfoldery)), cyChar, hWnd, (HMENU)ID_STATIC5,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		PodajPodfoldery = CreateWindowEx(0, TEXT("Edit"), NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | WS_HSCROLL | WS_TABSTOP
			, 7, 285, (cxChar * 40), (40), hWnd, (HMENU)ID_EDIT5,
			GetModuleHandle(NULL), NULL);

		LokalizacjaSciezkiTEXT = CreateWindowEx(0, TEXT("STATIC"), s_Lokalizacja_Sciezki,
			WS_CHILD | WS_VISIBLE | SS_LEFT
			, 5, 330, (wxChar * wcslen(s_Lokalizacja_Sciezki)), cyChar, hWnd, (HMENU)ID_STATIC6,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		LokalizacjaSciezki = CreateWindowEx(0, TEXT("Edit"), NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | WS_HSCROLL | WS_TABSTOP
			, 7, 350, (cxChar * 40), (40), hWnd, (HMENU)ID_EDIT6,
			GetModuleHandle(NULL), NULL);

		PodajDateWpisuTEXT = CreateWindowEx(0, TEXT("STATIC"), s_Podaj_Date_Wpisu,
			WS_CHILD | WS_VISIBLE | SS_LEFT
			, 5, 430, (wxChar * wcslen(s_Podaj_Date_Wpisu)), cyChar, hWnd, (HMENU)ID_STATIC9,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		PodajDateWpisu = CreateWindowEx(0, TEXT("Edit"), NULL,
			WS_CHILD | WS_BORDER | WS_VISIBLE | ES_MULTILINE | ES_LEFT | ES_AUTOVSCROLL | WS_HSCROLL | WS_TABSTOP
			, 7, 450, (cxChar * 40), (40), hWnd, (HMENU)ID_EDIT7,
			GetModuleHandle(NULL), NULL);

		xPrawy = wcslen(s_Rozdzielczosc);

		

		xStart = 57;

		ListaRozdzielczoscTEXT = CreateWindowEx(0, TEXT("STATIC"), s_Rozdzielczosc,
			WS_CHILD | WS_VISIBLE | SS_RIGHT
			, (cxChar * 39) + xPrawy + 7, 265, (wxChar * wcslen(s_Rozdzielczosc)), cyChar, hWnd, (HMENU)ID_STATIC7,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		ListaRozdzielczosc = CreateWindow(TEXT("COMBOBOX"), NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN | WS_VSCROLL
			, (cxChar * 59) + xPrawy + 7, 265, (cxChar * 20), 100, hWnd, (HMENU)ID_LIST1,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		ListaFormatTEXT = CreateWindowEx(0, TEXT("STATIC"), s_Format,
			WS_CHILD | WS_VISIBLE | SS_RIGHT
			, ((cxChar * 39) + xPrawy + 7) + xStart, 330, (wxChar * wcslen(s_Format)), cyChar, hWnd, (HMENU)ID_STATIC8,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		ListaFormat = CreateWindow(TEXT("COMBOBOX"), NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWN | WS_VSCROLL
			, (cxChar * 59) + xPrawy + 7, 330, (cxChar * 20), 100, hWnd, (HMENU)ID_LIST2,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		Save = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Zapisz"), WS_CHILD | WS_VISIBLE,
			(cxChar * 59) + xPrawy + 7, 220, 150, 30, hWnd, (HMENU)ID_SAVE
			, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		Zapisz_w = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Pochodzenie :"), WS_CHILD | WS_VISIBLE,
			(cxChar * 39) + xPrawy + 7, 220, 150, 30, hWnd, (HMENU)ID_ZAPISZ_W
			, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		Sciezka_do = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Przegl¹daj..."), WS_CHILD | WS_VISIBLE
			, 7, 395, 150, 30, hWnd, (HMENU)ID_LOKALIZACJA_ZAPISU
			, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
/*
		CreateWindowEx(0, TEXT("BUTTON"), TEXT("Odczyt"), WS_CHILD | WS_VISIBLE
			, 500, 100, 150, 30, hWnd, (HMENU)ID_OPCJE
			, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);*/

		CreateWindowEx(0, TEXT("BUTTON"), TEXT("RESET"), WS_CHILD | WS_VISIBLE
			, 170, 395, 150, 30, hWnd, (HMENU)ID_RESET
			, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		
		/*CreateWindowEx(0, TEXT("BUTTON"), TEXT("Zapisz opcje konfiguracyjne"), WS_CHILD | WS_VISIBLE
			, 330, 170, 150, 30, hWnd, (HMENU)ID_ZAPISZ_OPCJE
			, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
			*/
		CreateWindowEx(0, TEXT("BUTTON"), TEXT("Nastêpny Wpis"), WS_CHILD | WS_VISIBLE
			, (cxChar * 59) + xPrawy + 7, 450, 150, 30, hWnd, (HMENU)ID_NASTEPNY_WPIS
			, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		CreateWindowEx(0, TEXT("BUTTON"), TEXT("Otwórz Katalog"), WS_CHILD | WS_VISIBLE
			, (cxChar * 39) + xPrawy + 7, 450, 150, 30, hWnd, (HMENU)ID_OTWORZ_KATALOG
			, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		Okno_Odtwarzacza = CreateWindowEx(0, TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_WHITEFRAME
			, (cxChar * 39) + xPrawy + 7, 5, 320, 200, hWnd, (HMENU)ID_STATIC10
			, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		CreateWindowEx(0, TEXT("BUTTON"), TEXT("16:9"), WS_CHILD | WS_VISIBLE
			, 675, 5, 150, 30, hWnd, (HMENU)ID_PANORAMA
			, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		
		CreateWindowEx(0, TEXT("BUTTON"), TEXT("4:3"), WS_CHILD | WS_VISIBLE
			, 675, 35, 150, 30, hWnd, (HMENU)ID_TRADYCYJNY
			, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		CreateWindowEx(0, TEXT("BUTTON"), TEXT("Pe³ny"), WS_CHILD | WS_VISIBLE
			, 675, 65, 150, 30, hWnd, (HMENU)ID_CALE_POLE
			, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		CreateWindowEx(0, TEXT("BUTTON"), TEXT("Wczytaj Wideo"), WS_CHILD | WS_VISIBLE
			, 675, 95, 150, 30, hWnd, (HMENU)ID_WCZYTAJ_WIDEO
			, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

		KomunikatyTEXT = CreateWindowEx(0, TEXT("STATIC"), NULL,
			WS_CHILD | WS_VISIBLE 
			, 675, 330, 200, 100, hWnd, (HMENU)ID_STATIC11,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
		
		KOMUNIKAT = KomunikatyTEXT;

		PODAJLOKALIZACJE = PodajLokalizacje;

		LOKALIZACJASCIEZKI = LokalizacjaSciezki;

		SendMessage(hWnd, WM_COMMAND, ID_WCZYTAJ, 0);

		SetFocus(PodajNazwePliku);

		//  Get the edit window handle to each combo box. 
		//pt.x = 3;
		//pt.y = 3;
		//hwndEdit1 = ChildWindowFromPoint(ListaRozdzielczosc, pt);
		//hwndEdit2 = ChildWindowFromPoint(ListaFormat, pt);


		for (int i = 1; i < 8; i++)
		{
			StaraKontrolka[i] = (WNDPROC)SetWindowLong(GetDlgItem(hWnd, i),
				GWL_WNDPROC, (LONG)TabProc);

		}

		

		ComboEdit1 = subclass_combo_box(PodajNazweKomputera);
		ComboEdit2 = subclass_combo_box(ListaRozdzielczosc);
		ComboEdit3 = subclass_combo_box(ListaFormat);
		 
		hBrushStatic = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));

		MCIFrame = MCIWndCreate(Okno_Odtwarzacza, (HINSTANCE)ghInst, WS_CHILD | WS_VISIBLE | MCIWNDF_NOOPEN | MCIWNDF_NOTIFYALL, L"");

		Show(Okno_Odtwarzacza);
		
		
		MCIWndClose(MCIFrame);
		
		//MCIWndOpen(MCIFrame, L"D:\\Moje Dokumenty\\Moje wideo\\filmy\\bambi 2\\film.avi", 0);
		
		MoveWindow(MCIFrame, 0, 0, 320, 200, TRUE);

		StaraKontrolka[100] = (WNDPROC)SetWindowLong(Okno_Odtwarzacza, GWL_WNDPROC, (LONG)OdtwaraczProc);

		StaraKontrolka[101] = (WNDPROC)SetWindowLong(PodajLokalizacje, GWL_WNDPROC, (LONG)WalidacjaProc);

		StaraKontrolka[102] = (WNDPROC)SetWindowLong(PodajNazwePliku, GWL_WNDPROC, (LONG)WalidacjaNazwyProc);

		StaraKontrolka[103] = (WNDPROC)SetWindowLong(KOMUNIKAT, GWL_WNDPROC, (LONG)KomunikatProc);

//		StaraKontrolka[104] = (WNDPROC)SetWindowLong(LokalizacjaSciezki, GWL_WNDPROC, (LONG)WalidacjaProc);

		INITCOMMONCONTROLSEX iccex;
		iccex.dwICC = ICC_WIN95_CLASSES;
		iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		InitCommonControlsEx(&iccex);



		// dalej do WM_SETTINGCHANGE
	case WM_SETTINGCHANGE:
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &ulScrollLines, 0);

		// ulScrollLines usually equals 3 or 0 (for no scrolling)
		// WHEEL_DELTA equals 120, so iDeltaPerLine will be 40

		if (ulScrollLines)
			iDeltaPerLine = ulScrollLines;
		else
			iDeltaPerLine = 0;

		return 0;
	
	case WM_TAB:
		if (GetFocus() == ComboEdit1)
			SetFocus(GetDlgItem(hWnd, 3));
		else
			if (GetFocus() == ComboEdit2)
				SetFocus(GetDlgItem(hWnd, 9));
			else
				if (GetFocus() == ComboEdit3)
					SetFocus(GetDlgItem(hWnd, 1));
		break;
	case WM_CLOSE:
		if (Kontrola_Zapisu == 1)
		{
			MB1 = MessageBox(hWnd, TEXT("Czy zapisaæ zmiany?"), TEXT("Zamykanie programu"), MB_YESNOCANCEL | MB_ICONQUESTION);
		}
		switch (MB1)
		{
		case IDNO:

			return DefWindowProc(hWnd, message, wParam, lParam);

		case IDYES:
		{
			SendMessage(hWnd, WM_COMMAND, (WPARAM)ID_SAVE, 0);

			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		case IDCANCEL:

			return 0;
		}
		return 0;
	case WM_PAINT:
		hdc1 = BeginPaint(hWnd, &ps1);
	
		EndPaint(hWnd, &ps1);

		return 0;


	case WM_COMMAND:
		
		switch (LOWORD(wParam))
		{
		case IDM_FILE_SAVE:
			
			SendMessage(hWnd, WM_COMMAND, ID_SAVE, 0);
			return 0;

		case ID_PLIK_WYJ40006:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case ID_PLIK_ZAPISZ:
			SendMessage(hWnd,WM_COMMAND,ID_SAVE,0);
			break;

		case ID_OPJCE_WSZYSTKIEOPCJE:
			SendMessage(hWnd, WM_COMMAND, ID_PRZYCISK, 0);
			break;

		case ID_OPJCE_RESET:
			SendMessage(hWnd, WM_COMMAND, ID_RESET, 0);
			break;

		case ID_POMOC_OPROGRAMIE:
			DialogBox(hInstance, MAKEINTRESOURCE(103), hWnd, OProgramieDlgProc);
			break;
		}
		
		switch (wParam)
		{
		
		case ID_SAVE:
			if (Kontrola_Walidacji_K == 0 || (Kontrola_Walidacji_N == 0))
			{
				MessageBox(NULL, TEXT("Niepoprawna œcie¿ka Zapisu. Zmieñ dane w polach nowy katalog i lokalizacja folderu z filmem."), TEXT("B£¥D Zapisu")
						   , MB_ICONEXCLAMATION);
				SetWindowText(KOMUNIKAT, TEXT("Niepoprawna œcie¿ka Zapisu. Zmieñ dane w polach nowy katalog i lokalizacja folderu z filmem."));
				break;
			}
			
			wsprintf(sz_LokalizacjaZapisu, TEXT("%s"), UtworzNowyfolder(LokalizacjaSciezki, PodajNazwePliku));
			Plik = OtworzNowyPlik(sz_LokalizacjaZapisu, PodajNazweKomputera, CREATE_NEW);

			if (Plik != 0)
			{
				Kontrola_Zapisu = 0;

				if (!ZapisDanychdoPlikuA(0, s_Wczytana_Data, Plik))
				{
					break;
				}

				ZapisDanychdoPlikuA(PodajDate, 0, Plik);

				ZapisDanychdoPlikuA(PodajDateWpisu, 0, Plik);

				ZapisDanychdoPlikuA(0, " ;\r\n", Plik);

				ZapisDanychdoPlikuA(0, s_Wczytana_Lokalizacja, Plik);

				ZapisDanychdoPlikuA(PodajNazweKomputera, 0, Plik);

				ZapisDanychdoPlikuA(0, " : ", Plik);

				ZapisDanychdoPlikuA(0, " \"", Plik);

				ZapisDanychdoPlikuA(PodajLokalizacje, 0, Plik);

				ZapisDanychdoPlikuA(0, "\";\r\n", Plik);

				ZapisDanychdoPlikuA(0, s_Wczytane_Podfoldery, Plik);

				ZapisDanychdoPlikuA(PodajPodfoldery, 0, Plik);

				ZapisDanychdoPlikuA(0, " ;\r\n", Plik);

				ZapisDanychdoPlikuA(0, s_Wczytana_Rozdzielczosc, Plik);

				ZapisDanychdoPlikuA(ListaRozdzielczosc, 0, Plik);

				ZapisDanychdoPlikuA(0, " ;\r\n", Plik);

				ZapisDanychdoPlikuA(0, s_Wczytany_Format, Plik);

				ZapisDanychdoPlikuA(ListaFormat, 0, Plik);

				ZapisDanychdoPlikuA(0, " ;\r\n", Plik);

				//wsprintf(s_Komputer, TEXT("folder: %s znajdujê siê na %s"), TekstZKontrolki(PodajNazwePliku), TekstZKontrolki(PodajNazweKomputera));

				UtworzSkrot(PodajLokalizacje, sz_LokalizacjaZapisu, PodajNazweKomputera, PodajNazwePliku);

				ZamknijPlik(Plik);

				
			}
			else
			{
				ZamknijPlik(Plik);

				MB2 = MessageBox(hWnd, TEXT("Nie mo¿na otworzyæ pliku. Najprawdopodniej ju¿ istnieje.\nCzy chcesz spróbowaæ nadpisaæ plik?"),
					TEXT("Nadpisanie pliku"), MB_YESNO | MB_ICONQUESTION);

				if (MB2 == IDYES)
				{

					Kontrola_Zapisu = 0;

					Plik = OtworzNowyPlik(sz_LokalizacjaZapisu, PodajNazweKomputera, CREATE_ALWAYS);

					if (!ZapisDanychdoPlikuA(0, s_Wczytana_Data, Plik))
					{
						break;
					}

					ZapisDanychdoPlikuA(PodajDate, 0, Plik);

					ZapisDanychdoPlikuA(PodajDateWpisu, 0, Plik);

					ZapisDanychdoPlikuA(0, ";\r\n", Plik);

					ZapisDanychdoPlikuA(0, s_Wczytana_Lokalizacja, Plik);

					ZapisDanychdoPlikuA(PodajNazweKomputera, 0, Plik);

					ZapisDanychdoPlikuA(0, " : ", Plik);

					ZapisDanychdoPlikuA(0, " \"", Plik);

					ZapisDanychdoPlikuA(PodajLokalizacje, 0, Plik);

					ZapisDanychdoPlikuA(0, "\";\r\n", Plik);

					ZapisDanychdoPlikuA(0, s_Wczytane_Podfoldery, Plik);

					ZapisDanychdoPlikuA(PodajPodfoldery, 0, Plik);

					ZapisDanychdoPlikuA(0, " ;\r\n", Plik);

					ZapisDanychdoPlikuA(0, s_Wczytana_Rozdzielczosc, Plik);

					ZapisDanychdoPlikuA(ListaRozdzielczosc, 0, Plik);

					ZapisDanychdoPlikuA(0, " ;\r\n", Plik);

					ZapisDanychdoPlikuA(0, s_Wczytany_Format, Plik);

					ZapisDanychdoPlikuA(ListaFormat, 0, Plik);

					ZapisDanychdoPlikuA(0, " ;\r\n", Plik);

					//wsprintf(s_Komputer, TEXT("folder: %s znajdujê siê na %s"), TekstZKontrolki(PodajNazwePliku), TekstZKontrolki(PodajNazweKomputera));

					UtworzSkrot(PodajLokalizacje, sz_LokalizacjaZapisu, PodajNazweKomputera, PodajNazwePliku);

					ZamknijPlik(Plik);

					
				}
			}
//			ShellExecute(hWnd, L"open", L"H:\\DOM\\wspólne\\Katalog wideo", NULL, NULL, SW_SHOW);
			break;

		case ID_ZAPISZ_W:
			wcscpy_s(sz_Lokalizacja, WyborFolderu(NULL));

			SetWindowText(PodajLokalizacje, sz_Lokalizacja);
			break;

		case ID_LOKALIZACJA_ZAPISU:
			wcscpy_s(sz_Sciezka, WyborFolderu(NULL));

			SetWindowText(LokalizacjaSciezki, sz_Sciezka);

			break;
		
		case ID_PRZYCISK:
			
			DialogBox(hInstance, MAKEINTRESOURCE(110), hWnd, OpcjeDlgProc);
			
			
			break;

		case ID_WCZYTAJ:
			
				if (OdczytZPliku(sz_LokalizacjaOdczytu) != 0)
				{
					
					wcscpy_s(sz_BufornaTekstZKontrolki, OdczytZPliku(sz_LokalizacjaOdczytu));

					wcscpy_s(sz_UstawieniaKonfiguracyjne, sz_BufornaTekstZKontrolki);
					
					DzielNaKontrolki(sz_BufornaTekstZKontrolki, PodajNazweKomputera, ListaRozdzielczosc, ListaFormat, LokalizacjaSciezki, PodajPodfoldery);
					
					wcscpy_s(sz_BufornaTekstZKontrolki, sz_UstawieniaKonfiguracyjne);
				}
			
			break;

		case ID_WczytajZPliku:
			
			wcscpy_s(sz_LokalizacjaOdczytu, OknoOtwieraniePliku());

			SendMessage(hWnd, WM_COMMAND, ID_WCZYTAJ, 0);

			break;
		case ID_RESET:
			
			wcscpy_s(sz_UstawieniaKonfiguracyjne, sz_BufornaTekstZKontrolki);
			SendMessage(hWnd, WM_COMMAND, ID_CZYSC, 0);
			SendMessage(hWnd, WM_COMMAND, ID_WCZYTAJ_PONOWNIE, 0);
			break;
		case ID_WCZYTAJ_PONOWNIE:
			DzielNaKontrolki(sz_UstawieniaKonfiguracyjne, PodajNazweKomputera, ListaRozdzielczosc, ListaFormat, LokalizacjaSciezki, PodajPodfoldery);
			break;
		case ID_CZYSC:
			SendMessage(hWnd, WM_COMMAND, ID_CZYSC_COMBOBOX, 0);

			CzyscKontrolki(hWnd, PodajNazwePliku, PodajDate, PodajLokalizacje, PodajDateWpisu, LokalizacjaSciezki, PodajPodfoldery);
			break;
		case ID_ZAPISZ_OPCJE:
			OknoZapisywaniePliku(LokalizacjaSciezki, PodajNazwePliku, PodajDate);
			break;
		case ID_CZYSC_COMBOBOX:
			CzyscComboBox(PodajNazweKomputera, ListaRozdzielczosc, ListaFormat);
			break;
		case ID_NASTEPNY_WPIS:
			
			SendMessage(hWnd, WM_COMMAND, ID_RESET, 0);
			ZamknijPlik(Plik);
			Kontrola_Zapisu = 0;
			
			break;
		case ID_OTWORZ_KATALOG:
			Dlugosc_Katalogu = GetWindowTextLength(LokalizacjaSciezki);
			Bufor_na_Katalog = (LPWSTR)GlobalAlloc(GPTR, (Dlugosc_Katalogu + 1)*sizeof(TCHAR));
			GetWindowText(LokalizacjaSciezki, Bufor_na_Katalog, Dlugosc_Katalogu + 1);
			
			ShellExecute(hWnd, TEXT("open"), Bufor_na_Katalog, NULL, NULL, SW_SHOW);
			GlobalFree(Bufor_na_Katalog);
			break;
		

		case ID_PANORAMA:
			//MCIWndPlay(MCIFrame);
			
			MoveWindow(MCIFrame, 0, 0, 320, 180, TRUE); //16:9
			
			break;
		case ID_TRADYCYJNY:
			//MCIWndSM(MCIFrame, MCI_STOP, 0, 0);
			
			
			MoveWindow(MCIFrame, 0, 0, 266, 200, TRUE); //4:3

			break;
		case ID_CALE_POLE:
			MoveWindow(MCIFrame, 0, 0, 320, 200, TRUE); //Ca³e Pole
			break;
		case ID_WCZYTAJ_WIDEO:
			
			dwdlugosc = GetWindowTextLength(PodajLokalizacje);
			Bufor = (LPWSTR)GlobalAlloc(GPTR, (dwdlugosc + 1)*sizeof(TCHAR));
			GetWindowText(PodajLokalizacje, Bufor, dwdlugosc + 1);
			
			wcscpy_s(Bufor_Sciezki, OknoOtwieraniePliku(Bufor));
			
			MCIWndOpen(MCIFrame, Bufor_Sciezki, 0);
			SendMessage(hWnd, WM_COMMAND, ID_CALE_POLE, 0);
			GlobalFree( Bufor);
			
			break;
		case ID_KOMUNIKAT:
			CreateDialog(hInstance, MAKEINTRESOURCE(112), hWnd, KomunikatDlgProc);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);

			//Koniec WM_COMMAND
		}
		
	case WM_KEYDOWN:
		switch (wParam)
		{
// Obs³uga Pasków Przesuwu przez klawiaturê
		case VK_HOME:
			SendMessage(hwndMain, WM_VSCROLL, SB_TOP, 0);
			break;

		case VK_END:
			SendMessage(hWnd, WM_VSCROLL, SB_BOTTOM, 0);
			break;

		case VK_PRIOR:
			SendMessage(hWnd, WM_VSCROLL, SB_PAGEUP, 0);
			break;

		case VK_NEXT:
			SendMessage(hWnd, WM_VSCROLL, SB_PAGEDOWN, 0);
			break;

		case VK_UP:
			SendMessage(hWnd, WM_VSCROLL, SB_LINEUP, 0);
			break;

		case VK_DOWN:
			SendMessage(hWnd, WM_VSCROLL, SB_LINEDOWN, 0);
			break;

		case VK_LEFT:
			SendMessage(hWnd, WM_HSCROLL, SB_PAGEUP, 0);
			break;

		case VK_RIGHT:
			SendMessage(hWnd, WM_HSCROLL, SB_PAGEDOWN, 0);
			break;

		// Wyjœcie przez klawisz ESC
		case VK_ESCAPE:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}

		return 0;

		//case WM_SETFOCUS:
		//SetFocus(hwndMain);
		//break;

	case WM_DESTROY:
		DeleteObject(hBrushStatic);
		DeleteObject(hBrushEdit);

		MCIWndClose(MCIFrame);
		

		PostQuitMessage(0);
		return 0;

	} // end switch
	return DefWindowProc(hWnd, message, wParam, lParam);
} // end WndProc


LRESULT CALLBACK TabProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	int id = GetWindowLong(hwnd, GWL_ID);


	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_TAB)
		{
			
			SetFocus(GetDlgItem(GetParent(hwnd), (id + 1) % 12));
			
			return 0;
			
		}
		else
			if (wParam == VK_ESCAPE)
			{

				SetFocus(hwndMain);
				
			}
		break;
	case WM_SETFOCUS:
		idFocus = id;

		break;
	case WM_KEYUP:
	case WM_CHAR:
		
		switch (wParam)
		{
		case VK_TAB:
		case VK_ESCAPE:
		
			return 0;
		}
		Kontrola_Zapisu = 1;

	default:
		
		break;
	}
	
	
	// Call the original window procedure for default processing
	return CallWindowProc(StaraKontrolka[id], hwnd, msg, wParam, lParam);
}


LRESULT CALLBACK COMBOPROC(HWND hWndEdit, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_TAB:
			SendMessage(hwndMain, WM_TAB, 0, 0);
			return 0;
		case VK_ESCAPE:
			SetFocus(hwndMain);
		}
		break;

	case WM_KEYUP:
	case WM_CHAR:
		switch (wParam)
		{
		case VK_TAB:
		case VK_ESCAPE:
		
			return 0;
		}
		Kontrola_Zapisu = 1;

	default:
		
		break;
	}
	// Call the original window procedure for default processing
	return CallWindowProc(lpfnEditWndProc, hWndEdit, msg, wParam, lParam);
}

HWND subclass_combo_box(HWND hWndCombo)
{
	HWND    hWndEdit;


	hWndEdit = GetDlgItem(hWndCombo, 1001);

	// Change the window procedure for the Edit window 
	// to the subclass procedure.

	lpfnEditWndProc = (WNDPROC)SetWindowLong(hWndEdit, GWL_WNDPROC,
		(LONG)COMBOPROC);
	return hWndEdit;

}

BOOL CALLBACK OpcjeDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case WM_DESTROY:
		case ID_WYJDZ:
			EndDialog(hDlg, 0);
			return TRUE;
		case IDC_ODCZYT:
			SendMessage(GetParent(hDlg), msg, ID_WCZYTAJ, lParam);
			break;
		case IDC_RESET:
			SendMessage(GetParent(hDlg), msg, ID_RESET, lParam);
			break;
		case IDC_CZYSC:
			SendMessage(GetParent(hDlg), msg, ID_CZYSC, lParam);
			break;
		case IDC_ODCZYTZPLIKU:
			SendMessage(GetParent(hDlg), msg, ID_WczytajZPliku, lParam);
			break;
			
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK OProgramieDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case WM_DESTROY:
		case IDC_ZAMKNIJ:
			EndDialog(hDlg, 0);
			return TRUE;
		
		}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK OdtwaraczProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	
	HDC hdc_Okno_Odtwarzacza;
	HBRUSH Pedzel, Pudelko;
	HPEN Olowek, Piornik;

	switch (msg)
	{
	case WM_PAINT:
	
		hdc_Okno_Odtwarzacza = GetDC(hwnd);
		
		Pedzel = CreateSolidBrush(0xFFFFFF);
		Olowek = CreatePen(PS_INSIDEFRAME, 2, 0xFFFFFF);
		Pudelko = (HBRUSH)SelectObject(hdc_Okno_Odtwarzacza, Pedzel);
		Piornik = (HPEN)SelectObject(hdc_Okno_Odtwarzacza, Olowek);
		Rectangle(hdc_Okno_Odtwarzacza, 0, 200, 320, 0);
		SelectObject(hdc_Okno_Odtwarzacza, Pudelko);
		SelectObject(hdc_Okno_Odtwarzacza, Piornik);
		DeleteObject(Olowek);
		DeleteObject(Pedzel);
		InvalidateRect(hwnd, NULL, TRUE);
	
		ReleaseDC(hwnd, hdc_Okno_Odtwarzacza);
		
		break;
	
	

	default:

		break;
	}


	// Call the original window procedure for default processing
	return CallWindowProc(StaraKontrolka[100], hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK WalidacjaProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LPWSTR Bufor;
	
	switch (msg)
	{
		case WM_KILLFOCUS:
		{
		Bufor = PobierzTekstZKontrolki(hwnd);
		if (Bufor > 0)
		{
			Kontrola_Walidacji_K = WalidacjaDanych(Bufor, KOMUNIKAT, 0);
			SendMessage(GetParent(hwnd), msg, WM_CTLCOLOREDIT, lParam);

		}
		else
		{
			Kontrola_Walidacji_K = 0;
			SendMessage(GetParent(hwnd), msg, WM_CTLCOLOREDIT, lParam);
		}

		}
		break;
		case WM_SETFOCUS:
		{
			
			Kontrola_Walidacji_K = 1;
			SendMessage(GetParent(hwnd), msg, WM_CTLCOLOREDIT, lParam);
			
		}
		
	break;
	}
	if (PODAJLOKALIZACJE == hwnd)
	{
		return CallWindowProc(StaraKontrolka[101], hwnd, msg, wParam, lParam);
	}
	else
		if (LOKALIZACJASCIEZKI == hwnd)
		{
			return CallWindowProc(StaraKontrolka[104], hwnd, msg, wParam, lParam);
		}
	return 0;
}

LRESULT CALLBACK WalidacjaNazwyProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LPWSTR Bufor;

	switch (msg)
	{
		case WM_KILLFOCUS:
		{
		Bufor = PobierzTekstZKontrolki(hwnd);
		if (Bufor > 0)
		{
			Kontrola_Walidacji_N = WalidacjaDanych(Bufor, KOMUNIKAT, 1);
			SendMessage(GetParent(hwnd), msg, WM_CTLCOLOREDIT, lParam);
		}
		else
		{
			Kontrola_Walidacji_N = 0;
			SendMessage(GetParent(hwnd), msg, WM_CTLCOLOREDIT, lParam);
		}
		break;
		case WM_SETFOCUS:
		{

			Kontrola_Walidacji_N = 1;
			SendMessage(GetParent(hwnd), msg, WM_CTLCOLOREDIT, lParam);

		}
		}
	break;
	}

	return CallWindowProc(StaraKontrolka[102], hwnd, msg, wParam, lParam);
}

BOOL CALLBACK KomunikatDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:

		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case WM_DESTROY:
		case IDC_ZAMKNIJ:
			DestroyWindow(hDlg);
			return TRUE;

		}
		break;
	}
	return FALSE;
}


LRESULT CALLBACK KomunikatProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{

	HDC hdc_Okno_Komunikatu;
	HBRUSH Pedzel, Pudelko;
	HPEN Olowek, Piornik;

	switch (msg)
	{
	case WM_PAINT:

		hdc_Okno_Komunikatu = GetDC(hwnd);

		Pedzel = CreateSolidBrush(RGB(0, 152, 242));
		Olowek = CreatePen(PS_INSIDEFRAME, 2, RGB(255, 0, 0));
		Pudelko = (HBRUSH)SelectObject(hdc_Okno_Komunikatu, Pedzel);
		Piornik = (HPEN)SelectObject(hdc_Okno_Komunikatu, Olowek);
		Rectangle(hdc_Okno_Komunikatu, -2, -2, 202, 102);
		SelectObject(hdc_Okno_Komunikatu, Pudelko);
		SelectObject(hdc_Okno_Komunikatu, Piornik);
		DeleteObject(Olowek);
		DeleteObject(Pedzel);
		InvalidateRect(hwnd, NULL, TRUE);

		ReleaseDC(hwnd, hdc_Okno_Komunikatu);

		break;



	default:

		break;
	}


	// Call the original window procedure for default processing
	return CallWindowProc(StaraKontrolka[103], hwnd, msg, wParam, lParam);
}