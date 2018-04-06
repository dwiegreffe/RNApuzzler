 /**
 *  Linked List
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



struct element * createList(double start, double end, struct segment *line)
{
    struct element * temp = (struct element*)malloc(1*sizeof(struct element));
    temp->start = start;
    temp->end = end;
    temp->line = line;
    
    temp->next= NULL;
    
    return temp;
}
void addElement(struct element * head, double start, double end,struct segment *line)
{
 
    head->next = (struct element*)malloc(1*sizeof(struct element));
    head->next->start = start;
    head->next->end = end;
    head->next->line = line;
    head->next->next = NULL;
    
}
void deleteList(struct element *head)
{
     struct element * temp = (struct element*)malloc(1*sizeof(struct element));
   //  printf("pointer auf head %p\n", head);
      if(head ==NULL)
     {
         // printf("FOO");
      }
      // printf("FsOO");
     while(head->next !=NULL)
     {
         temp = head->next;
        //  printf("%f loeschen\n", head->start);
         free(head);
         
         head = temp;
     }
    //  printf("%f\n", head->start);
     if(head !=NULL)
     {
     free(head);
     }
    //  printf("%p\n", head);
     
     
}