#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 100
#define MAX_TASK 20
#define LAST_OPTION 5
#define ADDING_SUCCEEDED 1
#define EDITED_SUCCEEDED 2
#define DELETING_SUCCEEDED 3
#define SEARCHING_SUCCEDED 4

typedef struct Task {
	int id[MAX_TASK];
	char detail[MAX_TASK][MAX_TITLE];
	int status[MAX_TASK];
} Task;

void INPUT_read_file(const char* file, Task *myTask, int *list_length);
int INPUT_get_option(void);
int INPUT_get_progress(void);
int INPUT_new_task(int id[], char detail[][MAX_TITLE], int status[], int *list_length);
int INPUT_get_ID(int list_length);
int SYSTEM_delete_task(char detail[][MAX_TITLE], int status[], int *list_length, int ID);
int SYSTEM_edit_task(char detail[][MAX_TITLE], int status[], int ID);
void SYSTEM_shell_sort_task(int id[], char detail[][MAX_TITLE], int status[], int list_length);
void OUTPUT_response(int signal);
void OUTPUT_printing_text(char c, int num);
void OUTPUT_print_tasks(int id[], char detail[][MAX_TITLE], int status[], int list_length);
void OUTPUT_view_task(int id[], char detail[][MAX_TITLE], int status[], int list_length, int display_mode);
int OUTPUT_search_task(int id[], char detail[][MAX_TITLE], int status[], int list_length);

int is_next_integer(const char* ptr);
int is_detail_empty(const char* ptr);

int main(void) {
	Task myTask;
	const char* file = "./data/task.csv";
	int list_length = 0;
	int option, ID, signal;
	int is_searched = 0, display_mode = 0;

	system("clear"); 

	INPUT_read_file(file, &myTask, &list_length);
	while (1) {
		if (!is_searched) {
			system("clear"); 
			OUTPUT_view_task(myTask.id, myTask.detail, myTask.status, list_length, display_mode);
		}
		is_searched = 0;

		printf("\nTO DO LIST");
		printf("\nPlease select an option:");
		printf("\n1. Add a task");
		printf("\n2. Edit a task");
		printf("\n3. Delete a task");
		printf("\n4. Search task");
		if (!display_mode) printf("\n5. Switch to status view");
		else printf("\n5. Switch to ID view");
		printf("\n0. Exit");
		printf("\n");

		option = INPUT_get_option();
		printf("Received option: %d\n", option);

		switch (option) {
			case 1:
				signal = INPUT_new_task(myTask.id, myTask.detail, myTask.status, &list_length);
				OUTPUT_response(signal);
				break;
			case 2:
				ID = INPUT_get_ID(list_length);
				signal = SYSTEM_edit_task(myTask.detail, myTask.status, ID);
				OUTPUT_response(signal);
				break;
			case 3:
				ID = INPUT_get_ID(list_length);
				signal = SYSTEM_delete_task(myTask.detail, myTask.status, &list_length, ID);
				OUTPUT_response(signal);
				break;
			case 4:
				is_searched = OUTPUT_search_task(myTask.id, myTask.detail, myTask.status, list_length);
				if (is_searched) {
					printf("\nPress ENTER to return to menu...");
					while (getchar() != '\n');
				} else {
					printf("No matching task found!");
					printf("\nPress ENTER to return to menu...");
					while (getchar() != '\n');
				}
				break;
			case 5:
				display_mode = 1 - display_mode;
				break;
			case 0:
				system("clear");
				return 0;	
		}
	}
}

int is_next_integer(const char* ptr) {
    int temp;
    return sscanf(ptr, "%d", &temp) == 1;
}

int is_detail_empty(const char* ptr) {
    char temp[2];
    return (sscanf(ptr, "\"%1[^\"]\"", temp) != 1);
}

void INPUT_read_file(const char* file, Task *myTask, int *list_length) {
	FILE* fptr = fopen(file, "r");
	int count_line = 0;
	char line[512];
	
	if (!fptr) {
		perror("Cannot open file");
		return;
	}

	fgets(line, sizeof(line), fptr);
	fgets(line, sizeof(line), fptr);
	
	while ((fgets(line, sizeof(line), fptr)) && count_line < MAX_TASK)
	{
		char s[MAX_TITLE] = "";
        int cur_pos_in_line = 0;
        char* l_ptr = line;
		cur_pos_in_line = 0;

		if (is_next_integer(l_ptr)) {
            sscanf(l_ptr, "%d,%*[^,],%n", &myTask->id[count_line], &cur_pos_in_line);
            l_ptr += cur_pos_in_line;
        } else {
            myTask->id[count_line] = -1;
            l_ptr = strchr(l_ptr, '\"');
        }

		if (sscanf(l_ptr, "\"%[^\"]\"%n", s, &cur_pos_in_line) == 1) {
            strncpy(myTask->detail[count_line], s, MAX_TITLE - 1);
            myTask->detail[count_line][MAX_TITLE - 1] = '\0';
            l_ptr += cur_pos_in_line;
        } else {
            strcpy(myTask->detail[count_line], "\0");
            l_ptr = strchr(l_ptr, ',');
        }

		if (sscanf(l_ptr, ",%d%%,", &myTask->status[count_line]) != 1) {
            myTask->status[count_line] = -1;
        }

		count_line++;
	}
	
	*list_length = count_line;
	fclose(fptr);
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
	int status;
	while (!is_valid) {
		printf("\tYour status [0-100]: ");
		scanf("%d", &status);
		is_valid = (status >= 0 && status <= 100);
	}
	return status;
}

