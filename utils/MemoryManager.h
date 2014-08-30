#include "def.h"

// 4K page sizes
#define PAGE_SIZE 4096
#define page_count( bytes ) ( ( PAGE_SIZE + bytes - 1 ) / PAGE_SIZE )
#define _MEM_MANAGER_

class UTIL_API MemoryManager {
private:
	typedef struct PAGE {
		int free;
		unsigned int used;
	};
	
	// Memory allocated for system
	void* memory;
	PAGE *page_table;
	
	unsigned int memory_size = 0;
	unsigned int page_size = 0;
	unsigned int num_pages;
	
public:
	MemoryManager(unsigned int startSize);
	
	~MemoryManager();
	
	void releaseMemory(void* pointer);

	void* requestMemory(unsigned int bytes);
};