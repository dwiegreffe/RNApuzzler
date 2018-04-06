/*
        PostScript and other output formats for RNA secondary structure plots

                 c  Ivo Hofacker, Peter F Stadler, Ronny Lorenz
                          Vienna RNA package
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "ViennaRNA/model.h"
#include "ViennaRNA/utils.h"
#include "ViennaRNA/fold_vars.h"
#include "ViennaRNA/aln_util.h"
#include "ViennaRNA/gquad.h"
#include "ViennaRNA/plot_layouts.h"
#include "ViennaRNA/plot_structure.h"

/*
#################################
# PRIVATE MACROS                #
#################################
*/

#ifndef PI
#define PI 3.141592654
#endif
#ifndef PIHALF
#define PIHALF PI/2.
#endif
#ifndef SIZE
#define SIZE 452.
#endif

/*
#################################
# GLOBAL VARIABLES              #
#################################
*/

/*
#################################
# PRIVATE VARIABLES             #
#################################
*/
static const char *RNAss_head =
"%%BeginProlog\n"
"/RNAplot 100 dict def\n"
"RNAplot begin\n"
"/fsize  14 def\n"
"/outlinecolor {0.2 setgray} bind def\n"
"/paircolor    {0.2 setgray} bind def\n"
"/seqcolor     {0   setgray} bind def\n"
"/cshow  { dup stringwidth pop -2 div fsize -3 div rmoveto show} bind def\n"
"/min { 2 copy gt { exch } if pop } bind def\n"
"/max { 2 copy lt { exch } if pop } bind def\n"
"/arccoords { % i j arccoords\n"
"  % puts optimal x1 y1 x2 y2 coordinates used in bezier curves from i to j\n"
"  % onto the stack\n"
"  dup 3 -1 roll dup 4 -1 roll lt dup dup 5 2 roll {exch} if\n"
"  dup 3 -1 roll dup 3 -1 roll exch sub 1 sub dup\n"
"  4 -2 roll 5 -1 roll {exch} if 4 2 roll\n"
"  sequence length dup 2 div exch 3 1 roll lt \n"
"  {exch 5 -1 roll pop 4 -2 roll exch 4 2 roll}\n"
"  { 4 2 roll 5 -1 roll dup 6 1 roll {exch} if\n"
"    4 -2 roll exch pop dup 3 -1 roll dup 4 1 roll\n"
"    exch add 4 -1 roll dup 5 1 roll sub 1 sub\n"
"    5 -1 roll not {4 -2 roll exch 4 2 roll} if\n"
"  }ifelse\n"
"   % compute the scalingfactor and prepare (1-sf) and sf*r\n"
"  2 mul exch cpr 3 1 roll div dup\n"
"  3 -1 roll mul exch 1 exch sub exch\n"
"   % compute the coordinates\n"
"  3 -1 roll 1 sub coor exch get aload pop % get coord for i\n"
"  4 -1 roll dup 5 1 roll mul 3 -1 roll dup 4 1 roll add exch % calculate y1\n"
"  4 -1 roll dup 5 1 roll mul 3 -1 roll dup 4 1 roll add exch % calculate x1\n"
"  5 -1 roll 1 sub coor exch get aload pop % get coord for j\n"
"  % duplicate j coord\n"
"  dup 3 -1 roll dup 4 1 roll exch 8 2 roll\n"
"  6 -1 roll dup 7 1 roll mul 5 -1 roll dup 6 1 roll add exch % calculate y2\n"
"  6 -1 roll mul 5 -1 roll add exch % calculate x2\n"
"  6 -2 roll % reorder\n"
"} bind def\n"
"\n"
"% draw outline\n"
"/drawoutline {\n"
"    gsave outlinecolor\n"
"    newpath\n"
"    coor 0 get aload pop 0.8 0 360 arc % draw 5' circle of 1st sequence\n"
"    currentdict /cutpoint known        % check if cutpoint is defined\n"
"    {\n"
"        coor 0 cutpoint getinterval\n"
"        { aload pop lineto } forall             % draw outline of 1st sequence\n"
"        coor cutpoint 1 add get aload pop\n"
"        2 copy moveto 0.8 0 360 arc             % draw 5' circle of 2nd sequence\n"
"        coor cutpoint 1 add coor length cutpoint 1 add sub getinterval\n"
"        { aload pop lineto } forall             % draw outline of 2nd sequence\n"
"    } {\n"
"        /i 0 def\n"
"        coor {\n"
"            /p coor i get def\n"
"            /a arcs i get def\n"
"            /l a length def\n"
"            l 0 eq {\n"
"                %% standard point... draw simple line\n"
"                p 0 2 getinterval aload pop lineto\n"
"            } {\n"
"                %% point in loop... draw arc around loop center\n"
"                /clockwise a 5 get def\n"
"                clockwise 0.0000000 gt {\n"
"                    a 0 5 getinterval aload pop arcn\n"
"                } {\n"
"                    a 0 5 getinterval aload pop arc\n"
"                } ifelse\n"
"            } ifelse\n"
"            /i i 1 add def\n"
"        } forall\n"
"    }\n"
"    ifelse\n"
"    stroke grestore\n"
"} bind def\n"
"\n"
"% draw pairs\n"
"/drawpairs {\n"
"    paircolor\n"
"    0.7 setlinewidth\n"
"    [9 3.01] 9 setdash\n"
"    newpath\n"
"    /i 0 def\n"
"    pairs {\n"
"        /pair pairs i get def                     % saving current -> pair\n"
"        /p1 pair 0 get def                        % partner1 -> p1\n"
"        /p2 pair 1 get def                        % partner2 -> p2\n"
"        /xy1 coor p1 1 sub get def                % reading xy for p1 -> xy1\n"
"        /xy2 coor p2 1 sub get def                % reading xy for p2 -> xy2\n"
"        xy1 0 2 getinterval aload pop moveto      % move cursor to position of p1\n"
"        xy2 0 2 getinterval aload pop lineto      % draw line to position of p2\n"
"        /i i 1 add def\n"
"    } forall\n"
"    stroke\n"
"} bind def\n"
"\n"
"% draw bases\n"
"/drawbases {\n"
"    [] 0 setdash\n"
"    seqcolor\n"
"    /i 0 def\n"
"    coor {\n"
"        /p coor i get def                         % saving coor[i] -> p\n"
"        p 0 2 getinterval aload pop moveto        % reading xy for p an move cursor to position of p\n"
"        sequence i 1 getinterval cshow            % draw base character to position of p\n"
"        /i i 1 add def\n"
"    } forall\n"
"    pop\n"
"} bind def\n"
"\n"
"/init {\n"
"    /Helvetica findfont fsize scalefont setfont\n"
"    1 setlinejoin\n"
"    1 setlinecap\n"
"    0.8 setlinewidth\n"
"    72 216 translate\n"
"    % find the coordinate range\n"
"    /xmax -1000 def /xmin 10000 def\n"
"    /ymax -1000 def /ymin 10000 def\n"
"    \n"
"    /i 0 def\n"
"    coor {\n"
"        /p coor i get def\n"
"        /x p 0 get def\n"
"        /y p 1 get def\n"
"        y ymin lt { /ymin y def } if\n"
"        y ymax gt { /ymax y def } if\n"
"        x xmin lt { /xmin x def } if\n"
"        x xmax gt { /xmax x def } if\n"
"        /l p length def\n"
"        l 8 eq {\n"
"            /r p 4 get def\n"
"            /x1 p 2 get r add def\n"
"            /x2 p 2 get r sub def\n"
"            /y1 p 3 get r add def\n"
"            /y2 p 3 get r sub def\n"
"            y1 ymin lt { /ymin y1 def } if\n"
"            y1 ymax gt { /ymax y1 def } if\n"
"            x1 xmin lt { /xmin x1 def } if\n"
"            x1 xmax gt { /xmax x1 def } if\n"
"            y2 ymin lt { /ymin y2 def } if\n"
"            y2 ymax gt { /ymax y2 def } if\n"
"            x2 xmin lt { /xmin x2 def } if\n"
"            x2 xmax gt { /xmax x2 def } if\n"
"        } if\n"
"        /i i 1 add def\n"
"    } forall\n"
"    \n"
"    /size {xmax xmin sub ymax ymin sub max} bind def\n"
"    72 6 mul size div dup scale\n"
"    size xmin sub xmax sub 2 div size ymin sub ymax sub 2 div\n"
"    translate\n"
"} bind def\n"
"end\n";

