#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 51
#define MAX_TASK 10
#define LAST_OPTION 4
#define ADDING_SUCCEEDED 1
#define EDITED_SUCCEEDED 2
#define DELETING_SUCCEEDED 3
#define SEARCHING_SUCCEDED 4

int INPUT_get_option(void) {
	int is_valid = 0;
	int option;
	while (!is_valid) {
		printf("Your option: ");
		scanf("%d", &option);
		while (getchar() != '\n');
		is_valid = (option >= 0 && option <= LAST_OPTION);
		if (is_valid) return option;
	}
}

int INPUT_get_progress(void) {
	int is_valid = 0;
	int progress;
	while (!is_valid) {
		printf("\tYour progress [0-100]: ");
		scanf("%d", &progress);
		is_valid = (progress >= 0 && progress <= 100);
		if (is_valid) return progress;
	}
}

int INPUT_new_task(int id[], char list[][MAX_TITLE], int progress[], int *list_length) {
	if (*list_length == MAX_TASK) return 0;

	printf("\tYour task: ");
	scanf("%50[^\n]", list[*list_length]);
	while (getchar() != '\n');
	progress[*list_length] = INPUT_get_progress();
	id[*list_length] = *list_length + 1;
	(*list_length)++;
	return ADDING_SUCCEEDED;
}

int INPUT_get_ID(int list_length) {
	int is_valid = 0;
	int id;
	while (!is_valid) {
		printf("\n\tYour task's ID: ");
		scanf("%d", &id);
		while (getchar() != '\n');
		is_valid = (id >= 0 && id <= list_length + 1);
		if (is_valid) return id;
	}
}

int SYSTEM_delete_task(int id[], char list[][MAX_TITLE], int progress[], int *list_length, int ID) {
	if (*list_length == MAX_TASK) return 0;

	for (int i = ID - 1; i < *list_length; i++) {
		// id[i] = id[i + 1];
		progress[i] = progress[i + 1];
		strcpy(list[i], list[i + 1]);
	}
	(*list_length)--;
	return DELETING_SUCCEEDED;
}

int SYSTEM_edit_task(char list[][MAX_TITLE], int progress[], int ID) {
	printf("\tYour task: ");
	scanf("%50[^\n]", list[ID - 1]);
	while (getchar() != '\n');
	progress[ID - 1] = INPUT_get_progress();
	return EDITED_SUCCEEDED;
}

void OUTPUT_response(int signal) {
	if (!signal) printf("\nFailed!");
	else {
		switch (signal)
		{
		case 1:
			printf("\nTask added successfully!");
			break;
		case 2:
			printf("\nTask edited successfully");
			break;
		case 3:
			printf("\nTask deleted successfully!");
			break;
		}
	}
}

void OUTPUT_printing_text(char c, int num) {
	for (int i = 0; i < num; i++) printf("%c", c);
}

void OUTPUT_view_task(int id[], char list[][MAX_TITLE], int progress[], int list_length) {
	if (!list_length) printf("\nNo task yet!\n");
	else {
		printf("\n");
		OUTPUT_printing_text('=', 68); 
		printf("\nID    PROGRESS   ");
		OUTPUT_printing_text(' ', 22);
		printf("TITLE\n");
		OUTPUT_printing_text('-', 68);
		for (int i = 0; i < list_length; i++) {
			printf("\n[%d]%7d%% %5s%s", id[i], progress[i], " ", list[i]);
		}
		printf("\n");
		OUTPUT_printing_text('=', 68); 
		printf("\n");
	}
}

void OUTPUT_search_task(int id[], char list[][MAX_TITLE], int progress[], int list_length) {
	if (!list_length) printf("\nNo task yet!\n");
	else {
		char searching_title[MAX_TITLE];
		scanf("%50[^\n]", searching_title);
		while (getchar() != '\n');
		int is_found = 0;
		for (int i = 0; i < list_length; i++) {
			if (strstr(list[i], searching_title) != NULL) {
				printf("\n[%d]%7d%% %5s%s", id[i], progress[i], " ", list[i]);
				is_found = 1;
			}
		}
		if(is_found) printf("\nNo matching tasks found!");
	}
}

int main() {
	int id[MAX_TASK];
	char list[MAX_TASK][MAX_TITLE];
	int progress[MAX_TASK];
	int list_length = 0;
	int option, ID, signal, is_searched;
	system("clear"); 

	while (1) {
		if (is_searched) {
			is_searched = 0;
		} else {
			OUTPUT_view_task(id, list, progress, list_length);
		}

		printf("\nTO DO LIST");
		printf("\nPlease select an option:");
		printf("\n1. Add a task");
		printf("\n2. Edit a task");
		printf("\n3. Delete a task");
		// printf("\n4. View all task");
		printf("\n0. Exit");
		printf("\n");

		option = INPUT_get_option();
		printf("Received option: %d", option);
		printf("\n");

		switch (option) {
			case 1:
				signal = INPUT_new_task(id, list, progress, &list_length);
				OUTPUT_response(signal);
				break;
			case 2:
				ID = INPUT_get_ID(list_length);
				signal = SYSTEM_edit_task(list, progress, ID);
				OUTPUT_response(signal);
				break;
			case 3:
				ID = INPUT_get_ID(list_length);
				signal = SYSTEM_delete_task(id, list, progress, &list_length, ID);
				OUTPUT_response(signal);
				break;
			// case 4:
			// 	OUTPUT_view_task(id, list, progress, list_length);
			// 	break;
			case 0:
				system("clear");
				return 0;	
		}
		system("clear"); 
	}
}