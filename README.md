# Matrix-Multiplication-Multithreading

### Optimization of Matrix Multication using following technics
<div>
1. Cache friendly multiplication (Indexing) -- kij.cpp <br>
The first step is indexing. <br>
Because the elements of the array are stored in consecutive addresses, I thought that column-wise memory access should be avoided.
Therefore, I replaced the original IJK method with the KIJ method.
</div>
<br>
<div>
2. Multithreading with algorithm (Thread-Level-Parallelism) -- dns.cpp <br>

The second step is multi-threading. <br>
I came up with the idea after studying Cannon and DNS algorithm from the textbook. 
The algorithm is as follows. 

![알고설명](https://user-images.githubusercontent.com/61370901/87932630-c96e3600-cac6-11ea-93f7-43cadc23601a.png)
<br>
Divide A and B into 16 pieces. <br>
The result array is the product of A's row, B's column, and the elements are obtained. <br>
For example, the first piece of the Result matrix is represented by the product of A's first row and B's first column. <br>
All 16 columns of the other results can be expressed like this. <br>
<br>
This is where parallelization takes place. Create 16 threads and allocate proper operation for each thread 0-15 to results from 0-15. <br>
In the first step, one of the 16 divided A and B matrices in the upper left corner of my picture is multiplied. <br>
When 16 threads complete their respective operations, the threads join. <br>
In the second step, I create 16 threads to do the operation located in the upper right corner of the picture and join them when finished. <br>
The third and fourth steps follow a similar process, and then end the function.<br>
</div>
<br>
<div>
3. Cache friendly multiplication (Tiling) -- dns_tiling64.cpp <br>
The third step is tiling. <br>
Although parallel processing was carried out through DNS algorithm, LLC miss rate could not be improved because the multi function remained KIJ method as it is.<br>
To improve this, I introduce the Tiling method. <br>
  
![쭉](https://user-images.githubusercontent.com/61370901/87933085-911b2780-cac7-11ea-9d8f-93000d23bb85.png) 
![나눠서](https://user-images.githubusercontent.com/61370901/87933082-8fe9fa80-cac7-11ea-9dc1-2ede8fbeb632.png)
<br>
As you access memory in the matrix, if you navigate with the left picture, if the size of the matrix is larger than the size of the cache, then it will not remain in the cache when you access the first element of the matrix again after navigating the matrix.<br>
To prevent this, I introduced the Tiling method, which is to efficiently write the cache by performing all the operations on the array when it remains in the cache.
<br>
</div>
<br>
<div>
<br>4. Using SIMD (Data-Level-Parallelism) -- dns_tiling64_avx512.cpp <br>
As a last step, I used SIMD using intrinsics.<br>
Two things have changed since using SIMD.<br>
First, the number of instruction decreased noticeably with each use of sse, avx, and avx512.<br>
Second, when using AVX512, the clock rate decreased compared to when using SISD/SSE/AVX.<br>
</div>
### Settings
Compile option : g++ -mavx -pthread -fstrict-aliasing file.cpp -o file –lrt (g++ version 7.5.0) <br>
OS : Linux version 4.15.0-76-generic <br>
CPU : Intel® Xeon® Silver  4210 CPU @ 2.20GHz <br>
Perf : perf version 4.15.18 <br>
Matrix size: 4096 <br>

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
