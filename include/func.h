void* m_malloc(unsigned sz);
void heap_free();
void* heap_malloc();	// initial heap init
void* allocate_block(void **bp_addr, unsigned sz);
void free_block(void *bp);
void m_free(void *bp);
