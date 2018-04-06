 /**
 * \file linkedlist.h
 *
 * \brief Headers for a fancy linked list
 *
 *  c Daniel Gerighausen
 *    The ViennaRNA Package
 */
#ifndef  __VIENNA_RNA_PACKAGE_LINKEDLIST_H__
#define  __VIENNA_RNA_PACKAGE_LINKEDLIST_H__
#include <ViennaRNA/plot_layouts.h>
struct element
{
    double start;
    double end;
    double value;
    struct segment *line;
    struct element *next;
};


struct element * createList(double start, double end, struct segment *line);

void addElement(struct element * head, double start, double end,  struct segment *line);



#endif