//I N C L U D E S
///////////////////////////////////////////////////////////////////
#include "Nag³ówek g³ówny.h"


// F U N K C J E
///////////////////////////////////////////////////////////////////
LPWSTR UtworzNowyfolder(HWND UchwytKontrolki1, HWND UchwytKontrolki2)
{
	static TCHAR szBuffer1[1000], szBuffer2[1000], szBuffer3[1000];
	int dlugosc1 = GetWindowTextLength(UchwytKontrolki1);
	int dlugosc2 = GetWindowTextLength(UchwytKontrolki2);

	

	if (dlugosc1 > 0 && dlugosc2 > 0)
	{
		if (dlugosc1 == 8)
		{
			dlugosc1 = 9;
		}

		if (dlugosc2 == 8)
		{
			dlugosc2 = 9;
		}

		GetWindowText(UchwytKontrolki1, szBuffer1, dlugosc1 + 1);
		GetWindowText(UchwytKontrolki2, szBuffer2, dlugosc2 + 1);
		
		wsprintf(szBuffer3, TEXT("%s\\%s"), szBuffer1, szBuffer2);
		CreateDirectory(szBuffer3, NULL);
	}
	return szBuffer3;
}

HANDLE OtworzNowyPlik(LPWSTR Lokalizacja, HWND Komputer, DWORD Distribution)
{
	static HANDLE hPlik;

	int dlugosc1 = wcslen(Lokalizacja);
	int dlugosc2 = GetWindowTextLengthA(Komputer);
	if (dlugosc1 > 0 && dlugosc2 > 0)
	{
		
		static TCHAR szKomputer[1000];
		GetWindowText(Komputer, szKomputer, dlugosc2 + 1);


		static TCHAR katalog[1000];
		static TCHAR sciezka[] = TEXT("%s\\Dane-%s.txt");

		wsprintf(katalog, sciezka, Lokalizacja, szKomputer);

		hPlik = CreateFile(katalog, GENERIC_WRITE, 0, NULL, Distribution, 0, NULL); //Otwórz plik
		if (hPlik == INVALID_HANDLE_VALUE)
		{
			MessageBoxA(NULL, "Ÿle otwarty plik", "B£¥D", MB_ICONEXCLAMATION);
			return 0;
		}

	}

	return hPlik;
}

HANDLE OtworzNowyPlikKonfiguracyjny(LPWSTR Lokalizacja, DWORD Distribution)
{
	static HANDLE hPlik;
	int dlugosc1 = wcslen(Lokalizacja);
	if (dlugosc1 > 0)
	{
		static TCHAR katalog[1000];
		hPlik = CreateFile(Lokalizacja, GENERIC_WRITE, 0, NULL, Distribution, 0, NULL); //Otwórz plik
		if (hPlik == INVALID_HANDLE_VALUE)
		{
			MessageBox(NULL, TEXT("Ÿle otwarty plik"), TEXT("B£¥D"), MB_ICONEXCLAMATION);
			return 0;
		}

		
	}

	return hPlik;
}

int ZapisDanychdoPlikuA(HWND UchwytKontrolki, LPSTR TekstDoSkopiowania, HANDLE UchwytdoPliku)
{
	DWORD dwRozmiar, dwZapisane;
	LPSTR Bufor;
	//Wybór Ÿród³a na d³ugoœæ
	if (UchwytKontrolki != 0)
	{
		dwRozmiar = GetWindowTextLengthA(UchwytKontrolki);
	}
	else
	{
		dwRozmiar = strlen(TekstDoSkopiowania);

	}
	//Test d³ugoœci
	if (dwRozmiar == 0)
	{
		
		return 0;
	}
	//Alokowanie pamiêci na Bufor
	Bufor = (LPSTR)GlobalAlloc(GPTR, dwRozmiar + 1);
	if (Bufor == NULL)
	{
		MessageBoxA(NULL, "Za ma³o pamiêci!", "B£¥D", MB_ICONEXCLAMATION);
		return 0;
	}

	//Zape³nianie bufora
	if (UchwytKontrolki != 0)
	{
		GetWindowTextA(UchwytKontrolki, Bufor, dwRozmiar + 1); // skopiuj do bufora tekst z UchwytKontrolki
		Bufor[dwRozmiar] = 0; // dodaj zero na koñcu stringa
	}
	else
	{
		Bufor = TekstDoSkopiowania;
	}

	//Zapis danych
	if (!WriteFile(UchwytdoPliku, Bufor, dwRozmiar, &dwZapisane, NULL))
	{
		MessageBoxA(NULL, "B³¹d zapisu do pliku", "B£¥D", MB_ICONEXCLAMATION);

		ZamknijPlik(UchwytdoPliku);

		return 0;
	}

	
	return 1;
}

