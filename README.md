## TWENTY4 - a fun stream cipher

*THIS IS JUST FOR LEARINING CRYPTO, DO NOT EVER USE THIS FOR ANYTHING*

This is the implementation of the fun stream cipher TWENTY4 by T.v. Dein, 09/2015.
Published under the public domain, Creative Commons Zero License. It works bytewise,
with keys between 1-256 bits in 17 rounds, uses S-Boxes and key output-feedback mode.
The cipher also works with CBC or ECB mode (sample CBC implementation included).

The name TWENTY4 is a reference to article 20 paragraph 4 of the german constitution
which reads:

> All Germans shall have  the right to resist  any person seeking to
> abolish this constitutional order, if no other remedy is available.

Also, the cipher uses the contents of the german constitution as the source for its
S-Boxes.

## S-Box generation

TWENTY4 uses the german constitution (called "basic law" in germany) as
the source for S-Boxes. The EPUB version (included in sbox/ subdir)
is encrypted using AES-256-CBC with the passphrase
"grundgesetz\n". The resulting byte stream is used as the source for
S-Boxes.

The following version of the german consitution is used:

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

The input key will be expanded into a 17 byte array. Maximum key size is
17 bytes (136 bit).

    IV = KU[0]
    for ROUND in 0..16
      if KU[ROUND]
        K[ROUND] = IV xor KU[ROUND]
      else
        K[ROUND] = IV yor KBOX[ROUND * 8];
      endif
      K[ROUND] = KBOX[K[ROUND]]
      IV = K[ROUND]
    endfor
    
    for KROUND in 0..31
      for ROUND in 0..17
        K[ROUND] = IV xor (rotateleft-3(K[ROUND]) xor KBOX[rcon(IV)])
        IV = K[ROUND]
      endfor
    endfor

where:

    KU: input key
    K[17]: initial round key array
    ROUND: encryption round 1-17
    KROUND: key expansion round 1-32
    KBOX[256]: pre computed S-Box for key expansion

## Encryption

    for INBYTE in <INSTREAM>
      OUTBYTE = INBYTE
      for ROUND in 0..17
        OUTBYTE = OUTBYTE xor K[ROUND]
        OUTBYTE = OUTBYTE xor SBOX[OUTBYTE]
        OUTBYTE = rotateleft-ROUND%8(OUTBYTE)
        OUTBYTE = rotateright-4(K[ROUND])
      endfor
      rotatekey(K, OUTBYTE)
      OUTBYTE => <OUTSTREAM>
    endfor
    
    func rotatekey(K, B)
      [rotate K[17] array elementy 1 to the right]
      for N in 0..16:
        K[N] = KBOX[K[N] xor B]
      endfor
    endfunc
    

where:

    K[17]: expanded key
    ROUND: encryption round 1-17
    INBYTE: one input byte
    OUTBYTE: encrypted result for output
    SBOX[256]: pre computed S-Box for encryption


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
         Char entropy: 7.995333 bits/char
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
    #=============================================================================#
    #            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
    #=============================================================================#
       rng_name    |rands/second|   Seed   |
    stdin_input_raw|  1.86e+05  |2067533949|
    #=============================================================================#
            test_name   |ntup| tsamples |psamples|  p-value |Assessment
    #=============================================================================#
       diehard_birthdays|   0|       100|     100|0.11286983|  PASSED  
          diehard_operm5|   0|   1000000|     100|0.14228207|  PASSED  
      diehard_rank_32x32|   0|     40000|     100|0.08372938|  PASSED  
        diehard_rank_6x8|   0|    100000|     100|0.47630577|  PASSED  
       diehard_bitstream|   0|   2097152|     100|0.68878582|  PASSED  
            diehard_opso|   0|   2097152|     100|0.36965490|  PASSED  
            diehard_oqso|   0|   2097152|     100|0.85360068|  PASSED  
             diehard_dna|   0|   2097152|     100|0.41389081|  PASSED  
    diehard_count_1s_str|   0|    256000|     100|0.64198483|  PASSED  
    diehard_count_1s_byt|   0|    256000|     100|0.48126427|  PASSED  
     diehard_parking_lot|   0|     12000|     100|0.61281762|  PASSED  
        diehard_2dsphere|   2|      8000|     100|0.98794548|  PASSED  
        diehard_3dsphere|   3|      4000|     100|0.86553337|  PASSED  
         diehard_squeeze|   0|    100000|     100|0.47837267|  PASSED  
            diehard_sums|   0|       100|     100|0.26661852|  PASSED  
            diehard_runs|   0|    100000|     100|0.78455791|  PASSED  
            diehard_runs|   0|    100000|     100|0.56428921|  PASSED  
           diehard_craps|   0|    200000|     100|0.81900152|  PASSED  
           diehard_craps|   0|    200000|     100|0.54592338|  PASSED  
    ctrl-c

(FIXME: I aborted here, I'll repeat that one later)

So, all those checks don't look that bad, but of course this doesn't
say much about TWENTY4. However, not THAT bad for the first cipher :)

