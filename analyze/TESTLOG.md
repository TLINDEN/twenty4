## Varous tests on the cipher

Last updated on Fri Oct  2 18:49:26 2015

### Crypto test

    % make test
    sha256 LICENSE | cut -f 4 -d ' ' > .xsum
    cat LICENSE   | ./twenty4 123456trewqasdfgbvcx X  > .xenc
    cat .xenc     | ./twenty4 123456trewqasdfgbvcx X  > .xdec
    sha256 .xdec  | cut -f 4 -d ' '  > .xnsum
    if test "x`cat .xsum`" = "x`cat .xnsum`"; then echo OK; else echo FAIL; fi
    OK

### Variance/Diffusion test

same clear text slightly different key:

    IN: 111111111111, KEY: 00000000000000000001, NONCE: 1, OUT: 0a0e 802c dd87 e04c 5247 11ee          
    IN: 111111111111, KEY: 00000000000000000002, NONCE: 2, OUT: 4f31 b7ed 9670 2fb3 d0e0 8ab2          
    IN: 111111111111, KEY: 00000000000000000003, NONCE: 3, OUT: cc63 a54f 75a3 2f3f 253f 0da8          
    IN: 111111111111, KEY: 00000000000000000004, NONCE: 4, OUT: 9f1d 2052 73cd 4952 59ec 30ae          
    IN: 111111111111, KEY: 00000000000000000005, NONCE: 5, OUT: 0628 1196 cf34 28fc 254f 5e3a          
    IN: 111111111111, KEY: 00000000000000000006, NONCE: 6, OUT: 41ec 47e3 6629 2de4 c8eb 9d1b          
    IN: 111111111111, KEY: 00000000000000000007, NONCE: 7, OUT: 1b29 3fae 61e3 f7dd 2fd3 9299          
    IN: 111111111111, KEY: 00000000000000000008, NONCE: 8, OUT: de29 89f7 3200 f9ae 46b1 915f          
    
same key, slightly different clear text:

    IN: 111111111111, KEY: 00000000000000000001, NONCE: 1, OUT: 0a0e 802c dd87 e04c 5247 11ee          
    IN: 111111111112, KEY: 00000000000000000001, NONCE: 2, OUT: 97e3 f661 0ba6 2cc6 06e6 8960          
    IN: 111111111113, KEY: 00000000000000000001, NONCE: 3, OUT: 88f3 5f69 3c48 f098 188a 4e60          
    IN: 111111111114, KEY: 00000000000000000001, NONCE: 4, OUT: c552 112c 145b 89c5 1ef1 7993          
    IN: 111111111115, KEY: 00000000000000000001, NONCE: 5, OUT: f14c dc4f 5353 0856 1646 27ef          
    IN: 111111111116, KEY: 00000000000000000001, NONCE: 6, OUT: 23d7 871e 3b24 a1f2 a85e 9b61          
    IN: 111111111117, KEY: 00000000000000000001, NONCE: 7, OUT: 8654 a2ba 9c5f 6b8b 798e 100d          
    IN: 111111111118, KEY: 00000000000000000001, NONCE: 8, OUT: 1036 cf47 b636 5cd7 17fc ea50          


### Check using analyze.c on 10Mb file containing zeros

            File size: 10240000 bytes
    Char distribution: 100.000000%
      Char redundancy: 0.000000%
         Char entropy: 7.999982 bits/char
     Compression rate: 0.000000% (10240000 => 10243131 bytes)

For comparision, AES result:

            File size: 10240032 bytes
    Char distribution: 100.000000%
      Char redundancy: 0.000000%
         Char entropy: 7.999981 bits/char
     Compression rate: 0.000000% (10240032 => 10243163 bytes)

### Check using 'ent'