LPWSTR OdczytZPliku(LPWSTR KatalogOdczytu)
{
	LPSTR Bufor;
	LPWSTR Wynik;
	DWORD dwRozmiarA, dwRozmiarW, dwPrzeczyt;
	HANDLE hPlik;

	//Otwiera plik
	hPlik = CreateFile(KatalogOdczytu, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hPlik == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, TEXT("Nie mo¿na wczytaæ ustawieñ"), TEXT("B³¹d odczytu"), MB_ICONEXCLAMATION);
		return 0;
	}

	//Sprawdza d³ugoœæ pliku
	dwRozmiarA = GetFileSize(hPlik, NULL);
	if (dwRozmiarA == 0xFFFFFFFF) {
		MessageBox(NULL, TEXT("Nieprawid³owy rozmiar pliku!"), TEXT("B³¹d"), MB_ICONEXCLAMATION);
		return 0;
	}

	dwRozmiarW = 2 * dwRozmiarA;

	//Alokuje pamiêæ na bufor
	Bufor = (LPSTR)GlobalAlloc(GPTR, dwRozmiarA + 1);
	Wynik = (LPWSTR)GlobalAlloc(GPTR, dwRozmiarW + 1);
	if (Bufor == NULL) {
		MessageBox(NULL, TEXT("Za ma³o pamiêci!"), TEXT("B³¹d"), MB_ICONEXCLAMATION);
		return 0;
	}

	//Wczytuje dane z pliku
	if (!ReadFile(hPlik, Bufor, dwRozmiarA, &dwPrzeczyt, NULL)) {
		MessageBox(NULL, TEXT("B³¹d czytania z pliku"), TEXT("B³¹d"), MB_ICONEXCLAMATION);
		return 0;
	}

	Bufor[dwRozmiarA] = 0; // dodaje zero do koñca stringa
	
	MultiByteToWideChar(CP_ACP, 0, Bufor, -1, Wynik, dwRozmiarW); //Konwertuje z ASCII do UNICODE


	GlobalFree(Bufor); // Zwolnia bufor
	CloseHandle(hPlik); // Zamyka plik

	return Wynik;
}

