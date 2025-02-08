#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "provide_books.h"
#include "types.h"

BooksProvider * books_provider;

void initial_books_provider(void) {
	books_provider = malloc(sizeof(BooksProvider));
	books_provider->head = NULL;
	books_provider->tail = NULL;
	books_provider->books_number = 0;
	books_provider->next_id = 1;
}

void load_books_provider_data(void) {
	initial_books_provider();
		
	FILE *filePointer;
	filePointer = fopen("books_provider.txt", "r");
	
	u8 chunk[30];
	u32 i;
	
	while(fgets(chunk, sizeof(chunk), filePointer) != NULL) {
		if(strcmp(chunk, "\n") == 0) continue;
		NeededBook * needed_book = malloc(sizeof(NeededBook));
		
		needed_book->id = atoi(chunk);
		
		fgets(chunk, sizeof(chunk), filePointer);
		for (i = 0; chunk[i] != '\n'; i++) {
			needed_book->book_name[i] = chunk[i];
		}
		needed_book->book_name[i] = '\0';
		
		fgets(chunk, sizeof(chunk), filePointer);
		for (i = 0; chunk[i] != '\n'; i++) {
			needed_book->book_author[i] = chunk[i];
		}
		needed_book->book_author[i] = '\0';
		
		add_needed_book(needed_book);
	}
	fclose(filePointer);
}

void save_books_provider_data(void) {
	FILE *filePointer;
	filePointer = fopen("books_provider.txt", "w");
	
	NeededBook * current_needed_book = books_provider->head;
	
	while(current_needed_book != NULL) {
		
		u8 id[10];
		itoa(current_needed_book->id, id, 10);
		fputs(id, filePointer);
		fputs("\n", filePointer);
		
		fputs(current_needed_book->book_name, filePointer);
		fputs("\n", filePointer);
		
		fputs(current_needed_book->book_author, filePointer);
		fputs("\n\n", filePointer);
		
		current_needed_book = current_needed_book->next;
	}
	fclose(filePointer);
}

NeededBook * make_needed_book(u8 * name, u8 * author) {
	
	NeededBook * needed_book = malloc(sizeof(NeededBook));
	
	needed_book->id = 0;

	strcpy(needed_book->book_name, name);
	
	strcpy(needed_book->book_author, author);
	
	return needed_book;
}

void add_needed_book(NeededBook * needed_book) {
		
	if(!needed_book->id) {
		needed_book->id = books_provider->next_id++;
	}
	else {
		if(needed_book->id >= books_provider->next_id) {
			books_provider->next_id = needed_book->id + 1;
		}
	}
	
	needed_book->next = NULL;
	needed_book->previous = books_provider->tail;
	
	if(!books_provider->books_number) {
		books_provider->tail = needed_book;
		books_provider->head = needed_book;
	}
	else {
		books_provider->tail->next = needed_book;
		books_provider->tail = needed_book;
	}
	
	books_provider->books_number++;
}

void delete_needed_book(NeededBook * needed_book) {
	
	
	if(needed_book->previous == NULL && needed_book->next == NULL) {
		books_provider->head = NULL;
		books_provider->tail = NULL;
	}
	else if(needed_book->previous == NULL) {
		needed_book->next->previous = NULL;
		books_provider->head = needed_book->next;
	}
	else if(needed_book->next == NULL) {
		needed_book->previous->next = NULL;
		books_provider->tail = needed_book->previous;
	}
	else {
		needed_book->previous->next = needed_book->next;
		needed_book->next->previous = needed_book->previous;
	}
	
	free(needed_book);
	
	books_provider->books_number--;
	
}

void display_needed_book(NeededBook * needed_book) {
	printf("Id : %-5u Book Name : %s", needed_book->id , needed_book->book_name);
	for(int i = strlen(needed_book->book_name); i < 30; i++) {
		printf(" ");
	}
	
	printf("Author : %s\n", needed_book->book_author);
}

bool display_all_needed_books(void) {
	
	NeededBook * current_needed_book = books_provider->head;
	bool found = 0;
	printf("\n");
	while(current_needed_book != NULL) {
		display_needed_book(current_needed_book);
		found = 1;
		current_needed_book = current_needed_book->next;
	}
	return found;
}

NeededBook * get_needed_book_object_by_id(u32 id) {
	NeededBook * current_needed_book = books_provider->head;
	while(current_needed_book != NULL) {
		if(id == current_needed_book->id){
			return current_needed_book;
		} 
		current_needed_book = current_needed_book->next;
	}
	return NULL;
}