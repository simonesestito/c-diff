#!/bin/bash

# File identici
./hw1 test_files/a1_20.txt  test_files/a2_20.txt -u -v -o test1.log
cat test1.log
./hw1 test_files/a1_50.txt  test_files/a2_50.txt -d
./hw1 test_files/a1_60.txt  test_files/a2_60.txt -u
./hw1 test_files/a1_100.txt test_files/a2_100.txt -g
./hw1 test_files/a1_105.txt test_files/a2_105.txt -s

# File diversi stesse righe
./hw1 test_files/a1_20.txt  test_files/b_20.txt -u -v -o test6.log
cat test6.log
./hw1 test_files/a1_50.txt  test_files/b_50.txt -d -v
./hw1 test_files/a1_60.txt  test_files/b_60.txt -g
./hw1 test_files/a1_100.txt test_files/b_100.txt -s
./hw1 test_files/a1_105.txt test_files/b_105.txt -u

# File inesistenti
./hw1 test_files/a1_20.txt  test_files/non-existing.txt -u -v
./hw1 test_files/non-existing.txt test_files/a2_20.txt -s -g -o test12.log
cat test12.log
./hw1 test_files/non-existing.txt test_files/non-existing-2.txt -d

# Numero diverso di righe
./hw1 test_files/a1_20.txt  test_files/b_50.txt -d -v
./hw1 test_files/a1_50.txt  test_files/a2_105.txt -u
./hw1 test_files/a1_60.txt  test_files/b_100.txt -d -s
./hw1 test_files/a1_100.txt test_files/a2_50.txt -g -s
./hw1 test_files/a1_105.txt test_files/b_100.txt -d -g

# Opzioni errate
./hw1 test_files/a1_20.txt  test_files/a1_20.txt -A
./hw1 test_files/a1_20.txt  test_files/a1_20.txt -d -u
./hw1 test_files/a1_20.txt  test_files/a1_20.txt -9
./hw1 test_files/a1_20.txt  test_files/a1_20.txt -ciao
./hw1 test_files/a1_20.txt  test_files/a1_20.txt -
./hw1 test_files/a1_20.txt  test_files/a1_20.txt -v
./hw1 test_files/a1_20.txt
./hw1

# Senza output, exit code
./hw1 test_files/b_20.txt   test_files/b_20.txt
echo "Exit code: $?"
./hw1 test_files/a1_20.txt   test_files/b_20.txt
echo "Exit code: $?"