static const char *anote_macros =
"RNAplot begin\n"
"% extra definitions for standard anotations\n"
"/min { 2 copy gt { exch } if pop } bind def\n"
"/BLACK { 0 0 0 } def\n"
"/RED   { 1 0 0 } def\n"
"/GREEN { 0 1 0 } def\n"
"/BLUE  { 0 0 1 } def\n"
"/WHITE { 1 1 1 } def\n"
"/LabelFont { % font size LabelFont\n"
"  exch findfont exch fsize mul scalefont setfont\n"
"} bind def\n"
"/Label { % i dx dy (text) Label\n"
"  % write text at base i plus offset dx, dy\n"
"  4 3 roll 1 sub coor exch get aload pop moveto\n"
"  3 1 roll fsize mul exch fsize mul exch rmoveto\n"
"  show\n"
"} bind def\n"
"/cmark { % i cmark   draw circle around base i\n"
"  newpath 1 sub coor exch get aload pop\n"
"  fsize 2 div 0 360 arc stroke\n"
"} bind def\n"
"/gmark { % i j c gmark\n"
"  % draw basepair i,j with c counter examples in gray\n"
"  gsave\n"
"  3 min [0 0.33 0.66 0.9] exch get setgray\n"
"  1 sub dup coor exch get aload pop moveto\n"
"  sequence exch 1 getinterval cshow\n"
"  1 sub dup coor exch get aload pop moveto\n"
"  sequence exch 1 getinterval cshow\n"
"  grestore\n"
"} bind def\n"
"/segmark { % f i j lw r g b segmark\n"
"  % mark segment [i,j] with outline width lw and color rgb\n"
"  % use omark and Fomark instead\n"
"  gsave\n"
"  setrgbcolor setlinewidth\n"
"  newpath\n"
"  1 sub exch 1 sub dup\n"
"  coor exch get aload pop moveto\n"
"  currentdict (cpr) known\n"
"  {\n"
"    3 -1 roll dup 4 1 roll dup\n"
"    {\n"
"      3 1 roll dup 3 -1 roll dup\n"
"      4 1 roll exch 5 2 roll exch\n"
"    }\n"
"    {\n"
"      3 1 roll exch\n"
"    } ifelse\n"
"    1 exch { coor exch get aload pop lineto } for\n"
"    {\n"
"      dup 3 1 roll 1 add exch 1 add arccoords pop pop\n"
"      4 2 roll 5 -1 roll coor exch get aload pop curveto\n"
"    } if\n"
"  }\n"
"  {\n"
"    exch 1 exch {\n"
"      coor exch get aload pop lineto\n"
"    } for\n"
"  } ifelse\n"
"  { closepath fill } if  stroke\n"
"  grestore\n"
"} bind def\n"
"/omark { % i j lw r g b omark\n"
"  % stroke segment [i..j] with linewidth lw, color rgb\n"
"  false 7 1 roll segmark\n"
"} bind def\n"
"/Fomark { % i j r g b Fomark\n"
"  % fill segment [i..j] with color rgb\n"
"  % should precede drawbases\n"
"  1 4 1 roll true 7 1 roll segmark\n"
"} bind def\n"
"/BFmark{ % i j k l r g b BFmark\n"
"  % fill block between pairs (i,j) and (k,l) with color rgb\n"
"  % should precede drawbases\n"
"  gsave\n"
"  setrgbcolor\n"
"  newpath\n"
"  currentdict (cpr) known\n"
"  {\n"
"    dup 1 sub coor exch get aload pop moveto % move to l\n"
"    dup 1 sub 4 -1 roll dup 5 1 roll 1 sub 1 exch\n"
"    { coor exch get aload pop lineto } for % lines from l to j\n"
"    3 -1 roll 4 -1 roll dup 5 1 roll arccoords curveto % curve from j to i\n"
"    exch dup 4 -1 roll 1 sub exch 1 sub 1 exch\n"
"    { coor exch get aload pop lineto } for % lines from i to k\n"
"    exch arccoords curveto% curve from k to l\n"
"  }\n"
"  {  exch 4 3 roll exch 1 sub exch 1 sub dup\n"
"     coor exch get aload pop moveto\n"
"     exch 1 exch { coor exch get aload pop lineto } for\n"
"     exch 1 sub exch 1 sub dup\n"
"     coor exch get aload pop lineto\n"
"     exch 1 exch { coor exch get aload pop lineto } for\n"
"  } ifelse\n"
"    closepath fill stroke\n"
"   grestore\n"
"} bind def\n"
"/hsb {\n"
"  dup 0.3 mul 1 exch sub sethsbcolor\n"
"} bind def\n"
"/colorpair { % i j hue sat colorpair\n"
"  % draw basepair i,j in color\n"
"  % 1 index 0.00 ne {\n"
"  gsave\n"
"  newpath\n"
"  hsb\n"
"  fsize setlinewidth\n"
"  currentdict (cpr) known\n"
"  {\n"
"    exch dup\n"
"    coor  exch 1 sub get aload pop moveto\n"
"    exch arccoords curveto\n"
"  }\n"
"  { 1 sub coor exch get aload pop moveto\n"
"    1 sub coor exch get aload pop lineto\n"
"  } ifelse\n"
"   stroke\n"
"   grestore\n"
"   % } if\n"
"} bind def\n"
 "end\n\n";

/*
#################################
# PRIVATE FUNCTION DECLARATIONS #
#################################
*/

PRIVATE char **annote(const char *structure, const char *AS[]);


/*
#################################
# BEGIN OF FUNCTION DEFINITIONS #
#################################
*/

PUBLIC int
vrna_file_PS_rnaplot( const char *string,
                      const char *structure,
                      const char *ssfile,
                      vrna_md_t  *md_p){

  return vrna_file_PS_rnaplot_a(string, structure, ssfile, NULL, NULL, md_p);
}

