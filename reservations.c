#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "books.h"
#include "users.h"
#include "reservations.h"
#include "types.h"

Reservations * reservations;

void initial_reservations(void) {
	reservations = malloc(sizeof(Reservations));
	reservations->head = NULL;
	reservations->tail = NULL;
	reservations->requests_number = 0;
	reservations->next_id = 1;
}

void load_reservations_data(void) {
	initial_reservations();
		
	FILE *filePointer;
	filePointer = fopen("reservations.txt", "r");
	
	u8 chunk[10];
	
	while(fgets(chunk, sizeof(chunk), filePointer) != NULL) {
		if(strcmp(chunk, "\n") == 0) continue;
		Request * request = malloc(sizeof(Request));
		
		request->id = atoi(chunk);
		
		fgets(chunk, sizeof(chunk), filePointer);
		request->user_id = atoi(chunk);
		
		fgets(chunk, sizeof(chunk), filePointer);
		request->book_id = atoi(chunk);
			
		fgets(chunk, sizeof(chunk), filePointer);
		request->request_status = atoi(chunk);
		
		add_request(request);
	}
	fclose(filePointer);
}

void save_reservations_data(void) {
	FILE *filePointer;
	filePointer = fopen("reservations.txt", "w");
	
	Request * current_request = reservations->head;
	
	while(current_request != NULL) {
		
		u8 id[10];
		itoa(current_request->id, id, 10);
		
		fputs(id, filePointer);
		fputs("\n", filePointer);
		
		u8 user_id[10];
		itoa(current_request->user_id, user_id, 10);
		
		fputs(user_id, filePointer);
		fputs("\n", filePointer);
		
		u8 book_id[10];
		itoa(current_request->book_id, book_id, 10);
		
		fputs(book_id, filePointer);
		fputs("\n", filePointer);
		
		u8 request_status[5];
		itoa(current_request->request_status, request_status, 5);
		
		fputs(request_status, filePointer);			
		fputs("\n\n", filePointer);
		
		current_request = current_request->next;
	}
	fclose(filePointer);
}

Request * make_request(u32 user_id, u32 book_id) {
	Request * request = malloc(sizeof(Request));
	
	request->id = 0;
	request->user_id = user_id;
	request->book_id = book_id;
	request->request_status = 0;
	
	return request;
}

bool add_request(Request * request) {
	
	if(!is_book_available(request->book_id) && !request->id) return 0;
		
	if(!request->id) {
		request->id = reservations->next_id++;
	}
	else {
		if(request->id >= reservations->next_id) {
			reservations->next_id = request->id + 1;
		}
	}
	
	request->next = NULL;
	request->previous = reservations->tail;
	
	if(!reservations->requests_number) {
		reservations->tail = request;
		reservations->head = request;
	}
	else {
		reservations->tail->next = request;
		reservations->tail = request;
	}
	
	reservations->requests_number++;
	
	return 1;
}

bool delete_request(u32 id) {
	
	if(id > reservations->next_id) return 0;
	
	Request * current_request = reservations->head;
	
	while(current_request != NULL) {
		if(current_request->id == id) break;
		current_request = current_request->next;
	}
	if(current_request == NULL) return 0;
	
	if(current_request->previous == NULL && current_request->next == NULL) {
		reservations->head = NULL;
		reservations->tail = NULL;
	}
	else if(current_request->previous == NULL) {
		current_request->next->previous = NULL;
		reservations->head = current_request->next;
	}
	else if(current_request->next == NULL) {
		current_request->previous->next = NULL;
		reservations->tail = current_request->previous;
	}
	else {
		current_request->previous->next = current_request->next;
		current_request->next->previous = current_request->previous;
	}
	
	free(current_request);
	
	reservations->requests_number--;
	
	return 1;
}

void display_requested_book(Book * book, s8 statue, u32 requset_id) {
	
	printf("Request id : %-5u Book Name : %s", requset_id , book->book_name);
	for(int i = strlen(book->book_name); i < 30; i++) {
		printf(" ");
	}
	
	printf("Author : %s", book->book_author);
	for(int i = strlen(book->book_author); i < 30; i++) {
		printf(" ");
	}
	
	printf("Statue : ");
	if(statue == 0) printf("Waiting");
	else if(statue == 1) printf("approved");
	else if(statue == 2) printf("denied");
	else if(statue == 3) printf("returned back to library");
	
	printf("\n");
	
}

bool display_user_requests(u32 user_id) {
	Request * current_request = reservations->head;
	bool found = 0;
	printf("\n");
	while(current_request != NULL) {
		if(current_request->user_id == user_id){
			display_requested_book(get_book_object_by_id(current_request->book_id), current_request->request_status, current_request->id);
			found = 1;
		} 
		current_request = current_request->next;
	}
	return found;
}

bool display_approved_user_requests(u32 user_id) {
	Request * current_request = reservations->head;
	bool found = 0;
	printf("\n");
	while(current_request != NULL) {
		if(current_request->user_id == user_id && current_request->request_status == 1){
			display_requested_book(get_book_object_by_id(current_request->book_id), current_request->request_status, current_request->id);
			found = 1;
		} 
		current_request = current_request->next;
	}
	return found;
}

bool display_all_approved_requests(void) {
	Request * current_request = reservations->head;
	bool found = 0;
	printf("\n");
	while(current_request != NULL) {
		if(current_request->request_status == 1){
			display_requested_book(get_book_object_by_id(current_request->book_id), current_request->request_status, current_request->id);
			found = 1;
		} 
		current_request = current_request->next;
	}
	return found;
}

bool display_waiting_user_requests(u32 user_id) {
	Request * current_request = reservations->head;
	bool found = 0;
	printf("\n");
	while(current_request != NULL) {
		if(current_request->user_id == user_id && current_request->request_status == 0){
			display_requested_book(get_book_object_by_id(current_request->book_id), current_request->request_status, current_request->id);
			found = 1;
		} 
		current_request = current_request->next;
	}
	return found;
}

bool display_all_waiting_requests(void) {
	Request * current_request = reservations->head;
	bool found = 0;
	printf("\n");
	while(current_request != NULL) {
		if(current_request->request_status == 0){
			printf("User : %s --> ", get_user_object_by_id(current_request->user_id)->username);
			display_requested_book(get_book_object_by_id(current_request->book_id), current_request->request_status, current_request->id);
			found = 1;
		} 
		current_request = current_request->next;
	}
	return found;
}

Request * get_request_object_by_id(u32 id) {
	Request * current_request = reservations->head;
	while(current_request != NULL) {
		if(id == current_request->id){
			return current_request;
		} 
		current_request = current_request->next;
	}
	return NULL;
}

void * deny_all_waiting_user_requests(u32 user_id) {
	Request * current_request = reservations->head;
	while(current_request != NULL) {
		if(user_id == current_request->user_id && current_request->request_status == 0){
			current_request->request_status += 2;
		} 
		current_request = current_request->next;
	}
}

void * deny_all_waiting_book_requests(u32 book_id) {
	Request * current_request = reservations->head;
	while(current_request != NULL) {
		if(book_id == current_request->book_id && current_request->request_status == 0){
			current_request->request_status += 2;
		} 
		current_request = current_request->next;
	}
}