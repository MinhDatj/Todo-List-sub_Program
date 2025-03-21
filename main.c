#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 50
#define MAX_TASK 10
#define LAST_OPTION 4

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


int main() {
	int id[MAX_TASK];
	char list[MAX_TASK][MAX_TITLE];
	int progress[MAX_TASK];
	int list_length = 0;
	int option, ID;

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