HRESULT UtworzSkrot(HWND MiejscePochodzenia, LPWSTR Lokalizacja, HWND Komputer, HWND NazwaPliku)
{
	int dlugosc1 = wcslen(Lokalizacja);
	int dlugosc2 = GetWindowTextLength(Komputer);
	int dlugosc3 = GetWindowTextLength(NazwaPliku);
	int dlugosc4 = GetWindowTextLength(MiejscePochodzenia);
	if (dlugosc1 > 0 && dlugosc2 > 0 && dlugosc3 > 0 && dlugosc4 > 0)
	{
		
		static TCHAR szKomputer[1000] = TEXT("b³¹d");
		
		if (dlugosc2 == 8)
		{
			dlugosc2 = 9;
		}
		GetWindowText(Komputer, szKomputer, dlugosc2 + 1);

		static TCHAR szNazwa[1000] = TEXT("b³¹d");
		
		if (dlugosc3 == 8)
		{
			dlugosc3 = 9;
		}
		GetWindowText(NazwaPliku, szNazwa, dlugosc3 + 1);
		
		TCHAR szwMiejscePochodzenia[1000] = TEXT("b³¹d");
		
		if (dlugosc4 == 8)
		{
			dlugosc4 = 9;
		}
		GetWindowText(MiejscePochodzenia, szwMiejscePochodzenia, dlugosc4 + 1);
		
		static TCHAR wsz[MAX_PATH];
		static TCHAR sciezka[] = TEXT("%s\\%s — %s.lnk");

		wsprintf(wsz, sciezka, Lokalizacja, szNazwa, szKomputer);



		HRESULT hres;
		IShellLink* psl;

		CoInitialize(NULL);
		// Get a pointer to the IShellLink interface. It is assumed that CoInitialize
		// has already been called.
		hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);

		if (SUCCEEDED(hres))
		{
			IPersistFile* ppf;

			// Set the path to the shortcut target and add the description. 
			psl->SetPath(szwMiejscePochodzenia);
			

			// Query IShellLink for the IPersistFile interface, used for saving the 
			// shortcut in persistent storage. 
			hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

			if (SUCCEEDED(hres))
			{

				// Save the link by calling IPersistFile::Save. 
				hres = ppf->Save(wsz, TRUE);

				ppf->Release();
			}
			psl->Release();
		}

		return hres;
	}
	else
	{
		return 0;
	}

}

int WstawTekstDoKontrolki(HWND UchwytComboBox, LPWSTR TekstDoKontrolki)
{
	if (SendMessage(UchwytComboBox, CB_ADDSTRING, 0, (LPARAM)TekstDoKontrolki) < 0)
	{
		MessageBox(NULL, TEXT("Nie uda³o siê wys³aæ tekstu do Kontrolki"), TEXT("B£¥D"), 0);
		return -1;
	}
	return 1;
}

INT ZamknijPlik(HANDLE hPlik)
{
	
	if (CloseHandle(hPlik))
	{
		return 1;
	}
	else
		return 0;
}

LPWSTR TekstZKontrolki(HWND UchwytKontrolki)
{
	int dlugosc = ComboBox_GetTextLength(UchwytKontrolki);
	LPWSTR NapisZcomboBoxa = (LPWSTR)GlobalAlloc(GPTR, dlugosc + 1);
	ComboBox_GetText(UchwytKontrolki, NapisZcomboBoxa, dlugosc + 1);

	return NapisZcomboBoxa;
}

LPWSTR WyborFolderu(HWND hWnd)
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	BROWSEINFO   bi;
	ZeroMemory(&bi, sizeof(bi));
	TCHAR   szDisplayName[MAX_PATH];
	static TCHAR szPusty[] = TEXT("");
	szDisplayName[0] = ' ';

	bi.hwndOwner = hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDisplayName;
	bi.lpszTitle = TEXT("Wybierz folder zapisu :");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lParam = NULL;
	bi.iImage = 0;

	LPITEMIDLIST   pidl = SHBrowseForFolder(&bi);
	static TCHAR   szPathName[MAX_PATH];
	if (NULL != pidl)
	{
		BOOL bRet = SHGetPathFromIDList(pidl, szPathName);
		if (FALSE == bRet)
			return szPusty;
		return szPathName;
	}
	return szPusty;
}

VOID CzyscComboBox(HWND KontrolkaComboBox1,...)
{
	//inicjalizuje listê argumentów
	va_list ListArg;
	HWND WykonywanaKontrolka = KontrolkaComboBox1;
	// wskazuje na pierwszy argument do wykonania
	va_start(ListArg, KontrolkaComboBox1);
	while (WykonywanaKontrolka != 0) 
	{

		SendMessage(WykonywanaKontrolka, CB_RESETCONTENT, 0, 0);
		//³aduje kolejn¹ kontrolkê
		WykonywanaKontrolka = va_arg(ListArg, HWND);

	}
	// zamyka listê argumentów
	va_end(ListArg);
}

