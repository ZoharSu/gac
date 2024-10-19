# Ideas

## Garbage Allocator(GAc)
- Maybe an actual one using *brk*/*mmap*?
- Should also have a free function...
- Maybe a chunk allocator from a pre-allocated global variable

## Garbage Collector(GaC)
- Mark and Sweep
- The mark will probably be in a header of each GAc allocation, a single byte
- Scanning the roots will occur via *esp* register at *main* and the maximum stack size via *rlimit*
- Pausing the main thread will be via a single?
- Asynchronous sweeping