PRIVATE int
vrna_file_PS_rnaplot_a_base( const char *seq,
                        const char *structure,
                        const char *ssfile,
                        const char *pre,
                        const char *post,
                        vrna_md_t  *md_p,
                        puzzlerOptions* puzzler
) {

  float  xmin, xmax, ymin, ymax;
  int    i, length;
  int    ee, gb, ge, Lg, l[3];
  float *X, *Y;
  FILE  *xyplot;
  short *pair_table, *pair_table_g;
  char  *c, *string;
  vrna_md_t   md;

  if(!md_p){
    set_model_details(&md);
    md_p  = &md;
  }
    
  string = strdup(seq);
  length = strlen(string);

  xyplot = fopen(ssfile, "w");
  if (xyplot == NULL) {
    vrna_message_warning("can't open file %s - not doing xy_plot", ssfile);
    return 0;
  }

  pair_table = vrna_ptable(structure);
  pair_table_g = vrna_ptable(structure);

  ge=0;
  while ( (ee=parse_gquad(structure+ge, &Lg, l)) >0 ) {
    ge += ee;
    gb=ge-Lg*4-l[0]-l[1]-l[2]+1;
    /* add pseudo-base pair encloding gquad */
    for (i=0; i<Lg; i++) {
      pair_table_g[ge-i]=gb+i;
      pair_table_g[gb+i]=ge-i;
    }
  } 
      
  X = (float *) vrna_alloc((length+1)*sizeof(float));
  Y = (float *) vrna_alloc((length+1)*sizeof(float));

  double *arc_coords;
  arc_coords = (double *) vrna_alloc(6*length*sizeof(double));
  for (i = 0; i < length; i++) {
    arc_coords[6*i+0] = -1.;
    arc_coords[6*i+1] = -1.;
    arc_coords[6*i+2] = -1.;
    arc_coords[6*i+3] = -1.;
    arc_coords[6*i+4] = -1.;
    arc_coords[6*i+5] = -1.;
  }

  switch(rna_plot_type){
    case VRNA_PLOT_TYPE_SIMPLE:   i = simple_xy_coordinates(pair_table_g, X, Y);
                                  break;

    case VRNA_PLOT_TYPE_CIRCULAR: {
                                    int radius = 3*length;
                                    i = simple_circplot_coordinates(pair_table_g, X, Y);
                                    for (i = 0; i < length; i++) {
                                      X[i] *= radius;
                                      X[i] += radius;
                                      Y[i] *= radius;
                                      Y[i] += radius;
                                    }
                                  }
                                  break;

    case VRNA_PLOT_TYPE_NAVIEW:   i = naview_xy_coordinates(pair_table_g, X, Y);
                                  break;

    case VRNA_PLOT_TYPE_TURTLE:   i = layout_turtle(pair_table, string, X, Y, arc_coords);
                                  break;

    case VRNA_PLOT_TYPE_PUZZLER:  i = layout_puzzler(pair_table, string, X, Y, arc_coords, puzzler);
                                  break;

    default:                      i = layout_puzzler(pair_table, string, X, Y, arc_coords, puzzler);
                                  break;
  }
  if(i!=length)
    vrna_message_warning("strange things happening in PS_rna_plot...");

  xmin = xmax = X[0];
  ymin = ymax = Y[0];
  for (i = 1; i < length; i++) {
     xmin = X[i] < xmin ? X[i] : xmin;
     xmax = X[i] > xmax ? X[i] : xmax;
     ymin = Y[i] < ymin ? Y[i] : ymin;
     ymax = Y[i] > ymax ? Y[i] : ymax;
  }

  fprintf(xyplot,
          "%%!PS-Adobe-3.0 EPSF-3.0\n"
          "%%%%Creator: ViennaRNA-%s\n"
          "%%%%CreationDate: %s"
          "%%%%Title: RNA Secondary Structure Plot\n"
          "%%%%BoundingBox: 0 0 700 700\n"
          "%%%%DocumentFonts: Helvetica\n"
          "%%%%Pages: 1\n"
          "%%%%EndComments\n\n"
          "%%Options: %s\n", VERSION, vrna_time_stamp(), vrna_md_option_string(md_p));
  fprintf(xyplot, "%% to switch off outline pairs of sequence comment or\n"
          "%% delete the appropriate line near the end of the file\n\n");
  fprintf(xyplot, "%s", RNAss_head);

  if (pre || post) {
    fprintf(xyplot, "%s", anote_macros);
  }
  fprintf(xyplot, "%%%%EndProlog\n");

  fprintf(xyplot, "RNAplot begin\n"
          "%% data start here\n");

  /* cut_point */
  if ((c = strchr(structure, '&'))) {
    int cutpoint;
    cutpoint = c - structure;
    string[cutpoint] = ' '; /* replace & with space */
    fprintf(xyplot, "/cutpoint %d def\n", cutpoint);
  }

  /* sequence */
  fprintf(xyplot,"/sequence (\\\n");
  i=0;
  while (i<length) {
    fprintf(xyplot, "%.255s\\\n", string+i);  /* no lines longer than 255 */
    i+=255;
  }
  fprintf(xyplot,") def\n");
  /* coordinates */
  fprintf(xyplot, "/coor [\n");
  for (i = 0; i < length; i++)
  {
      fprintf(xyplot, "[%3.8f %3.8f]\n",
        X[i], Y[i]
      );
  }
  fprintf(xyplot, "] def\n");
  fprintf(xyplot, "/arcs [\n");
  for (i = 0; i < length; i++)
  {
    if (arc_coords[6*i + 2] > 0) { // 6*i+2 is the radius parameter
      fprintf(xyplot, "[%3.8f %3.8f %3.8f %3.8f %3.8f %3.8f]\n",
        arc_coords[6*i + 0],
        arc_coords[6*i + 1],
        arc_coords[6*i + 2],
        arc_coords[6*i + 3],
        arc_coords[6*i + 4],
        arc_coords[6*i + 5]
      );
    } else {
      fprintf(xyplot, "[]\n");
    }
  }
  free(arc_coords);
  fprintf(xyplot, "] def\n");

  /* correction coordinates for quadratic beziers in case we produce a circplot */
  if(rna_plot_type == VRNA_PLOT_TYPE_CIRCULAR)
    fprintf(xyplot, "/cpr %6.2f def\n", (float)3*length);
  /* base pairs */
  fprintf(xyplot, "/pairs [\n");
  for (i = 1; i <= length; i++)
    if (pair_table[i]>i)
      fprintf(xyplot, "[%d %d]\n", i, pair_table[i]);
  /* add gquad pairs */
  ge=0;
  while ( (ee=parse_gquad(structure+ge, &Lg, l)) >0 ) {
    int k;
    fprintf(xyplot, "%% gquad\n");
    ge += ee;
    gb=ge-Lg*4-l[0]-l[1]-l[2]+1; /* add pseudo-base pair encloding gquad */
    for (k=0; k<Lg; k++) {
      int ii, jj, il;
      for (il=0, ii=gb+k; il<3; il++) {
        jj = ii+l[il]+Lg;
        fprintf(xyplot, "[%d %d]\n", ii, jj);
        ii = jj;
      }
      jj = gb+k;
      fprintf(xyplot, "[%d %d]\n", jj, ii);
    }
  }

  fprintf(xyplot, "] def\n\n");

  fprintf(xyplot, "init\n\n");
  /* draw the data */
  if (pre) {
    fprintf(xyplot, "%% Start Annotations\n");
    fprintf(xyplot, "%s\n", pre);
    fprintf(xyplot, "%% End Annotations\n");
  }
  fprintf(xyplot,
          "%% switch off outline pairs or bases by removing these lines\n"
          "drawoutline\n"
          "drawpairs\n"
          "drawbases\n");

  if (post) {
    fprintf(xyplot, "%% Start Annotations\n");
    fprintf(xyplot, "%s\n", post);
    fprintf(xyplot, "%% End Annotations\n");
  }
  fprintf(xyplot, "%% show it\nshowpage\n");
  fprintf(xyplot, "end\n");
  fprintf(xyplot, "%%%%EOF\n");

  fclose(xyplot);

  free(string);
  free(pair_table);
  free(pair_table_g);
  free(X); free(Y);
  return 1; /* success */
}

