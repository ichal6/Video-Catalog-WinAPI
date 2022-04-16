//Includes

#include <Windows.h>
#include <Shobjidl.h>
#include <winnls.h>
#include <objbase.h>
#include <objidl.h>
#include <shlguid.h> 
#include <Windowsx.h>
#include <StdIO.h>
#include <TCHAR.H>
#include <Commctrl.h>
#include <Shlobj.h>
#include <TCHAR.H>
#include <StdIO.h>
#include <iostream>
#include <vfw.h>
#include <regex>



//Funkcje
LPWSTR UtworzNowyfolder(HWND UchwytKontrolki1, HWND UchwytKontrolki2);

HANDLE OtworzNowyPlik(LPWSTR Lokalizacja, HWND Komputer, DWORD Distribution);

int ZapisDanychdoPlikuA(HWND UchwytKontrolki, LPSTR TekstDoSkopiowania, HANDLE UchwytdoPliku);

HRESULT UtworzSkrot(HWND MiejscePochodzenia, LPWSTR Lokalizacja, HWND Komputer, HWND NazwaPliku);

int WstawTekstDoKontrolki(HWND UchwytComboBox, LPWSTR TekstDoKontrolki);

INT ZamknijPlik(HANDLE hPlik);

LPWSTR TekstZKontrolki(HWND UchwytKontrolki);

LPWSTR WyborFolderu(HWND hWnd);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK TabProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK COMBOPROC(HWND hWndEdit, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK OdtwaraczProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND subclass_combo_box(HWND hWndCombo);

LPWSTR OdczytZPliku(LPWSTR KatalogOdczytu);

VOID CzyscComboBox(HWND KontrolkaComboBox1, ...);

VOID CzyscKontrolki(HWND OknoMatka, HWND Kontrolka1, ...);

VOID DzielWczytaneDane(LPWSTR TablicaZnakow, HWND UchwytKontrolki);

VOID DzielNaKontrolki(LPWSTR TablicaZnakow, HWND Kontrolka1, ...);

TCHAR * OknoOtwieraniePliku();

TCHAR * OknoOtwieraniePliku(LPWSTR Lokalizacja_Sciezki);

INT OknoZapisywaniePliku(HWND Kontrolka1, ...);

HANDLE OtworzNowyPlikKonfiguracyjny(LPWSTR Lokalizacja, DWORD Distribution);

BOOL CALLBACK OpcjeDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK OProgramieDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

INT WalidacjaDanych(LPWSTR SciezkaDoFolderu, HWND UchwytOkna, INT WersjaProgramu);

LRESULT CALLBACK WalidacjaProc(HWND hwnd, UINT mesg, WPARAM wParam, LPARAM lParam);

LPWSTR PobierzTekstZKontrolki(HWND UchwytKontrolki);

LRESULT CALLBACK WalidacjaNazwyProc(HWND hwnd, UINT mesg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK KomunikatDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

void CreateTooltip(HWND hParent, LPWSTR Text);

LRESULT CALLBACK KomunikatProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);