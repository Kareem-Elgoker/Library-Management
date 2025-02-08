#ifndef RESERVATIONS_H
#define RESERVATIONS_H

	#include <stdbool.h>
	#include "types.h"
	
	struct Node4 {
		u32 id;
		
		u32 user_id;
		u32 book_id;
		
		// 0) waiting,  1) approved,  2)  denied,  3) returned back
		u8 request_status;
		
		struct Node4 * next;
		struct Node4 * previous;
	};

	typedef struct Node4 Request;

	struct LinkedList4 {
		
		Request * head;
		Request * tail;
		
		u32 requests_number;
		u32 next_id;
		
	};

	typedef struct LinkedList4 Reservations;
	
	void initial_reservations(void);
	
	void load_reservations_data(void);
	void save_reservations_data(void);
	
	Request * make_request(u32 user_id, u32 book_id);
	bool add_request(Request * request);
	bool delete_request(u32 id);
	
	void display_requested_book(Book * book, s8 statue, u32 requset_id);
	
	bool display_approved_user_requests(u32 user_id);
	bool display_all_approved_requests(void);
	
	bool display_waiting_user_requests(u32 user_id);
	bool display_user_requests(u32 user_id);
	
	bool display_all_waiting_requests(void);
	
	Request * get_request_object_by_id(u32 id);
	
	void * deny_all_waiting_user_requests(u32 user_id);
	void * deny_all_waiting_book_requests(u32 book_id);
#endif //RESERVATIONS_H