PUBLIC int
vrna_file_PS_rnaplot_a( const char *seq,
                        const char *structure,
                        const char *ssfile,
                        const char *pre,
                        const char *post,
                        vrna_md_t  *md_p
) {
    puzzlerOptions* puzzler = createPuzzlerOptions();

    int ret = vrna_file_PS_rnaplot_a_base(seq, structure, ssfile, pre, post, md_p, puzzler);

    destroyPuzzlerOptions(puzzler);

    return ret;
}

PUBLIC int
vrna_file_PS_rnaplot_a_puzzler( const char *seq,
                        const char *structure,
                        const char *ssfile,
                        const char *pre,
                        const char *post,
                        vrna_md_t  *md_p,
                        puzzlerOptions* puzzler
) {
    int ret = vrna_file_PS_rnaplot_a_base(seq, structure, ssfile, pre, post, md_p, puzzler);
    return ret;
}

/* options for gml output:
   uppercase letters: print sequence labels
   lowercase letters: no sequence lables
   graphics information:
   x X  simple xy plot
   (nothing else implemented at present)
   default:           no graphics data at all
*/

PUBLIC int gmlRNA(char *string, char *structure, char *ssfile, char option)
{
  FILE *gmlfile;
  int i;
  int length;
  short *pair_table;
  float *X, *Y;

  gmlfile = fopen(ssfile, "w");
  if (gmlfile == NULL) {
     vrna_message_warning("can't open file %s - not doing xy_plot", ssfile);
     return 0;
  }

  length = strlen(string);

  pair_table = vrna_ptable(structure);

  switch(option){
  case 'X' :
  case 'x' :
    /* Simple XY Plot */
    X = (float *) vrna_alloc((length+1)*sizeof(float));
    Y = (float *) vrna_alloc((length+1)*sizeof(float));
    if (rna_plot_type == 0)
      i = simple_xy_coordinates(pair_table, X, Y);
    else
      i = naview_xy_coordinates(pair_table, X, Y);

    if(i!=length)
      vrna_message_warning("strange things happening in gmlRNA ...");
    break;
  default:
    /* No Graphics Information */
    X = NULL;
    Y = NULL;
  }

  fprintf(gmlfile,
          "# Vienna RNA Package %s\n"
          "# GML Output\n"
          "# CreationDate: %s\n"
          "# Name: %s\n"
          "# Options: %s\n", VERSION, vrna_time_stamp(), ssfile, option_string());
  fprintf(gmlfile,
          "graph [\n"
          " directed 0\n");
  for (i=1; i<=length; i++){
     fprintf(gmlfile,
          " node [ id %d ", i);
     if (option) fprintf(gmlfile,
          "label \"%c\"",string[i-1]);
     if ((option == 'X')||(option=='x'))
       fprintf(gmlfile,
               "\n  graphics [ x %9.4f y %9.4f ]\n", X[i-1], Y[i-1]);
     fprintf(gmlfile," ]\n");
  }
  for (i=1; i<length; i++)
    fprintf(gmlfile,
            "edge [ source %d target %d ]\n", i, i+1);
  for (i=1; i<=length; i++) {
     if (pair_table[i]>i)
        fprintf(gmlfile,
                "edge [ source %d target %d ]\n", i, pair_table[i]);
  }
  fprintf(gmlfile, "]\n");
  fclose(gmlfile);

  free(pair_table);
  free(X); free(Y);
  return 1; /* success */
}



