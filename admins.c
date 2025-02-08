#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "admins.h"
#include "users.h"
#include "books.h"
#include "reservations.h"
#include "provide_books.h"
#include "types.h"

Admins * admins;
Admin * online_admin = NULL;

void initial_admins(void) {
	admins = malloc(sizeof(Admins));
	admins->head = NULL;
	admins->tail = NULL;
	admins->admins_number = 0;
	admins->next_id = 1;
}

void load_admins_data(void) {
	initial_admins();
		
	FILE *filePointer;
	filePointer = fopen("admins.txt", "r");
	
	u8 chunk[30];
	u32 i;
	
	while(fgets(chunk, sizeof(chunk), filePointer) != NULL) {
		if(strcmp(chunk, "\n") == 0) continue;
		Admin * admin = malloc(sizeof(Admin));
		
		admin->id = atoi(chunk);
		
		fgets(chunk, sizeof(chunk), filePointer);
		for (i = 0; chunk[i] != '\n'; i++) {
			admin->username[i] = chunk[i];
		}
		admin->username[i] = '\0';
		
		fgets(chunk, sizeof(chunk), filePointer);
		for (i = 0; chunk[i] != '\n'; i++) {
			admin->password[i] = chunk[i];
		}
		admin->password[i] = '\0';
		
		fgets(chunk, sizeof(chunk), filePointer);
		admin->permissions[0] = (bool)(chunk[0] - '0');
		admin->permissions[1] = (bool)(chunk[2] - '0');
		admin->permissions[2] = (bool)(chunk[4] - '0');
		
		add_admin(admin);
	}
	fclose(filePointer);
}

void save_admins_data(void) {
	FILE *filePointer;
	filePointer = fopen("admins.txt", "w");
	
	Admin * current_admin = admins->head;
	
	while(current_admin != NULL) {
		
		u8 id[10];
		itoa(current_admin->id, id, 10);
		
		fputs(id, filePointer);
		fputs("\n", filePointer);
		
		fputs(current_admin->username, filePointer);
		fputs("\n", filePointer);
		
		fputs(current_admin->password, filePointer);
		fputs("\n", filePointer);
		
		u8 permissions_str[6];
		permissions_str[0] = (u8)(current_admin->permissions[0] + '0');
		permissions_str[1] = ' ';
		permissions_str[2] = (u8)(current_admin->permissions[1] + '0');
		permissions_str[3] = ' ';
		permissions_str[4] = (u8)(current_admin->permissions[2] + '0');
		permissions_str[5] = '\0';
		fputs(permissions_str, filePointer);
		
		fputs("\n\n", filePointer);
		
		current_admin = current_admin->next;
	}
	fclose(filePointer);
}

Admin * make_admin(u8 * username, u8 * password) {
	
	Admin * current_admin = admins->head;
	
	while(current_admin != NULL) {
		if(strcmp(current_admin->username, username) == 0) return current_admin;
		current_admin = current_admin->next;
	}
	
	Admin * admin = malloc(sizeof(Admin));
	
	admin->id = 0;

	strcpy(admin->username, username);
	
	strcpy(admin->password, password);
	
	admin->permissions[0] = 1;
	admin->permissions[1] = 1;
	admin->permissions[2] = 1;
	
	return admin;
}

void add_admin(Admin * admin) {
	if(!admin->id) {
		admin->id = admins->next_id++;
	}
	else {
		if(admin->id >= admins->next_id) {
			admins->next_id = admin->id + 1;
		}
	}
	
	admin->next = NULL;
	admin->previous = admins->tail;
	
	if(!admins->admins_number) {
		admins->tail = admin;
		admins->head = admin;
	}
	else {
		admins->tail->next = admin;
		admins->tail = admin;
	}
	
	admins->admins_number++;
}

bool delete_admin(u32 id) {
	
	if(id > admins->next_id) return 0;
	
	Admin * current_admin = admins->head;
	
	while(current_admin != NULL) {
		if(current_admin->id == id) break;
		current_admin = current_admin->next;
	}
	if(current_admin == NULL) return 0;
	
	if(current_admin->previous == NULL && current_admin->next == NULL) {
		admins->head = NULL;
		admins->tail = NULL;
	}
	else if(current_admin->previous == NULL) {
		current_admin->next->previous = NULL;
		admins->head = current_admin->next;
	}
	else if(current_admin->next == NULL) {
		current_admin->previous->next = NULL;
		admins->tail = current_admin->previous;
	}
	else {
		current_admin->previous->next = current_admin->next;
		current_admin->next->previous = current_admin->previous;
	}
	
	free(current_admin);
	
	admins->admins_number--;
	
	return 1;
}

