# Sealbox — branding notes

Sealbox is a rebranded build of VeraCrypt (https://veracrypt.jp), which is itself
based on TrueCrypt 7.1a. The rebrand was applied on 2026-11-15.

Product name:  Sealbox
Company:       Shubh Ventures  (https://shubhventures.co.in)
Copyright:     Copyright (C) 2026 Shubh Ventures. All rights reserved.
               (for Shubh Ventures' modifications and the product as a whole)

## What was changed (user-visible only)

- Application / window titles, dialogs, menus, message boxes and UI strings
  (src/Common/Language.xml and all Translations/Language.*.xml)
- Windows dialog resources and version info in every .rc file:
  ProductName, FileDescription, LegalTrademarks, CompanyName, new LegalCopyright
- About box text (Windows: src/Common/Dlgcode.c; Linux/macOS: src/Main/Forms/AboutDialog.cpp)
- "Released by ..." line, homepage and app link (src/Common/Tcdefs.h)
- Installer / uninstaller text and MSI manufacturer (src/Setup, src/SetupDLL,
  src/Release/Setup Files/*.wxs, Strings-en.wxl)
- Linux/FreeBSD .desktop entries, DEB/RPM vendor + contact (src/Build/CMakeLists.txt)
- BIOS boot loader banner (src/Boot/Windows/BootMain.cpp)
- HTML documentation in doc/html (en, ru, zh-cn): all page text, titles, meta
  descriptions and image alt text. Links, file names and the licence page are untouched.
- README.md and src/Readme.txt headers
- All icons and bitmaps, generated from src/Resources/Icons/Sealbox-logo.png:
  Windows .ico (app, volume, mounted, setup), About-box textual logo, main-window
  and Expander logos, setup banner, Volume Creation Wizard sidebar, Linux PNG set,
  XPM, SVG and macOS .icns. File names were kept so the build needs no changes.

## What was deliberately NOT changed

- Executable, driver, DLL and .sys file names (VeraCrypt.exe, veracrypt.sys, ...)
- Registry keys, service names, config folder names, window class names,
  mutex / IPC names, EFI paths (\EFI\VeraCrypt), XML config root tag
- The volume format and all cryptography — Sealbox volumes stay 100% compatible
  with VeraCrypt volumes
- All existing copyright notices, License.txt, License.html, NOTICE,
  and the "Based on TrueCrypt" / "Based on VeraCrypt" credits.
  The TrueCrypt License 3.0 requires these to stay.
- Documentation page 'VeraCrypt License.html' (verbatim licence text)
- Source-code comment headers (per-file copyright notices of IDRIX / AM Crypto /
  TrueCrypt) — these are legal notices, not branding.

## Build changes

- src/Sealbox.props defines SEALBOX_NO_SIGNATURE_CHECK, imported by the user-mode
  projects. Upstream VeraCrypt refuses to start unless the .exe is signed with the
  IDRIX/AM Crypto certificate; this switch disables that self-check (Common/Dlgcode.c).
- The Windows kernel driver is NOT rebuilt. The installer packages the Microsoft-signed
  driver files shipped with the VeraCrypt 1.26.31 source (src/Release/Setup Files/
  veracrypt-x64.sys, veracrypt-arm64.sys and their .cat files), unmodified.
- .github/workflows/build-windows.yml builds the x64/ARM64/Win32 projects on GitHub
  Actions and packages 'Sealbox Setup 1.26.31.exe' and 'Sealbox Portable 1.26.31.exe'
  as a downloadable artifact (and attaches them to a release for tags named v*).
- The resulting installer and executables are unsigned, so Windows SmartScreen shows
  a warning on first run until a code-signing certificate is used.

## Installer behaviour added for Sealbox

- Default install folder: C:\Program Files\Sealbox (an existing VeraCrypt installation's
  folder is reused if one is found in the registry, as upstream does).
- Start-menu folder and shortcuts are named Sealbox.
- Installation password: src/Setup/SealboxLicense.h holds the SHA-256 hash of the
  password customers must enter before setup starts (src/Setup/SealboxPassword.c,
  dialog IDD_SEALBOX_PASSWORD). Empty hash = no prompt. The MSI installer is not built
  because it has no password prompt.

## Still to do before release

- Code-sign the installer and executables (an ordinary OV certificate is enough,
  because the driver is already signed by Microsoft)
- Publish the full Sealbox source code (required by the TrueCrypt License)
- Trademark search for "Sealbox" in class 9 (software) at ipindia.gov.in
