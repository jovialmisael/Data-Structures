#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>>

// Definition of the node structure
struct node {
    char name[50];
    char cls[20];
    char ID[20];
    int height;
    struct node* left;
    struct node* right;
};

bool deleteFound=false;

// Function to create a new node
struct node* createNewNode(char name[], char cls[], char ID[]) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    strcpy(newNode->name, name);
    strcpy(newNode->cls, cls);
    strcpy(newNode->ID, ID);
    newNode->height = 1; // New node is initially added at leaf
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Function to find the maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Function to get the height of the tree
int height(struct node* root) {
    if (root == NULL)
        return 0;
    return root->height;
}

// Function to get the balance factor of a node
int balanceFactor(struct node* root) {
    if (root == NULL)
        return 0;
    return height(root->left) - height(root->right);
}

// Right rotation
struct node* rightRotate(struct node* root) {
    struct node* leftChild = root->left;
    struct node* rightLeftChild = leftChild->right;

    leftChild->right = root;
    root->left = rightLeftChild;

    root->height = max(height(root->left), height(root->right)) + 1;
    leftChild->height = max(height(leftChild->left), height(leftChild->right)) + 1;

    return leftChild;
}

// Left rotation
struct node* leftRotate(struct node* root) {
    struct node* rightChild = root->right;
    struct node* leftRightChild = rightChild->left;

    rightChild->left = root;
    root->right = leftRightChild;

    root->height = max(height(root->left), height(root->right)) + 1;
    rightChild->height = max(height(rightChild->left), height(rightChild->right)) + 1;

    return rightChild;
}

// Function to rebalance the tree in deletion
struct node* rebalance(struct node* root) {
    int balance = balanceFactor(root);

    // Left heavy
    if (balance > 1) {
        if (balanceFactor(root->left) >= 0) {
            return rightRotate(root);
        } else {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
    }
    // Right heavy
    else if (balance < -1) {
        if (balanceFactor(root->right) <= 0) {
            return leftRotate(root);
        } else {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
    }
    return root;
}

// Function to insert data into the AVL tree
struct node* insertData(struct node* root, char name[], char cls[], char ID[]) {
    if (root == NULL) {
        return createNewNode(name, cls, ID);
    }

    if (strcmp(ID, root->ID) < 0) {
        root->left = insertData(root->left, name, cls, ID);
    }
	else if (strcmp(ID, root->ID) > 0) {
        root->right = insertData(root->right, name, cls, ID);
    }
	else {
        return NULL; // Duplicate IDs are not allowed
    }

    root->height = max(height(root->left), height(root->right)) + 1;
    int balance = balanceFactor(root);
    
//left left case
   if(balance > 1 && strcmp(ID,root->left->ID)<0){
        return rightRotate(root);
   }
    
//left right case
    if(balance > 1 && strcmp(ID,root->left->ID)>0){
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

//right right case
    if(balance < -1 && strcmp(ID,root->right->ID)>0){
        return leftRotate(root);
	}


//right left case
    if(balance < -1 && strcmp(ID,root->right->ID)<0){
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
	
	return root;
}

// Function to delete a node from the AVL tree
struct node* deleteNode(struct node* root, char ID[]) {
    if (root == NULL) {
        return NULL;
    }

    if (strcmp(ID, root->ID) < 0) {
        root->left = deleteNode(root->left, ID);
    } else if (strcmp(ID, root->ID) > 0) {
        root->right = deleteNode(root->right, ID);
    } else {
    	deleteFound=true;
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        } else if (root->left == NULL) {
            struct node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct node* temp = root->left;
            free(root);
            return temp;
        } else {
            struct node* temp = root->right;
            while (temp->left != NULL) {
                temp = temp->left;
            }
            strcpy(root->name, temp->name);
            strcpy(root->cls, temp->cls);
            strcpy(root->ID, temp->ID);
            root->right = deleteNode(root->right, temp->ID);
        }
    }

    root->height = max(height(root->left), height(root->right)) + 1;
    return rebalance(root);
}

// Function to search for a node in the AVL tree
struct node* searchNode(struct node* root, char ID[]) {
    if (root == NULL || strcmp(root->ID, ID) == 0) {
        return root;
    }
    if (strcmp(root->ID, ID) < 0) {
        return searchNode(root->right, ID);
    }
    return searchNode(root->left, ID);
}

// Function to view data based on a specific cls
void viewDataByClass(struct node* root, char cls[]) {
    if (root != NULL) {
        viewDataByClass(root->left, cls);
        if (strcmp(root->cls, cls) == 0) {
            printf("Name: %s\nClass: %s\nID: %s\n\n", root->name, root->cls, root->ID);
        }
        viewDataByClass(root->right, cls);
    }
}

// Preorder traversal
void preorder(struct node* root) {
    if (root != NULL) {
        printf("Name: %s\nClass: %s\nID: %s\n\n", root->name, root->cls, root->ID);
        preorder(root->left);
        preorder(root->right);
    }
}

// Inorder traversal
void inorder(struct node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("Name: %s\nClass: %s\nID: %s\n\n", root->name, root->cls, root->ID);
        inorder(root->right);
    }
}

// Postorder traversal
void postorder(struct node* root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("Name: %s\nClass: %s\nID: %s\n\n", root->name, root->cls, root->ID);
    }
}

int main() {
	char name[50];
    char cls[20];
    char ID[20];
    struct node* root = NULL;

    // Sample insertions
    root = insertData(root, "Alice", "Math", "ID123");
    root = insertData(root, "Bob", "Science", "ID124");
    root = insertData(root, "Charlie", "Math", "ID125");
	root = insertData(root, "Ryan", "Science", "ID126");
	root = insertData(root, "Jovial", "Math", "ID127");
	root = insertData(root, "Michael", "Science", "ID128");
	root = insertData(root, "Mary", "Math", "ID129");
	root = insertData(root, "Stevan", "Science", "ID130");
	root = insertData(root, "Marcell", "Math", "ID131");
	root = insertData(root, "Elroy", "Science", "ID132");
	
	printf("Preview 10 prefix data\n");
	preorder(root);
	
	int choice=0;
	do{
		printf("1. View All\n2. View By Class\n3. Delete\n4. Search\n5. Exit\n>> ");
		scanf("%d",&choice);
		switch(choice){
			case 1:
				char mode[6];
				printf("View All Data [Pre | In | Post]: ");
				scanf("%s",&mode);getchar();
				if(strcmp(mode,"Pre")==0){
					printf("Preorder traversal:\n");
    				preorder(root);
				}else if(strcmp(mode,"In")==0){
					printf("Inorder traversal:\n");
   					inorder(root);
				}else if(strcmp(mode,"Post")==0){
					printf("Postorder traversal:\n");
   					postorder(root);
				}
				break;
			case 2:
				char input[10];
				printf("View Data By Class [Math | Science]: ");
				scanf("%s",&input);getchar();
				viewDataByClass(root,input);
				break;
			case 3:
				char delID[6];
				inorder(root);
				printf("Input ID to delete: ");
				scanf("%s",&delID);getchar();
				root=deleteNode(root,delID);
				break;
			case 4:
				char searching[6];
				printf("Input ID to Search: ");
				scanf("%s",&searching);getchar();
				struct node* found = searchNode(root, searching);
				if (found){
       				printf("Found node:\nName: %s\nClass: %s\nID: %s\n\n", found->name, found->cls, found->ID);
    			}else {
        			printf("Node not found.\n");
    			}
				break;
		}
	}while(choice!=5);

    return 0;
}
