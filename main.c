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

int INPUT_get_option(void);
int INPUT_get_progress(void);
int INPUT_new_task(int id[], char list[][MAX_TITLE], int progress[], int *list_length);
int INPUT_get_ID(int list_length);
int SYSTEM_delete_task(char list[][MAX_TITLE], int progress[], int *list_length, int ID);
int SYSTEM_edit_task(char list[][MAX_TITLE], int progress[], int ID);
void SYSTEM_shell_sort_task(int id[], char list[][MAX_TITLE], int progress[], int list_length);
void OUTPUT_response(int signal);
void OUTPUT_printing_text(char c, int num);
void OUTPUT_print_tasks(int id[], char list[][MAX_TITLE], int progress[], int list_length);
void OUTPUT_view_task(int id[], char list[][MAX_TITLE], int progress[], int list_length);
int OUTPUT_search_task(int id[], char list[][MAX_TITLE], int progress[], int list_length);

int main(void) {
	int id[MAX_TASK];
	char list[MAX_TASK][MAX_TITLE];
	int progress[MAX_TASK];
	int list_length = 0;
	int option, ID, signal, is_searched = 0;
	system("clear"); 

	while (1) {
		if (!is_searched) {
			system("clear"); 
			OUTPUT_view_task(id, list, progress, list_length);
		}
		is_searched = 0;

		printf("\nTO DO LIST");
		printf("\nPlease select an option:");
		printf("\n1. Add a task");
		printf("\n2. Edit a task");
		printf("\n3. Delete a task");
		printf("\n4. Search task");
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
				signal = SYSTEM_delete_task(list, progress, &list_length, ID);
				OUTPUT_response(signal);
				break;
			case 4:
				is_searched = OUTPUT_search_task(id, list, progress, list_length);
				if (is_searched) {
					printf("\nPress ENTER to return to menu...");
					while (getchar() != '\n');
				} else {
					printf("No matching task found!");
					printf("\nPress ENTER to return to menu...");
					while (getchar() != '\n');
				}
				break;
			case 0:
				system("clear");
				return 0;	
		}
	}
}

int INPUT_get_option(void) {
	int is_valid = 0;
	int option;
	while (!is_valid) {
		printf("Your option: ");
		scanf("%d", &option);
		while (getchar() != '\n');
		is_valid = (option >= 0 && option <= LAST_OPTION);
	}
	return option;
}

int INPUT_get_progress(void) {
	int is_valid = 0;
	int progress;
	while (!is_valid) {
		printf("\tYour progress [0-100]: ");
		scanf("%d", &progress);
		is_valid = (progress >= 0 && progress <= 100);
	}
	return progress;
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
	}
	return id;
}

int SYSTEM_delete_task(char list[][MAX_TITLE], int progress[], int *list_length, int ID) {
	if (*list_length == MAX_TASK) return 0;

	for (int i = ID - 1; i < *list_length; i++) {
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

void SYSTEM_shell_sort_task(int id[], char list[][MAX_TITLE], int progress[], int list_length) {
	for (int gap = list_length / 2; gap > 0; gap /= 2) {
		for (int i = gap; i < list_length; i++) {
			int tmp_id = id[i];
			int tmp_progress = progress[i];
			char tmp_title[MAX_TITLE];
			int j;
			strcpy(tmp_title, list[i]);

			for (j = i; j >= gap && progress[j - gap] < tmp_progress; j -= gap) {
				progress[j] = progress[j - gap];
				id[j] = id[j - gap];
				strcpy(list[j], list[j - gap]);
			}

			progress[j] = tmp_progress;
			id[j] = tmp_id;
			strcpy(list[j], tmp_title);
		}
	}
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

void OUTPUT_print_tasks(int id[], char list[][MAX_TITLE], int progress[], int list_length) {
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

void OUTPUT_view_task(int id[], char list[][MAX_TITLE], int progress[], int list_length) {
	if (!list_length) printf("\nNo task yet!\n");
	else {
		int choice;
		do {
			printf("\nChoose display order:");
			printf("\n1. Sort by ID");
			printf("\n2. Sort by progress");
			printf("\n0. Return to Menu");
			printf("\n");
			scanf("%d", &choice);
			while (getchar() != '\n');
			system("clear");

			if (choice == 1) {
				printf("\nViewing by ID");
				OUTPUT_print_tasks(id, list, progress, list_length);
			}
			
			if (choice == 2) {
				int tmp_id[MAX_TASK], tmp_progress[MAX_TASK];
				char tmp_list[MAX_TASK][MAX_TITLE];

				for (int i = 0; i < list_length; i++) {
					tmp_id[i] = id[i];
					tmp_progress[i] = progress[i];
					strcpy(tmp_list[i], list[i]);
				}

				SYSTEM_shell_sort_task(tmp_id, tmp_list, tmp_progress, list_length);
				printf("\nViewing by progress");
				OUTPUT_print_tasks(tmp_id, tmp_list, tmp_progress, list_length);
			}
		} while (choice != 0);
	}
}

int OUTPUT_search_task(int id[], char list[][MAX_TITLE], int progress[], int list_length) {
	int is_found = 0;
	if (!list_length) printf("\nNo task yet!\n");
	else {
		char searching_title[MAX_TITLE];
		printf("\tSearching title: ");
		scanf("%50[^\n]", searching_title);
		while (getchar() != '\n');

		OUTPUT_printing_text('=', 68); 
		printf("\nID    PROGRESS   ");
		OUTPUT_printing_text(' ', 22);
		printf("TITLE\n");
		OUTPUT_printing_text('-', 68);

		for (int i = 0; i < list_length; i++) {
			if (strstr(list[i], searching_title) != NULL) {
				printf("\n[%d]%7d%% %5s%s", id[i], progress[i], " ", list[i]);
				is_found = 1;
			}
		}

		printf("\n");
		OUTPUT_printing_text('=', 68);
		printf("\n");
	}
	return is_found;
}
