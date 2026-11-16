/* Sealbox: installation password prompt. Copyright (c) 2026 Shubh Ventures. */

#include "Tcdefs.h"
#include <windows.h>
#include <wincrypt.h>
#include <strsafe.h>
#include "Resource.h"
#include "SealboxLicense.h"

#pragma comment (lib, "advapi32.lib")

static const char* ExpectedHash = SEALBOX_INSTALL_PASSWORD_SHA256;

/* Returns TRUE when the SHA-256 of the UTF-8 password matches ExpectedHash. */
static BOOL SealboxPasswordMatches (const wchar_t* password)
{
	HCRYPTPROV prov = 0;
	HCRYPTHASH hash = 0;
	BYTE digest[32];
	DWORD digestLen = sizeof (digest);
	char utf8[1024];
	char hex[65];
	int len, i;
	BOOL ok = FALSE;

	len = WideCharToMultiByte (CP_UTF8, 0, password, -1, utf8, sizeof (utf8), NULL, NULL);
	if (len <= 0)
		return FALSE;

	if (!CryptAcquireContext (&prov, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
		return FALSE;

	if (CryptCreateHash (prov, CALG_SHA_256, 0, 0, &hash)
		&& CryptHashData (hash, (const BYTE*) utf8, len - 1, 0)
		&& CryptGetHashParam (hash, HP_HASHVAL, digest, &digestLen, 0))
	{
		for (i = 0; i < 32; i++)
			StringCchPrintfA (hex + i * 2, 3, "%02x", digest[i]);
		ok = (_stricmp (hex, ExpectedHash) == 0);
	}

	if (hash) CryptDestroyHash (hash);
	if (prov) CryptReleaseContext (prov, 0);
	SecureZeroMemory (utf8, sizeof (utf8));
	return ok;
}

static INT_PTR CALLBACK SealboxPasswordDlgProc (HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int attempts = 0;

	switch (msg)
	{
	case WM_INITDIALOG:
		attempts = 0;
		SendDlgItemMessage (hwndDlg, IDC_SEALBOX_PASSWORD, EM_LIMITTEXT, 256, 0);
		SetFocus (GetDlgItem (hwndDlg, IDC_SEALBOX_PASSWORD));
		return 0;

	case WM_COMMAND:
		if (LOWORD (wParam) == IDOK)
		{
			wchar_t pwd[257];
			GetDlgItemTextW (hwndDlg, IDC_SEALBOX_PASSWORD, pwd, ARRAYSIZE (pwd));
			if (SealboxPasswordMatches (pwd))
			{
				SecureZeroMemory (pwd, sizeof (pwd));
				EndDialog (hwndDlg, IDOK);
				return 1;
			}
			SecureZeroMemory (pwd, sizeof (pwd));
			attempts++;
			if (attempts >= 5)
			{
				MessageBoxW (hwndDlg, L"Too many incorrect attempts. Setup will now close.", L"Sealbox Setup", MB_ICONERROR);
				EndDialog (hwndDlg, IDCANCEL);
				return 1;
			}
			MessageBoxW (hwndDlg, L"The installation password is incorrect.\n\nIf you purchased Sealbox, please use the password you received from Shubh Ventures (shubhventures.co.in).", L"Sealbox Setup", MB_ICONEXCLAMATION);
			SetDlgItemTextW (hwndDlg, IDC_SEALBOX_PASSWORD, L"");
			SetFocus (GetDlgItem (hwndDlg, IDC_SEALBOX_PASSWORD));
			return 1;
		}
		if (LOWORD (wParam) == IDCANCEL)
		{
			EndDialog (hwndDlg, IDCANCEL);
			return 1;
		}
		return 0;

	case WM_CLOSE:
		EndDialog (hwndDlg, IDCANCEL);
		return 1;
	}
	return 0;
}

/* Shows the password prompt. Returns TRUE when setup may continue. */
BOOL SealboxAskInstallPassword (HINSTANCE hInstance)
{
	if (ExpectedHash[0] == 0)
		return TRUE;	/* no password configured */

	return DialogBoxParamW (hInstance, MAKEINTRESOURCEW (IDD_SEALBOX_PASSWORD), NULL, SealboxPasswordDlgProc, 0) == IDOK;
}