int PS_rna_plot_snoop_a(char *string, char *structure, char *ssfile, int *relative_access, const char *seqs[])
{
  int    i, length;
  float *X, *Y;
  FILE  *xyplot;
  short *pair_table;
  short *pair_table_snoop;

  length = strlen(string);

  xyplot = fopen(ssfile, "w");
  if (xyplot == NULL) {
    vrna_message_warning("can't open file %s - not doing xy_plot", ssfile);
    return 0;
  }

  pair_table = vrna_ptable(structure);
  pair_table_snoop = vrna_pt_snoop_get(structure);

  X = (float *) vrna_alloc((length+1)*sizeof(float));
  Y = (float *) vrna_alloc((length+1)*sizeof(float));
  if (rna_plot_type == 0)
    i = simple_xy_coordinates(pair_table, X, Y);
  else
    i = naview_xy_coordinates(pair_table, X, Y);
  if(i!=length)
    vrna_message_warning("strange things happening in PS_rna_plot...");
/*   printf("cut_point %d\n", cut_point); */

/*   for (i = 1; i < length; i++) { */
/*     printf("%d X %f Y %f \n", i, X[i], Y[i]); */
/*     xmin = X[i] < xmin ? X[i] : xmin; */
/*     xmax = X[i] > xmax ? X[i] : xmax; */
/*     ymin = Y[i] < ymin ? Y[i] : ymin; */
/*     ymax = Y[i] > ymax ? Y[i] : ymax; */
/*   } */
  /* localize centre of the interaction bucket. Geometry */
  
  for (i = 1; i < cut_point; i++) {  /* interior loop of size 0 */
    if(pair_table_snoop[i] != 0){ 
      X[i-1]=X[pair_table_snoop[i]-1]; 
      Y[i-1]=Y[pair_table_snoop[i]-1]; 
    }
    else if(pair_table_snoop[i-1] && pair_table_snoop[i+1]){ /* interior loop of size 1 */
      X[i-1]=X[pair_table_snoop[i-1] -1-1];
      Y[i-1]=Y[pair_table_snoop[i-1] -1-1];
    } 
    else if(pair_table_snoop[i-1] && pair_table_snoop[i+2]){ /* interior loop of size 2 */
      if(pair_table_snoop[i-1] - pair_table_snoop[i+2] ==2){
        X[i-1]=X[pair_table_snoop[i-1]-2];
        Y[i-1]=Y[pair_table_snoop[i-1]-2];
        X[i]=X[pair_table_snoop[i+2]];
        Y[i]=Y[pair_table_snoop[i+2]];
        i++;
      }
      else if(pair_table[pair_table_snoop[i-1]-1]){
        X[i-1]=X[pair_table_snoop[i-1]-2];
        Y[i-1]=Y[pair_table_snoop[i-1]-2];
        X[i]=X[pair_table[pair_table_snoop[i-1]-1]-1];
        Y[i]=Y[pair_table[pair_table_snoop[i-1]-1]-1];
        i++;
      }
      else if(pair_table[pair_table_snoop[i-1]-2]){
        X[i-1]=X[pair_table_snoop[i-1]-3];
        Y[i-1]=Y[pair_table_snoop[i-1]-3];
        X[i]=X[pair_table[pair_table_snoop[i-1]-2]-1];
        Y[i]=Y[pair_table[pair_table_snoop[i-1]-2]-1];
        i++;
      }
      else if(pair_table[pair_table_snoop[i-1]-3]){
        X[i-1]=X[pair_table_snoop[i-1]-4];
        Y[i-1]=Y[pair_table_snoop[i-1]-4];
        X[i]=X[pair_table[pair_table_snoop[i-1]-3]-1];
        Y[i]=Y[pair_table[pair_table_snoop[i-1]-3]-1];
        i++;
      }
      else{
        X[i-1]=X[pair_table_snoop[i-1]-2];
        Y[i-1]=Y[pair_table_snoop[i-1]-2];
        X[i]=X[pair_table_snoop[i+2]];
        Y[i]=Y[pair_table_snoop[i+2]];
        i++;
      }
    }
    else if(pair_table_snoop[i-1] && pair_table_snoop[i+3]){ /* interior loop of size 2 */
      if(pair_table[pair_table_snoop[i-1]-1]){
        X[i-1]=0.5*(X[pair_table_snoop[i-1]-1]+X[pair_table_snoop[i-1]-2]);
        Y[i-1]=0.5*(Y[pair_table_snoop[i-1]-1]+Y[pair_table_snoop[i-1]-2]);
        X[i]=  0.5*(X[pair_table[pair_table_snoop[i-1]-1]-1]+X[pair_table_snoop[i-1]-2]);
        Y[i]=  0.5*(Y[pair_table[pair_table_snoop[i-1]-1]-1]+Y[pair_table_snoop[i-1]-2]);
        X[i+1]=0.5*(X[pair_table[pair_table_snoop[i-1]-1]-2]+X[pair_table[pair_table_snoop[i-1]-1]-1]);
        Y[i+1]=0.5*(Y[pair_table[pair_table_snoop[i-1]-1]-2]+Y[pair_table[pair_table_snoop[i-1]-1]-1]);
        i++;i++;

      }
      else if(pair_table[pair_table_snoop[i-1]-2]){
        X[i-1]=0.5*(X[pair_table_snoop[i-1]-2]+X[pair_table_snoop[i-1]-3]);
        Y[i-1]=0.5*(Y[pair_table_snoop[i-1]-2]+Y[pair_table_snoop[i-1]-3]);
        X[i]=  0.5*(X[pair_table[pair_table_snoop[i-1]-2]-1]+X[pair_table_snoop[i-1]-3]);
        Y[i]=  0.5*(Y[pair_table[pair_table_snoop[i-1]-2]-1]+Y[pair_table_snoop[i-1]-3]);
        X[i+1]=0.5*(X[pair_table[pair_table_snoop[i-1]-2]-2]+X[pair_table[pair_table_snoop[i-1]-2]-1]);
        Y[i+1]=0.5*(Y[pair_table[pair_table_snoop[i-1]-2]-2]+Y[pair_table[pair_table_snoop[i-1]-2]-1]);
        i++;i++;
      }
      else if(pair_table[pair_table_snoop[i-1]-3]){
        X[i-1]=0.5*(X[pair_table_snoop[i-1]-3]+X[pair_table_snoop[i-1]-4]);
        Y[i-1]=0.5*(Y[pair_table_snoop[i-1]-3]+Y[pair_table_snoop[i-1]-4]);
        X[i]=  0.5*(X[pair_table[pair_table_snoop[i-1]-3]-1]+X[pair_table_snoop[i-1]-4]);
        Y[i]=  0.5*(Y[pair_table[pair_table_snoop[i-1]-3]-1]+Y[pair_table_snoop[i-1]-4]);
        X[i+1]=0.5*(X[pair_table[pair_table_snoop[i-1]-3]-2]+X[pair_table[pair_table_snoop[i-1]-3]-1]);
        Y[i+1]=0.5*(Y[pair_table[pair_table_snoop[i-1]-3]-2]+Y[pair_table[pair_table_snoop[i-1]-3]-1]);
        i++;i++;
      }
      else{
        X[i-1]=X[pair_table_snoop[i-1]-2];
        Y[i-1]=Y[pair_table_snoop[i-1]-2];
        X[i]=X[pair_table_snoop[i-1]-2];
        Y[i]=Y[pair_table_snoop[i-1]-2];
        X[i+1]=X[pair_table_snoop[i-1]-2];
        Y[i+1]=Y[pair_table_snoop[i-1]-2];
        i++;i++;
      }
    }
  }
  double xC;
  double yC;
  float X0=-1,Y0=-1,X1=-1,Y1=-1,X2=-1,Y2=-1;
/*   int c1,c2,c3; */
  for(i=1;i<cut_point; i++){
    if(pair_table_snoop[i]){
      X0=X[pair_table_snoop[i]-1];Y0=Y[pair_table_snoop[i]-1];
  /*     c1=pair_table_snoop[i]; */
      i++;
      break;
    }
  }
  for(;i<cut_point; i++){
    if(pair_table_snoop[i]){
      X1=X[pair_table_snoop[i]-1];Y1=Y[pair_table_snoop[i]-1];
    /*   c2=pair_table_snoop[i]; */
      i++;
      break;
    }
  }
  for(;i<cut_point; i++){
    if(pair_table_snoop[i]){
      X2=X[pair_table_snoop[i]-1];Y2=Y[pair_table_snoop[i]-1];
    /*   c3=pair_table_snoop[i]; */
      i++;
      break;
    }
  }
/*   for(i=cut_point-2;i>pair_table_snoop[c1]; i--){ */
/*     if(pair_table_snoop[i]){ */
/*       X1=X[pair_table_snoop[i]-1];Y1=Y[pair_table_snoop[i]-1]; */
/*       c2=pair_table_snoop[i]; */
/*       i++; */
/*       break; */
/*     } */
/*   } */
/*   for(i=pair_table_snoop[c1]+1;i<pair_table_snoop[c2]; i++){ */
/*     if(pair_table_snoop[i]){ */
/*       X2=X[pair_table_snoop[i]-1];Y2=Y[pair_table_snoop[i]-1]; */
/*       c3=pair_table_snoop[i]; */
/*       i++; */
/*       break; */
/*     } */
/*   } */ 
 if(X0 < 0 || X1 < 0 || X2 < 0){
   printf("Could not get the center of the binding bucket. No ps file will be produced!\n");
   fclose(xyplot);
   free(pair_table);
   free(pair_table_snoop);
   free(X);free(Y);
   pair_table=NULL;pair_table_snoop=NULL;X=NULL;Y=NULL;
   return 0;
 }
  double alpha   =   (X0 -X1)/(Y1-Y0);
  double alpha_p =   (X1 -X2)/(Y2-Y1);
  double b =         (Y0+Y1 -alpha*(X0+X1))*0.5;
  double b_p =       (Y1+Y2 -alpha_p*(X1+X2))*0.5;
  /*    if(abs(alpha -alpha_p) > 0.0000001){ */
  xC  =  (b_p - b) / (alpha - alpha_p);
  yC  =  alpha * xC + b;
  for (i = 1; i < cut_point; i++) {  
     X[i-1] = X[i-1] + 0.25*(xC-X[i-1]);  
     Y[i-1] = Y[i-1] + 0.25*(yC-Y[i-1]);  
  }  

  fprintf(xyplot,
          "%%!PS-Adobe-3.0 EPSF-3.0\n"
          "%%%%Creator: ViennaRNA-%s\n"
          "%%%%CreationDate: %s"
          "%%%%Title: RNA Secondary Structure Plot\n"
          "%%%%BoundingBox: 0 0 700 700\n"
          "%%%%DocumentFonts: Helvetica\n"
          "%%%%Pages: 1\n"
          "%%%%EndComments\n\n"
          "%%Options: %s\n", VERSION, vrna_time_stamp(), option_string());
  fprintf(xyplot, "%% to switch off outline pairs of sequence comment or\n"
          "%% delete the appropriate line near the end of the file\n\n");
  fprintf(xyplot, "%s", RNAss_head);
  char **A;
  fprintf(xyplot, "%s", anote_macros);
  if(seqs){
    fprintf(xyplot, "%s", anote_macros);
    A = annote(structure, (const char**) seqs);
  }
  fprintf(xyplot, "%%%%EndProlog\n");
  
  fprintf(xyplot, "RNAplot begin\n"
          "%% data start here\n");
  /* cut_point */
  if (cut_point > 0 && cut_point <= strlen(string))
    fprintf(xyplot, "/cutpoint %d def\n", cut_point-1);
  /* sequence */
  fprintf(xyplot,"/sequence (\\\n");
  i=0;
  while (i<length) {
    fprintf(xyplot, "%.255s\\\n", string+i);  /* no lines longer than 255 */
    i+=255;
  }
  fprintf(xyplot,") def\n");
  /* coordinates */
  fprintf(xyplot, "/coor [\n");
  double *arc_coords;
  for (i = 0; i < length; i++)
  {
    if (arc_coords[6*i + 2] > 0) {
      fprintf(xyplot, "[%3.8f %3.8f %3.8f %3.8f %3.8f %3.8f %3.8f %3.8f]\n",
              X[i], Y[i],
              arc_coords[6*i + 0],
              arc_coords[6*i + 1],
              arc_coords[6*i + 2],
              arc_coords[6*i + 3],
              arc_coords[6*i + 4],
              arc_coords[6*i + 5]);
    } else {
      fprintf(xyplot, "[%3.8f %3.8f]\n", X[i], Y[i]);
    }
  }
  free(arc_coords);

    
  fprintf(xyplot, "] def\n");
  /* base pairs */
  fprintf(xyplot, "/pairs [\n");
  for (i = 1; i <= length; i++)
    if (pair_table[i]>i)
      fprintf(xyplot, "[%d %d]\n", i, pair_table[i]);
  for (i = 1; i <= length; i++)
    if (pair_table_snoop[i]>i)
      fprintf(xyplot, "[%d %d]\n", i, pair_table_snoop[i]);
  fprintf(xyplot, "] def\n\n");
  if(relative_access){
    fprintf(xyplot,"/S [\n");
    for(i=0;i<cut_point-1; i++){
      fprintf(xyplot, " %f\n", (float)relative_access[i]/100);
    }
    fprintf(xyplot,"]\n bind def\n");
    fprintf(xyplot,"/invert false def\n");
    fprintf(xyplot,"/range 0.8 def\n");
    fprintf(xyplot,"/drawreliability {\n"                      
                   "/Smax 2.6 def\n"                         
                   "  0        \n"                              
                   "  coor 0 cutpoint getinterval {\n"
                   "    aload pop\n"
                   "    S 3 index get\n"
                   "    Smax div range mul\n"     
                   "    invert {range exch sub} if\n"  
                   "    1 1 sethsbcolor\n"
                   "    newpath\n"
                   "    fsize 2.5 div 0 360 arc\n"
                   "    fill\n"
                   "    1 add\n"
                   "  } forall\n"
                   "\n"
                   "} bind def\n"); 
  }
  fprintf(xyplot, "init\n\n");
  /*raw the data */
  if (seqs) { 
     fprintf(xyplot, "%% Start Annotations\n"); 
     fprintf(xyplot, "%s\n", A[0]); 
     fprintf(xyplot, "%% End Annotations\n"); 
   } 


  fprintf(xyplot,"%%switch off outline pairs or bases by removing these lines\n");
  if(relative_access){
    fprintf(xyplot,"drawreliability\n");
  }
  fprintf(xyplot,
          "drawoutline\n"
          "drawpairs\n"
          "drawbases\n");
  /* fprintf(xyplot, "%d cmark\n",c1); */
  /* fprintf(xyplot, "%d cmark\n",c2); */
  /* fprintf(xyplot, "%d cmark\n",c3); */
  if (seqs) { 
     fprintf(xyplot, "%% Start Annotations\n"); 
     fprintf(xyplot, "%s\n", A[1]); 
     fprintf(xyplot, "%% End Annotations\n"); 
   } 
  fprintf(xyplot, "%% show it\nshowpage\n");
  fprintf(xyplot, "end\n");
  fprintf(xyplot, "%%%%EOF\n");

  fclose(xyplot);
  if(seqs){free(A[0]);free(A[1]);free(A);}
  free(pair_table);free(pair_table_snoop);
  free(X); free(Y);
  return 1; /* success */
}