VOID CzyscKontrolki(HWND OknoMatka, HWND Kontrolka1,...)
{
	//inicjalizuje listê argumentów
	va_list ListArg;
	HWND WykonywanaKontrolka = Kontrolka1;
	// wskazuje na pierwszy argument do wykonania
	va_start(ListArg, Kontrolka1);
	while (WykonywanaKontrolka != 0)
	{

		SendMessage(WykonywanaKontrolka, WM_SETTEXT, 0, LPARAM(TEXT("")));
		//³aduje kolejn¹ kontrolkê
		WykonywanaKontrolka = va_arg(ListArg, HWND);

	}
	// zamyka listê argumentów
	va_end(ListArg);

	SendMessage(OknoMatka, WM_SETTEXT, 0, LPARAM(TEXT("Program do Katalogów")));
}


VOID DzielWczytaneDane(LPWSTR TablicaZnakow, HWND UchwytKontrolki)
{
	
	wchar_t * pwc;
	wchar_t *next_token1 = NULL;
	wchar_t *next_token2 = NULL;
	wchar_t znak = L'/';
	wchar_t znak2 = L'>';
	
	
	pwc = wcstok_s(TablicaZnakow, L";\r\n", &next_token1);
	
	while (pwc != NULL)
	{
		
		if (pwc[0] != znak)
		{
			WstawTekstDoKontrolki(UchwytKontrolki, pwc);
		}
		if (pwc[0] == znak2)
		{
			pwc = wcstok_s(pwc, L">", &next_token2);
			SetWindowText(UchwytKontrolki, pwc);
		}
		pwc = wcstok_s(NULL, L";\r\n", &next_token1);
		
	} 
	
}

VOID DzielNaKontrolki(LPWSTR TablicaZnakow, HWND Kontrolka1,...)
{
	//inicjalizuje listê argumentów
	va_list ListArg;
	wchar_t * pwc;
	wchar_t *next_token1 = NULL;
	wchar_t znak = L'/';

	HWND WykonywanaKontrolka = Kontrolka1;

	va_start(ListArg, Kontrolka1);

	pwc = wcstok_s(TablicaZnakow, L"?", &next_token1);
	
	if (WykonywanaKontrolka != 0)
	{
		while (pwc != NULL)
		{
			
			DzielWczytaneDane(pwc, WykonywanaKontrolka);
			
			pwc = wcstok_s(NULL, L"?", &next_token1);
			
			//³aduje kolejn¹ kontrolkê
			WykonywanaKontrolka = va_arg(ListArg, HWND);
		}

	}

	// zamyka listê argumentów
	va_end(ListArg);

}
TCHAR * OknoOtwieraniePliku()
{                                       
	OPENFILENAME ofn;
	TCHAR szNazwaPliku[MAX_PATH] = TEXT("B³¹d");
	
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFilter = TEXT("Pliki konfiguracyjne (*.ini)\0*.ini\0Wszystkie pliki\0*.*\0");
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFile = szNazwaPliku;
	ofn.lpstrDefExt = TEXT("ini");
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	if (GetOpenFileName(&ofn))
	{
		return szNazwaPliku;
	}
	else
		MessageBox(NULL, TEXT("Nie uda³o siê odczytaæ pliku!"), TEXT("B£¥D"), MB_ICONEXCLAMATION);
	return szNazwaPliku;
}

