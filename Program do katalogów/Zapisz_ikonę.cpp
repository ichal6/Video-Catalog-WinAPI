#include "Zapisz_ikonê.h"



void SaveIconToFile(HICON hico, LPCSTR szFileName, BOOL bAutoDelete )
{
	PICTDESC pd = { sizeof(pd), PICTYPE_ICON };
	pd.icon.hicon = hico;

	CComPtr<IPicture> pPict = NULL;
	CComPtr<IStream>  pStrm = NULL;
	LONG cbSize = 0;

	BOOL res = FALSE;

	res = SUCCEEDED( ::CreateStreamOnHGlobal(NULL, TRUE, &pStrm) );
	res = SUCCEEDED( ::OleCreatePictureIndirect(&pd, IID_IPicture, bAutoDelete, (void**)&pPict) );
	res = SUCCEEDED( pPict->SaveAsFile( pStrm, TRUE, &cbSize ) );

	if( res )
	{
		// rewind stream to the beginning
		LARGE_INTEGER li = {0};
		pStrm->Seek(li, STREAM_SEEK_SET, NULL);

		// write to file
		HANDLE hFile = ::CreateFileA(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
		if( INVALID_HANDLE_VALUE != hFile )
		{
			DWORD dwWritten = 0, dwRead = 0, dwDone = 0;
			BYTE  buf[10000];
			while( dwDone < cbSize )
			{
				if( SUCCEEDED(pStrm->Read(buf, sizeof(buf), &dwRead)) )
				{
					::WriteFile(hFile, buf, dwRead, &dwWritten, NULL);
					if( dwWritten != dwRead )
						break;
					dwDone += dwRead;
				}
				else
					break;
			}

			_ASSERTE(dwDone == cbSize);
			::CloseHandle(hFile);
		}
	}
}

