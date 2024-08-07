#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    rbtree *t = NULL;
    node_t *temp;

    printf("1: Create a RB-tree.\n");
    printf("2: Insert to the RB-tree.\n");
	printf("3: Find node from the RB-tree.\n");
	printf("4: Erase node from the RB-tree.\n");
	printf("5: Print the RB-tree in ascending order.\n");
    printf("0: Delete the RB-tree and Quit;\n");

    int i, c =1;
    
    while (c != 0)
	{
		printf("\nPlease input your choice(1/2/3/4/5/0): ");
		scanf("%d", &c);

		switch (c)
		{
		case 1:
            t = new_rbtree();
            printf("One empty RB-tree is successfully created.");
			break;
		case 2:
			printf("Input an integer that you want to add to the RB-tree: ");
			scanf("%d", &i);
			rbtree_insert(t, i);
			if (t != NULL) printTree(t, t->root);
			break;
		case 3:
			printf("Input an integer that you want to find in the RB-tree: ");
			scanf("%d", &i);
			temp = rbtree_find(t, i);
			if (temp != NULL)
            	printf("The node's color : (%d), key : %d", temp->color, temp->key);
			else printf("There is no node with key value %d in the RB-tree.", i);
			break;
		case 4:
			printf("Input an integer that you want to erase in the RB-tree: ");
			scanf("%d", &i);
			temp = rbtree_find(t, i);
			rbtree_erase(t, temp);
            printf("Result tree: ");
			if (t != NULL)
				printTree(t, t->root);
			else if(temp == NULL)
				printf("\nThere is no node with key value %d in the RB-tree.", i);
			break;
		case 5:
			key_t *res = calloc(14, sizeof(key_t));
	
			rbtree_to_array(t, res, 14);
			
			printf("Result LIST: ");
			for (int i = 0; i < 14; i++) {
				printf("%d ", res[i]);
			}
			break;
		case 0:
			delete_rbtree(t);
			break;
		default:
			printf("Choice unknown;\n");
			break;
		}
	}

	return 0;
}