INT OknoZapisywaniePliku(HWND Kontrolka1,...)
{
	OPENFILENAME ofn;
	TCHAR szNazwaPliku[MAX_PATH] = TEXT("");
	HANDLE Plik;
	HWND WykonywanaKontrolka = Kontrolka1;
	char MultiByte[1000];
	TCHAR TekstUnicodeZKontrolki[1000];

	//inicjalizuje listê argumentów
	va_list ListArg;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFilter = L"Pliki tekstowe (*.ini)\0*.ini\0Wszystkie pliki\0*.*\0";
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFile = szNazwaPliku;
	ofn.lpstrDefExt = L"ini";
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;

	// wskazuje na pierwszy argument do wykonania
	va_start(ListArg, Kontrolka1);

	if (GetSaveFileName(&ofn))
	{
		
		Plik = OtworzNowyPlikKonfiguracyjny(szNazwaPliku, CREATE_ALWAYS);
		while (WykonywanaKontrolka != 0)
		{
			
			wcscpy_s(TekstUnicodeZKontrolki, TekstZKontrolki(WykonywanaKontrolka) );

			
			WideCharToMultiByte(CP_ACP, 0, TekstUnicodeZKontrolki, -1, MultiByte, 1000, NULL, NULL);
			ZapisDanychdoPlikuA(0, MultiByte, Plik);
			//Wczytuje kolejn¹ kontrolkê
			WykonywanaKontrolka = va_arg(ListArg, HWND);
			
		}
		
		ZamknijPlik(Plik);
		va_end(ListArg);
		return 1;
	}
	else
	{
		MessageBox(NULL, TEXT("Nie uda³o siê odczytaæ pliku!"), TEXT("B£¥D"), MB_ICONEXCLAMATION);
		va_end(ListArg);
		return 0;
	}
}

TCHAR * OknoOtwieraniePliku(LPWSTR Lokalizacja_Sciezki)
{
	OPENFILENAME ofn;
	TCHAR szNazwaPliku[MAX_PATH] = TEXT("B³¹d");

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFilter = TEXT("Pliki Wideo \0*.avi;*.mpg;*.rmvb;*.wmv\0Wszystkie pliki\0*.*\0");
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFile = szNazwaPliku;
	ofn.lpstrDefExt = NULL;
	ofn.lpstrInitialDir = Lokalizacja_Sciezki;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	if (GetOpenFileName(&ofn))
	{
		return szNazwaPliku;
	}
	else
		MessageBox(NULL, TEXT("Nie uda³o siê odczytaæ pliku!"), TEXT("B£¥D"), MB_ICONEXCLAMATION);
	return szNazwaPliku;
}

INT WalidacjaDanych(LPWSTR SciezkaDoFolderu,HWND UchwytOkna, INT WersjaProgramu) // Wersja Programu: - 0 dla sprawdzania ca³ego adresu; 
																				 //					 - 1 dla sprawdzania tylko nazwy folderu
{
	std::wstring string_a;


	WCHAR *Tablica[7];
	size_t znalezionaPozycja = NULL;
	int i = 0;
	std::wregex wzorzec(TEXT("[a-zA-Z]"));
	std::wsmatch wynik;

	Tablica[0] = L"*";
	Tablica[1] = L"?";
	Tablica[2] = L"\"";
	Tablica[3] = L"<";
	Tablica[4] = L">";
	Tablica[5] = L"|";
	

	if (SciezkaDoFolderu == 0)
	{
		//MessageBox(NULL, TEXT("Niewprowadzono œcie¿ki!"), TEXT("B£¥D"), MB_ICONEXCLAMATION);
		SetWindowText(UchwytOkna, TEXT("Niewprowadzono œcie¿ki!")); 
		return 0;
	}
	string_a = SciezkaDoFolderu;

	


	// Przerwij program jeœli w adresie znajduj¹ siê niedozwolone znaki:
	while (i < 6)
	{

		znalezionaPozycja = string_a.find(Tablica[i]);

		if (znalezionaPozycja != std::string::npos)
		{
			//MessageBox(NULL, TEXT("B³êdna œcie¿ka folderu. Wykryto niepoprawny znak! Znaki niedozwolone to: *, ?, <, >, \", |"), TEXT("B£¥D"), MB_ICONEXCLAMATION);
			SetWindowText(UchwytOkna, TEXT("B³êdna œcie¿ka folderu. Wykryto niepoprawny znak! Znaki niedozwolone to: * ? < > \" |"));
			return 0;
		}

		i++;

	}

	//Jeœli adres jest pe³n¹ œcie¿k¹ dostêpu
	if (WersjaProgramu == 0)
	{
		if (!std::regex_search(string_a, wynik, wzorzec))
		{

			//MessageBox(NULL, TEXT("B³êdna litera dysku!"), TEXT("B£¥D"), MB_ICONEXCLAMATION);
			SetWindowText(UchwytOkna, TEXT("B³êdna litera dysku!"));
			return 0;
		}
		//Oraz jeœli adres nie jest adresem sieciowym wykonaj:
		if ((string_a[0] != '\\') || (string_a[1] != '\\'))
		{

			if (string_a[1] != ':')
				{
					//MessageBox(NULL, TEXT("Niepoprawna œcie¿ka folderu!"), TEXT("B£¥D"), MB_ICONEXCLAMATION);
					SetWindowText(UchwytOkna, TEXT("Niepoprawna œcie¿ka folderu!"));
					return 0;
				}

			if (string_a[2] != '\\')
				if (string_a[2] != '/')
				{
					//MessageBox(NULL, TEXT("Niepoprawna œcie¿ka folderu!"), TEXT("B£¥D"), MB_ICONEXCLAMATION);
					SetWindowText(UchwytOkna, TEXT("Niepoprawna œcie¿ka folderu!"));
					return 0;
				}
		}
	}
	if (WersjaProgramu == 1)
	{
		Tablica[6] = L"\\";

		znalezionaPozycja = string_a.find(Tablica[6]);

		if (znalezionaPozycja != std::string::npos)
		{
			//MessageBox(NULL, TEXT("B³êdna œcie¿ka folderu. Wykryty znak to \\. Jeœli chcesz zmieniæ folder zapisu uczyñ to w polu Lokalizacji Katalogu", TEXT("B£¥D"), MB_ICONEXCLAMATION);
			SetWindowText(UchwytOkna, L"B³êdna œcie¿ka folderu. Wykryty znak to \\. Jeœli chcesz zmieniæ folder zapisu uczyñ to w polu Lokalizacji Katalogu");
			return 0;
		}
	}


	if (znalezionaPozycja == std::string::npos)
	{
		SetWindowText(UchwytOkna, TEXT(""));
		return 1;
	}
	return 0;
}

