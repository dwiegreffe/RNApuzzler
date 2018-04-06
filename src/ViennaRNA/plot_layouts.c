

/**
 * This file is a container for all plotting layout algorithms
 *
 *  c Ronny Lorenz
 *    The ViennaRNA Package
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "ViennaRNA/utils.h"
#include "ViennaRNA/plot_layouts.h"
#include "ViennaRNA/binarytree.h"
#include "ViennaRNA/linkedlist.h"

#include "ViennaRNA/RNApuzzler/definitions.h"
#include "ViennaRNA/RNApuzzler/data/cfg_reader.h"
#include "ViennaRNA/RNApuzzler/RNAturtle.h"
#include "ViennaRNA/RNApuzzler/RNApuzzler.h"

#ifdef _OPENMP
#include <omp.h>
#endif

#ifndef max
#define max(a,b) (a>=b ? a : b)
#endif

#define PUBLIC
#define PRIVATE static

PUBLIC  int     rna_plot_type = VRNA_PLOT_TYPE_NAVIEW;

PRIVATE float   *angle;
PRIVATE int     *loop_size, *stack_size;
PRIVATE int     lp, stk;

struct btnode *event, *temp ,*status=NULL;

PRIVATE void loop(int i, int j, short *pair_table);

#ifdef _OPENMP
/* NOTE: all threadprivate variables are uninitialized when entering a thread! */
#pragma omp threadprivate(angle, loop_size, stack_size, lp, stk)
#endif

/*---------------------------------------------------------------------------*/

PUBLIC int simple_xy_coordinates(short *pair_table, float *x, float *y) {
    float INIT_ANGLE = 0.; /* initial bending angle */
    float INIT_X = 0.; /* coordinate of first digit */
    float INIT_Y = 0.; /* see above */
    float RADIUS = 20.;

    int i, length;
    float alpha;

    length = pair_table[0];
    angle = (float*) vrna_alloc((length + 5) * sizeof (float));
    loop_size = (int*) vrna_alloc(16 + (length / 5) * sizeof (int));
    stack_size = (int*) vrna_alloc(16 + (length / 5) * sizeof (int));
    lp = stk = 0;
    for (i = 1; i < length; i++) {
        //		printf("Angles %d -> %f\n",i,angle[i]);
    }
    loop(0, length + 1, pair_table);
    loop_size[lp] -= 2; /* correct for cheating with function loop */

    alpha = INIT_ANGLE;
    x[0] = INIT_X;
    y[0] = INIT_Y;
    //printf("%d: %f %f %f %f\n",0,x[0],y[0],angle[0],alpha);
    for (i = 1; i <= length; i++) {
        x[i] = x[i - 1] + RADIUS * cos(alpha);
        y[i] = y[i - 1] + RADIUS * sin(alpha);

        alpha += PI - angle[i + 1];
        //		printf("%d: %f %f %f %f\n",i,x[i],y[i],angle[i],alpha);
    }
    for (i = 0; i < length; i++) {
        //		printf("Angles %d -> %f\n",i,angle[i]);
    }

    free(angle);
    free(loop_size);
    free(stack_size);

    return length;

}

/*---------------------------------------------------------------------------*/

PRIVATE void loop(int i, int j, short *pair_table)
/* i, j are the positions AFTER the last pair of a stack; i.e
                i-1 and j+1 are paired. */ {
    int count = 2; /* counts the VERTICES of a loop polygon; that's
                           NOT necessarily the number of unpaired bases!
                           Upon entry the loop has already 2 vertices, namely
                           the pair i-1/j+1.  */

    int r = 0, bubble = 0; /* bubble counts the unpaired digits in loops */

    int i_old, partner, k, l, start_k, start_l, fill, ladder;
    int begin, v, diff;
    float polygon;

    short *remember;

    remember = (short *) vrna_alloc((1 + (j - i) / 5)*2 * sizeof (short));

    i_old = i - 1, j++; /* j has now been set to the partner of the
                               previous pair for correct while-loop
                               termination.  */
    while (i != j) {
        //		printf("foo: %d %d\n",i,j);
        partner = pair_table[i];
        if ((!partner) || (i == 0)) {
            //			printf("!partner %d\n",i);
            i++, count++, bubble++;
        } else {
            count += 2;
            k = i, l = partner; /* beginning of stack */
            remember[++r] = k;
            remember[++r] = l;
            i = partner + 1; /* next i for the current loop */

            start_k = k, start_l = l;
            ladder = 0;
            do {
                k++, l--, ladder++; /* go along the stack region */
            } while (pair_table[k] == l);

            fill = ladder - 2;
            if (ladder >= 2) {
                angle[start_k + 1 + fill] += PIHALF; /*  Loop entries and    */
                angle[start_l - 1 - fill] += PIHALF; /*  exits get an        */
                angle[start_k] += PIHALF; /*  additional PI/2.    */
                angle[start_l] += PIHALF; /*  Why ? (exercise)    */
                if (ladder > 2) {
                    for (; fill >= 1; fill--) {
                        angle[start_k + fill] = PI; /*  fill in the angles  */
                        angle[start_l - fill] = PI; /*  for the backbone    */
                    }
                }
            }
            stack_size[++stk] = ladder;
            loop(k, l, pair_table);
        }
    }
    //		printf ("count %d\n",count);
    polygon = PI * (count - 2) / (float) count; /* bending angle in loop polygon */
    //	printf("Polygon %f\n",polygon);
    remember[++r] = j;
    begin = i_old < 0 ? 0 : i_old;
    for (v = 1; v <= r; v++) {
        diff = remember[v] - begin;
        for (fill = 0; fill <= diff; fill++)
            angle[begin + fill] += polygon;
        if (v > r)
            break;
        begin = remember[++v];
    }
    loop_size[++lp] = bubble;
    free(remember);
}

