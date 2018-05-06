
// |HDR|______HEAP_______|FTR|
//
//		 |								 |
// block start					block end

#include <stdlib.h>
#define block_sz 15	/*	bits used to store size of given bloc. if 9 (as in this case)
										max size of block is 1023.*/

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)       (*(unsigned int *)(p))
#define PUT(p, val)  (*(unsigned int *)(p) = (val))  

typedef unsigned char byte;
const byte word_sz = sizeof(unsigned int*);
const byte header_sz = 2; /* full size of header in bytes */
const byte footer_sz = 2; /* full size of footer in bytes. added as separate var just for convenience*/

/* Read the size and allocated fields from address p */
#define GET_BLK_SZ(p)  (GET(HDRP(p)) & ~0x1)
#define GET_ALLOC(p) (GET(HDRP(p)) & 0x1)

#define SET_BLOCK_SZ(p)  (PUT(HDRP(p)) & ~0x1)
#define SET_ALLOC(p) (GET(HDRP(p)) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - header_sz)  
#define FTRP(bp)       ((char *)(bp) + GET_BLK_SZ(bp)) 

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  (bp + GET_BLK_SZ(bp) + footer_sz + header_sz)
#define PREV_BLKP(bp)  (bp - GET_BLK_SZ( HDRP(bp) - footer_sz ) )


static void *start_heap_p;
const unsigned heap_sz = 1<<13;

const byte is_free = 0;
const byte is_alloc = 1;

void* heap_malloc()	// initial heap init
{
	const unsigned full_heap_sz = heap_sz + header_sz + footer_sz;
	start_heap_p = malloc(full_heap_sz);		// get all mem
	void *bp = start_heap_p + header_sz;					// move to start of block

	PUT(HDRP(bp), PACK(heap_sz, is_free));	//	set size of block and that it is free
	PUT(FTRP(bp), PACK(heap_sz, is_free));

	return bp;
}

void* m_malloc(unsigned sz)
{
	if(sz == 0)
		return NULL;

	void *bp = start_heap_p + header_sz;					// move to start of block
	while(GET_BLK_SZ(bp) != 0)
	{
		if( GET_BLK_SZ(bp) > sz ) 
		{
			//bp = allocate_block(sz);
		}
		else
		{
			;
		}
		bp = NEXT_BLKP(bp);
	}

	return 0;
}

void heap_free()
{
	free(start_heap_p);
}