bool display_all_admins(void) {
	
	Admin * current_admin = admins->head;
	bool found = 0;
	printf("\n");
	while(current_admin != NULL) {
		found = 1;
		printf("Id : %u ---> username : %s\n", current_admin->id , current_admin->username);
		printf("\tAdd/remove member       : %d\n", current_admin->permissions[0]);
		printf("\tAdd/remove book         : %d\n", current_admin->permissions[1]);
		printf("\tMake a book reservation : %d\n\n", current_admin->permissions[2]);
		current_admin = current_admin->next;
	}
	return found;
}

bool admin_auth(u8 * username, u8 * password) {
	Admin * current_admin = admins->head;
	
	while(current_admin != NULL) {
		if(strcmp(username, current_admin->username) == 0 && strcmp(password, current_admin->password) == 0) {
			online_admin = current_admin;
			return 1;
		}
		current_admin = current_admin-> next;
	}
	return 0;
}

bool change_admin_password(char * old_password, char * new_password) {
    if (strcmp(online_admin->password, old_password) == 0) {
        strcpy(online_admin->password, new_password);
		save_admins_data();
        return 1;
    }
    return 0;
}

u8 admin_set_permissions(u32 id, u32 permission_number) {
	Admin * current_admin = admins->head;
	
	while(current_admin != NULL) {
		if(current_admin->id == id) {
			if(current_admin->permissions[permission_number - 1] == 1) return 2;
			current_admin->permissions[permission_number - 1] = 1;
			return 1;
		}
		current_admin = current_admin-> next;
	}
	return 0;
}

u8 admin_deny_permissions(u32 id, u32 permission_number) {
	Admin * current_admin = admins->head;
	
	while(current_admin != NULL) {
		if(current_admin->id == id) {
			if(current_admin->permissions[permission_number - 1] == 0) return 2;
			current_admin->permissions[permission_number - 1] = 0;
			return 1;
		}
		current_admin = current_admin-> next;
	}
	return 0;
}

u32 admin_choices(void) {
	printf("\n1 . View all users' data\n");
	printf("2 . View all books with all data\n");
	printf("3 . Add a book to the library\n");
	printf("4 . Remove a book from the library\n");
	printf("5 . Add users\n");
	printf("6 . Remove users\n");
	printf("7 . Change your password\n");
	printf("8 . Search for a book\n");
	printf("9 . Make a reservation for requested books from the users.\n");
	printf("10. Make a reservation of any book to any user\n");
	printf("11. Make a reserved book available in stock again\n");
	printf("12. Logout\n");
	
	
	u32 choice;
	do {
		printf("\nAnswer : ");
		scanf("%u", &choice); f;
	} while(choice > 12 || choice < 1);
	
	return choice;
}

