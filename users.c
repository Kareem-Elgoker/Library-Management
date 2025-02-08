#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "books.h"
#include "users.h"
#include "payments.h"
#include "reservations.h"
#include "provide_books.h"
#include "types.h"

Users * users;
User * online_user = NULL;

void initial_users(void) {
	users = malloc(sizeof(Users));
	users->head = NULL;
	users->tail = NULL;
	users->users_number = 0;
	users->next_id = 1;
}

void load_users_data(void) {
	initial_users();
		
	FILE *filePointer;
	filePointer = fopen("users.txt", "r");
	
	u8 chunk[30];
	u32 i;
	
	while(fgets(chunk, sizeof(chunk), filePointer) != NULL) {
		if(strcmp(chunk, "\n") == 0) continue;
		User * user = malloc(sizeof(User));
		
		user->id = atoi(chunk);
		
		fgets(chunk, sizeof(chunk), filePointer);
		for (i = 0; chunk[i] != '\n'; i++) {
			user->username[i] = chunk[i];
		}
		user->username[i] = '\0';
		
		fgets(chunk, sizeof(chunk), filePointer);
		for (i = 0; chunk[i] != '\n'; i++) {
			user->password[i] = chunk[i];
		}
		user->password[i] = '\0';
		
		fgets(chunk, sizeof(chunk), filePointer);
		user->borrowed_books = atoi(chunk);
		
		fgets(chunk, sizeof(chunk), filePointer);
		if(strcmp(chunk, "1\n") == 0) {
			user->has_card = 1;
			
			fgets(chunk, sizeof(chunk), filePointer);
			user->card_id = atoi(chunk);
			
			fgets(chunk, sizeof(chunk), filePointer);
			for (i = 0; chunk[i] != '\n'; i++) {
				user->card_password[i] = chunk[i];
			}
			user->card_password[i] = '\0';
			
			fgets(chunk, sizeof(chunk), filePointer);
			user->card_money = atoi(chunk);
		}
		else user->has_card = 0;
		
		add_user(user);
	}
	fclose(filePointer);
}

void save_users_data(void) {
	FILE *filePointer;
	filePointer = fopen("users.txt", "w");
	
	User * current_user = users->head;
	
	while(current_user != NULL) {
		
		u8 id[10];
		itoa(current_user->id, id, 10);
		
		fputs(id, filePointer);
		fputs("\n", filePointer);
		
		fputs(current_user->username, filePointer);
		fputs("\n", filePointer);
		
		fputs(current_user->password, filePointer);
		fputs("\n", filePointer);
		
		u8 borrowed_books[5];
		itoa(current_user->borrowed_books, borrowed_books, 5);
		
		fputs(borrowed_books, filePointer);
		fputs("\n", filePointer);
		
		if(current_user->has_card) {
			fputs("1\n", filePointer);
			
			u8 card_id[10];
			itoa(current_user->card_id, card_id, 10);
			
			fputs(card_id, filePointer);
			fputs("\n", filePointer);
			
			fputs(current_user->card_password, filePointer);
			fputs("\n", filePointer);
			
			u8 card_money[10];
			itoa(current_user->card_money, card_money, 10);
			
			fputs(card_money, filePointer);
			fputs("\n", filePointer);
		}
		else fputs("0\n", filePointer);
			
		fputs("\n\n", filePointer);
		
		current_user = current_user->next;
	}
	fclose(filePointer);
}

User * make_user(u8 * username, u8 * password) {
	
	User * current_user = users->head;
	
	while(current_user != NULL) {
		if(strcmp(current_user->username, username) == 0) return current_user;
		current_user = current_user->next;
	}
	
	
	User * user = malloc(sizeof(User));
	
	user->id = 0;

	strcpy(user->username, username);
	
	strcpy(user->password, password);
	
	user->borrowed_books = 0;
	
	user->has_card = 0;
	
	return user;
}

void add_user(User * user) {
	if(!user->id) {
		user->id = users->next_id++;
	}
	else {
		if(user->id >= users->next_id) {
			users->next_id = user->id + 1;
		}
	}
	
	user->next = NULL;
	user->previous = users->tail;
	
	if(!users->users_number) {
		users->tail = user;
		users->head = user;
	}
	else {
		users->tail->next = user;
		users->tail = user;
	}
	
	users->users_number++;
}

