#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "types.h"
#include "books.h"
#include "users.h"
#include "reservations.h"

extern Books * books;

s32 payment(u32 book_id) {
	
	Book * current_book = books->head;
	s32 found = -1;
	
	while(current_book != NULL) {
		if(book_id == current_book->id){
			if(!current_book->quantity) found = -2;
			else {
				found = current_book->price;
				current_book->quantity--;
				if(!get_book_object_by_id(book_id)->quantity) {
					deny_all_waiting_book_requests(book_id);
				}
			}
			break;
		} 
		current_book = current_book->next;
	}
	return found;
}

void cash_confirm(void) {
	u8 buy_confirm;
	printf("\nDo You want to confirm this payment? Enter(y) to confirm, otherwise enter anything : ");
	scanf(" %c", &buy_confirm); f;

	if(tolower(buy_confirm) == 'y') {
		save_books_data();
		save_reservations_data();
		printf("\nSuccessfully buying by cash ............\n");
	}
	else {
		load_books_data();
		load_reservations_data();
		printf("\nRedirecting to main menu ..........\n");
	}
}

bool card_payment(User * user, u64 total_money) {
	
	u32 card_id, counter = 0;
	u8 * password; password = malloc(30 * sizeof(u8));

	do {
		if(counter) {
			printf("\nInvalid, try entering library card credintials again or buying with cash?\n");
			u8 buy_cash;
			printf("\nEnter(c) to buy by cash, otherwise(try library card again) enter anything : ");
			scanf(" %c", &buy_cash); f;
			if(tolower(buy_cash) == 'c') return 0;
		}
		
		printf("\nEnter your card ID : ");
		scanf("%u", &card_id); f;
	
		printf("Enter your card password : ");
		fgets(password, sizeof(u8) * 30, stdin);
	
		u8 i;
		for(i = 0; password[i] != '\0' && password[i] != '\n'; i++);
		password[i] = '\0';
		
		counter++;
	}while(!(strcmp(user->card_password, password) == 0 && user->card_id == card_id));
	
	if(user->card_money < total_money) {
		printf("\nYour library card money isn't enough, make this payment by cash.\n");
		return 0;
	} 

	u8 buy_confirm;
	printf("\nDo You want to confirm this payment? Enter(y) to confirm, otherwise enter anything : ");
	scanf(" %c", &buy_confirm); f;

	if(tolower(buy_confirm) == 'y') {
		user->card_money -= total_money;
		save_users_data();
		save_books_data();
		printf("\nSuccessfully buying by your library card, amount of money remaining in it = %u\n", user->card_money);
	}
	else {
		load_books_data();
		printf("\nRedirecting to main menu ..........\n");
	}
	
	return 1;
}