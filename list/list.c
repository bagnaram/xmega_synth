#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"


int g=0;
/**
 * Returns a new linked list node filled in with the given order. The order is copied into the node as a new instance.
 */
struct onode* newNode (struct order* data) {
  struct onode *new;
  //printf("newNode");
  new = malloc(sizeof(struct onode));
  g++;
  if( new == NULL ) {
 fprintf(stderr,"Error: malloc failed in new\n");
 //exit(EXIT_FAILURE);
 }

  struct order* newOrder= malloc(sizeof(struct order)); 
  g++;
  if( newOrder== NULL ) {
 fprintf(stderr,"Error: malloc failed in newOrder\n");
 //exit(EXIT_FAILURE);
 }

 newOrder->id=data->id;
 newOrder->bank1_startstop=data->bank1_startstop;
 newOrder->bank1_note=data->bank1_note;
 newOrder->bank2_startstop=data->bank2_startstop;
 newOrder->bank2_note=data->bank2_note;
 newOrder->bank3_startstop=data->bank3_startstop;
 newOrder->bank3_note=data->bank3_note;
  //printf("\nadding:%s",newWord);
  new->data=newOrder;
  new->next=NULL;
  new->prev=NULL;
  return new;
}
/**
 * Returns a new linked list node filled in with the given order. The order is inserted by reference.
 */
struct onode* newNodeByRef (struct order* data) {
  struct onode *new;
  new = malloc(sizeof(struct onode));
  if( new == NULL ) {
 fprintf(stderr,"Error: malloc failed in newNodeByRef\n");
 //exit(EXIT_FAILURE);
 }
  new->data=data;
  new->next=NULL;
  new->prev=NULL;
  return new;
}

/**
 * In a linked list with *head as the head pointer, adds the given node to the
 * front of the list.
 */
void pushNode (struct onode** head, struct onode* node) {
  //printf("pushNode");

  //empty list`
  if(*head==NULL)
  {
    *head=node;
    node->next=NULL;
    node->prev=NULL;
    return;
  }


  if(*head!=NULL )
  {

  //printf("\nhead:%s\n",(*head)->text);
  (*head)->prev=node;
  node->next=*head;
  node->prev=NULL;

  *head=node;
  }

}


/**
 * Returns the pointer to the node containing the given word in the linked list
 * with head as the head pointer. If a node with the given word cannot be found,
 * the function returns NULL.
 */

struct onode* getOrderNode (struct onode* head, int id) {
  //printf("getnode");

  //empty list
  if(head==NULL)
    return NULL;

  struct onode* current = head;
  while(current!=NULL )
  {
 

    if(current->data->id==id)
    {

      //printf("(%s,%s)",current->text,word);
      return current;
    }
    current=(current->next);
  }
  return NULL;
}

void sort (struct onode** head)
{
  struct onode* current = *head;

  while(current!=NULL)
  {
    struct onode* hole = current;
    while(hole!=NULL && getPrevOrder(hole)!=NULL && getOrderId(getPrevOrder(hole)->data) > getOrderId(current->data))
    {
        //printf("swap(%lf,%lf)",getOrderPrice(current->data) , getOrderPrice(hole->data));
        //swap(head,hole,getPrevOrder(hole));
	hole=getPrevOrder(hole);
    }
    swap(head,hole,current);
    current=getNextOrder(hole);
  }
}

void swap (struct onode** head, struct onode* node1, struct onode* node2)
{
  struct order* temp = node1->data;
  node1->data=node2->data;
  node2->data=temp;

}


void insertNode(struct onode** head, struct onode* prevNode, struct onode* insertingNode) {
if(prevNode==NULL)
{
  //insert as head
  pushNode(head,insertingNode);
  return;
}

struct onode* nextguy = prevNode->next;

if(nextguy)
{
  //insert somewhere in middle
  insertingNode->next=prevNode->next;
  insertingNode->prev=prevNode;
  (insertingNode->next)->prev=insertingNode;
  prevNode->next=insertingNode;
  return;
}
else
{
  //insert to tail
  insertingNode->next=NULL;
  insertingNode->prev=prevNode;
  prevNode->next=insertingNode;
  return;
}

}


