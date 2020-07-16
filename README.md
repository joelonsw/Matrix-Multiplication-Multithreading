# Matrix-Multiplication-Multithreading

### Optimization of Matrix Multication using following technics

1. Cache friendly multiplication (Indexing) -- kij.cpp
2. Multithreading with algorithm (thread-level-parallelism) -- dns.cpp
3. Cache friendly multiplication (Tiling) -- dns_tiling64.cpp
4. Using SIMD (data-level-parallelism) -- dns_tiling64_avx512.cpp

### Settings
Compile option : g++ -mavx -pthread -fstrict-aliasing file.cpp -o file –lrt (g++ version 7.5.0)
OS : Linux version 4.15.0-76-generic (buildd@lcy01-amd64-029) (gcc version 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)) #86-Ubuntu SMP
CPU : Intel® Xeon® Silver  4210 CPU @ 2.20GHz
Perf : perf version 4.15.18

Matrix size: 4096

### Execution result
-unopt.cpp

![unopt](https://user-images.githubusercontent.com/61370901/87638412-b2ee7480-c77e-11ea-89db-b6428def3659.png)

-kij.cpp

![kij](https://user-images.githubusercontent.com/61370901/87638524-e29d7c80-c77e-11ea-9f41-3eb00addb445.png)

-dns.cpp

![dns](https://user-images.githubusercontent.com/61370901/87638537-ea5d2100-c77e-11ea-92be-0f9ad0552765.png)

-dns_tiling64.cpp

![dns_tiling64](https://user-images.githubusercontent.com/61370901/87638543-ecbf7b00-c77e-11ea-90e0-299d5316afa9.png)

-dns_tiling64_avx512.cpp

![dns_tiling64_avx512](https://user-images.githubusercontent.com/61370901/87638546-ee893e80-c77e-11ea-9d7b-b8439b795dd8.png)
