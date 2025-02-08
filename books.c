#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "books.h"
#include "types.h"

Books * books;

void initial_books(void) {
	books = malloc(sizeof(Books));
	books->head = NULL;
	books->tail = NULL;
	books->books_number = 0;
	books->next_id = 1;
}

void load_books_data(void) {
	initial_books();
		
	FILE *filePointer;
	filePointer = fopen("books.txt", "r");
	
	u8 chunk[30];
	u32 i;
	
	while(fgets(chunk, sizeof(chunk), filePointer) != NULL) {
		if(strcmp(chunk, "\n") == 0) continue;
		Book * book = malloc(sizeof(Book));
		
		book->id = atoi(chunk);
		
		fgets(chunk, sizeof(chunk), filePointer);
		for (i = 0; chunk[i] != '\n'; i++) {
			book->book_name[i] = chunk[i];
		}
		book->book_name[i] = '\0';
		
		fgets(chunk, sizeof(chunk), filePointer);
		for (i = 0; chunk[i] != '\n'; i++) {
			book->book_author[i] = chunk[i];
		}
		book->book_author[i] = '\0';
		
		fgets(chunk, sizeof(chunk), filePointer);
		book->quantity = atoi(chunk);
		
		fgets(chunk, sizeof(chunk), filePointer);
		book->price = atoi(chunk);
		
		add_book(book);
	}
	fclose(filePointer);
}

void save_books_data(void) {
	FILE *filePointer;
	filePointer = fopen("books.txt", "w");
	
	Book * current_book = books->head;
	
	while(current_book != NULL) {
		
		u8 id[10];
		itoa(current_book->id, id, 10);
		fputs(id, filePointer);
		
		fputs("\n", filePointer);
		fputs(current_book->book_name, filePointer);
		fputs("\n", filePointer);
		fputs(current_book->book_author, filePointer);
		fputs("\n", filePointer);
		
		u8 quantity[10];
		itoa(current_book->quantity, quantity, 10);
		fputs(quantity, filePointer);
		
		fputs("\n", filePointer);
		
		u8 price[10];
		itoa(current_book->price, price, 10);
		fputs(price, filePointer);
		
		fputs("\n\n", filePointer);
		current_book = current_book->next;
	}
	fclose(filePointer);
}

Book * make_book(u8 * name, u8 * author, u32 quantity, u32 price) {
	
	Book * book = malloc(sizeof(Book));
	
	book->id = 0;

	strcpy(book->book_name, name);
	
	strcpy(book->book_author, author);
	
	book->quantity = quantity;
	
	book->price = price;
	
	return book;
}

void add_book(Book * book) {
	
	if(!book->id) {
		book->id = books->next_id++;
	}
	else {
		if(book->id >= books->next_id) {
			books->next_id = book->id + 1;
		}
	}
	
	book->next = NULL;
	book->previous = books->tail;
	
	if(!books->books_number) {
		books->tail = book;
		books->head = book;
	}
	else {
		books->tail->next = book;
		books->tail = book;
	}
	
	books->books_number++;
}

bool delete_book(u32 id) {
	
	if(id > books->next_id) return 0;
	
	Book * current_book = books->head;
	
	while(current_book != NULL) {
		if(current_book->id == id) break;
		current_book = current_book->next;
	}
	if(current_book == NULL) return 0;
	
	if(current_book->previous == NULL && current_book->next == NULL) {
		books->head = NULL;
		books->tail = NULL;
	}
	else if(current_book->previous == NULL) {
		current_book->next->previous = NULL;
		books->head = current_book->next;
	}
	else if(current_book->next == NULL) {
		current_book->previous->next = NULL;
		books->tail = current_book->previous;
	}
	else {
		current_book->previous->next = current_book->next;
		current_book->next->previous = current_book->previous;
	}
	
	free(current_book);
	
	books->books_number--;
	
	return 1;
}

