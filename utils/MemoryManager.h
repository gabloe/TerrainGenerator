
#ifndef _MEM_MANAGER_

// 4K page sizes
#define PAGE_SIZE 4096

#define page_count( bytes ) ( ( PAGE_SIZE + bytes - 1 ) / PAGE_SIZE )

#define _MEM_MANAGER_

class MemoryManager {
private:
	typedef struct PAGE {
		void *location;
	}
	
	unsigned int memory_size = 0;
	unsigned int page_size = 0;
	
	// Allocated memory
	PAGE *page_table;
	void* memory;
public:
	MemoryManager( unsigned int startSize) {
		unsigned int _pages = page_count( startSize );
		memory = malloc(  PAGE_SIZE * _pages );
		
		if( memory == NULL ) {
			// EXIT 
			return;
		}		
		page_table = malloc( sizeof( PAGE ) * _pages );
		
		if( page_table == NULL ) {
			free( memory );
			return;
		}
		
		
		for( int i = 0 ; i < _pages ; i++ ) { 
			page_table[i].location = (memory + PAGE_SIZE * i);
		}
		
		pages = _pages;
		memory_size = pages * PAGE_SIZE;
		
	}
	
	~MemoryManager() {
		free( page_table );
		free( memory );
	}

	void* requestMemory( unsigned int bytes ) 
	{
		if( memory != NULL ) {
			int pages  =  page_count( bytes );
		}
		return NULL;
	}
}

#endif