PRIVATE char **annote(const char *structure, const char *AS[]) {
  char *ps, *colorps, **A;
  int i, n, s, pairings, maxl;
  short *ptable;
  char * colorMatrix[6][3] = {
    {"0.0 1", "0.0 0.6",  "0.0 0.2"},  /* red    */
    {"0.16 1","0.16 0.6", "0.16 0.2"}, /* ochre  */
    {"0.32 1","0.32 0.6", "0.32 0.2"}, /* turquoise */
    {"0.48 1","0.48 0.6", "0.48 0.2"}, /* green  */
    {"0.65 1","0.65 0.6", "0.65 0.2"}, /* blue   */
    {"0.81 1","0.81 0.6", "0.81 0.2"} /* violet */
  };

  vrna_md_t   md;
  set_model_details(&md);

  n = strlen(AS[0]);
  maxl = 1024;

  A = (char **) vrna_alloc(sizeof(char *)*2);
  ps = (char *) vrna_alloc(maxl);
  colorps = (char *) vrna_alloc(maxl);
  ptable = vrna_pt_ali_get(structure);
  for (i=1; i<=n; i++) {
    char pps[64], ci='\0', cj='\0';
    int j, type, pfreq[8] = {0,0,0,0,0,0,0,0}, vi=0, vj=0;
    if ((j=ptable[i])<i) continue;
    for (s=0; AS[s]!=NULL; s++) {
      type = md.pair[vrna_nucleotide_encode(AS[s][i-1], &md)][vrna_nucleotide_encode(AS[s][j-1], &md)];
      pfreq[type]++;
      if (type) {
        if (AS[s][i-1] != ci) { ci = AS[s][i-1]; vi++;}
        if (AS[s][j-1] != cj) { cj = AS[s][j-1]; vj++;}
      }
    }
    for (pairings=0,s=1; s<=7; s++) {
      if (pfreq[s]) pairings++;
    }

    if ((maxl - strlen(ps) < 192) || ((maxl - strlen(colorps)) < 64)) {
      maxl *= 2;
      ps = realloc(ps, maxl);
      colorps = realloc(colorps, maxl);
      if ((ps==NULL) || (colorps == NULL))
          vrna_message_error("out of memory in realloc");
    }

    if (pfreq[0]<=2) {
      snprintf(pps, 64, "%d %d %s colorpair\n",
               i,j, colorMatrix[pairings-1][pfreq[0]]);
      strcat(colorps, pps);
    }

    if (pfreq[0]>0) {
      snprintf(pps, 64, "%d %d %d gmark\n", i, j, pfreq[0]);
      strcat(ps, pps);
    }
    if (vi>1) {
      snprintf(pps, 64, "%d cmark\n", i);
      strcat(ps, pps);
    }
    if (vj>1) {
      snprintf(pps, 64, "%d cmark\n", j);
      strcat(ps, pps);
    }
  }
  free(ptable);
  A[0]=colorps;
  A[1]=ps;
  return A;
}