void display_book(Book * book) {
	printf("Id : %-5u Book Name : %s", book->id , book->book_name);
	for(int i = strlen(book->book_name); i < 30; i++) {
		printf(" ");
	}
	
	printf("Author : %s", book->book_author);
	for(int i = strlen(book->book_author); i < 30; i++) {
		printf(" ");
	}
	
	if(book->quantity) printf("Quantity : %-10u", book->quantity);
	else printf("Unavailable          ", book->quantity);
	
	printf("Price : %u\n", book->price);
}

bool display_all_books() {
	
	Book * current_book = books->head;
	bool found = 0;
	printf("\n");
	while(current_book != NULL) {
		found = 1;
		display_book(current_book);
		current_book = current_book->next;
	}
	return found;
}

bool display_all_available_books() {
	
	Book * current_book = books->head;
	bool found = 0;
	printf("\n");
	while(current_book != NULL) {
		found = 1;
		if(current_book->quantity) display_book(current_book);
		current_book = current_book->next;
	}
	return found;
}

bool display_author_books(u8 * author) {
	Book * current_book = books->head;
	bool found = 0;
	printf("\n");
	while(current_book != NULL) {
		if(strcmpi(author, current_book->book_author) == 0){
			display_book(current_book);
			found = 1;
		} 
		current_book = current_book->next;
	}
	return found;
}

bool display_book_by_name(u8 * name) {
	Book * current_book = books->head;
	bool found = 0;
	printf("\n");
	while(current_book != NULL) {
		if(strcmpi(name, current_book->book_name) == 0){
			display_book(current_book);
			found = 1;
			break;
		} 
		current_book = current_book->next;
	}
	return found;
}

bool display_book_by_id(u32 id) {
	Book * current_book = books->head;
	bool found = 0;
	printf("\n");
	while(current_book != NULL) {
		if(id == current_book->id){
			display_book(current_book);
			found = 1;
			break;
		} 
		current_book = current_book->next;
	}
	return found;
}

Book * get_book_object_by_id(u32 id) {
	Book * current_book = books->head;
	while(current_book != NULL) {
		if(id == current_book->id){
			return current_book;
		} 
		current_book = current_book->next;
	}
	return NULL;
}

bool is_book_available(u32 id) {
	Book * current_book = books->head;
	while(current_book != NULL) {
		if(id == current_book->id && current_book->quantity){
			return 1;
		} 
		current_book = current_book->next;
	}
	return 0;
}

bool is_book_name_author_found(u8 * name, u8 * author) {
	Book * current_book = books->head;
	while(current_book != NULL) {
		if(strcmpi(current_book->book_name, name) == 0 && strcmpi(current_book->book_author, author) == 0){
			current_book->quantity++;
			return 1;
		} 
		current_book = current_book->next;
	}
	return 0;
}

void book_search_methods(void) {
	printf("\nEnter search method: 1) By ID     2) By book name     3) By author name.\n");
	u32 method;
	do {
		printf("\nAnswer : ");
		scanf("%u", &method); f;
	} while(method > 3 || method < 1);
	
	switch(method) {
		case 1:
		{
			u32 ID;
			printf("\nEnter book ID : ");
			scanf("%u", &ID); f;
			if(!display_book_by_id(ID)) printf("\nInvalid ID, try again\n");
			
			break;
		}
		case 2:
		{
			u8 * book_name; book_name = malloc(30 * sizeof(u8));
	
			printf("\nEnter book name : ");
			fgets(book_name, sizeof(u8) * 30, stdin);
			
			u8 i;
			for(i = 0; book_name[i] != '\0' && book_name[i] != '\n'; i++);
			book_name[i] = '\0';
			
			if(!display_book_by_name(book_name)) printf("\nInvalid Book name, try again\n");
			
			break;
		}
		case 3:
		{
			u8 * book_author; book_author = malloc(30 * sizeof(u8));
	
			printf("\nEnter book name : ");
			fgets(book_author, sizeof(u8) * 30, stdin);
			
			u8 i;
			for(i = 0; book_author[i] != '\0' && book_author[i] != '\n'; i++);
			book_author[i] = '\0';
			
			if(!display_author_books(book_author)) printf("\nInvalid Author name, try again\n");
		
			break;
		}
	}
}