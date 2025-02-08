#ifndef ADMINS_H
#define ADMINS_H

	#include <stdbool.h>
	#include "types.h"
	
	struct Node2 {
		u32 id;
		u8 username[30];
		u8 password[30];
		bool permissions[3];
		
		struct Node2 * next;
		struct Node2 * previous;
	};

	typedef struct Node2 Admin;

	struct LinkedList2 {
		
		Admin * head;
		Admin * tail;
		
		u32 admins_number;
		u32 next_id;
		
	};

	typedef struct LinkedList2 Admins;
	
	void initial_admins(void);
	
	void load_admins_data(void);
	void save_admins_data(void);
	
	Admin * make_admin(u8 * username, u8 * password);
	void add_admin(Admin * admin);
	bool delete_admin(u32 id);
	
	bool display_all_admins(void);
	
	bool admin_auth(u8 * username, u8 * password);
	bool change_admin_password(char * old_password, char * new_password);
	
	// return (0) for invalid admin id, return (1) for scuccess, return (2) if the permission is the same as required
	u8 admin_set_permissions(u32 id, u32 permission_number);
	u8 admin_deny_permissions(u32 id, u32 permission_number);
	
	u32 admin_choices(void);
	bool admin_flow(u32 choice);
	
#endif //ADMINS_H