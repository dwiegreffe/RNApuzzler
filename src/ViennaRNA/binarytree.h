 /**
 * \file binarytree.h
 *
 * \brief Headers for a fancy binary tree
 *
 *  c Daniel Gerighausen
 *    The ViennaRNA Package
 */


#ifndef  __VIENNA_RNA_PACKAGE_BINARYTREE_H__
#define  __VIENNA_RNA_PACKAGE_BINARYTREE_H__
#include <ViennaRNA/plot_layouts.h>

struct btnode
{
    double value;
    double end;
    struct segment *seg;
    
    struct btnode *l;
    struct btnode *r;
};




void delete1(struct btnode *t,struct btnode *rootv);
void insert(double data, struct btnode *rootv, struct segment *line, double end);
int delete(double data, struct btnode *rootv);
void inorder(struct btnode *t, struct btnode *rootv);
struct btnode * create(double data, struct segment *line, double end);
void search(struct btnode *t, double data, struct segment *line, double end);
void preorder(struct btnode *t, struct btnode *rootv);
void postorder(struct btnode *t, struct btnode *rootv);
void search1(struct btnode *t,double data,struct btnode *rootv);
void searchv(struct btnode *t, double data);
struct btnode * smallest(struct btnode *t);
struct btnode * largest(struct btnode *t);
struct btnode * createRoot(double data,  struct segment *line, double end);
void check_lines(struct btnode t);
void deleteLinkedList();
void delete_passed_segments(struct btnode t, double start,float * x,float * y);
void print_list(struct element *current);
void  delete_vertex_in_list(struct btnode *rootv);
struct element * getList();
void modifyVertex(struct btnode *t, double data, double newData,struct segment *line, double end);
#endif