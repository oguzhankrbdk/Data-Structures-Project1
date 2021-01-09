/*Oguzhan Karabudak 150118005
	Purpose of the program: Developing algorithms using binary trees for desired situations.
		1: Build bst with the key "word". 
		2: Calculate total access time for first tree.
		3: Construct a BT to keep these records in the main memory so as to minimize the total access time, where one time
		unit is the time taken to compare the key of a tree with the key searched.
		4: Calculate total access time for second tree.
	
	Input file format: sequence, word, frequency
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct node{
    char key[20];
    int frequency;
    struct node *left, *right;
};

struct listElement{
    int frequency;
    char key[20];
    struct listElement *next;
};

typedef struct listElement listElement;
typedef struct listElement* listElementPtr;

typedef struct node node;
typedef struct node* nodePtr;

listElementPtr head, tail = NULL;
int totalAccessTime = 0;

//insert function for linked list
void addElement(char key[], int frequency){
    listElementPtr newElement = (listElementPtr)malloc(sizeof(struct listElement));
    newElement->frequency = frequency;
    strcpy(newElement->key, key);
    newElement->next = NULL;

    if(head == NULL){
        head = newElement;
        tail = newElement;
    }
    else{
        tail->next = newElement;
        tail = newElement;
    }
}

//sorting linked list
void sortList(){
    listElementPtr current = head, iter = NULL;
    int temp;
    char key[20];
    if(head == NULL){
        return;
    }
    else{
        while(current != NULL){
            iter = current->next;
            while(iter != NULL){
                if(current->frequency < iter->frequency){
                    temp = current->frequency;
                    strcpy(key, current->key);
                    current->frequency = iter->frequency;
                    strcpy(current->key, iter->key);
                    iter->frequency = temp;
                    strcpy(iter->key, key);
                }
                iter = iter->next;
            }
            current = current->next;
        }
    }
}

nodePtr newNode(char key[], int frequency){
    nodePtr newNode = malloc(sizeof(node));
    strcpy(newNode->key, key);
    newNode->frequency = frequency;
    newNode->right = newNode->left = NULL;
    return newNode;
}
//Inserting function for bt
nodePtr insert(nodePtr node, char* key, int frequency){
    if(node == NULL){
       return newNode(key, frequency);
    }
    if(strcmp(key, node->key) < 0){
        node->left = insert(node->left, key, frequency);
    }
    else if(strcmp(key, node->key) >= 0){
        node->right = insert(node->right, key, frequency);
    }
    return node;
}
//Finding a level of a node in tree
int getLevel(nodePtr node, int frequency, int level){
    if(node == NULL){
        return 0;
    }
    if(node->frequency == frequency){
        return level;
    }
    int lowerLevel = getLevel(node->left, frequency, level + 1);
    if(lowerLevel != 0){
        return lowerLevel;
    }
    lowerLevel = getLevel(node->right, frequency, level + 1);
    return lowerLevel;
}

void inorder(nodePtr root, nodePtr head){
    if(root != NULL){
        inorder(root->left, head);
        int level = getLevel(head, root->frequency, 1);
        totalAccessTime += level * root->frequency;
        printf("%s\n", root->key);
        inorder(root->right, head);
    }
}
//Converting a sorted linked list to binary tree
nodePtr llToBt(int index, int size){
    int i;
    nodePtr temp;
    if(index >= size)
        return NULL;
    if(index < size){
        temp = newNode("AA", 0);
        listElementPtr iter = head;
        for(i = 0; i < index; i++){
            iter = iter->next;
        }
        temp->frequency = iter->frequency;
        strcpy(temp->key, iter->key);

        temp->left = llToBt(2 * index + 1, size);
        temp->right = llToBt(2 * index + 2, size);

    }
    return temp;
}

char* clearString(char string[]){
    char str[20];
    strcpy(str, string);
    int i;
    /*Converting mixed case string to lower case string*/
    for(i= 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
    /*cleaning spaces from string*/
    int j, k;
    i = 0;
    while(str[i] != '\0'){
        k = 0;
        if(str[i] == ' '){
            j = i;
            while(str[j - 1] != '\0'){
                str[j] = str[j + 1];
                j++;
            }
            k = 1;
        }
        if(k == 0){
            i++;
        }
    }
    char* temp = malloc(20 * sizeof(char));
    strcpy(temp, str);
    return temp;
}

/*If mode is 0 then it will take word as a key, if mode is 1 then it will take frequency as a key(for same input format)*/
nodePtr buildBt(char *direc, int mode){
    FILE *fp;
    char buffer[20];
    char *array[3];
    char *p;
    int i, count = 0;
    const char delim[2] = ",";
    nodePtr root = NULL;
    fp = fopen(direc, "r");

    if (fp == NULL){
        printf("Error failed to open file\n");
        return NULL;
    }

    while (!feof(fp)){
        fscanf(fp, "%s", buffer);
        strcpy(buffer, clearString(buffer));
        p = strtok(buffer, delim);
        i = 0;
        //Splitting string into tokens and saving them in array
        while (p != NULL){
            array[i++] = p;
            p = strtok (NULL, delim);
        }
        //If mode == 0 then we'll take word as a key
        if(mode == 0){
            if(root == NULL){
                root = insert(root, array[1], atoi(array[2]));
            }else{
                insert(root, array[1], atoi(array[2]));
            }
        }
        //If mode == 1 we'll take frequency as a key and generate a linked list element
        else if(mode == 1){
            addElement(array[1], atoi(array[2]));
            count++;
        }

    }
    fclose(fp);

    if(mode == 1){
        //Sorting linked list then generating a binary tree from linked list
        sortList();
        root = llToBt(0, count);
    }

    return root;
}



int main(){
    nodePtr root = NULL, root2 = NULL;
    //Input file's path
    char *direc = "/home/oguzhan/CLionProjects/bst/input.txt";
    //root is for first bt which is take word as a key
    root = buildBt(direc, 0);
    printf("FIRST BT KEY: WORD\n");
    inorder(root, root);
    printf("Total access time: %d\n", totalAccessTime);
    //root2 is for second bt which is take frequency as a key
    totalAccessTime = 0;
    root2 = buildBt(direc, 1);
    printf("\n\nSECOND BT KEY: FREQUENCY\n");
    inorder(root2, root2);
    printf("Total access time: %d\n", totalAccessTime);

    return 0;
}
