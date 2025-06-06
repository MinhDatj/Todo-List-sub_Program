# **TO-DO LIST SUB_PROJECT**
## **1. Overview** 
Short description: This is a simple console-based application written in C that allows users to create to-do list to manage their daily tasks and all tasks will be saved to a csv file.  
Goal: Manage daily tasks by creating a to-do list.  
Objective: Improve C programming skill with static arrays & handling users input and output. Enhance console user interface skills.

## **2. Features**  
This program include serveral functions:  
+ Creating task.
+ Editing task.  
+ Deleting task.  
+ Searching task.
+ Viewing every tasks by completion progress.  

![task_flow_diagram](docs/function_diagram.drawio.png)

## **3. Getting Started**
### **3.1. Prerequisites**
Assuming you don't have any tools, then you will need:  
+ OS: Ubuntu (Tested successfully on version 22.04.05 LTS)  
+ git  (Tested successfully on version 2.34.1)
+ gcc  (Tested successfully on version 11.4.0)  
(You can check if your computer has had git and gcc compiler or not by using `git --version` & `gcc --version`)  
If you haven't had any, first try to install Ubuntu, then type this command on Ubuntu to install the rest:  
``` bash
sudo apt install git build-essential
```
### **3.2. Quick Start**
These belowing steps have been tested successfully on Ubuntu.  
**Step 1**: Clone this repository to your computer by using:  
 ```
 git clone https://github.com/MinhDatj/Todo-List-sub_Program.git
 ```   
**Step 2**: If cloned successfully, you will see a directory named "Todo-List-sub_Program" (use `ls`), go to that directory by using:  
```
cd Todo-List-sub_Program/
```  
After that, you should see yourself in `Todo-List-sub_Program` directory  
**Step 3**: Grant execute permission to all .sh files so you can run them:  
```
chmod +x utils/*.sh
```   
If succeeded, use `ls -l`. You will see a "x" mark at the end of the characters series.  
**Step 4**: To build the program, use:  
```
./utils/build.sh
```  
If succeeded, you will see a directory named `build` that contains a `main` file (use `ls build/`)  
**Step 5**: To run the program, use:  
```
./utils/run.sh
```  
**Bonus**: When you need to delete build directory that was created during process, use:  
```
./utils/clean.sh
```   
