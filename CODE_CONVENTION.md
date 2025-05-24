# **C PROGRAMMING STANDARDS**  
This document is a guildline defining the coding conventions for this C project to ensure clarity, consistency, and ease of maintenance. Use `clang-format` to auto-format accordingly. 

## **1. Variables and Functions naming convention**
It's important to follow these naming rules consistently throughout the program to improve readability, maintanability, and collaboration.
## **🔹1.1 Naming variables**
-  Use **`snake_case`** when naming variables.  
- Use **nouns that are descriptive** unless they are loop counters (`i`, `j`) or very short-lived. Avoid cryptic abbreviations.
<details>
<summary><strong>Example:</strong></summary>

```c
✅
int total_score;    
float user_input;   
char file_path[256];    
❌
int tmp1;   //unclear meaning
float total_Page;   //wrong naming case
```
</details>

-  <u>Macro and constants</u> variables must be written in **`UPPER_CASE_WITH_UNDERSCORES`**. 
<details> 
<summary><strong>Example:</strong></summary>

```c
✅
#define MAX_TASK 1024
const float PI = 3.14159;
❌
#define max_TASK 1024
const float speed = 100;
```
</details>

-  Variables used for <u>checking conditions</u> must start with prefixes like: `is_`, `has_`, etc.  

<details>
<summary><strong>Example:</strong></summary>

```c
✅
int is_valid;
int has_done;
❌
int check;
int valid;
```
</details> 

## **🔹1.2 Naming functions**  
- Use **`snake_case`** when naming functions.  
- Name functions with a **verb or verb_noun** structure to describe what the function does.
- Prefix function names with the **module** they belong to, like: `input_`, `output_`, `file_`, `util_`, etc. (To make it simple, these 3 modules can be used: `input`, `output`, `system`)
<details>
<summary><strong>Example:</strong></summary>

```c
✅
int input_get_id();
void output_show_tasks();
void file_read_tasks();
char* util_trim_string();
❌
void handle(); //too generic
int do_sorting(); //unclear prefixes
```
</details> 

<details>
<summary><strong>Some ideas of prefixes based on functional role</strong></summary>  

| Role     | Prefix Example      | Example Function Name     |
| -------- | ------------------- | ------------------------- |
| Input    | `input_`, `read_`   | `input_get_value()`       |
| Output   | `output_`, `print_` | `output_display_result()` |
| Logic    | `calc_`, `process_` | `calc_average_score()`    |
| File I/O | `file_`, `save_`    | `file_write_data()`       |
| Utility  | `util_`, `helper_`  | `util_trim_string()`      |
</details>
<br>

# **2. Commenting conventions**
Always write clear and consistent comments.

## **Types of comments**
- **Single-line comment:** Use `//` for brief, inline explanations.  
- **Multi-line comment:** Use `/* ... */` for longer or block-level documentation.  

## **🔹2.1 Header comments for functions** 
- Every functions must have comments explain their purpose, parameters, and return value (if applicable).

<details>
<summary><strong>Example:</strong></summary>

```c
/**
* Printing out a character multiple times
*
* @param c      The printed character
* @param num    Number of times the character will be printed
* @return       None
*/
void output_printing_text(char c, int num)
```
</details> 

## **🔹2.2 Inline function comments**
✅ Try to explain the "why" - reason behind the code ❌ not the "what" - what is the code.  

<details>
<summary><strong>Example:</strong></summary>

```c
printf("\nPress ENTER to return to menu...");
✅// Wait for the User to press Enter before continuing
❌// Get newline character 
while (getchar() != '\n');
```
```c
//Check if the User using display mode by status or by ID
if (!display_mode) 
    printf("\n5. Switch to status view");
else 
    printf("\n5. Switch to ID view");
```
</details> 
<br>

# **3. File Structure Conventions**
## **🔹3.1 Structure of a `.c` file**
- Each `.c` file must have **comments describing the purpose** of that `.c` file.  
- Then follow this order of components in a `.c` file:  
    1. Include **standard system libraries**.
    2. Include **project header files**.
    3. Define **macros and constants**.
    4. Internal **structs** (if needed).
    5. Define **global variables** (if any).
    6. **Functions decleration**.
    7. **Functions definitions**.

<details>
<summary><strong>Example:</strong></summary>

```c
✅
// Standard and Project header files
#include <stdio.h>
#include "user.h"

#define MAX_LENGTH 128
const int driving_speed = 128;

// Struct definition
typedef struct {
    int task_id;
    char task_detail[64];
} Task;

// Static/global variables
static int list_length = 0;
int task_count = 0;

// Static/public function declarations
static int is_next_integer(const char* ptr);
void output_printing_text(char c, int num);

// Public function definition
void output_printing_text(char c, int num) {
	for (int i = 0; i < num; i++) printf("%c", c);
}

// Private function definition
static int is_next_integer(const char* ptr) {
    int temp;
    return sscanf(ptr, "%d", &temp) == 1;
}
```
</details> 

# **4. Code Format Conventions**
## **4.1 Indentation & Layout**
- Use **4 spaces** per indentation level.
- Do **not use tab characters** [`\t`] for indentation. Use spaces only.
- **If tabs must be used**, configure them to be equivalent to 4 spaces.
- Indentation **continuation lines by 4 spaces** from the previous line.
- When **breaking params list** into multiple lines, indent them by **4 spaces**.
- **Inline or block comments inside functions** should be **indented to match the indentation level** of the code they describe.

<details>
<summary><strong>Example:</strong></summary>

