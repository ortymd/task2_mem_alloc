
// |HDR|______HEAP_______|FTR|
//
//		 |								 |
// block start					block end

#include <stdlib.h>
#include <func.h>

#define block_sz 15	/*	bits used to store size of given bloc. if 9 (as in this case)
										max size of block is 1023.*/

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)       (*(unsigned short *)(p))
#define PUT(p, val)  (*(unsigned short *)(p) = (val))  

typedef unsigned char byte;
const byte word_sz = sizeof(unsigned int*);
const byte header_sz = 2; /* full size of header in bytes */
const byte footer_sz = 2; /* full size of footer in bytes. added as separate var just for convenience*/

/* Read the size and allocated fields from address p */
#define GET_BLK_SZ(p)  (GET(HDRP(p)) & ~0x1) // provides size of block. args: bp
#define GET_SZ(hp)  (GET(hp) & ~0x1)		// provides size of block. args: header or footer
#define GET_ALLOC(p) (GET(HDRP(p)) & 0x1)

#define SET_BLOCK_SZ(p)  (PUT(HDRP(p)) & ~0x1)
#define SET_ALLOC(p) (GET(HDRP(p)) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - header_sz)  
#define FTRP(bp)       ((char *)(bp) + GET_BLK_SZ(bp)) 

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  (bp + GET_BLK_SZ(bp) + footer_sz + header_sz)
#define PREV_BLKP(bp)  (bp - GET_SZ( bp - header_sz - footer_sz ) - header_sz - footer_sz )


static void *main_heap_p;
static void *heap_hdr;
const unsigned heap_sz = 1<<13;

const byte is_free = 0;
const byte is_alloc = 1;

void* heap_malloc()	// initial heap init
{
	const unsigned full_heap_sz = heap_sz + header_sz + footer_sz;
	main_heap_p = malloc(full_heap_sz);	// allocate mem for heap and offset header
																										// offset header used to check end of heap
	void *bp = main_heap_p + header_sz;					// move to start of block

	PUT(HDRP(bp), PACK(0, is_alloc));	//	set offset header

	PUT(FTRP(bp+heap_sz), PACK(0, is_alloc));	//	set offset footer
	heap_hdr = main_heap_p;				//	move start of heap(header) immediately after offset

	unsigned sz = heap_sz - header_sz - footer_sz;	// subtract space for original block header/footer
	bp += header_sz;			//	move to the actual space
	PUT(HDRP(bp), PACK(sz, is_free));	//	set size of block and that it is free
	PUT(FTRP(bp), PACK(sz, is_free));

	return bp;
}

// |HDR|______space______|FTR||HDR|______space______|FTR|
//															                              
//		 |								 |		    |								  |
// block start					end     block start					block end

void* m_malloc(unsigned sz)
{
	if(sz == 0)
		return NULL;

	void *bp = heap_hdr + header_sz + header_sz;					// move to start of block. add heap header and block header
	unsigned required_sz = sz + footer_sz + header_sz +1; 		//	as we devide given block in 2, we heed space for h/f to both blocks and 
																														//	at least 1 byte of space in second part of orig block.
																														//	Additionally, we reuse space for header and footer of the original block.
																														//	So we need space for footer of the 1st part of original block
																														//	and header for second part of original block
	while(GET_BLK_SZ(bp) != 0)
	{
		if( GET_BLK_SZ(bp) >= required_sz && GET_ALLOC(bp) == 0) 
		{
			allocate_block(&bp, sz);
			break;
		}

		bp = NEXT_BLKP(bp);
	}

	return bp;
}
//														 here starts to second part of original block
//														|
// |HDR|______space______|FTR||HDR|______space______|FTR|
//															                              
//		 |								 |		    |								  |
// block start					end     block start					block end
void* allocate_block(void **bp_addr, unsigned sz)
{
	void *bp = *bp_addr;
	void *bp2 = NULL;	//	this is the second part of original block
	unsigned orig_sz = GET_BLK_SZ(bp);

	PUT(HDRP(bp), PACK(sz, is_alloc));	//	set size of block and that it is allocated 
	PUT(FTRP(bp), PACK(sz, is_alloc));

	sz = orig_sz - sz - footer_sz - header_sz;	//	sz left after allocation is what remains after required size was allocated
	bp2 = bp + GET_BLK_SZ(bp) + footer_sz + header_sz;	// get to the second part of original block
	PUT(HDRP(bp2), PACK(sz, is_free));	//	set size of block and that it is allocated 
	PUT(FTRP(bp2), PACK(sz, is_free));

	return bp;
}

void m_free(void *bp)
{
	free_block(bp);
}

void free_block(void *bp)
{
	unsigned prev_sz = 0;
	unsigned next_sz = 0;
	unsigned sz = GET_BLK_SZ(bp);
	unsigned free_sz = 0;
	void *prev_bp = NULL;
	void *next_bp = NULL;

	prev_bp = PREV_BLKP(bp);
	prev_sz = GET_BLK_SZ(prev_bp);
	if(prev_sz > 0 && GET_ALLOC( prev_bp ) == 0 )//	check previous 
	{
		sz = GET_BLK_SZ(bp);
		free_sz = prev_sz + sz + footer_sz + header_sz;

		PUT(HDRP(prev_bp), PACK(free_sz, is_free));	//	set size of block and that it is free 
		PUT(FTRP(prev_bp), PACK(free_sz, is_free));
	}

	next_bp = NEXT_BLKP(bp);
	next_sz = GET_BLK_SZ(next_bp);
	if(next_sz > 0 && GET_ALLOC( next_bp ) == 0 )//	check next 
	{
		sz = GET_BLK_SZ(bp);
		free_sz = next_sz + sz + footer_sz + header_sz;

		PUT(HDRP(bp), PACK(free_sz, is_free));	//	set size of block and that it is free 
		PUT(FTRP(bp), PACK(free_sz, is_free));
	}

}

void heap_free()
{
	free(main_heap_p);
}
