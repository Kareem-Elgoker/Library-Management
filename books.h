#ifndef BOOKS_H
#define BOOKS_H

	#include <stdbool.h>
	#include "types.h"
	
	struct Node1 {
		u32 id;
		u32 quantity;
		u32 price;
		u8 book_name[30];
		u8 book_author[30];
		struct Node1 * next;
		struct Node1 * previous;
		
	};

	typedef struct Node1 Book;

	struct LinkedList1 {
		
		Book * head;
		Book * tail;
		
		u32 books_number;
		u32 next_id;
		
	};

	typedef struct LinkedList1 Books;
	
	void initial_books(void);
	
	void load_books_data(void);
	void save_books_data(void);
	
	Book * make_book(u8 * name, u8 * author, u32 quantity, u32 price);
	void add_book(Book * book);
	bool delete_book(u32 id);
	
	void display_book(Book * book);
	bool display_all_books();
	bool display_all_available_books();
	bool display_author_books(u8 * author);
	bool display_book_by_name(u8 * name);
	bool display_book_by_id(u32 id);
	
	Book * get_book_object_by_id(u32 id);
	bool is_book_available(u32 id);
	
	bool is_book_name_author_found(u8 * name, u8 * author);
	
	void book_search_methods(void);
	
	
#endif //BOOKS_H