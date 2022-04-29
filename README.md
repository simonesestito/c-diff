# "Diff" C software
Diff program developed in C, as an homework for the "Operating Systems" university course.

## Homework Report (Italian)

https://docs.google.com/document/d/1ynnmrZ6o_y_MNkrBKwG56GYn-4y8Ea2GgoZZ5xOZKlA/edit?usp=sharing

## Run

This program uses a Makefile. You can simply run the `make` command, then run the `./hw1` executable.

## Demo video

https://asciinema.org/a/491021

## Tests

The following tests were performed.

**Identical files**

```
./hw1 test_files/a1_20.txt  test_files/a2_20.txt -u -v -o test1.log
cat test1.log
./hw1 test_files/a1_50.txt  test_files/a2_50.txt -d
./hw1 test_files/a1_60.txt  test_files/a2_60.txt -u
./hw1 test_files/a1_100.txt test_files/a2_100.txt -g
./hw1 test_files/a1_105.txt test_files/a2_105.txt -s
```

**Different files, but with same number of lines**

```
./hw1 test_files/a1_20.txt  test_files/b_20.txt -u -v -o test6.log
cat test6.log
./hw1 test_files/a1_50.txt  test_files/b_50.txt -d -v
./hw1 test_files/a1_60.txt  test_files/b_60.txt -g
./hw1 test_files/a1_100.txt test_files/b_100.txt -s
./hw1 test_files/a1_105.txt test_files/b_105.txt -u
```

**Non existing files**

```
./hw1 test_files/a1_20.txt  test_files/non-existing.txt -u -v
./hw1 test_files/non-existing.txt test_files/a2_20.txt -s -g -o test12.log
cat test12.log
./hw1 test_files/non-existing.txt test_files/non-existing-2.txt -d
```

**Empty file**

```
./hw1 test_files/a1_20.txt  test_files/empty.txt -d -v -o test14.log
cat test14.log
./hw1 test_files/empty.txt  test_files/b_105.txt -u
```

**Different number of lines**

```
./hw1 test_files/a1_20.txt  test_files/b_50.txt -d -v
./hw1 test_files/a1_50.txt  test_files/a2_105.txt -u
./hw1 test_files/a1_60.txt  test_files/b_100.txt -d -s
./hw1 test_files/a1_100.txt test_files/a2_50.txt -g -s
./hw1 test_files/a1_105.txt test_files/b_100.txt -d -g
```

**Wrong options**

```
./hw1 test_files/a1_20.txt  test_files/a1_20.txt -A
./hw1 test_files/a1_20.txt  test_files/a1_20.txt -d -u
./hw1 test_files/a1_20.txt  test_files/a1_20.txt -9
./hw1 test_files/a1_20.txt  test_files/a1_20.txt -ciao
./hw1 test_files/a1_20.txt  test_files/a1_20.txt -
./hw1 test_files/a1_20.txt  test_files/a1_20.txt -v
./hw1 test_files/a1_20.txt
./hw1
``` 

**Quiet output (exit code)**

```
./hw1 test_files/b_20.txt   test_files/b_20.txt
echo “Exit code: $?”
./hw1 test_files/a1_20.txt   test_files/b_20.txt
echo “Exit code: $?”
```


## License

    Copyright 2022 Simone Sestito
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
