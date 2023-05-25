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


Node * add_newNode(Node* head, pid_t new_pid, char* new_path){
	Node* new_node = (Node*) malloc(sizeof(Node));
	new_node->pid = new_pid;
	strcpy(new_node->path, new_path);
	new_node->next = NULL;
	if (head == NULL) {
		head = new_node;
	} else {
		Node* temp = head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = new_node;
	}
	return head;
}


Node * deleteNode(Node* head, pid_t pid){
	if(PifExist(head, pid) == false) {
		return NULL;
	}
	Node* cur = head;
	Node* prev = NULL;
	while (cur != NULL) {
		if(cur->pid == pid) {
			if(cur == head) {
				head=head->next;
				return head;
			} else {
				prev->next = cur->next;
			}
			free(cur);
			return head;
		}
		prev = cur;
		cur = cur->next;
	}
	return head;
}


void printList(Node *node){
	int count = 0;
	while (node != NULL) {
		count++;
		printf("%d: %s\n", node->pid, node->path);
		node = node->next;
		
	}
	printf("Total Background Jobs: %d\n", count);
}


int PifExist(Node *node, pid_t pid){
   while (node != NULL) {
    if (node->pid == pid) {
      return true;
    }
    node = node->next;
  }
  return false;
}
