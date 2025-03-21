#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 50
#define MAX_TASK 10
#define LAST_OPTION 4
#define ADDING_SUCCEEDED 1
#define FIXING_SUCCEEDED 2
#define DELETING_SUCCEEDED 3

int INPUT_get_option(void) {
	int is_valid = 0;
	int option;
	while (!is_valid) {
		printf("Your option: ");
		scanf("%d", &option);
		is_valid = (option >= 0 && option <= LAST_OPTION);
		if (is_valid) return option;
	}
}

int INTPUT_get_progress(void) {
	int is_valid = 0;
	int progress;
	while (!is_valid) {
		printf("Your progress [0-100]: ");
		scanf("%d", &progress);
		is_valid = (progress >= 0 && progress <= 100);
		if (is_valid) return progress;
	}
}

int INPUT_new_task(int id[], char list[][MAX_TITLE], int progress[], int *list_length) {
	if (*list_length == MAX_TASK) return 0;

	printf("\t\nYour task: ");
	scanf("%49s%*c", list[*list_length]);
	progress[*list_length] = INTPUT_get_progress();
	(*list_length)++;
	return ADDING_SUCCEEDED;
}

void OUTPUT_response(int signal) {
	if (!signal) printf("\nFailed!");

	switch (signal)
	{
	case 1:
		printf("\nAdding task successfully!");
		break;
	}
}

int main() {
	int id[MAX_TASK];
	char list[MAX_TASK][MAX_TITLE];
	int progress[MAX_TASK];
	int list_length = 0;
	int option, ID, signal;

	printf("\nTO DO LIST");
    printf("\nPlease select an option:");
    printf("\n1. Add a task");
    printf("\n2. Edit a task");
    printf("\n3. Delete a task");
    printf("\n4. View all task");
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
			break;
		case 3:
			break;
		case 4:
			break;
		case 0:
			return 0;	
	}

	return 0;
}