/**
 *  Binary Tree
 *
 *  c Daniel Gerighausen
 *    The ViennaRNA Package
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ViennaRNA/utils.h"
#include "ViennaRNA/plot_layouts.h"
#include "ViennaRNA/binarytree.h"
#include "ViennaRNA/linkedlist.h"
int flag = 1;
struct btnode *tempv = NULL, *t2, *t1;


struct element *head, *posi = NULL;

/* To insert a node in the tree */
void insert(double data, struct btnode *rootv, struct segment *line, double end) {
  
    search(rootv, data, line, end);
}

struct btnode* createRoot(double data, struct segment *line, double end) {
    printf("Miep\n");
    return create(data, line, end);
}

/* To create a node */
struct btnode * create(double data, struct segment *line, double end) {

  //Idee: Baue auch noch gleich einen Pointer mit einem Line Segment dazu.
    struct btnode *temp = (struct btnode *) malloc(1 * sizeof (struct btnode));
    //struct segment *newline = (struct segment *) malloc(1 * sizeof(struct segment));
    
   // newline = &line;
    
     // printf("Count Lines %d\n", line->index);
    
    
    temp->value = data;
    temp->end = end;
    temp->seg = line;
    temp->l = temp->r = NULL;
    return temp;
}

/* Function to search the appropriate position to insert the new node */
void search(struct btnode *t, double data, struct segment *line, double end) {
    if ((data > t->value) && (t->r != NULL)) /* value more than rootv node value insert at right */
        search(t->r, data, line, end);
    else if ((data > t->value) && (t->r == NULL))
        t->r = create(data, line, end);
    else if ((data < t->value) && (t->l != NULL)) /* value less than rootv node value insert at left */
        search(t->l, data, line, end);
    else if ((data < t->value) && (t->l == NULL))
        t->l = create(data, line, end);
    else {

        // The element is already in the tree, however, since we are dealing with floats, just add 0.000001 only for the intersection
        //Dirty hack, i'll change it to buckets later.
        search(t, data + 0.000001, line, end);

    }
}

/* recursive function to perform inorder traversal of tree */
void inorder(struct btnode *t, struct btnode *rootv) {
    if (rootv == NULL) {
        printf("No elements in a tree to display");
        return;
    }
    if (t->l != NULL)
        inorder(t->l, rootv);
    printf("(%f|%f) %d-> ", t->value, t->end, t->seg->index);
    if (t->r != NULL)
        inorder(t->r, rootv);
}

/* To check for the deleted node */
int delete(double data, struct btnode *rootv) {
    if (rootv == NULL) {
        printf("No elements in a tree to delete");
        return;
    }
    if ((rootv->l == NULL) && (rootv->r == NULL)) {

        rootv = NULL;
        free(rootv);
        printf("Loesche den ganzen Baum\n");
        return 1;
    }
   //  printf("Loesche %f\n", data);
    t1 = rootv;
    t2 = rootv;
    search1(rootv, data, rootv);


    return 0;
}

/* To find the preorder traversal */
void preorder(struct btnode *t, struct btnode *rootv) {
    if (rootv == NULL) {
        printf("No elements in a tree to display");
        return;
    }
    printf("%f -> ", t->value);
    if (t->l != NULL)
        preorder(t->l, rootv);
    if (t->r != NULL)
        preorder(t->r, rootv);
}

/* To find the postorder traversal */
void postorder(struct btnode *t, struct btnode *rootv) {
    if (rootv == NULL) {
        printf("No elements in a tree to display ");
        return;
    }
    if (t->l != NULL)
        postorder(t->l, rootv);
    if (t->r != NULL)
        postorder(t->r, rootv);
    printf("%f -> ", t->value);
}

/* Search for the appropriate position to insert the new node */
void search1(struct btnode *t, double data, struct btnode *rootv) {
   //  printf("Find mich doch %f\n" ,t->value);
    if ((data > t->value)) {
        t1 = t;
        search1(t->r, data, rootv);
    } else if ((data < t->value)) {
        t1 = t;
        search1(t->l, data, rootv);
    } else if ((data == t->value)) {
       // printf("Find mich doch %f\n", data);
        delete1(t, rootv);

    }
    else 
    {
        printf("Did not find a vertex to delete for the value %f\n", data);
    }
}

