## TWENTY4/160 - a fun stream cipher

*THIS IS JUST FOR LEARINING CRYPTO, DO NOT EVER USE THIS FOR ANYTHING*

This is the implementation of the fun stream cipher TWENTY4/160 by T.v. Dein, 09/2015.
Published under the public domain, Creative Commons Zero License. It works bytewise,
uses a 160 bit key bits in 8 rounds, applies an S-Box. From the key various PRNGs
are seeded, all those PRNGs are recombined into an output key stream, which is being
xored with the input (after applying of the sbox).

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

Both S-Boxes are bijective and have the following properties (calculated using analyze.c):

    Char distribution: 100.000000%
      Char redundancy: 0.000000%
         Char entropy: 8.000000 bits/char
     Compression rate: 0.000000%

TWENTY4 uses two S-Box arrays, one for key expansion and one for encryption.

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

        File size: 35147 bytes
Char distribution: 100.000000%
  Char redundancy: 0.000000%
     Char entropy: 7.994904 bits/char
 Compression rate: 0.000000% (35147 => 35168 bytes)

For comparision, AES result:

            File size: 35168 bytes
    Char distribution: 100.000000%
      Char redundancy: 0.000000%
         Char entropy: 7.994892 bits/char
     Compression rate: 0.000000% (35168 => 35189 bytes)

## Check using ent

(ent from http://www.fourmilab.ch/random/):

    Entropy = 7.995333 bits per byte.
    
    Optimum compression would reduce the size
    of this 35147 byte file by 0 percent.
    
    Chi square distribution for 35147 samples is 229.98, and randomly
    would exceed this value 86.79 percent of the times.
    
    Arithmetic mean value of data bytes is 127.6631 (127.5 = random).
    Monte Carlo value for Pi is 3.172955438 (error 1.00 percent).
    Serial correlation coefficient is -0.004405 (totally uncorrelated = 0.0).

    Entropy = 7.994904 bits per byte.
    
    Optimum compression would reduce the size
    of this 35147 byte file by 0 percent.
    
    Chi square distribution for 35147 samples is 248.29, and randomly
    would exceed this value 60.64 percent of the times.
    
    Arithmetic mean value of data bytes is 127.9724 (127.5 = random).
    Monte Carlo value for Pi is 3.101929315 (error 1.26 percent).
    Serial correlation coefficient is -0.000624 (totally uncorrelated = 0.0).

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

I fed the contents of my primary disk into TWENTY4 and its output
into diehard:
    
    dd if=/dev/sda4 of=/dev/stdout | ./stream 1 e | dieharder -a -g 200

FIXME: missing.
