/*
 Sealbox installation password.

 The installer asks for a password before it starts. Only the SHA-256 hash of the
 password is stored here (64 lower-case hex characters). To set or change it, run
 this in Windows PowerShell (it prompts for the password):

   $p = Read-Host "Password"; $b=[Text.Encoding]::UTF8.GetBytes($p); ([BitConverter]::ToString([Security.Cryptography.SHA256]::Create().ComputeHash($b)) -replace "-","").ToLower()

 and paste the result between the quotes below. An empty string disables the prompt.
 Note: the TrueCrypt licence requires the source code to be public, so this protects
 the official installer only; it is a licence key for customers, not copy protection.
*/
#define SEALBOX_INSTALL_PASSWORD_SHA256 "da946b0afcf3ae8cbe4f4c36c36f46715c1e4dff73ebbef44179cf8e747fafb7"
