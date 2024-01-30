# GPUParallel_CUDA

\<<GPU Parallel Program Development Using CUDA\>> 一书中代码的实现与翻新 

Implementation and renovation of the code in the book "GPU Parallel Program Development Using CUDA"

#### Chapter1: Overview of CPU Parallel Programming

changes
- use fstream to read raw image instead of read bmp files.
- rewrite horizontal and vertical flip function.
- use chrono library to calculuate the CPU cost time.

#### Chapter2: Develop the first CPU parallel program

changes
- consistent with the code ideas in the book about CPU parallelism.
- use c++ standard library "thread" instead of "pthread".
