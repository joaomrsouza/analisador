typedef struct char_list CharList;

// Create an empty char list.
CharList *clst_create();
// Verify if the char list is empty.
int clst_empty(CharList *l);
// Insert an char on the start of the char list.
CharList *clst_insert(CharList *l, char info);
// Search for a char in the char list.
CharList *clst_search(CharList *l, char info);
// Print the char list.
void clst_print(CharList *l);
// Remove an element from a char list.
CharList *clst_remove(CharList *l, char info);
// Free the char list.
void clst_free(CharList *l);
// Return the number of elements in a char list.
int clst_length(CharList *l);