/* To delete a node */
void delete1(struct btnode *t, struct btnode *rootv) {
    struct btnode *k;
   //  printf("Find mich doch?\n");
    /* To delete leaf node */
    if ((t->l == NULL) && (t->r == NULL)) {
        //  printf("Find mich doch leaf\n");
        if (t1->l == t) {
            t1->l = NULL;
        } else {
            t1->r = NULL;
        }
        t = NULL;
        free(t);
        return;
    }
        /* To delete node having one left hand child */
    else if ((t->r == NULL)) {
        if (t1 == t) {
         //     printf("Find mich doch left \n");
            double value = t->l->value;
            double end = t->l->end;
            struct segment *line = t->l->seg;
            struct btnode *right = t->l->r;
            struct btnode *left = t->l->l;
            rootv->value = value;
            rootv->end = end;
            rootv->seg = line;
            rootv->r = right;
            rootv->l = left;
            t1 = rootv;
        } else if (t1->l == t) {
            t1->l = t->l;

        } else {
            t1->r = t->l;
        }
        t = NULL;
        free(t);
        return;
    }
        /* To delete node having right hand child */
    else if (t->l == NULL) {
        if (t1 == t) {
          //    printf("Find mich doch right\n");
            double value = t->r->value;
            double end = t->r->end;
            struct segment *line = t->r->seg;
            struct btnode *right = t->r->r;
            struct btnode *left = t->r->l;
            rootv->value = value;
            rootv->end = end;
            rootv->seg = line;
            rootv->r = right;
            rootv->l = left;
            t1 = rootv;
        } else if (t1->r == t) {
            t1->r = t->r;
        } else {
            t1->l = t->r;
        }
        t == NULL;
        return;
    }
        /* To delete node having two child */
    else if ((t->l != NULL) && (t->r != NULL)) {
       //   printf("Find mich doch both \n");
        t2 = rootv;
        if (t->r != NULL) {
            k = smallest(t->r);
            flag = 1;
        } else {
            k = largest(t->l);
            flag = 2;
        }
        double value = k->value;
        double end = k->end;
        struct segment *line = k->seg;
       // printf("Line index %d\n", line->index);
        search1(rootv, k->value, rootv);
        t->value = value;
        t->end = end;
        t->seg = line;
       // printf("Line index %d\n", line->index);
    }

}

/* To find the smallest element in the right sub tree */
struct btnode * smallest(struct btnode *t) {

    if (t == NULL) {
        return;
    }
    t2 = t;
    if (t->l != NULL) {
        t2 = t;
        return (smallest(t->l));
    } else {
        return t;
    }
}

/* To find the largest element in the left sub tree */
struct btnode * largest(struct btnode *t) {
    if (t->r != NULL) {
        t2 = t;
        return (largest(t->r));
    } else
        //return(t->value);
        return t;
}

/* Search for the appropriate position to insert the new node */
void searchv(struct btnode *t, double data) {
    if ((data > t->value)) {
        t1 = t;
        searchv(t->r, data);
    } else if ((data < t->value)) {
        t1 = t;
        searchv(t->l, data);
    } else if ((data == t->value)) {
        tempv = t;

    }

}

void delete_passed_segments(struct btnode t, double start, float *x, float * y) {
    if (&t == NULL) {
        printf("No elements in a tree to display");
        return;
    }
    if (t.l != NULL) {
        delete_passed_segments(*t.l, start, x, y);
    }
    if (start > t.seg->x2) {
        if (head == NULL) {
            head = createList(t.value, t.end, t.seg);
            posi = head;
        } else {
            addElement(posi, t.value, t.end, t.seg);
            posi = posi->next;
        }
    }
    if (t.r != NULL) {
        delete_passed_segments(*t.r, start, x, y);
    }



}

void check_lines(struct btnode t) {
    if (&t == NULL) {
        printf("No elements in a tree to display");
        return;
    }
    if (t.l != NULL) {
        check_lines(*t.l);

    }
    //if (start <= t->end) {
        if (head == NULL) {
            head = createList(t.value, t.end, t.seg);
            posi = head;
        } else {
            addElement(posi, t.value, t.end, t.seg);
            posi = posi->next;

        }
  //  }
    if (t.r != NULL) {
        check_lines(*t.r);
    }



}

void deleteLinkedList() {
  
    if (head == NULL) {
        return;
    }
    deleteList(head);
    
    head = NULL;
}

void print_list(struct element *current) {
    //struct element * current = head;

    while (current != NULL) {
        printf("%f|%d|%p -->", current->start, current->line->start, current->line->start);
        current = current->next;
    }
}

void delete_vertex_in_list(struct btnode *rootv) {
    struct element * current = head;

    while (current != NULL) {
        delete(current->start, rootv);
        current = current->next;
    }
}

struct element * getList() {
    return head;
}


void modifyVertex(struct btnode *t, double data, double newData,struct segment *line, double end)
{
      if ((data > t->value) && (t->r != NULL)) /* value more than rootv node value insert at right */
      {
        modifyVertex(t->r, data,newData, line, end);
      }
    else if ((data < t->value) && (t->l != NULL)) /* value less than rootv node value insert at left */
    {
        modifyVertex(t->l, data,newData, line, end);
    }
    else if (data == t->value)
    {
         printf("Modified %f with %f\n", t->value, newData);
        t->value = newData;
        t->seg = line;
        t->end = end;
       
    }
    //  printf("Did not modify %f with %f\n", data, newData);
}