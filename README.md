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

<<<<<<< HEAD
Checking an nearly infinite output stream, encrypting NULL with 0x01, checked
by dieharder:

    % cat /dev/zero | ./twenty4 0x01 e | dieharder -g 200 -a
    #=============================================================================#
    #            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
    #=============================================================================#
       rng_name    |rands/second|   Seed   |
    stdin_input_raw|  6.82e+05  |1515115156|
    #=============================================================================#
            test_name   |ntup| tsamples |psamples|  p-value |Assessment
    #=============================================================================#
       diehard_birthdays|   0|       100|     100|0.66702401|  PASSED  
          diehard_operm5|   0|   1000000|     100|0.66534937|  PASSED  
      diehard_rank_32x32|   0|     40000|     100|0.64395558|  PASSED  
        diehard_rank_6x8|   0|    100000|     100|0.73972650|  PASSED  
       diehard_bitstream|   0|   2097152|     100|0.37109308|  PASSED  
            diehard_opso|   0|   2097152|     100|0.61131814|  PASSED  
            diehard_oqso|   0|   2097152|     100|0.78775192|  PASSED  
             diehard_dna|   0|   2097152|     100|0.34538786|  PASSED  
    diehard_count_1s_str|   0|    256000|     100|0.39973600|  PASSED  
    diehard_count_1s_byt|   0|    256000|     100|0.43596185|  PASSED  
     diehard_parking_lot|   0|     12000|     100|0.82562887|  PASSED  
        diehard_2dsphere|   2|      8000|     100|0.89930428|  PASSED  
        diehard_3dsphere|   3|      4000|     100|0.75039182|  PASSED  
         diehard_squeeze|   0|    100000|     100|0.84575916|  PASSED  
            diehard_sums|   0|       100|     100|0.17887215|  PASSED  
            diehard_runs|   0|    100000|     100|0.27394031|  PASSED  
            diehard_runs|   0|    100000|     100|0.38358466|  PASSED  
           diehard_craps|   0|    200000|     100|0.46344932|  PASSED  
           diehard_craps|   0|    200000|     100|0.90279468|  PASSED  
     marsaglia_tsang_gcd|   0|  10000000|     100|0.86230982|  PASSED  
     marsaglia_tsang_gcd|   0|  10000000|     100|0.71786320|  PASSED  
             sts_monobit|   1|    100000|     100|0.95022673|  PASSED  
                sts_runs|   2|    100000|     100|0.97598338|  PASSED  
              sts_serial|   1|    100000|     100|0.91209013|  PASSED  
              sts_serial|   2|    100000|     100|0.76413978|  PASSED  
              sts_serial|   3|    100000|     100|0.47828005|  PASSED  
              sts_serial|   3|    100000|     100|0.89677063|  PASSED  
              sts_serial|   4|    100000|     100|0.99879068|   WEAK   
              sts_serial|   4|    100000|     100|0.93262189|  PASSED  
              sts_serial|   5|    100000|     100|0.97585634|  PASSED  
              sts_serial|   5|    100000|     100|0.50478610|  PASSED  
              sts_serial|   6|    100000|     100|0.08698163|  PASSED  
              sts_serial|   6|    100000|     100|0.03039228|  PASSED  
              sts_serial|   7|    100000|     100|0.62431435|  PASSED  
              sts_serial|   7|    100000|     100|0.74890431|  PASSED  
              sts_serial|   8|    100000|     100|0.87532744|  PASSED  
              sts_serial|   8|    100000|     100|0.91641753|  PASSED  
              sts_serial|   9|    100000|     100|0.11259928|  PASSED  
              sts_serial|   9|    100000|     100|0.04014422|  PASSED  
              sts_serial|  10|    100000|     100|0.18509222|  PASSED  
              sts_serial|  10|    100000|     100|0.91447809|  PASSED  
              sts_serial|  11|    100000|     100|0.51473918|  PASSED  
              sts_serial|  11|    100000|     100|0.38194660|  PASSED  
              sts_serial|  12|    100000|     100|0.08996905|  PASSED  
              sts_serial|  12|    100000|     100|0.72068305|  PASSED  
              sts_serial|  13|    100000|     100|0.06321140|  PASSED  
              sts_serial|  13|    100000|     100|0.83106256|  PASSED  
              sts_serial|  14|    100000|     100|0.08691687|  PASSED  
              sts_serial|  14|    100000|     100|0.36681587|  PASSED  
              sts_serial|  15|    100000|     100|0.10396429|  PASSED  
              sts_serial|  15|    100000|     100|0.10353726|  PASSED  
              sts_serial|  16|    100000|     100|0.98923097|  PASSED  
              sts_serial|  16|    100000|     100|0.58151255|  PASSED  
             rgb_bitdist|   1|    100000|     100|0.83179852|  PASSED  
             rgb_bitdist|   2|    100000|     100|0.13343241|  PASSED  
             rgb_bitdist|   3|    100000|     100|0.64851901|  PASSED  
             rgb_bitdist|   4|    100000|     100|0.27381896|  PASSED  
             rgb_bitdist|   5|    100000|     100|0.53255869|  PASSED  
             rgb_bitdist|   6|    100000|     100|0.23874740|  PASSED  
             rgb_bitdist|   7|    100000|     100|0.24395463|  PASSED  
             rgb_bitdist|   8|    100000|     100|0.32082061|  PASSED  
             rgb_bitdist|   9|    100000|     100|0.23675202|  PASSED  
             rgb_bitdist|  10|    100000|     100|0.14315574|  PASSED  
             rgb_bitdist|  11|    100000|     100|0.49355287|  PASSED  
             rgb_bitdist|  12|    100000|     100|0.94148332|  PASSED  
    rgb_minimum_distance|   2|     10000|    1000|0.90096962|  PASSED  
    rgb_minimum_distance|   3|     10000|    1000|0.40166302|  PASSED  
    rgb_minimum_distance|   4|     10000|    1000|0.39664349|  PASSED  
    rgb_minimum_distance|   5|     10000|    1000|0.33642569|  PASSED  
        rgb_permutations|   2|    100000|     100|0.69292329|  PASSED  
        rgb_permutations|   3|    100000|     100|0.26000392|  PASSED  
        rgb_permutations|   4|    100000|     100|0.98030683|  PASSED  
        rgb_permutations|   5|    100000|     100|0.36223930|  PASSED  
          rgb_lagged_sum|   0|   1000000|     100|0.11113853|  PASSED  
          rgb_lagged_sum|   1|   1000000|     100|0.77935325|  PASSED  
          rgb_lagged_sum|   2|   1000000|     100|0.77721610|  PASSED  
          rgb_lagged_sum|   3|   1000000|     100|0.82307605|  PASSED  
          rgb_lagged_sum|   4|   1000000|     100|0.03644586|  PASSED  
          rgb_lagged_sum|   5|   1000000|     100|0.66058445|  PASSED  
          rgb_lagged_sum|   6|   1000000|     100|0.58785549|  PASSED  
          rgb_lagged_sum|   7|   1000000|     100|0.49955974|  PASSED  
          rgb_lagged_sum|   8|   1000000|     100|0.69106353|  PASSED  
          rgb_lagged_sum|   9|   1000000|     100|0.70361764|  PASSED  
          rgb_lagged_sum|  10|   1000000|     100|0.30173719|  PASSED  
          rgb_lagged_sum|  11|   1000000|     100|0.85554929|  PASSED  
          rgb_lagged_sum|  12|   1000000|     100|0.27246327|  PASSED  
          rgb_lagged_sum|  13|   1000000|     100|0.88260425|  PASSED  
          rgb_lagged_sum|  14|   1000000|     100|0.63519490|  PASSED  
          rgb_lagged_sum|  15|   1000000|     100|0.82192024|  PASSED  
          rgb_lagged_sum|  16|   1000000|     100|0.44669473|  PASSED  
          rgb_lagged_sum|  17|   1000000|     100|0.54589530|  PASSED  
          rgb_lagged_sum|  18|   1000000|     100|0.54788412|  PASSED  
          rgb_lagged_sum|  19|   1000000|     100|0.69862386|  PASSED  
          rgb_lagged_sum|  20|   1000000|     100|0.11040733|  PASSED  
          rgb_lagged_sum|  21|   1000000|     100|0.48511696|  PASSED  
          rgb_lagged_sum|  22|   1000000|     100|0.86570587|  PASSED  
          rgb_lagged_sum|  23|   1000000|     100|0.30541569|  PASSED  
          rgb_lagged_sum|  24|   1000000|     100|0.78320483|  PASSED  
          rgb_lagged_sum|  25|   1000000|     100|0.35409569|  PASSED  
          rgb_lagged_sum|  26|   1000000|     100|0.56956770|  PASSED  
          rgb_lagged_sum|  27|   1000000|     100|0.58603441|  PASSED  
          rgb_lagged_sum|  28|   1000000|     100|0.31877864|  PASSED  
          rgb_lagged_sum|  29|   1000000|     100|0.60119378|  PASSED  
          rgb_lagged_sum|  30|   1000000|     100|0.40966142|  PASSED  
          rgb_lagged_sum|  31|   1000000|     100|0.35013332|  PASSED  
          rgb_lagged_sum|  32|   1000000|     100|0.70995085|  PASSED  
         rgb_kstest_test|   0|     10000|    1000|0.13663827|  PASSED  
         dab_bytedistrib|   0|  51200000|       1|0.50308151|  PASSED  
                 dab_dct| 256|     50000|       1|0.50376390|  PASSED  
    Preparing to run test 207.  ntuple = 0
            dab_filltree|  32|  15000000|       1|0.30158012|  PASSED  
            dab_filltree|  32|  15000000|       1|0.66046482|  PASSED  
    Preparing to run test 208.  ntuple = 0
           dab_filltree2|   0|   5000000|       1|0.41413034|  PASSED  
           dab_filltree2|   1|   5000000|       1|0.93892964|  PASSED  
    Preparing to run test 209.  ntuple = 0
            dab_monobit2|  12|  65000000|       1|0.44041006|  PASSED  
    


So, all those checks don't look that bad, but of course this doesn't
say much about TWENTY4's security. However, not THAT bad for the first cipher :)
=======
I fed the contents of my primary disk into TWENTY4 and its output
into diehard:
    
    dd if=/dev/sda4 of=/dev/stdout | ./stream 1 e | dieharder -a -g 200
>>>>>>> 3c4788312c1cc97b9bc8f842797037e291ca9f0a

FIXME: missing.