int INPUT_new_task(int id[], char detail[][MAX_TITLE], int status[], int *list_length) {
	if (*list_length == MAX_TASK) return 0;

	printf("\tYour task: ");
	scanf("%50[^\n]", detail[*list_length]);
	while (getchar() != '\n');
	status[*list_length] = INPUT_get_progress();
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

int SYSTEM_delete_task(char detail[][MAX_TITLE], int status[], int *list_length, int ID) {
	if (*list_length == MAX_TASK) return 0;

	for (int i = ID - 1; i < *list_length; i++) {
		status[i] = status[i + 1];
		strcpy(detail[i], detail[i + 1]);
	}
	(*list_length)--;
	return DELETING_SUCCEEDED;
}

int SYSTEM_edit_task(char detail[][MAX_TITLE], int status[], int ID) {
	printf("\tYour task: ");
	scanf("%50[^\n]", detail[ID - 1]);
	while (getchar() != '\n');
	status[ID - 1] = INPUT_get_progress();
	return EDITED_SUCCEEDED;
}

void SYSTEM_shell_sort_task(int id[], char detail[][MAX_TITLE], int status[], int list_length) {
	for (int gap = list_length / 2; gap > 0; gap /= 2) {
		for (int i = gap; i < list_length; i++) {
			int tmp_id = id[i];
			int tmp_progress = status[i];
			char tmp_title[MAX_TITLE];
			int j;
			strcpy(tmp_title, detail[i]);

			for (j = i; j >= gap && status[j - gap] < tmp_progress; j -= gap) {
				status[j] = status[j - gap];
				id[j] = id[j - gap];
				strcpy(detail[j], detail[j - gap]);
			}

			status[j] = tmp_progress;
			id[j] = tmp_id;
			strcpy(detail[j], tmp_title);
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

void OUTPUT_print_tasks(int id[], char detail[][MAX_TITLE], int status[], int list_length) {
	printf("\n");
	OUTPUT_printing_text('=', 90); 
	printf("\nID    PROGRESS   ");
	OUTPUT_printing_text(' ', 22);
	printf("TITLE\n");
	OUTPUT_printing_text('-', 90);
	for (int i = 0; i < list_length; i++) {
		printf("\n[%2d]%7d%% %5s%s", id[i], status[i], " ", detail[i]);
	}
	printf("\n");
	OUTPUT_printing_text('=', 90); 
	printf("\n");
}

void OUTPUT_view_task(int id[], char detail[][MAX_TITLE], int status[], int list_length, int display_mode) {
	if (!list_length) printf("\nNo task yet!\n");
	else {
		if (!display_mode) {
			system("clear");
			printf("\n=== Viewing by ID ===");
			OUTPUT_print_tasks(id, detail, status, list_length);
		} else {
			int tmp_id[MAX_TASK], tmp_progress[MAX_TASK];
			char tmp_list[MAX_TASK][MAX_TITLE];

			for (int i = 0; i < list_length; i++) {
				tmp_id[i] = id[i];
				tmp_progress[i] = status[i];
				strcpy(tmp_list[i], detail[i]);
			}

			system("clear");
			SYSTEM_shell_sort_task(tmp_id, tmp_list, tmp_progress, list_length);
			printf("\n=== Viewing by status ===");
			OUTPUT_print_tasks(tmp_id, tmp_list, tmp_progress, list_length);
		}
	}
}

int OUTPUT_search_task(int id[], char detail[][MAX_TITLE], int status[], int list_length) {
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
			if (strstr(detail[i], searching_title) != NULL) {
				printf("\n[%d]%7d%% %5s%s", id[i], status[i], " ", detail[i]);
				is_found = 1;
			}
		}

		printf("\n");
		OUTPUT_printing_text('=', 68);
		printf("\n");
	}
	return is_found;
}
