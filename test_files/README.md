# Test files

The following tests were performed:

```
./hw1 test_files/a1_20.txt  test_files/a2_20.txt -u -v -o test1.log   # Identical files
./hw1 test_files/a1_50.txt  test_files/a2_50.txt -d                   # Identical files
./hw1 test_files/a1_60.txt  test_files/a2_60.txt -u                   # Identical files
./hw1 test_files/a1_100.txt test_files/a2_100.txt -g                  # Identical files
./hw1 test_files/a1_105.txt test_files/a2_105.txt -s                  # Identical files
./hw1 test_files/a1_20.txt  test_files/b_20.txt -u -v -o test6.log    # Different files
./hw1 test_files/a1_50.txt  test_files/b_50.txt -d -v                 # Different files
./hw1 test_files/a1_60.txt  test_files/b_60.txt -g                    # Different files
./hw1 test_files/a1_100.txt test_files/b_100.txt -s                   # Different files
./hw1 test_files/a1_105.txt test_files/b_105.txt -u                   # Different files
./hw1 test_files/a1_20.txt  test_files/non-existing.txt -u -v         # Non existing
./hw1 test_files/non-existing.txt test_files/a2_20.txt -s -g -o test12.log
./hw1 test_files/non-existing.txt test_files/non-existing-2.txt -d    # Non existing
./hw1 test_files/a1_20.txt  test_files/empty.txt -d -v -o test14.log	# Empty file
./hw1 test_files/empty.txt  test_files/b_105.txt -u                   # Empty file
./hw1 test_files/a1_20.txt  test_files/b_50.txt -d -v                 # Different sizes
./hw1 test_files/a1_50.txt  test_files/a2_105.txt -u                  # Different sizes
./hw1 test_files/a1_60.txt  test_files/b_100.txt -d -s                # Different sizes
./hw1 test_files/a1_100.txt test_files/a2_50.txt -g -s                # Different sizes
./hw1 test_files/a1_105.txt test_files/b_100.txt -d -g                # Different sizes
./hw1 test_files/a1_20.txt  test_files/a1_20.txt -A                   # Invalid option
./hw1 test_files/a1_20.txt  test_files/a1_20.txt -9                   # Invalid option
./hw1 test_files/a1_20.txt  test_files/a1_20.txt -ciao                # Invalid option
./hw1 test_files/a1_20.txt  test_files/a1_20.txt -                    # Invalid option
./hw1 test_files/b_20.txt   test_files/b_20.txt; echo “Exit code: $?”	# No options
```
