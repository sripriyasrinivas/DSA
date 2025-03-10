#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node {
    char name[50];
    char text[50];
    struct node *prev;
    struct node *next;
} chat;

void disp(chat* headp, chat* head);
chat* addchat(chat* headp, chat* head, char *n, char *t);
void pinchat(chat** headp, chat** head, chat* p);
void unpin(chat** headp, chat** head, chat* p);

int main() {
    chat* head = NULL;   // pointer to first node in unpinned chats
    chat* headp = NULL;  // pointer to first node in pinned chats
    int out = 0;

    printf("***************************************************************\n");
    printf("YOUR CHAT MESSENGER\n");
    printf("***************************************************************\n");

    while (out == 0) {
        printf("Pick your action:\n");
        printf("To DISPLAY all chats enter 0\n");
        printf("To ADD a chat enter 1\n");
        printf("To PIN a chat enter 2\n");
        printf("To UNPIN a chat enter 3\n");

        int op = 0; // option input
        printf("Enter option: ");
        scanf("%d", &op);

        if (op == 1) {
            char n[50] = "";  // stores name of chat
            char t[50] = "";  // stores text in chat
            printf("Enter name: ");
            getchar();
            scanf("%[^\n]s", n);
            getchar();
            printf("Enter text: ");
            scanf("%[^\n]s", t);
            head = addchat(headp, head, n, t);
        }
        else if (op == 2) {
            printf("Enter name of contact from unpinned chats:\n");
            getchar();
            char cont[50] = "";  // name of contact
            disp(headp, head);
            scanf("%[^\n]s", cont);
            chat *p = head;
            while (strcmp(p->name, cont) != 0)  // move pointer to the specific chat
            {
                p = p->next;
            }
            pinchat(&headp, &head, p);
        }
        else if (op == 3) {
            printf("From the pinned chats enter a chat that you want to unpin:\n");
            disp(headp, head);
            char in[50] = "";
            getchar();
            scanf("%[^\n]s", in);
            chat* point2 = headp;
            while (strcmp(point2->name, in) != 0) {
                point2 = point2->next;
            }
            unpin(&headp, &head, point2);
        }
        else if (op == 0) {
            disp(headp, head);
        }
        else {
            printf("Wrong Input\n");
        }
    }
}

chat* addchat(chat* headp, chat* head, char *n, char *t) {
    chat *p = (chat *)malloc(sizeof(chat));
    strcpy(p->name, n);
    strcpy(p->text, t);
    p->next = NULL;
    p->prev = NULL;

    if (head == NULL) {
        head = p;
    }
    else if (headp == NULL && head != NULL) {
        p->next = head;
        head->prev = p;
        head = p;
    }
    else {
        p->next = head;
        if (head != NULL) {
            head->prev = p;
        }
        head = p;
    }
    return head;
}

void pinchat(chat** headp, chat** head, chat* p) {
    
    if (p == *head) {  
        *head = (*head)->next;
        if (*head) (*head)->prev = NULL;
    } 
    else {
        if (p->prev) p->prev->next = p->next;
        if (p->next) p->next->prev = p->prev;
    }
    p->prev = NULL;
    p->next = *headp;
    if (*headp != NULL) (*headp)->prev = p;
    *headp = p;
}

void unpin(chat** headp, chat** head, chat* p) {
    if (p == *headp) {  
        *headp = (*headp)->next;
        if (*headp) (*headp)->prev = NULL;
    } 
    else {
        if (p->prev) p->prev->next = p->next;
        if (p->next) p->next->prev = p->prev;
    }

    p->prev = NULL;
    p->next = *head;
    if (*head != NULL) (*head)->prev = p;
    *head = p;
}

void disp(chat* headp, chat* head) {
    chat *temp1 = headp;
    chat *temp2 = head;

    if (head == NULL && headp == NULL) {
        printf("No chats\n");
    }
    else {
        if (temp1 == NULL) {
            printf("No pinned chats\n");
        }
        else {
            printf("Pinned chats:\n");
            while (temp1 != NULL) {
                printf("%s\t%s\n", temp1->name, temp1->text);
                temp1 = temp1->next;
            }
        }

        if (temp2 == NULL) {
            printf("No unpinned chats\n");
        }
        else {
            printf("Unpinned chats:\n");
            while (temp2 != NULL) {
                printf("%s\t%s\n", temp2->name, temp2->text);
                temp2 = temp2->next;
            }
        }
    }
}
