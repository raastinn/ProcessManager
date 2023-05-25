#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "linked_list.h"

Node* head = NULL;


void func_BG(char **cmd){
	if (cmd[1] == NULL) {
		printf("Please enter a file name after bg. Eg. 'bg <file>'\n");
		return;
	}
	pid_t pid = fork();
	if (pid == 0) { // 0 is child
		char* input = cmd[1];
		execvp(cmd[1], cmd+1);
		printf("Error: execution of %s has failed\n", input);
		exit(-1);
	} else if(pid>0) { // >0 is parent
		int p_status;
		int retVal;
		retVal = waitpid(pid, &p_status, WNOHANG | WUNTRACED | WCONTINUED);
		if (retVal == -1) {
			printf("Error: failed at waitpid.\n");
			exit(-1);
		}
		char cwd[1024];
		char slash = '/';
		getcwd(cwd, sizeof(cwd));
		strncat(cwd, &slash, 1);
		strcat(cwd, cmd[1]);
		head = add_newNode(head, pid, cwd);
		sleep(1);
	} else {
		printf("Error: Failed to fork.\n");
	}
}


void func_BGlist(char **cmd){
	printList(head);
}


void func_BGkill(char * str_pid){
	if (str_pid == NULL) {
		printf("Error: no pid entered. Use syntax 'bgkill <pid>'\n");
		return;
	}
	pid_t pid = atoi(str_pid);
	int pid_exists = PifExist(head, pid);
	if (pid_exists == false) {
		printf("Error: Process %d does not exist.\n", pid);
		return;
	} else {
		int retVal = kill(pid, SIGTERM);
		if (retVal == 0) {
			sleep(1);
		} else {
			printf("Error: failed bgkill <pid>\n");
		}
	}
}


void func_BGstop(char * str_pid){
	if (str_pid == NULL) {
		printf("Error: no pid entered. Use syntax 'bgstop <pid>'\n");
		return;
	}
	pid_t pid = atoi(str_pid);
	int pid_exists = PifExist(head, pid);
	if (pid_exists == false) {
		printf("Error: Process %d does not exist.\n", pid);
		return;
	} else {
		int retVal = kill(pid, SIGSTOP);
		if (retVal == 0) {
			sleep(1);
		} else {
			printf("Error: failed bgstop <pid>\n");
		}
	}
}


void func_BGstart(char * str_pid){
	if (str_pid == NULL) {
		printf("Error: nod pid entered. Use syntax 'bgstart <pid>\n");
		return;
	}
	pid_t pid = atoi(str_pid);
	int pid_exists = PifExist(head, pid);
	if (pid_exists == false) {
		printf("Error: Process %d does not exist.\n", pid);
		return;
	} else {
		int retVal = kill(pid, SIGCONT);
		if (retVal == 0) {
			sleep(1);
		} else {
			printf("Error: failed bgstart <pid>\n");
		}
	}
}


void func_pstat(char * str_pid){
	if (str_pid == NULL) {
		printf("Error: no pid entered. Use syntax 'pstat <pid>'\n");
		return;
	}
	pid_t pid = atoi(str_pid);
	int pid_exists = PifExist(head, pid);
	if (pid_exists == false) {
		printf("Error: Process %d does not exist.\n", pid);
		return;
	}
	char filename[1000];
	sprintf(filename, "/proc/%d/stat", pid);
	FILE *file_ptr = fopen(filename, "r");
	if (file_ptr != NULL) {
		char fileBuff[256];
		int i = 0;
		while(fscanf(file_ptr, "%s", fileBuff) != EOF) {
			if (i == 1) {
				printf("Comm:\t\t\t\t%s\n", fileBuff);
			} else if (i == 2) {
				printf("State:\t\t\t\t%s\n", fileBuff);
			} else if (i == 13) {
				float utime = atof(fileBuff)/sysconf(_SC_CLK_TCK);
				printf("Utime:\t\t\t\t%0.2fs\n", utime);
			} else if (i == 14) {
				float stime = atof(fileBuff)/sysconf(_SC_CLK_TCK);
				printf("Stime:\t\t\t\t%0.2fs\n", stime);
			} else if (i == 23) {
				printf("Rss:\t\t\t\t%s\n", fileBuff);
			}
			i++;
		}
		fclose(file_ptr);
	} else {
		printf("Error: stat file could not open.\n");
		return;
	}
	file_ptr = NULL;
	memset(&filename[0], 0, sizeof(filename)); // empties the filename array
	sprintf(filename, "/proc/%d/status", pid);
	file_ptr = fopen(filename, "r");
	if (file_ptr != NULL) {
		char fileBuff[256];
		while (fscanf(file_ptr,"%s", fileBuff) != EOF) {
			if (strcmp(fileBuff, "voluntary_ctxt_switches:") == 0) {
				fscanf(file_ptr, "%s", fileBuff);
				printf("Voluntary_ctxt_switches:\t%s\n", fileBuff);
			}
			if (strcmp(fileBuff, "nonvoluntary_ctxt_switches:") == 0) {
				fscanf(file_ptr, "%s", fileBuff);
				printf("Nonvoluntary_ctxt_switches:\t%s\n", fileBuff);
			}
		}
		fclose(file_ptr);
	} else {
		printf("Error: status file could not open.\n");
	}
}

void func_BGupdate() {
	pid_t pid;
	int p_status;
	while(true) {
		pid = waitpid(-1, &p_status, WCONTINUED | WNOHANG | WUNTRACED);
		if(pid > 0) {
			if(WIFSIGNALED(p_status)) {
				printf("Background job %d killed.\n", pid);
				head = deleteNode(head, pid);
			} else if(WIFEXITED(p_status)) {
				printf("Background job %d terminated.\n", pid);
				head = deleteNode(head, pid);
			}
		} else {
			break;
		}
	}
}

int main(){
    char user_input_str[50];
    while (true) {
      func_BGupdate();
      printf("Pman: > ");
      fgets(user_input_str, 50, stdin);
      printf("User input: %s \n", user_input_str);
      char * ptr = strtok(user_input_str, " \n");
      if(ptr == NULL){
        continue;
      }
      char * lst[50];
      int index = 0;
      lst[index] = ptr;
      index++;
      while(ptr != NULL){
        ptr = strtok(NULL, " \n");
        lst[index]=ptr;
        index++;
      }
      if (strcmp("bg",lst[0]) == 0) {
        func_BG(lst);
      } else if (strcmp("bglist",lst[0]) == 0) {
        func_BGlist(lst);
      } else if (strcmp("bgkill",lst[0]) == 0) {
        func_BGkill(lst[1]);
      } else if (strcmp("bgstop",lst[0]) == 0) {
        func_BGstop(lst[1]);
      } else if (strcmp("bgstart",lst[0]) == 0) {
        func_BGstart(lst[1]);
      } else if (strcmp("pstat",lst[0]) == 0) {
        func_pstat(lst[1]);
      } else if (strcmp("q",lst[0]) == 0) {
        printf("Bye Bye \n");
        exit(0);
      } else {
        printf("Invalid input\n");
      }
    }

  return 0;
}
