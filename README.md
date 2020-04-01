To compile you have to add

```cpp
extern char _end;
#define UMM_MALLOC_CFG_HEAP_ADDR (&_end)
#define UMM_MALLOC_CFG_HEAP_SIZE (0x10000)      
```

