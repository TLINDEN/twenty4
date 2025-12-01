> [!CAUTION]
> This software is now being maintained on [Codeberg](https://codeberg.org/scip/twenty4/).


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

For results look in analyze/TESTLOG.md.

So, all those checks don't look that bad, but of course this doesn't
say much about TWENTY4/160's security. However, not THAT bad for the first cipher :)