/*---------------------------------------------------------------------------*/

PUBLIC int simple_circplot_coordinates(short *pair_table, float *x, float *y) {
    unsigned int length = (unsigned int) pair_table[0];
    unsigned int i;
    float d = 2 * PI / length;
    for (i = 0; i < length; i++) {
        x[i] = cos(i * d - PI / 2);
        y[i] = sin(i * d - PI / 2);
    }
    return length;
}

//--------------------------------------------------------------------------------------------------------------------------

/**
 * RNApuzzler layout
 *
 */
PUBLIC int layout_turtle(
        short *pair_table,
        char *sequence,
        float *x,
        float *y,
        double *arc_coords
) {
    layout_RNAturtle(pair_table, x, y, arc_coords);

    return pair_table[0];
}

/**
 * RNApuzzler layout
 *
 */
PUBLIC int layout_puzzler(
        short *pair_table,
        char *sequence,
        float *x,
        float *y,
        double *arc_coords,
        puzzlerOptions* puzzler
) {
    layout_RNApuzzler(pair_table, x, y, arc_coords, puzzler);

    return pair_table[0];
}

/** Everything below is deprecated: called from nowhere or by itself
 */
// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines
// intersect the intersection point may be stored in the floats i_x and i_y.
/*

short get_line_intersection(
        double p0_x, double p0_y, double p1_x, double p1_y,
        double p2_x, double p2_y, double p3_x, double p3_y,
        double *i_x, double *i_y
) {

    double s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;
    s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;
    s2_y = p3_y - p2_y;
    //   printf("Intersect23 %f %f %f %f\n", s1_x, s1_y, s2_x, s2_y);
    double s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    //  printf("Intersect6 s = (%f * (%f - %f) + %f * (%f - %f)) / (%f * %f + %f * %f);\n", -s1_y, p0_x, p2_x, s1_x,p0_y, p2_y, -s2_x,  s1_y, s1_x, s2_y  );

    t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);
    //    printf("Intersect4 s = %f t = %f\n", s, t);
    //Test if s and b is between 0 and 1 and not signed :)
    if (s >= 0 && s <= 1 && t >= 0 && t <= 1 && signbit(s) == 0 && signbit(t) == 0) {

        // printf("Intersect5 s = %f t = %f\n", s, t);
        // printf("%d\n",signbit(s));
        // Collision detected
        if (i_x != NULL)
            *i_x = p0_x + (t * s1_x);
        if (i_y != NULL)
            *i_y = p0_y + (t * s1_y);
        return 1;
    }

    return 0; // No collision
}

short get_intersection(struct segment * a, struct segment * b, double *i_x, double *i_y) {

    if (a->index + 1 == b->index || a->index - 1 == b->index) {
        return 0;
    }

    //  if(a->start== b->start)
    //  {
    //     return 0;
    //  }


    double p0_x = a->x1;
    double p0_y = a->y1;
    double p1_x = a->x2;
    double p1_y = a->y2;

    double p2_x = b->x1;
    double p2_y = b->y1;
    double p3_x = b->x2;
    double p3_y = b->y2;
    //   printf("Intersect3 %f %f %f %f\n",p0_x,p0_y,p1_x,p1_y);
    //   printf("Intersect3 %f %f %f %f\n",p2_x,p2_y,p3_x,p3_y);

    return get_line_intersection(p0_x, p0_y, p1_x, p1_y, p2_x, p2_y, p3_x, p3_y, i_x, i_y);
}

int get_intersectionLine(line a, line b, float *x, float *y, float *i_x, float *i_y) {

    if (a.index + 1 == b.index || a.index - 1 == b.index) {
        // printf("intersect3 \n");
        return 0;
    }






    //  printf("intersect3 \n");

    double p0_x = x[a.start - 1];
    double p0_y = y[a.start - 1];
    double p1_x = x[a.end - 1];
    double p1_y = y[a.end - 1];

    double p2_x = x[b.start - 1];
    double p2_y = y[b.start - 1];
    double p3_x = x[b.end - 1];
    double p3_y = y[b.end - 1];

    double s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;
    s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;
    s2_y = p3_y - p2_y;

    double s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);

    t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    //Test if s and b is between 0 and 1 and not signed :)
    if (s >= 0 && s <= 1 && t >= 0 && t <= 1 && signbit(s) == 0 && signbit(t) == 0) {

        if (i_x != NULL)
            *i_x = p0_x + (t * s1_x);
        if (i_y != NULL)
            *i_y = p0_y + (t * s1_y);
        return 1;
    }

    return 0; // No collision

}
*/