bool delete_user(u32 id) {
	
	if(id > users->next_id) return 0;
	
	User * current_user = users->head;
	
	while(current_user != NULL) {
		if(current_user->id == id) break;
		current_user = current_user->next;
	}
	if(current_user == NULL) return 0;
	
	if(current_user->previous == NULL && current_user->next == NULL) {
		users->head = NULL;
		users->tail = NULL;
	}
	else if(current_user->previous == NULL) {
		current_user->next->previous = NULL;
		users->head = current_user->next;
	}
	else if(current_user->next == NULL) {
		current_user->previous->next = NULL;
		users->tail = current_user->previous;
	}
	else {
		current_user->previous->next = current_user->next;
		current_user->next->previous = current_user->previous;
	}
	
	free(current_user);
	
	users->users_number--;
	
	return 1;
}

bool display_all_users(void) {
	
	User * current_user = users->head;
	bool found = 0;
	printf("\n");
	while(current_user != NULL) {
		found = 1;
		printf("\nId : %u ---> username : %-30s", current_user->id , current_user->username);
		
		printf("\n\nRequested books :-\n");
		if(display_waiting_user_requests(current_user->id)) {}
		else printf("Empty.\n");
		
		printf("\nBorrowed books (%u) :-\n", current_user->borrowed_books);
		if(display_approved_user_requests(current_user->id)) {}
		else printf("Empty.\n");
		printf("\n-----------------------------------------------------------------------------\n");
		current_user = current_user->next;
	}
	return found;
}

bool display_all_users_name_id(void) {
	
	User * current_user = users->head;
	bool found = 0;
	printf("\n");
	while(current_user != NULL) {
		found = 1;
		printf("\nId : %u ---> username : %-30s", current_user->id , current_user->username);
		current_user = current_user->next;
	}
	return found;
}

User * get_user_object_by_id(u32 id) {
	User * current_user = users->head;
	while(current_user != NULL) {
		if(id == current_user->id){
			return current_user;
		} 
		current_user = current_user->next;
	}
	return NULL;
}

bool user_auth(u8 * username, u8 * password) {
	User * current_user = users->head;
	
	while(current_user != NULL) {
		if(strcmp(username, current_user->username) == 0 && strcmp(password, current_user->password) == 0) {
			online_user = current_user;
			return 1;
		}
		current_user = current_user->next;
	}
	return 0;
}

bool change_user_password(char * old_password, char * new_password) {
    if (strcmp(online_user->password, old_password) == 0) {
        strcpy(online_user->password, new_password);
		save_users_data();
        return 1;
    }
    return 0;
}

u32 user_choices(void) {
	printf("\n1 . View your data\n");
	printf("2 . Change your password\n");
	printf("3 . View all books\n");
	printf("4 . Search for a book\n");
	printf("5 . Make a request to borrow an available book\n");
	printf("6 . Buy an available book\n");
	printf("7 . make a library card\n");
	printf("8 . Request a new book to be provided in the future\n");
	printf("9 . Show your current borrow requests\n");
	printf("10. Logout\n");
	
	u32 choice;
	do {
		printf("Answer : ");
		scanf("%u", &choice); f;
	} while(choice > 10 || choice < 1);
	
	return choice;
}