/*--------------------------------------------------------------------------*/


int svg_rna_plot(char *string, char *structure, char *ssfile)
{
  float  xmin, xmax, ymin, ymax, size;
  int    i, length;
  float *X, *Y, *R = NULL, *CX = NULL, *CY = NULL;
  FILE  *xyplot;
  short *pair_table;

  length = strlen(string);

  xyplot = fopen(ssfile, "w");
  if (xyplot == NULL) {
    vrna_message_warning("can't open file %s - not doing xy_plot", ssfile);
    return 0;
  }

  pair_table = vrna_ptable(structure);

  X = (float *) vrna_alloc((length+1)*sizeof(float));
  Y = (float *) vrna_alloc((length+1)*sizeof(float));

  switch(rna_plot_type){
    case VRNA_PLOT_TYPE_SIMPLE:   i = simple_xy_coordinates(pair_table, X, Y);
                                  break;
    case VRNA_PLOT_TYPE_CIRCULAR: {
                                    int radius = 3*length;
                                    int dr = 0;
                                    R = (float *) vrna_alloc((length+1)*sizeof(float));
                                    CX = (float *) vrna_alloc((length+1)*sizeof(float));
                                    CY = (float *) vrna_alloc((length+1)*sizeof(float));
                                    i = simple_circplot_coordinates(pair_table, X, Y);
                                    for (i = 0; i < length; i++) {
                                      if(i+1 < pair_table[i+1]){
                                        dr = (pair_table[i+1]-i+1 <= (length/2 + 1)) ? pair_table[i+1]-i : i + length - pair_table[i+1];
                                        R[i] = 1. - (2.*dr/(float)length);
                                      }
                                      else if(pair_table[i+1]){
                                        R[i] = R[pair_table[i+1]-1];
                                      }
                                      else{
                                        R[i] = 1.0;
                                      }
                                      CX[i] = X[i] * radius * R[i] + radius;
                                      CY[i] = Y[i] * radius * R[i] + radius;
                                      X[i] *= radius;
                                      X[i] += radius;
                                      Y[i] *= radius;
                                      Y[i] += radius;
                                    }
                                  }
                                  break;
    default:                      i = naview_xy_coordinates(pair_table, X, Y);
                                  break;
  }

  if(i!=length)
    vrna_message_warning("strange things happening in PS_rna_plot...");


  xmin = xmax = X[0];
  ymin = ymax = Y[0];
  for (i = 1; i < length; i++) {
     xmin = X[i] < xmin ? X[i] : xmin;
     xmax = X[i] > xmax ? X[i] : xmax;
     ymin = Y[i] < ymin ? Y[i] : ymin;
     ymax = Y[i] > ymax ? Y[i] : ymax;
  }
  for (i = 0; i < length; i++)
    Y[i] = ymin+ymax - Y[i]; /* mirror coordinates so they look as in PS */

  if(rna_plot_type == VRNA_PLOT_TYPE_CIRCULAR)
    for (i = 0; i < length; i++){
      CY[i] = ymin+ymax - CY[i]; /* mirror coordinates so they look as in PS */
    }
   
  size = MAX2((xmax-xmin),(ymax-ymin));
  size += 15; /* add some so the bounding box isn't too tight */

  fprintf(xyplot,
          "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
          "<svg xmlns=\"http://www.w3.org/2000/svg\" height=\"452\" width=\"452\">\n");
  fprintf(xyplot,
          "<script type=\"text/ecmascript\">\n"
          "      <![CDATA[\n"
          "        var shown = 1;\n"
          "        function click() {\n"
          "             var seq = document.getElementById(\"seq\");\n"
          "             if (shown==1) {\n"
          "               seq.setAttribute(\"style\", \"visibility: hidden\");\n"
          "               shown = 0;\n"
          "             } else {\n"
          "               seq.setAttribute(\"style\", \"visibility: visible\");\n"
          "               shown = 1;\n"
          "             }\n"
          "         }\n"
          "        ]]>\n"
          "</script>\n");
  fprintf(xyplot,
          "  <rect style=\"stroke: white; fill: white\" height=\"452\" x=\"0\" y=\"0\" width=\"452\" onclick=\"click(evt)\" />\n"
          "  <g transform=\"scale(%7f,%7f) translate(%7f,%7f)\">\n",
          SIZE/size, SIZE/size, (size-xmin-xmax)/2, (size-ymin-ymax)/2);

  fprintf(xyplot,
          "    <polyline style=\"stroke: black; fill: none; stroke-width: 1.5\" id=\"outline\" points=\"\n");
  for (i = 0; i < length; i++)
    fprintf(xyplot, "      %3.3f,%3.3f\n", X[i], Y[i]);
  fprintf(xyplot,"    \" />\n");

  fprintf(xyplot,"    <g style=\"stroke: black; stroke-width: 1; fill: none;\" id=\"pairs\">\n");
  for (i = 1; i <= length; i++) {
    int j;
    if ((j=pair_table[i])>i){
      if(rna_plot_type == VRNA_PLOT_TYPE_CIRCULAR)
        fprintf(xyplot,
                "      <path id=\"%d,%d\" d=\"M %6.15f %6.15f C %6.15f,%6.15f %6.15f,%6.15f %6.15f %6.15f\" />\n",
                i,j, X[i-1], Y[i-1], CX[i-1], CY[i-1], CX[j-1], CY[j-1], X[j-1], Y[j-1]);
      else
        fprintf(xyplot,
                "      <line id=\"%d,%d\" x1=\"%6.5f\" y1=\"%6.5f\" x2=\"%6.5f\" y2=\"%6.5f\" />\n",
                i,j, X[i-1], Y[i-1], X[j-1], Y[j-1]);
    }
  }
  fprintf(xyplot, "    </g>\n");
  fprintf(xyplot, "    <g style=\"font-family: SansSerif\" transform=\"translate(-4.6, 4)\" id=\"seq\">\n");
  for (i = 0; i < length; i++)
    fprintf(xyplot, "      <text x=\"%.3f\" y=\"%.3f\">%c</text>\n", X[i], Y[i], string[i]);
  fprintf(xyplot, "    </g>\n");
  fprintf(xyplot, "  </g>\n");
  fprintf(xyplot, "</svg>\n");

  fclose(xyplot);

  free(pair_table);
  free(X); free(Y);
  if(R) free(R);
  if(CX) free(CX);
  if(CY) free(CY);
  return 1; /* success */
}

