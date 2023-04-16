files stored also on IPFS
https://ipfs.best-practice.se/ipfs/QmPopxPtcbHFBQ5MP1r2jHpaXJsjHq7eUhAkVE9cHU61Ym/

# one-time-pad
implemented the strongest encryption in the world (quantum computer resistent)
Should work on linux, windows and macos

# why
For absolut security, there are no way to crack a OTP
A onetimepad is only secure if you are using it correctly. (https://en.wikipedia.org/wiki/One-time_pad)

# problem
true random numbers and the key is as long as the plain text, which makes it not so comfortable.
Each random sequence may only be used once!

# Solution
1.) Create a large true random number file (1 GB or more) (XR232USB)

2.) Create a key pair and share the decryption key with your partner (must be a secure channel, personally)

3.) For each encrypted file, the random numbers used are automatically subtracted.


If you are using a 1 GB key only for small textfiles (messages) the key is not used up so quickly.

# Suggestion
Only use a air gapped computer for storing and other tasks (encryption, decryption, keygeneration, TRN generation)

Requirement: True Random Number Generator - TRNG: example: XR232USB (http://jtxp.org/tech/xr232usb_en.htm)

# compile: 
gcc -o otp otp.c