bool user_flow(u32 choice) {
	switch(choice) { 
		case 1:
		{
			printf("\nId : %u ---> username : %-30s", online_user->id , online_user->username);
			
			if(online_user->has_card) {
				printf("library card id : %-10u", online_user->card_id);
				printf("Money in the card : %u\n", online_user->card_money);
			} else printf("\n");
			
			printf("\nBorrowed books (%u) :-\n", online_user->borrowed_books);
			if(display_approved_user_requests(online_user->id)) {}
			else printf("Empty.\n");
			printf("\n---------------------------------------------------------------------------------------------\n");
			break;
		}
		case 2:
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
			
			if(change_user_password(old_password, new_password)){
				printf("\n\nSuccessfully changed........, please log in again.\n\n");
				user_flow(10);
				return 0;
			}
			else printf("\n\nCurrent password entered is invalid.\n\n");
			
			break;
		}
		case 3:
		{
			if(display_all_books()) {}
			else printf("\n\nEmpty.\n\n");
			
			break;
		}
		case 4:
		{
			book_search_methods();
			break;
		}
		case 5:
		{
			if(display_all_available_books()) {}
			else {
				printf("\n\nAvailable books are Empty\n\n.");
				break;
			}
			
			u32 book_id;
			printf("\nEnter Book Id that you want to borrow : ");
			scanf("%u", &book_id); f;
			
			Request * request = make_request(online_user->id, book_id);
			
			if(request != NULL) {
				if(add_request(request)) {
					printf("\n\nRequest is successully sent to the admins ........\n\n");
					save_reservations_data();
				}
				else printf("\n\nThis book id is unavailable, rediricting to home screen ........\n\n");
			}
			else printf("\nsoryy, we can't make this request at the moment due to memory error.\n");
			
			break;
		}
		case 6:
		{
			if(display_all_available_books()) {}
			else {
				printf("\n\nAvailable books are Empty\n\n.");
				break;
			}
			
			u64 total_cash = 0;
			while(1) {
				
				u32 ID;
				printf("\nEnter book ID you want to buy : ");
				scanf("%u", &ID); f;
				
				s32 current_cash = payment(ID);
				if(current_cash == -1 || current_cash == -2) printf("\nInvalid ID, try again\n");
				else total_cash += current_cash;
				
				u8 buy_again;
				printf("\nDo You want to buy again? Enter(y) to buy again, otherwise enter anything : ");
				scanf(" %c", &buy_again); f;
				
				if(tolower(buy_again) != 'y') {
					break;
				}
			}
			
			if(!total_cash) printf("\nThere are no books to be bought.\n");
			else {
				printf("\nRequired money : %llu$\n\n", total_cash);
			
				if(online_user->has_card) {
					printf("Do you want to buy buy by cash or your library card?");
					u8 buy_way;
					do {
						printf("\nEnter (c) for cash and (l) for library card : ");
						scanf(" %c", &buy_way); f;
					} while(!(tolower(buy_way) == 'c' || tolower(buy_way) == 'l'));
					
					if(tolower(buy_way) =='c') cash_confirm();
					else {
						if(!card_payment(online_user, total_cash)) cash_confirm();
					}
				}
				
				else {
					printf("You will make this payment with cash, you don't have library card.");
					cash_confirm();
				}
			}
			break;
		}
		case 7:
		{
			if(online_user->has_card) printf("\n\nYou have already have one.\n\n");
			else {
				u32 card_id;
				printf("\nEnter library card Id : ");
				scanf("%u", &card_id); f;
				
				printf("Enter library card Password : ");
				u8 * card_password; card_password = malloc(30 * sizeof(u8));
				fgets(card_password, sizeof(u8) * 30, stdin);
				
				u8 i;
				for(i = 0; card_password[i] != '\0' && card_password[i] != '\n'; i++);
				card_password[i] = '\0';
				
				u32 card_money;
				printf("Enter amount of money to put in the library card : ");
				scanf("%u", &card_money); f;
				
				online_user->has_card = 1;
				online_user->card_id = card_id;
				strcpy(online_user->card_password, card_password);
				online_user->card_money = card_money;
				save_users_data();
				
				printf("\n\nLibrary card has been made successfully.\n\n");
				
			}
			
			break;
		}
		case 8:
		{
			u8 * book_name; book_name = malloc(30 * sizeof(u8));
			u8 * book_author; book_author = malloc(30 * sizeof(u8));
			
			printf("\nEnter Book Name : ");
			fgets(book_name, sizeof(u8) * 30, stdin);
			
			printf("Enter Book Author : ");
			fgets(book_author, sizeof(u8) * 30, stdin);
			
			u8 i;
			for(i = 0; book_name[i] != '\0' && book_name[i] != '\n'; i++);
			book_name[i] = '\0';
			
			for(i = 0; book_author[i] != '\0' && book_author[i] != '\n'; i++);
			book_author[i] = '\0';
			
			NeededBook * needed_book = make_needed_book(book_name, book_author);
			
			if(needed_book != NULL) {
				add_needed_book(needed_book);
				printf("\n\nSuccessully sent to the user ........\n\n");
				save_books_provider_data();
			}
			else printf("\nsoryy, we can't add any needed book at the moment due to memory error.\n");
			break;
		}
		case 9:
		{
			if(display_user_requests(online_user->id)) {}
			else printf("Empty.\n");
			break;
		}
		case 10:
		{
			online_user = NULL;
			printf("\n\nLogging out........\n\n");
			return 0;
			break;
		}
	}
	return 1;
}