/*--------------------------------------------------------------------------*/

PUBLIC int ssv_rna_plot(char *string, char *structure, char *ssfile)
{           /* produce input for the SStructView java applet */
  FILE *ssvfile;
  int i, bp;
  int length;
  short *pair_table;
  float *X, *Y;
  float xmin, xmax, ymin, ymax;

  ssvfile = fopen(ssfile, "w");
  if (ssvfile == NULL) {
     vrna_message_warning("can't open file %s - not doing xy_plot", ssfile);
     return 0;
  }
  length = strlen(string);
  pair_table = vrna_ptable(structure);

  /* make coordinates */
  X = (float *) vrna_alloc((length+1)*sizeof(float));
  Y = (float *) vrna_alloc((length+1)*sizeof(float));

  if (rna_plot_type == 0)
    i = simple_xy_coordinates(pair_table, X, Y);
  else
    i = naview_xy_coordinates(pair_table, X, Y);
  if (i!=length)
    vrna_message_warning("strange things happening in ssv_rna_plot...");

  /* make coords nonegative */
  xmin = xmax = X[0];
  ymin = ymax = Y[0];
  for (i = 1; i < length; i++) {
     xmin = X[i] < xmin ? X[i] : xmin;
     xmax = X[i] > xmax ? X[i] : xmax;
     ymin = Y[i] < ymin ? Y[i] : ymin;
     ymax = Y[i] > ymax ? Y[i] : ymax;
  }
  if (xmin<1) {
    for (i = 0; i <= length; i++)
      X[i] -= xmin-1;
    xmin = 1;
  }
  if (ymin<1) {
    for (i = 0; i <= length; i++)
      Y[i] -= ymin-1;
    ymin = 1;
  }
#if 0
  {
    float size, xoff, yoff;
    float JSIZE = 500; /* size of the java applet window */
    /* rescale coordinates, center on square of size HSIZE */
    size = MAX2((xmax-xmin),(ymax-ymin));
    xoff = (size - xmax + xmin)/2;
    yoff = (size - ymax + ymin)/2;
    for (i = 0; i <= length; i++) {
      X[i] = (X[i]-xmin+xoff)*(JSIZE-10)/size + 5;
      Y[i] = (Y[i]-ymin+yoff)*(JSIZE-10)/size + 5;
    }
  }
#endif
  /* */

  fprintf(ssvfile,
          "# Vienna RNA Package %s\n"
          "# SStructView Output\n"
          "# CreationDate: %s\n"
          "# Name: %s\n"
          "# Options: %s\n", VERSION, vrna_time_stamp(), ssfile, option_string());
  for (i=1; i<=length; i++)
    fprintf(ssvfile, "BASE\t%d\t%c\t%d\t%d\n",
            i, string[i-1], (int) (X[i-1]+0.5), (int) (Y[i-1]+0.5));
  for (bp=1, i=1; i<=length; i++)
    if (pair_table[i]>i)
      fprintf(ssvfile, "BASE-PAIR\tbp%d\t%d\t%d\n", bp++, i, pair_table[i]);
  fclose(ssvfile);

  free(pair_table);
  free(X); free(Y);
  return 1; /* success */
}

/*---------------------------------------------------------------------------*/
PUBLIC int xrna_plot(char *string, char *structure, char *ssfile)
{           /* produce input for XRNA RNA drawing program */
  FILE *ss_file;
  int i;
  int length;
  short *pair_table;
  float *X, *Y;

  ss_file = fopen(ssfile, "w");
  if (ss_file == NULL) {
    vrna_message_warning("can't open file %s - not doing xy_plot", ssfile);
    return 0;
  }

  length = strlen(string);
  pair_table = vrna_ptable(structure);

  /* make coordinates */
  X = (float *) vrna_alloc((length+1)*sizeof(float));
  Y = (float *) vrna_alloc((length+1)*sizeof(float));

  if (rna_plot_type == 0)
    i = simple_xy_coordinates(pair_table, X, Y);
  else
    i = naview_xy_coordinates(pair_table, X, Y);
  if (i!=length)
    vrna_message_warning("strange things happening in xrna_plot...");

  fprintf(ss_file,
          "# Vienna RNA Package %s, XRNA output\n"
          "# CreationDate: %s\n"
          "# Options: %s\n", VERSION, vrna_time_stamp(), option_string());
  for (i=1; i<=length; i++)
    /* XRNA likes to have coordinate mirrored, so we use (-X, Y) */
    fprintf(ss_file, "%d %c %6.2f %6.2f %d %d\n", i, string[i-1],
            -X[i-1], Y[i-1], (pair_table[i]?1:0), pair_table[i]);
  fclose(ss_file);

  free(pair_table);
  free(X); free(Y);
  return 1; /* success */
}

#ifdef VRNA_BACKWARD_COMPAT

/*###########################################*/
/*# deprecated functions below              #*/
/*###########################################*/

PUBLIC int
PS_rna_plot(char *string,
            char *structure,
            char *ssfile){

  return vrna_file_PS_rnaplot((const char*)string,
                              (const char*)structure,
                              (const char*) ssfile,
                              NULL);
}

PUBLIC int
PS_rna_plot_a(char *string,
              char *structure,
              char *ssfile,
              char *pre,
              char *post){

  return vrna_file_PS_rnaplot_a((const char*)string,
                                (const char*)structure,
                                (const char*)ssfile,
                                (const char*)pre,
                                (const char*)post,
                                NULL);
}

PUBLIC int
PS_rna_plot_a_gquad(char *string,
                    char *structure,
                    char *ssfile,
                    char *pre,
                    char *post){

  return vrna_file_PS_rnaplot_a((const char*)string,
                                (const char*)structure,
                                (const char*)ssfile,
                                (const char*)pre,
                                (const char*)post,
                                NULL);
}

#endif