```c
/**
* An example function to describe the indentation & format rules
*
* @param first_param      
* @param second_param
* @param third_param    
* @return       0
*/
int very_long_function_name_with_many_parameters(
    // first param
    int first_param,
    float second_param,
    const char* third_param) 
{
    return 0;
}
```
</details> 

## **4.2 Alignment**
- Do not align `=` signs in **consecutive assignments**.
- Do not align **consecutive variable declarations**.
- Do not align **operands in long expressions**.
- Do not **trailing comments**.
- Do not align **consecutive macros**.
- Escape lines in multi-line strings should be **left-aligned**.
- Do not align multi-line arguments/parameters after open bracket.

<details>
<summary><strong>Example:</strong></summary>

No.1
```c
✅
int a = 1;
int second_variable = 10;
int serial_number = 5310;
❌
int a               = 1;
int second_variable = 10;
int serial_number   = 5310;
```
No.2
```c
✅
double threshold;
int count;
char third_variable;
❌
double threshold;
int    count;
char   third_variable;
```
No.3
```c 
✅
int final_value = first_value + second_value +
    third_value + fourth_value;
❌
int final_value = first_value + second_value +
                  third_value + fourth_value;
```
No.4
```c
✅
int width = 640; // screen width
int height = 480; // screen height
❌
int width  = 640;  // screen width
int height = 480;  // screen height
```
No.5
```c
✅
#define MAX_SIZE 1024
#define MIN_SIZE 128
#define DEBUG_MODE 1
❌
#define MAX_SIZE   1024
#define MIN_SIZE   128
#define DEBUG_MODE 1
```
No.6
```c
✅
const char* message = "This is a very long message \
that spans multiple lines.";
❌
const char* message = "This is a very long message \
                       that spans multiple lines.";
```
No.7
```c
✅
void a_random_function(int first_variable,
    int second_variable, 
    int third_variable);
❌
void a_random_function(int first_variable,
                       int second_variable,
                       int third_variable);
```
</details> 

## **4.3 Braces & Line Breaks**
### **A. Braces**
- `{` is **placed on the same line** as the control statement or function definition or struct.
- Braces `{}` are needed for **multi-lines control statements**.

<details>
<summary><strong>Example:</strong></summary>

No.1
```c
✅ 
if (is_valid) {
    singing_out_loud();
    dancing_in_the_dark();
}
❌
if (is_valid)
{
    do_something();
}
```
No.2
```c
✅ 
if (is_valid) { 
    do_something();
    return 1;    
} 
if (is_valid) return 1;
```
</details> 

### **B. Line Breaks**
- Line break appear **after binary operators** (if needed).
- Line break appear **before ternary operators**. (**not recommended**, try to use if-else if line break is required).
- **Single-line control statment - if statement** stays on the same line.
- **Long string literals** should be broken into multiple lines.
- **Return type and function name** stay on the same line.
<details>
<summary><strong>Example:</strong></summary>

No.1
```c
int long_name_variable = first_long_variable +
    second_long_variable +
    third_long_variable;
int result = a + b + c;  //short variable
```
No.2
```c
int value = condition
    ? 10
    : 20;
```
No.3
```c
✅ if (is_valid) return 0; 
❌ if (is_valid) 
        return 0; 
```
</details> 


## **4.4 Spaces**
- No space in empty parentheses `{}`.
- No space inside C-style cast parentheses.
- No space inside container initialization.
- No space inside parentheses `()`.
- No space inside square brackets `[]`.
- 2 spaces before end-of-line comments.
- Space after commas `,` and semicolons `;` in statements.
- Space before parens in control statements.

<details>
<summary><strong>Example:</strong></summary>

No.1
```c
✅ void reset() {}
❌ void reset( ) { }
```
No.2 
```c
✅ int value = (int)pi;
❌ int value = (int) pi;
```
No.3 
```c
✅ int arr[] = {1, 2, 3};
❌ int arr[] = { 1, 2, 3 };
```
No.4 
```c
✅ printf("Hello");
❌ printf( "Hello" );
```
No.5 
```c
✅ int a = nums[3];
❌ int a = nums[ 3 ];
```
No.6 
```c
✅ int score = 0;  // initialize to zero
❌ int score = 0; // initialize to zero
❌ int score = 0;   // initialize to zero
```
No.7 
```c
✅ 
for (int i = 0; i < n; i++) {
    // ...
}
void process(int id, float value, char option);
int values[] = {1, 2, 3, 4};
❌ 
for (int i = 0;i < n;i++) {
    // ...
}
void process(int id,float value,char option);
int values[] = {1,2,3,4};
```
No.8 
```c
✅
if (valid) {
    run();
    return 1;
}
❌
if(valid){
    run();
    return 1;
}
```
</details>

## **4.5 Other specific rules**
- No empty line at the start of blocks.
- Line length limit: 80 characters.
- Do not use macros to define blocks.

# **5. Using clang-format**
-  **`clang-format`** is a tool that automatically formats your C/C++ code according to a set of style rules — making your code clean, consistent, and easy to read!
## **5.1 Prerequisites**
Assuming you have had all the tools mentioned in the **Prerequisites** section of [README prerequisites](README.md#31-prerequisites) 
- Check if you have already had `clang-format`:
```bash
clang-format --version
```
- **If not**, use:
```bash
sudo apt install clang-format
```

## **5.2 Quick Start**
These belowing step has been tested successfully on Ubuntu.  
- To format **a single file**, use the command:
```bash
clang-format -i file_name.c
```
- To format **all files**, use the command:
```bash
clang-format -i *.c
// or if you have header files
clang-format -i *.c *.h 
```
**NOTE**: It is recommended to **run `clang-format` before each commit** to ensure consistent formatting.