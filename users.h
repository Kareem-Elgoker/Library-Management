#ifndef USERS_H
#define USERS_H

	#include <stdbool.h>
	#include "types.h"
	
	struct Node3 {
		u32 id;
		u8 username[30];
		u8 password[30];
		
		u32 borrowed_books;
		
		bool has_card;
		
		u32 card_id;
		u8 card_password[30];
		u32 card_money;
		
		struct Node3 * next;
		struct Node3 * previous;
	};

	typedef struct Node3 User;

	struct LinkedList3 {
		
		User * head;
		User * tail;
		
		u32 users_number;
		u32 next_id;
		
	};

	typedef struct LinkedList3 Users;
	
	void initial_users(void);
	
	void load_users_data(void);
	void save_users_data(void);
	
	User * make_user(u8 * username, u8 * password);
	void add_user(User * user);
	bool delete_user(u32 id);
	
	bool display_all_users(void);
	bool display_all_users_name_id(void);
	
	User * get_user_object_by_id(u32 id);
	
	bool user_auth(u8 * username, u8 * password);
	bool change_user_password(char * old_password, char * new_password);
	
	u32 user_choices(void);
	bool user_flow(u32 choice);
#endif //USERS_H