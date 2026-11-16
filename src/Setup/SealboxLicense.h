/*
 Sealbox installation password.

 The installer asks for a password before it starts. Only the SHA-256 hash of the
 password is stored here (64 lower-case hex characters). To set or change it, run
 this in Windows PowerShell with your password in place of YourPassword:

   $b=[Text.Encoding]::UTF8.GetBytes("YourPassword"); ([BitConverter]::ToString([Security.Cryptography.SHA256]::Create().ComputeHash($b)) -replace "-","").ToLower()

 and paste the result between the quotes below. An empty string disables the prompt.
 Note: the TrueCrypt licence requires the source code to be public, so this protects
 the official installer only; it is a licence key for customers, not copy protection.
*/
#define SEALBOX_INSTALL_PASSWORD_SHA256 "8d195194ffa35666c670d7db6c26c6f9f0704f9b26ad50abd566a753c5f62e91"
