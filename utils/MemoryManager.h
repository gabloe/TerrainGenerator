
#ifndef _MEM_MANAGER_

// 4K page sizes
#define PAGE_SIZE 4096

#define page_count( bytes ) ( ( PAGE_SIZE + bytes - 1 ) / PAGE_SIZE )

#define _MEM_MANAGER_

class MemoryManager {
private:
	
	typedef struct PAGE {
		int free;
		unsigned int used;
	}
	
	// Memory allocated for system
	void* memory;
	PAGE *page_table;
	
	unsigned int memory_size = 0;
	unsigned int page_size = 0;
	
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
	
	void releaseMemory( void* pointer ) {
		unsigned int pos = pointer - memory;
		int size = page_table[pos].size;
		while( size ) {
			page_table[pos].free = false;
			pos++;
			size--;
		}
	}

	void* requestMemory( unsigned int bytes ) 
	{
		if( memory != NULL ) {
			int pages  =  page_count( bytes );
		}
		
		int pos = 0;
		while( pos < page_size && !page_table[pos]->free ) {
			pos++;
		}
		
		if( pos == page_size ) {
			return NULL;
		}
		
		void *result = NULL;
		
		int start = pos;
		while( pos < page_size ) {
			// search for a memory space
			while( page_table[pos].free ) {
				// Found memory space
				if( (pos - start) * PAGE_SIZE >= requested ) {
					page_table[pos].size = (pos - start);
					result = memory[start];
					while( start <= pos ) {
						page_table[start].free = false;
						start++;
					}
					return result;
				}
				pos++;
			}
			start = pos + 1;
		}
		
		// Could not find, return NULL
		return NULL;
	}
}

#endif