## TWENTY4/160 - a fun stream cipher

*THIS IS JUST FOR LEARINING CRYPTO, DO NOT EVER USE THIS FOR ANYTHING*

This is the implementation of the fun stream cipher TWENTY4/160 by T.v. Dein, 09/2015.
Published under the public domain, Creative Commons Zero License. It works bytewise,
uses a 160 bit key in 8 rounds including an S-Box. A random nonce is added for more
security as IV, each output byte is used as the next IV (like CBC mode). From the key
various PRNGs are seeded, all those PRNGs are recombined into an output key stream,
which is being xored with the IV and then applied to the sbox; the result is then xored
with the input..

The name TWENTY4 is a reference to article 20 paragraph 4 of the german constitution
which reads:

> All Germans shall have  the right to resist  any person seeking to
> abolish this constitutional order, if no other remedy is available.

Also, the cipher uses the contents of the german constitution as the source for its
S-Box.

## S-Box generation

TWENTY4 uses the german constitution (called "basic law" in germany) as
the source for S-Boxes. The EPUB version (included in sbox/ subdir)
is encrypted using AES-256-CBC with the passphrase
"grundgesetz\n". The resulting byte stream is used as the source for
S-Boxes.

The following version of the german constitution is used:

Basic Law for the Federal Republic of Germany in the revised version
published in the Federal Law Gazette Part III, classification number
100-1, as last amended by the Act of 23 December 2014
(Federal Law Gazette I p. 2438).

Linux Shell commands to generate the S-Boxes:

    curl -o BJNR000010949.epub http://www.gesetze-im-internet.de/gg/BJNR000010949.epub
    echo grundgesetz > BJNR000010949.pass
    cat BJNR000010949.epub | openssl aes-256-cbc -kfile BJNR000010949.pass | ./gen-static-sbox

'gen-static-sbox' compiled from gen-static-sbox.c in this directory, which has SHA256
checksum: 29bfd8bd6dbca696d4d8b7ca997497e091875d6bf939e9702b1edf669d0742b0.

However, it just prints out bytes which it reads from STDIN, collecting them into an 256
byte array, ignoring possible duplicates, and prints it out as hex.

The S-Box is bijective and has the following properties (calculated using analyze.c):

    Char distribution: 100.000000%
      Char redundancy: 0.000000%
         Char entropy: 8.000000 bits/char
     Compression rate: 0.000000%

## Key expansion

FIXME.

## Encryption

FIXME.

## Analysis so far

While this stuff only exists for me to play around with
crypto, I tried to test the cipher a little bit. Here are
my results using a couple of statistical measurements:

I encrypted the GPLv3 contents using the key "1". To compare,
I encrypted the same file with AES-256-CBC using the same
passphrase.


### Check using analyze.c

My own measurement, see analyze.c:

            File size: 10240000 bytes
    Char distribution: 99.609375%
      Char redundancy: 0.390625%
         Char entropy: 7.999984 bits/char
     Compression rate: 0.000000% (10240000 => 10243131 bytes)

For comparision, AES result:

            File size: 35168 bytes
    Char distribution: 100.000000%
      Char redundancy: 0.000000%
         Char entropy: 7.994892 bits/char
     Compression rate: 0.000000% (35168 => 35189 bytes)

## Check using ent

(ent from http://www.fourmilab.ch/random/):

    
    Entropy = 7.999984 bits per byte.
    
    Optimum compression would reduce the size
    of this 10240000 byte file by 0 percent.
    
    Chi square distribution for 10240000 samples is 221.67, and randomly
    would exceed this value 93.52 percent of the times.
    
    Arithmetic mean value of data bytes is 127.4901 (127.5 = random).
    Monte Carlo value for Pi is 3.142712165 (error 0.04 percent).
    Serial correlation coefficient is -0.000012 (totally uncorrelated = 0.0).


For comparision, AES result:

    Entropy = 7.994892 bits per byte.
    
    Optimum compression would reduce the size
    of this 35168 byte file by 0 percent.
    
    Chi square distribution for 35168 samples is 250.98, and randomly
    would exceed this value 55.94 percent of the times.
    
    Arithmetic mean value of data bytes is 127.8717 (127.5 = random).
    Monte Carlo value for Pi is 3.151680601 (error 0.32 percent).
    Serial correlation coefficient is 0.002014 (totally uncorrelated = 0.0).

## Check using dieharder

I ran the cipher against the dieharder test suite this way:

    dd if=/dev/zero of=/dev/stdout | ./twenty4 00000000000000000001 | dieharder -g 200 -a

Find the results in analyze/dieharder160.log 

## Output test

    - same clear text slightly different key
    IN: 111111111111, KEY: 00000000000000000001, NONCE: 1, OUT: 8201 cedd ec74 f55b f6a8 a7eb          
    IN: 111111111111, KEY: 00000000000000000002, NONCE: 2, OUT: 964d 0939 cf94 a158 a259 ff4e          
    IN: 111111111111, KEY: 00000000000000000003, NONCE: 3, OUT: db9a 4e08 9ac8 3297 6457 b8aa          
    IN: 111111111111, KEY: 00000000000000000004, NONCE: 4, OUT: 4946 2ce3 fd4a f4e8 95aa 985a          
    IN: 111111111111, KEY: 00000000000000000005, NONCE: 5, OUT: 5f5f 4eaf c0d2 4363 9b18 2eb4          
    IN: 111111111111, KEY: 00000000000000000006, NONCE: 6, OUT: e8df deb7 2afe 3783 98d6 8c3f          
    IN: 111111111111, KEY: 00000000000000000007, NONCE: 7, OUT: 6e3a 27d5 06ed eeca ad3b e7c0          
    IN: 111111111111, KEY: 00000000000000000008, NONCE: 8, OUT: 1c31 4f9b 58d4 1cbd c0cd 0885
    
    - same key, slightly different clear text
    IN: 111111111111, KEY: 00000000000000000001, NONCE: 1, OUT: 8201 cedd ec74 f55b f6a8 a7eb          
    IN: 111111111112, KEY: 00000000000000000001, NONCE: 2, OUT: 031c 6a54 b299 dcc1 5726 57e4          
    IN: 111111111113, KEY: 00000000000000000001, NONCE: 3, OUT: cd12 a615 1ce0 6b95 3ca8 d4b7          
    IN: 111111111114, KEY: 00000000000000000001, NONCE: 4, OUT: 4a6e a49f e68b 4fe7 61ac 4642          
    IN: 111111111115, KEY: 00000000000000000001, NONCE: 5, OUT: 999f 44a0 f563 1c06 64d1 e710          
    IN: 111111111116, KEY: 00000000000000000001, NONCE: 6, OUT: 92d0 e5e0 67c4 0076 c3d7 4130          
    IN: 111111111117, KEY: 00000000000000000001, NONCE: 7, OUT: f6c2 59fd bdd0 2298 9975 3757          
    IN: 111111111118, KEY: 00000000000000000001, NONCE: 8, OUT: 830b 63d4 15f9 fb41 3cd3 0c62


So, all those checks don't look that bad, but of course this doesn't
say much about TWENTY4/160's security. However, not THAT bad for the first cipher :)