LPWSTR PobierzTekstZKontrolki(HWND UchwytKontrolki)
{
	DWORD dwRozmiar, WCharRozmiar;
	LPWSTR Bufor;

	//Sprawdzanie d³ugoœci tekstu
	dwRozmiar = GetWindowTextLength(UchwytKontrolki);

	WCharRozmiar = 2 * dwRozmiar;

	//Test d³ugoœci
	if (WCharRozmiar == 0)
	{

		return 0;
	}

	//Alokowanie pamiêci na Bufor
	Bufor = (LPWSTR)GlobalAlloc(GPTR, WCharRozmiar + 1);
	if (Bufor == NULL)
	{
		MessageBoxA(NULL, "Za ma³o pamiêci!", "B£¥D", MB_ICONEXCLAMATION);
		return 0;
	}

	//Zape³nianie bufora
	if (UchwytKontrolki != 0)
	{
		GetWindowText(UchwytKontrolki, Bufor, WCharRozmiar + 1); // skopiuj do bufora tekst z Kontrolki
		Bufor[WCharRozmiar] = 0; // dodaj zero na koñcu stringa
	}

	return Bufor;
}

void CreateTooltip(HWND hParent, LPWSTR Text)
{
	HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, WS_POPUP |
		TTS_NOPREFIX | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hParent, NULL,
		(HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE), NULL);

	SetWindowPos(hwndTT, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	TOOLINFO ti;
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_SUBCLASS | TTF_IDISHWND;
	ti.hwnd = hParent;
	ti.hinst = NULL;
	ti.uId = (UINT_PTR)hParent;
	ti.lpszText = (LPWSTR)Text;

	RECT rect;
	GetClientRect(hParent, &rect);

	ti.rect.left = rect.left;
	ti.rect.top = rect.top;
	ti.rect.right = rect.right;
	ti.rect.bottom = rect.bottom;

	SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)& ti);
} // koniec funkcji CreateTooltip()