(see http://www.fourmilab.ch/random/)

    Entropy = 7.999982 bits per byte.
    
    Optimum compression would reduce the size
    of this 10240000 byte file by 0 percent.
    
    Chi square distribution for 10240000 samples is 248.61, and randomly
    would exceed this value 60.09 percent of the times.
    
    Arithmetic mean value of data bytes is 127.5026 (127.5 = random).
    Monte Carlo value for Pi is 3.142529353 (error 0.03 percent).
    Serial correlation coefficient is 0.000282 (totally uncorrelated = 0.0).

For comparision, AES result:

    Entropy = 7.999981 bits per byte.
    
    Optimum compression would reduce the size
    of this 10240032 byte file by 0 percent.
    
    Chi square distribution for 10240032 samples is 274.74, and randomly
    would exceed this value 18.90 percent of the times.
    
    Arithmetic mean value of data bytes is 127.4883 (127.5 = random).
    Monte Carlo value for Pi is 3.142633148 (error 0.03 percent).
    Serial correlation coefficient is -0.000300 (totally uncorrelated = 0.0).


### Check using dieharder

I ran the cipher against the dieharder test suite this way:

    dd if=/dev/zero of=/dev/stdout | ./twenty4 00000000000000000001 1 | dieharder -g 200 -a

Output:

    #=============================================================================#
    #            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
    #=============================================================================#
       rng_name    |rands/second|   Seed   |
    stdin_input_raw|  2.58e+06  |1513597900|
    #=============================================================================#
            test_name   |ntup| tsamples |psamples|  p-value |Assessment
    #=============================================================================#
       diehard_birthdays|   0|       100|     100|0.44405548|  PASSED  
          diehard_operm5|   0|   1000000|     100|0.41968438|  PASSED  
      diehard_rank_32x32|   0|     40000|     100|0.94128457|  PASSED  
        diehard_rank_6x8|   0|    100000|     100|0.46413251|  PASSED  
       diehard_bitstream|   0|   2097152|     100|0.18988778|  PASSED  
            diehard_opso|   0|   2097152|     100|0.30907373|  PASSED  
            diehard_oqso|   0|   2097152|     100|0.36684547|  PASSED  
             diehard_dna|   0|   2097152|     100|0.72800518|  PASSED  
    diehard_count_1s_str|   0|    256000|     100|0.18190762|  PASSED  
    diehard_count_1s_byt|   0|    256000|     100|0.97344107|  PASSED  
     diehard_parking_lot|   0|     12000|     100|0.63285114|  PASSED  
        diehard_2dsphere|   2|      8000|     100|0.89270394|  PASSED  
        diehard_3dsphere|   3|      4000|     100|0.84607328|  PASSED  
         diehard_squeeze|   0|    100000|     100|0.00916101|  PASSED  
            diehard_sums|   0|       100|     100|0.53013221|  PASSED  
            diehard_runs|   0|    100000|     100|0.52209635|  PASSED  
            diehard_runs|   0|    100000|     100|0.86136907|  PASSED  
           diehard_craps|   0|    200000|     100|0.41829511|  PASSED  
           diehard_craps|   0|    200000|     100|0.97778963|  PASSED  
     marsaglia_tsang_gcd|   0|  10000000|     100|0.02910980|  PASSED  
     marsaglia_tsang_gcd|   0|  10000000|     100|0.95391085|  PASSED  
             sts_monobit|   1|    100000|     100|0.48843210|  PASSED  
                sts_runs|   2|    100000|     100|0.33161487|  PASSED  
              sts_serial|   1|    100000|     100|0.43813666|  PASSED  
              sts_serial|   2|    100000|     100|0.37539053|  PASSED  
              sts_serial|   3|    100000|     100|0.61308717|  PASSED  
              sts_serial|   3|    100000|     100|0.97040715|  PASSED  
              sts_serial|   4|    100000|     100|0.00472683|   WEAK   
              sts_serial|   4|    100000|     100|0.07425392|  PASSED  
              sts_serial|   5|    100000|     100|0.52552345|  PASSED  
              sts_serial|   5|    100000|     100|0.10458291|  PASSED  
              sts_serial|   6|    100000|     100|0.60383631|  PASSED  
              sts_serial|   6|    100000|     100|0.95466429|  PASSED  
              sts_serial|   7|    100000|     100|0.91083199|  PASSED  
              sts_serial|   7|    100000|     100|0.76069249|  PASSED  
              sts_serial|   8|    100000|     100|0.98844871|  PASSED  
              sts_serial|   8|    100000|     100|0.80973493|  PASSED  
              sts_serial|   9|    100000|     100|0.95319391|  PASSED  
              sts_serial|   9|    100000|     100|0.92051699|  PASSED  
              sts_serial|  10|    100000|     100|0.88609114|  PASSED  
              sts_serial|  10|    100000|     100|0.77025807|  PASSED  
              sts_serial|  11|    100000|     100|0.90999930|  PASSED  
              sts_serial|  11|    100000|     100|0.97842039|  PASSED  
              sts_serial|  12|    100000|     100|0.37609901|  PASSED  
              sts_serial|  12|    100000|     100|0.10481537|  PASSED  
              sts_serial|  13|    100000|     100|0.22822077|  PASSED  
              sts_serial|  13|    100000|     100|0.66019102|  PASSED  
              sts_serial|  14|    100000|     100|0.26002872|  PASSED  
              sts_serial|  14|    100000|     100|0.87881975|  PASSED  
              sts_serial|  15|    100000|     100|0.42922502|  PASSED  
              sts_serial|  15|    100000|     100|0.68487544|  PASSED  
              sts_serial|  16|    100000|     100|0.58656170|  PASSED  
              sts_serial|  16|    100000|     100|0.53290142|  PASSED  
             rgb_bitdist|   1|    100000|     100|0.51900504|  PASSED  
             rgb_bitdist|   2|    100000|     100|0.79584778|  PASSED  
             rgb_bitdist|   3|    100000|     100|0.61693742|  PASSED  
             rgb_bitdist|   4|    100000|     100|0.98138944|  PASSED  
             rgb_bitdist|   5|    100000|     100|0.79680743|  PASSED  
             rgb_bitdist|   6|    100000|     100|0.40301904|  PASSED  
             rgb_bitdist|   7|    100000|     100|0.86271738|  PASSED  
             rgb_bitdist|   8|    100000|     100|0.05374503|  PASSED  
             rgb_bitdist|   9|    100000|     100|0.87172406|  PASSED  
             rgb_bitdist|  10|    100000|     100|0.75492489|  PASSED  
             rgb_bitdist|  11|    100000|     100|0.36375640|  PASSED  
             rgb_bitdist|  12|    100000|     100|0.97870218|  PASSED  
    rgb_minimum_distance|   2|     10000|    1000|0.64644256|  PASSED  
    rgb_minimum_distance|   3|     10000|    1000|0.35176212|  PASSED  
    rgb_minimum_distance|   4|     10000|    1000|0.81882879|  PASSED  
    rgb_minimum_distance|   5|     10000|    1000|0.59290965|  PASSED  
        rgb_permutations|   2|    100000|     100|0.52393977|  PASSED  
        rgb_permutations|   3|    100000|     100|0.29814762|  PASSED  
        rgb_permutations|   4|    100000|     100|0.29601751|  PASSED  
        rgb_permutations|   5|    100000|     100|0.93920367|  PASSED  
          rgb_lagged_sum|   0|   1000000|     100|0.85678805|  PASSED  
          rgb_lagged_sum|   1|   1000000|     100|0.76026887|  PASSED  
          rgb_lagged_sum|   2|   1000000|     100|0.97506899|  PASSED  
          rgb_lagged_sum|   3|   1000000|     100|0.75520196|  PASSED  
          rgb_lagged_sum|   4|   1000000|     100|0.39924791|  PASSED  
          rgb_lagged_sum|   5|   1000000|     100|0.99498131|  PASSED  
          rgb_lagged_sum|   6|   1000000|     100|0.76423409|  PASSED  
          rgb_lagged_sum|   7|   1000000|     100|0.37518368|  PASSED  
          rgb_lagged_sum|   8|   1000000|     100|0.32518164|  PASSED  
          rgb_lagged_sum|   9|   1000000|     100|0.29782458|  PASSED  
          rgb_lagged_sum|  10|   1000000|     100|0.33222498|  PASSED  
          rgb_lagged_sum|  11|   1000000|     100|0.28793810|  PASSED  
          rgb_lagged_sum|  12|   1000000|     100|0.75475293|  PASSED  
          rgb_lagged_sum|  13|   1000000|     100|0.90549286|  PASSED  
          rgb_lagged_sum|  14|   1000000|     100|0.75601003|  PASSED  
          rgb_lagged_sum|  15|   1000000|     100|0.77940074|  PASSED  
          rgb_lagged_sum|  16|   1000000|     100|0.65667602|  PASSED  
          rgb_lagged_sum|  17|   1000000|     100|0.40248634|  PASSED  
          rgb_lagged_sum|  18|   1000000|     100|0.61395852|  PASSED  
          rgb_lagged_sum|  19|   1000000|     100|0.57327052|  PASSED  
          rgb_lagged_sum|  20|   1000000|     100|0.16561333|  PASSED  
          rgb_lagged_sum|  21|   1000000|     100|0.39929834|  PASSED  
          rgb_lagged_sum|  22|   1000000|     100|0.49704905|  PASSED  
          rgb_lagged_sum|  23|   1000000|     100|0.98204460|  PASSED  
          rgb_lagged_sum|  24|   1000000|     100|0.69626086|  PASSED  
          rgb_lagged_sum|  25|   1000000|     100|0.11607617|  PASSED  
          rgb_lagged_sum|  26|   1000000|     100|0.13345423|  PASSED  
          rgb_lagged_sum|  27|   1000000|     100|0.47597394|  PASSED  
          rgb_lagged_sum|  28|   1000000|     100|0.20637857|  PASSED  
          rgb_lagged_sum|  29|   1000000|     100|0.99168483|  PASSED  
          rgb_lagged_sum|  30|   1000000|     100|0.63085174|  PASSED  
          rgb_lagged_sum|  31|   1000000|     100|0.74058543|  PASSED  
          rgb_lagged_sum|  32|   1000000|     100|0.51943588|  PASSED  
         rgb_kstest_test|   0|     10000|    1000|0.98352339|  PASSED  
         dab_bytedistrib|   0|  51200000|       1|0.76481752|  PASSED  
                 dab_dct| 256|     50000|       1|0.50340138|  PASSED  
    Preparing to run test 207.  ntuple = 0
            dab_filltree|  32|  15000000|       1|0.54182883|  PASSED  
            dab_filltree|  32|  15000000|       1|0.08567872|  PASSED  
    Preparing to run test 208.  ntuple = 0
           dab_filltree2|   0|   5000000|       1|0.20297371|  PASSED  
           dab_filltree2|   1|   5000000|       1|0.17653315|  PASSED  
    Preparing to run test 209.  ntuple = 0
            dab_monobit2|  12|  65000000|       1|0.41951689|  PASSED  
    