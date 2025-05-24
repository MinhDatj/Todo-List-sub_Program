#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DETAIL 128
#define MAX_TASK 20
#define TOTAL_OPTION 5
#define ADDING_SUCCEEDED 1
#define EDITING_SUCCEEDED 2
#define DELETING_SUCCEEDED 3
#define SEARCHING_SUCCEDED 4

typedef struct task {
	int id[MAX_TASK];
	char detail[MAX_TASK][MAX_DETAIL];
	int status[MAX_TASK];
} task;

void file_read(const char* file_path, task* my_task, int* list_length);
void file_write(const char* file_path, task* my_task, int line_count);
void file_update_task(const char* file_name, int id, const char* new_detail,
		int new_status, int mode);

int input_get_user_option(void);
int input_get_task_status(void);
int input_get_task_id(int list_length);
int input_get_new_task(int id[], char detail[][MAX_DETAIL], int status[],
		int* list_length);

int task_delete(char detail[][MAX_DETAIL], int status[], int* list_length,
		int id);
int task_edit(char detail[][MAX_DETAIL], int status[], int id);
void task_sort_by_status(int id[], char detail[][MAX_DETAIL], int status[],
		int list_length);

void output_respond_to_user(int action_code);
void output_print_multiple_times(char c, int num);
void output_print_all_task(int id[], char detail[][MAX_DETAIL], int status[],
		int list_length);
int output_search_task(int id[], char detail[][MAX_DETAIL], int status[],
		int list_length);

void util_handle_view_mode(int id[], char detail[][MAX_DETAIL], int status[],
		int list_length, int display_mode);
int util_is_next_integer(const char* ptr);

/**
 * @brief Everything run inside this function
 *
 * @return 0 on successful program exit.
 */