void evictNode(struct onode** head, struct onode* node)
{

if(*head==node)
{
  if(node->next!=NULL)
  {
    *head=node->next;
    (*head)->prev=NULL;
  }
 else
   *head=NULL;

node->next=NULL;
node->prev=NULL;
return;
}
else
{
  if(node->prev && node->next)
  {
  struct onode* p = node->prev;
    node->prev->next=node->next;
    node->next->prev=p;
    //printf(" ## %d ##",node->next->data->id);
  }
  else if(node->next && node->prev==NULL)
  {
    node->next->prev=NULL;
  }
  else if(node->prev && node->next==NULL)
  {
    node->prev->next=node->next;
  }
node->next=NULL;
node->prev=NULL;
}
}
void deleteNodeOnly (struct onode** head, struct onode* node)
{
evictNode(head, node);
free(node);
}

/**
 * Removes the specified node from the list, and frees all memory the node is
 * using. Remember if *head is the node being deleted, it must be updated.
 */
void deleteNode (struct onode** head, struct onode* node)
{

free(node->data);
deleteNodeOnly(head,node);
}


void printList (struct onode *node, void(*printItem)(struct order *,FILE *), FILE *out)
{
  struct onode* current = node;
  while(current!=NULL )
  {
 

    if(current->data!=NULL)
    {
      printItem(current->data,out);
    }
    current=(current->next);

  }
}
/**
 * Simply returns the next node in the list, or NULL if there are no further nodes.
 */
struct onode *getNextOrder(struct onode *order_node) {
  return order_node->next;
}
struct onode *getPrevOrder(struct onode *order_node) {
  return order_node->prev;
}
struct order *getOrderData(struct onode *order_node) {
  return order_node->data;
}

/*
 * Getterrs
 */


int  getOrderId(struct order* orderData)
{
  return orderData->id;
}

char getOrderStartstop(struct order* orderData,char bankNum)
{
  if(bankNum==1)
  {
    return orderData->bank1_startstop;
  }
  else if(bankNum==2)
  {
    return orderData->bank2_startstop;
  }
  else if(bankNum==3)
  {
    return orderData->bank3_startstop;
  }
  else
  {
    return -1;
  }
}
int getOrderNote(struct order* orderData,char bankNum)
{
  if(bankNum==1)
  {
    return orderData->bank1_note;
  }
  else if(bankNum==2)
  {
    return orderData->bank2_note;
  }
  else if(bankNum==3)
  {
    return orderData->bank3_note;
  }
  else
  {
    return -1;
  }
}

/*
 * Setters
 */

void setOrderStartstop(struct order* orderData, char newStartstop, char bankNum)
{
  if(bankNum==1)
  {
    orderData->bank1_startstop=newStartstop;
  }
  else if(bankNum==2)
  {
    orderData->bank2_startstop=newStartstop;
  }
  else if(bankNum==3)
  {
    orderData->bank3_startstop=newStartstop;
  }
  else
  {
    exit(1);
  }
}
void  setOrderNote(struct order* orderData, char newNote, char bankNum)
{
  if(bankNum==1)
  {
    orderData->bank1_note=newNote;
  }
  else if(bankNum==2)
  {
    orderData->bank2_note=newNote;
  }
  else if(bankNum==3)
  {
    orderData->bank3_note=newNote;
  }
  else
  {
    exit(1);
  }
}
void  setOrderId(struct order* orderData, int id)
{
  orderData->id=id;
}

/**
 * Deletes every node in the list with *head as the head pointer. After calling
 * this function, all memory used by the list should be freed, and *head
 * should be NULL.
 */
void deleteList(struct onode **head) {
  if(*head==NULL)
    return;
  struct onode* current = *head;
  while(current !=NULL)
  {
    struct onode* old = current;
    free(current->data);
    current=current->next;
    free(old);
  }
  *head=NULL;
}

void deleteJustList(struct onode **head) {
  if(*head==NULL)
    return;
  struct onode* current = *head;
  while(current !=NULL)
  {
    struct onode* old = current;
    current=current->next;
    free(old);
  }
  *head=NULL;
}

