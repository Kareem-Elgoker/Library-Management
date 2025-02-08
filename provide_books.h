#ifndef PROVIDE_BOOKS_H
#define PROVIDE_BOOKS_H

	#include <stdbool.h>
	#include "types.h"
	
	struct Node5 {
		u32 id;
		
		u8 book_name[30];
		u8 book_author[30];
		
		struct Node5 * next;
		struct Node5 * previous;
	};

	typedef struct Node5 NeededBook;

	struct LinkedList5 {
		
		NeededBook * head;
		NeededBook * tail;
		
		u32 books_number;
		u32 next_id;
		
	};

	typedef struct LinkedList5 BooksProvider;
	
	void initial_books_provider(void);
	
	void load_books_provider_data(void);
	void save_books_provider_data(void);
	
	NeededBook * make_needed_book(u8 * name, u8 * author);
	void add_needed_book(NeededBook * needed_book);
	
	void delete_needed_book(NeededBook * needed_book);
	
	void display_needed_book(NeededBook * needed_book);
	bool display_all_needed_books(void);
	
	NeededBook * get_needed_book_object_by_id(u32 id);
	
#endif //PROVIDE_BOOKS_H