int main(void) {
	task my_task;
	const char* file_path = "./data/task.csv";
	int list_length = 0, is_searched = 0, display_mode = 0;
	int option, id, action_code;

	system("clear");

	file_read(file_path, &my_task, &list_length);
	while (1) {
		if (!is_searched) {
			system("clear");
			util_handle_view_mode(my_task.id, my_task.detail, my_task.status,
					list_length, display_mode);
		}
		is_searched = 0;

		printf("\nTO DO LIST");
		printf("\nPlease select an option:");
		printf("\n1. Add a task");
		printf("\n2. Edit a task");
		printf("\n3. Delete a task");
		printf("\n4. Search task");

		if (!display_mode)
			printf("\n5. Switch to status view");
		else
			printf("\n5. Switch to id view");

		printf("\n0. Exit");
		printf("\n");

		option = input_get_user_option();
		printf("Received option: %d\n", option);

		switch (option) {
		case 1:
			action_code = input_get_new_task(my_task.id, my_task.detail,
					my_task.status, &list_length);
			output_respond_to_user(action_code);
			file_write(file_path, &my_task, list_length);
			break;
		case 2:
			id = input_get_task_id(list_length);
			action_code = task_edit(my_task.detail, my_task.status, id);
			output_respond_to_user(action_code);
			file_update_task(file_path, id, my_task.detail[id - 1],
					my_task.status[id - 1], 1);
			break;
		case 3:
			id = input_get_task_id(list_length);
			action_code = task_delete(my_task.detail, my_task.status,
					&list_length, id);
			output_respond_to_user(action_code);
			file_update_task(file_path, id, my_task.detail[id - 1],
					my_task.status[id - 1], 0);
			break;
		case 4:
			is_searched = output_search_task(my_task.id, my_task.detail,
					my_task.status, list_length);
			if (is_searched) {
				printf("\nPress ENTER to return to menu...");
				while (getchar() != '\n')
					;
			} else {
				printf("No matching task found!");
				printf("\nPress ENTER to return to menu...");
				while (getchar() != '\n')
					;
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

/**
 * @brief function check if the character in the data is an integer or not to
 *read the task in the file
 *
 * @param ptr		Pointer to that character.
 * @return 1 if it is an integer,
 *		   0 if it is not.
 */
int util_is_next_integer(const char* ptr) {
	int temp;
	// if scanning succesfully, it means that character is an integer
	return sscanf(ptr, "%d", &temp) == 1;
}

/**
 * @brief function read all tasks from the file and turning them into data in
 * the program.
 *
 * @param file_path		The path of the file containing data.
 * @param my_task		Pointer to a task struct containing task data arrays.
 * @param list_length	Pointer to the current number of tasks.
 */
void file_read(const char* file_path, task* my_task, int* list_length) {
	FILE* file_pointer = fopen(file_path, "r");
	int line_count = 0;
	char line[512];

	if (!file_pointer) {
		perror("Cannot open file_path");
		return;
	}

	// skip two first line in the file
	fgets(line, sizeof(line), file_pointer);
	fgets(line, sizeof(line), file_pointer);

	while ((fgets(line, sizeof(line), file_pointer)) && line_count < MAX_TASK) {
		char task_detail_buffer[MAX_DETAIL] = "";
		int chars_consumed = 0;
		char* line_pointer = line;
		chars_consumed = 0;

		/* Check if the id of a task is not missing then read normally,
		else mark id value as -1
		*/
		if (util_is_next_integer(line_pointer)) {
			sscanf(line_pointer, "%d,%*[^,],%n", &my_task->id[line_count],
					&chars_consumed);
			line_pointer += chars_consumed;  // Move the pointer forward
		} else {
			my_task->id[line_count] = -1;
			line_pointer = strchr(line_pointer,
					'\"');  // Move to the start of the detail
		}

		/* Read the detail string enclosed in double quotes & move the pointer
		to that position if detail is empty, set detail of the task empty too
		*/
		if (sscanf(line_pointer, "\"%[^\"]\"%n", task_detail_buffer,
					&chars_consumed)
				== 1) {
			strncpy(my_task->detail[line_count], task_detail_buffer,
					MAX_DETAIL - 1);
			// Ensure null-termination
			my_task->detail[line_count][MAX_DETAIL - 1] = '\0';
			line_pointer += chars_consumed;  // Move the pointer forward
		} else {
			strcpy(my_task->detail[line_count], "\0");
			line_pointer = strchr(line_pointer,
					',');  // Move to the start of status data
		}

		// Extract the task status. If status is empty, set the value to -1
		if (sscanf(line_pointer, ",%d%%,", &my_task->status[line_count]) != 1) {
			my_task->status[line_count] = -1;
		}

		line_count++;
	}

	*list_length = line_count;
	fclose(file_pointer);
}

/**
 * @brief the user's option from the menu and make sure the option is
 * validating.
 *
 * @param void
 * @return option	The selected menu option.
 */
int input_get_user_option(void) {
	int is_valid = 0;
	int option;

	while (!is_valid) {
		printf("Your option: ");
		scanf("%d", &option);
		while (getchar() != '\n')
			;
		// Make sure the option from the user stays in the range of the menu
		// option
		is_valid = (option >= 0 && option <= TOTAL_OPTION);
	}

	return option;
}

/**
 * @brief the task's status from the user, status value must stays between 0 &
 * 100
 *
 * @param void
 * @return status	The status of that task as an integer.
 */
int input_get_task_status(void) {
	int is_valid = 0;
	int status;

	while (!is_valid) {
		printf("\tYour status [0-100]: ");
		scanf("%d", &status);
		// Make sure status value stay between 0 to 100 percent
		is_valid = (status >= 0 && status <= 100);
	}

	return status;
}

/**
 * @brief function gets inputs from the user to create a new task. Including:
 *task's status, detail. Increase task count by 1.
 *
 * @param id 			Array storing task IDs.
 * @param detail 		2D array storing task details.
 * @param status		Array storing task statuses.
 * @param list_length 	Pointer to the current number of tasks.
 * @return ADDING_SUCCEEDED	if the new task is successfully added,
 *		   or 0 if the task list is full.
 */
int input_get_new_task(int id[], char detail[][MAX_DETAIL], int status[],
		int* list_length) {
	if (*list_length == MAX_TASK) return 0;

	// Scan for detail of new task
	printf("\tYour task: ");
	scanf("%127[^\n]", detail[*list_length]);
	while (getchar() != '\n')
		;

	// Scan for status of new task
	status[*list_length] = input_get_task_status();
	id[*list_length] = *list_length + 1;
	// Increase the total tasks by 1
	(*list_length)++;

	return ADDING_SUCCEEDED;
}

/**
 * @brief function gets a task's id from the user and make sure that id exists.
 *
 * @param list_length 	Total number of tasks in the list.
 * @return id			The id needed to get.
 */
int input_get_task_id(int list_length) {
	int is_valid = 0;
	int id;

	while (!is_valid) {
		printf("\n\tYour task's id: ");
		scanf("%d", &id);
		while (getchar() != '\n')
			;
		// Make sure id value stay in range of the list length
		is_valid = (id >= 0 && id <= list_length + 1);
	}

	return id;
}

/**
 * @brief function delete a task by the user sending the task's id.
 *
 * @param detail		2D array storing task details.
 * @param status		Array storing task statuses.
 * @param list_length	Pointer to the current number of tasks.
 * @param id			The id of the task
 * @return DELETING_SUCCEEDED	if the new task is successfully deleted,
 * 		   or 0 if the task list is full.
 */
int task_delete(char detail[][MAX_DETAIL], int status[], int* list_length,
		int id) {
	if (*list_length == MAX_TASK) return 0;

	// Move up all the tasks behind the deleted one
	for (int i = id - 1; i < *list_length; i++) {
		status[i] = status[i + 1];
		strcpy(detail[i], detail[i + 1]);
	}

	(*list_length)--;  // Decrease total tasks by 1

	return DELETING_SUCCEEDED;
}

/**
 * @brief function edit detail, status of a task by the user sending the task's
 * id.
 *
 * @param detail		2D array to store task details.
 * @param status		Array to store task statuses.
 * @param id			The id of the task
 * @return EDITING_SUCCEEDED 	if the new task is successfully edited,
 */
int task_edit(char detail[][MAX_DETAIL], int status[], int id) {
	char temp[MAX_DETAIL];

	// the user can press Enter to skip the detail-input part
	printf("\tYour new task [press Enter to skip]: ");
	fgets(temp, sizeof(temp), stdin);
	temp[strcspn(temp, "\n")] = 0;

	if (strlen(temp) > 0) { strcpy(detail[id - 1], temp); }

	status[id - 1] = input_get_task_status();

	return EDITING_SUCCEEDED;
}

/**
 * @brief function sort and rearrange the task list by status using shell sort
 * algorithm. Task with higher status will go on top.
 *
 * @param id 			Array storing task IDs.
 * @param detail		2D array storing task details.
 * @param status		Array storing task statuses.
 * @param list_length	Total number of tasks in the list.
 */
void task_sort_by_status(int id[], char detail[][MAX_DETAIL], int status[],
		int list_length) {
	for (int gap = list_length / 2; gap > 0; gap /= 2) {
		for (int i = gap; i < list_length; i++) {
			int id_buffer = id[i];
			int status_buffer = status[i];
			char detail_buffer[MAX_DETAIL];
			int j;
			strcpy(detail_buffer, detail[i]);

			for (j = i; j >= gap && status[j - gap] < status_buffer; j -= gap) {
				status[j] = status[j - gap];
				id[j] = id[j - gap];
				strcpy(detail[j], detail[j - gap]);
			}

			status[j] = status_buffer;
			id[j] = id_buffer;
			strcpy(detail[j], detail_buffer);
		}
	}
}

/**
 * @brief function appends the most recently added task to the file
 *
 * @param file_path 	Path of the file to which the task will be writen.
 * @param my_task		Pointer to a task struct containg task data.
 * @param list_length 	Total number of tasks in the list.
 */
void file_write(const char* file_path, task* my_task, int list_length) {
	FILE* file_pointer = fopen(file_path, "a");

	if (!file_pointer) {
		printf("File pointer is NULL.\n");
		return;
	}

	list_length--;
	fprintf(file_pointer, "%d, ,\"%s\",%d%%, , \n", my_task->id[list_length],
			my_task->detail[list_length], my_task->status[list_length]);

	fclose(file_pointer);
}

/**
 * @brief function either edits a task or delete a task in the file by using the
 * task's id to determine which line to edit.
 *
 * @param file_path 	Path of the file to which the task will be writen.
 * @param id            ID of the task to edit or delete.
 * @param new_detail	New detail of the task (only for edit).
 * @param list_length	Pointer to the current number of tasks.
 * @param new_status 	New status of the task (only for edit).
 * @param mode			Operation mode: 1 is edit, 0 is delete.
 */
void file_update_task(const char* file_name, int id, const char* new_detail,
		int new_status, int mode) {
	FILE* original_file = fopen(file_name, "r");
	FILE* temp_file = fopen("temp.csv", "w");

	char line[512];
	int current_line = 0;

	/*
	If mode is 1: edits the task in the file.
	If mode is 0: deletes the task in the file.
	*/
	if (mode) {
		/* Read and rewrite every tasks to another file until the required task,
		then write the edited one, and finish the rest.
		*/
		while (fgets(line, sizeof(line), original_file)) {
			// When it is on the required line correspond to the id
			if (current_line == (id + 1)) {
				char title[50];
				char the_rest[15];
				sscanf(line, "%*d,%[^,],\"%*[^\"]\",%*d%%,%s", title, the_rest);
				fprintf(temp_file, "%d,%s,\"%s\",%d%%,%s\n", id, title,
						new_detail, new_status, the_rest);
			} else {
				fputs(line, temp_file);
			}
			current_line++;
		}
	} else {
		/* Read and rewrite every tasks to another file,
		but skip the line suppose to be deleted
		*/
		while (fgets(line, sizeof(line), original_file)) {
			if (current_line != (id + 1)) { fputs(line, temp_file); }
			current_line++;
		}
	}

	fclose(original_file);
	fclose(temp_file);

	remove(file_name);  // Delete the original file
	rename("temp.csv", file_name);  // Rename the temp file to the original file
}

/**
 * @brief function displays a response message to the user based on the task
 * action performed.
 *
 * @param action_code	A value representing the aaction result.
 */
void output_respond_to_user(int action_code) {
	if (!action_code)
		printf("\nThe task list is full!");
	else {
		switch (action_code) {
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

/**
 * @brief function prints out a character multiple times.
 *
 * @param character		The character to print.
 * @param num			The number of times to print the character.
 */
void output_print_multiple_times(char character, int num) {
	for (int i = 0; i < num; i++)
		printf("%c", character);
}

/**
 * @brief function prints the full list of tasks in a formatted table.
 *
 * @param id			Array storing task ids.
 * @param detail		2D array storing task detials.
 * @param status		Array storing task statuses.
 * @param list_length	Total number of tasks in the list.
 */
void output_print_all_task(int id[], char detail[][MAX_DETAIL], int status[],
		int list_length) {
	printf("\n");
	output_print_multiple_times('=', 90);
	printf("\nID    PROGRESS   ");
	output_print_multiple_times(' ', 22);
	printf("TITLE\n");
	output_print_multiple_times('-', 90);

	// Print every tasks in the list
	for (int i = 0; i < list_length; i++) {
		printf("\n[%2d]%7d%% %5s%s", id[i], status[i], " ", detail[i]);
	}

	printf("\n");
	output_print_multiple_times('=', 90);
	printf("\n");
}

/**
 * @brief Handles task list display based on selected view mode (by ID or by
 * status).
 *
 * @param id 			Array of task ids.
 * @param detail		2D array of task details.
 * @param status		Array of task statuses.
 * @param list_length	Total number of task in the lists.
 * @param display_mode	0 for viewing by id, 1 for viewing by status.
 */
void util_handle_view_mode(int id[], char detail[][MAX_DETAIL], int status[],
		int list_length, int display_mode) {
	if (!list_length)
		printf("\nNo task yet!\n");
	else {
		system("clear");

		if (!display_mode) {
			printf("\n=== Viewing by id ===");
			output_print_all_task(id, detail, status, list_length);
		} else {
			// Create all temporary array to store tasks sorted by status
			int id_buffer[MAX_TASK], status_buffer[MAX_TASK];
			char list_buffer[MAX_TASK][MAX_DETAIL];

			for (int i = 0; i < list_length; i++) {
				id_buffer[i] = id[i];
				status_buffer[i] = status[i];
				strcpy(list_buffer[i], detail[i]);
			}

			task_sort_by_status(id_buffer, list_buffer, status_buffer,
					list_length);
			printf("\n=== Viewing by status ===");
			output_print_all_task(id_buffer, list_buffer, status_buffer,
					list_length);
		}
	}
}

/**
 * @brief Searches and displays tasks that match a given title substring.
 *
 * @param id            Array of task ids.
 * @param detail        2D array of task details.
 * @param status        Array of task statuses.
 * @param list_length   The number of tasks in the list.
 * @return is_found             1 if a match is found, 0 otherwise.
 */
int output_search_task(int id[], char detail[][MAX_DETAIL], int status[],
		int list_length) {
	int is_found = 0;
	if (!list_length)
		printf("\nNo task yet!\n");
	else {
		char searching_title[MAX_DETAIL];
		printf("\tSearching title: ");
		scanf("%127[^\n]", searching_title);
		while (getchar() != '\n')
			;

		output_print_multiple_times('=', 68);
		printf("\nID    PROGRESS   ");
		output_print_multiple_times(' ', 22);
		printf("TITLE\n");
		output_print_multiple_times('-', 68);

		for (int i = 0; i < list_length; i++) {
			if (strstr(detail[i], searching_title) != NULL) {
				printf("\n[%d]%7d%% %5s%s", id[i], status[i], " ", detail[i]);
				is_found = 1;
			}
		}

		printf("\n");
		output_print_multiple_times('=', 68);
		printf("\n");
	}
	return is_found;
}
