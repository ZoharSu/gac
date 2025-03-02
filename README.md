# Beware the **GAC**!

## Garbage Allocator(GAc)
- [ ] Maybe an actual one using *brk*/*mmap*?
- [ ] Should also have a free function...
- [ ] Maybe a chunk allocator from a pre-allocated global variable

## Garbage Collector(GaC)
- [X] Mark and Sweep
- [X] The mark will probably be in a header of each GAc allocation, a single byte
- [ ] Scanning the roots will occur from the *esp* register at *main* and the maximum stack size will be *rlimit*
- [ ] Pausing the main thread will be via a signal - namely, SIGUSR1
- [ ] Asynchronous sweeping