bool admin_flow(u32 choice) {
	switch(choice) {
		case 1:
		{
			if(display_all_users()) {}
			else printf("\n\nEmpty.\n\n");
			
			break;
		}
		case 2:
		{
			if(display_all_books()) {}
			else printf("\n\nEmpty.\n\n");
			
			break;
		}
		case 3:
		{
			if(!online_admin->permissions[1]){
				printf("\nYou don't have permission to do that, ask the owner to enable it for you.\n");
				break;
			}
			
			printf("\nDo to want to see users books needed or add another one else.\n");
			u8 choice;
				
			do {
				printf("Enter (u) to see useres books, (a) to add your own : ");
				scanf(" %c", &choice); f;
			} while(!(tolower(choice) == 'u' || tolower(choice) == 'a'));
			
			
			u8 * book_name; book_name = malloc(30 * sizeof(u8));
			u8 * book_author; book_author = malloc(30 * sizeof(u8));

			u32 quantity, price;
			
			if(tolower(choice) == 'u') {
				if(display_all_needed_books()) {
					printf("\n\nEnter request book id to add to the library : ");
					u32 request_id;
					scanf("%u", &request_id); f;
					
					NeededBook * needed_book = get_needed_book_object_by_id(request_id);
					
					if(needed_book == NULL) {
						printf("\n\nThis request needed book id is invalid, rediricting to home screen ........\n\n");
						break;
					}
					
					strcpy(book_name, needed_book->book_name);
					strcpy(book_author, needed_book->book_author);
					
					delete_needed_book(needed_book);
					save_books_provider_data();
				}
				else{
					printf("\n\nEmpty.\n\n");
					break;
				}
			}
			else {
				
				printf("\nEnter Book Name : ");
				fgets(book_name, sizeof(u8) * 30, stdin);
				
				printf("Enter Book Author : ");
				fgets(book_author, sizeof(u8) * 30, stdin);
				
				u8 i;
				for(i = 0; book_name[i] != '\0' && book_name[i] != '\n'; i++);
				book_name[i] = '\0';
				
				for(i = 0; book_author[i] != '\0' && book_author[i] != '\n'; i++);
				book_author[i] = '\0';
				
			}
			
			if(is_book_name_author_found(book_name, book_author)) {
				printf("\n\nThis book is already in the library we will add its quantity by 1.\n\n");
				break;
			}
			
			printf("Enter Book Quantity : ");
			scanf("%u", &quantity); f;
			
			printf("Enter Book Price : ");
			scanf("%u", &price); f;
			
			Book * book = make_book(book_name, book_author, quantity, price);
			
			if(book != NULL) {
				add_book(book);
				printf("\n\nSuccessully added ........\n\n");
				save_books_data();
			}
			else printf("\nsoryy, we can't add any book at the moment due to memory error.\n");
			break;
		}
		case 4:
		{
			if(!online_admin->permissions[1]){
				printf("\nYou don't have permission to do that, ask the owner to enable it for you.\n");
				break;
			}
			
			extern Books * books;
			
			if(!books->books_number) {
				printf("\n\nThere are no books to be removed.\n\n");
				break;
			}
			
			u32 ID;
			printf("\nEnter book ID : ");
			scanf("%u", &ID); f;
			if(delete_book(ID)) {
				save_books_data();
				printf("\n\nSuccessully deleted ........\n\n");
			} else printf("\nInvalid ID, try again\n");
			break;
		}
		case 5:
		{
			if(!online_admin->permissions[0]){
				printf("\nYou don't have permission to do that, ask the owner to enable it for you.\n");
				break;
			}
				
			u8 * user_name; user_name = malloc(30 * sizeof(u8));
			
			printf("\nEnter UserName to the new user : ");
			fgets(user_name, sizeof(u8) * 30, stdin);
			
			u8 i;
			for(i = 0; user_name[i] != '\0' && user_name[i] != '\n'; i++);
			user_name[i] = '\0';
			
			User * user = make_user(user_name, "00000");
			
			if(user != NULL && !user->id) {
				add_user(user);
				printf("\n\nSuccessully added with initial password '00000' ........\n\n");
				save_users_data();
			}
			else if(user->id) printf("\nsoryy, there is a user with the same username.\n");
			else printf("\nsoryy, we can't add any user at the moment due to memory error.\n");
			
			break;
		}
		case 6:
		{
			if(!online_admin->permissions[0]){
				printf("\nYou don't have permission to do that, ask the owner to enable it for you.\n");
				break;
			}
			
			extern Users * users;
			
			if(!users->users_number) {
				printf("\n\nThere are no users to be removed.\n\n");
				break;
			}
			
			u32 ID;
			printf("\nEnter user ID : ");
			scanf("%u", &ID); f;
			if(delete_user(ID)) {
				printf("\n\nSuccessully deleted ........\n\n");
				save_users_data();
			} else printf("\nInvalid ID, try again\n");
			break;
		}
		case 7:
		{
			u8 * old_password; old_password = malloc(30 * sizeof(u8));
			u8 * new_password; new_password = malloc(30 * sizeof(u8));
			
			printf("\nEnter your current password : ");
			fgets(old_password, sizeof(u8) * 30, stdin);
			
			printf("Enter your new password : ");
			fgets(new_password, sizeof(u8) * 30, stdin);
			
			u8 i;
			for(i = 0; old_password[i] != '\0' && old_password[i] != '\n'; i++);
			old_password[i] = '\0';
			
			for(i = 0; new_password[i] != '\0' && new_password[i] != '\n'; i++);
			new_password[i] = '\0';
			
			if(change_admin_password(old_password, new_password)){
				printf("\n\nSuccessfully changed........, please log in again.\n\n");
				admin_flow(12);
				return 0;
			}
			else printf("\n\nCurrent password entered is invalid.\n\n");
			break;
		}
		case 8:
		{
			book_search_methods();
			break;
		}
		case 9:
		{
			if(!online_admin->permissions[2]){
				printf("\nYou don't have permission to do that, ask the owner to enable it for you.\n");
				break;
			}
			
			if(display_all_waiting_requests()) {
				printf("\n\nEnter request id to detrmine its state : ");
				u32 request_id;
				scanf("%u", &request_id);
				
				Request * request = get_request_object_by_id(request_id);
				
				if(request == NULL || request->request_status != 0) printf("\n\nThis request id is invalid, rediricting to home screen ........\n\n");
				else {
					
					u8 choice;
						
					do {
						printf("Enter (a) to approve, (d) to deny : ");
						scanf(" %c", &choice);
					} while(!(tolower(choice) == 'a' || tolower(choice) == 'd'));
						
					if(tolower(choice) == 'a') {
						 get_user_object_by_id(request->user_id)->borrowed_books++;
						 request->request_status++;
						 get_book_object_by_id(request->book_id)->quantity--;
						
						 printf("\n\nSuccessfully approved ..........\n\n");
						
						 if(!get_book_object_by_id(request->book_id)->quantity) {
							 printf("\nNote this book becomes unavailable, all requests of this book will be denied automatically.\n");
							 deny_all_waiting_book_requests(request->book_id);
						 }
						
						 if(get_user_object_by_id(request->user_id)->borrowed_books == 2) {
							 printf("\nNote this user has borrowed books equal to 2, all requests of this user will be denied automatically.\n");
							 deny_all_waiting_user_requests(request->user_id);
						}
							
							save_books_data();
							save_users_data();
							save_reservations_data();
							
					}
					else {
						request->request_status+=2;
						save_reservations_data();
						printf("\n\nSuccessfully denied ..........\n\n");
					}
					
				}
			}
			else printf("Empty.\n");
			
			break;
		}
		case 10:
		{
			if(!online_admin->permissions[2]){
				printf("\nYou don't have permission to do that, ask the owner to enable it for you.\n");
				break;
			}
			
			if(display_all_users_name_id()) {}
			else {
				printf("\n\nUseres list is Empty\n\n.");
				break;
			}
			
			printf("\n\nEnter user id to borrow him a book : ");
			u32 user_id;
			scanf("%u", &user_id);
			
			User * user = get_user_object_by_id(user_id);
			
			if(user == NULL) {
				printf("\n\nInvalid user id, rediricting to home screen ........\n\n");
				break;
			}
			
			if(user->borrowed_books == 2) {
				printf("\n\nSorry, This user has already borrowed 2 books, rediricting to home screen ........\n\n");
				break;
			}
			
			if(display_all_available_books()) {}
			else {
				printf("\n\nAvailable books are Empty\n\n.");
				break;
			}
			
			printf("\n\nEnter book id to borrow to this user : ");
			u32 book_id;
			scanf("%u", &book_id);
			
			Book * book = get_book_object_by_id(book_id);
			
			if(book == NULL || !book->quantity) {
				printf("\n\nThis Book id is unavailable, rediricting to home screen ........\n\n");
				break;
			}
			
			Request * request = make_request(user_id, book_id);
			
			request->request_status = 1;
			
			add_request(request);
			
			user->borrowed_books++;
			
			book->quantity--;
			
			save_reservations_data();
			save_books_data();
			save_users_data();
			
			printf("\n\nSuccessfully making reservation ..................\n\n");
			
			if(!book->quantity) {
				printf("\nNote this book becomes unavailable, all requests of this book will be denied automatically.\n");
				deny_all_waiting_book_requests(request->book_id);
			}
			
			if(user->borrowed_books == 2) {
				printf("\nNote this user has borrowed books equal to 2, all requests of this user will be denied automatically.\n");
				deny_all_waiting_user_requests(user_id);
			}
			
			break;
		}
		case 11:
		{
			if(!online_admin->permissions[2]){
				printf("\nYou don't have permission to do that, ask the owner to enable it for you.\n");
				break;
			}
			
			if(display_all_approved_requests()) {
			
				printf("\n\nEnter request id to return borrowed book to library again : ");
				u32 request_id;
				scanf("%u", &request_id);
				
				Request * request = get_request_object_by_id(request_id);
				
				if(request == NULL || request->request_status != 1) printf("\n\nThis request id is invalid, rediricting to home screen ........\n\n");
				else {
					request->request_status += 2;
					get_book_object_by_id(request->book_id)->quantity++;
					get_user_object_by_id(request->user_id)->borrowed_books--;
					save_books_data();
					save_users_data();
					save_reservations_data();
					
					printf("\n\nSuccessfully returned back to the library ..................\n\n");
				}
			}
			else printf("Empty.\n");
			break;
		}
		case 12:
		{
			online_admin = NULL;
			printf("\n\nLogging out........\n\n");
			return 0;
			break;
		}
	}
	return 1;
}