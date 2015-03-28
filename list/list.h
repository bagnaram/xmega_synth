#ifndef LIST_H_
#define LIST_H_

struct order 
{
  int id;
  char bank1_startstop;
  int bank1_note;
  char bank2_startstop;
  int bank2_note;
  char bank3_startstop;
  int bank3_note;
};

struct onode 
{
  struct order* data;
  struct onode* next;
  struct onode* prev;
};


//List management

/**
 * Returns a new linked list node filled in with the given order, The function
 * allocates a new order and copy the values stored in data then allocate a 
 * linked list node. If you are implementing this function make sure that you
 * duplicate, as the original data may be modified by the calling function.
 */
struct onode* newNode (struct order* data);
/**
 * Returns a new linked list node filled in with the given order, The function
 * allocates a new order and copy the values stored in data then allocate a 
 * linked list node. This function is different that newNode in that the new
 * node is created by reference and not copied
 */
struct onode* newNodeByRef (struct order* data);
/**
 * In a linked list with *head as the head pointer, adds the given node to the
 * front of the list.
 */
void pushNode (struct onode** head, struct onode* node) ;

/**
 * In a linked list with *head as the head pointer, returns the onode with the
 * given order id
 */
struct onode* getOrderNode (struct onode* head, int id);

/**
 * Insert the given node after the prevNode. If the  prevNode is NULL,
 * then the given node is inserted at the head of the list.
 */
void insertNode(struct onode** head, struct onode* prevNode,
                           struct onode* insertingNode);
/**
 * Internally removes the specified node from the list, and updates 
 * the linked list.
 */
void evictNode(struct onode** head, struct onode* node);

/**
 * Removes the specified node from the list, and frees all memory the node is
 * using. Remember if *head is the node being deleted, it must be updated.
 */
void deleteNode(struct onode** head, struct onode* node);
/**
 * Removes the specified node from the list, and does NOT free memory the node is
 * using. Remember if *head is the node being deleted, it must be updated.
 */
void deleteNodeOnly(struct onode** head, struct onode* node);

/**
 * Deletes every node in the list with *head as the head pointer. After calling
 * this function, all memory used by the list should be freed, and *head
 * should be NULL.
 */
void deleteList(struct onode** head);
/**
 * Deletes every node EXCEPT the head in the list with *head as the head pointer. After calling
 * this function, all memory used by the list should be freed, and *head
 * should be NULL.
 */
void deleteJustList(struct onode** head);

void printList(struct onode *node, void(*printItem)(struct order *, FILE *), FILE *out);

/** @name group1 Content Management: Setters and Getters for linkedList
  * Setters and Getters for linkedList
  */
//@{
struct onode* getOrderNode(struct onode* head, int id);
struct onode* getNextOrder(struct onode* order_node);
struct onode* getPrevOrder(struct onode* order_node);
struct order* getOrderData(struct onode* order_node);
//@}

/** @name group2 Content Management: Setters and Getters for order data
  * Setters and Getters for linkedList
  * @{
  */
//@{
int getOrderId (struct order* orderData);
char getOrderStartstop (struct order* orderData,char bankNum);
int getOrderNote(struct order* orderData,char bankNum);

void setOrderId (struct order* orderData, int id);
void setOrderStartstop (struct order* orderData, char newStartstop, char bankNum);
void setOrderNote(struct order* orderData, char newNote, char bankNum);
//@}


/** @name group3 Sorting functions 
  * @{
  */
//@{
void swap (struct onode** head, struct onode* n1, struct onode* n2);
/**
 * Performes insertion sort on the list by comparing the OrderID. Sorts
 * from largest(head) to smallest.
 */
void sort (struct onode** head);
//@}

#endif //LIST_H_
