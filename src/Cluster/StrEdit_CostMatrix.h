#define   DINF  100000.0
#define   GAP        1.0

/* -------------------------------------------------------------------------- */

/*    BinCode: 'B'    ... RNA sequences, but RY-distance only  */

char  StrEdit_BinCodeA[6] = "_GCAU";
/*       _      G    C     A     U      */
float StrEdit_BinCodeM[5][5] =
    { { 5.,    1.,   1.,   1.,   1. },     /* _ */
      { 1.,    0.,   1.,   0.,   1. },     /* G */
      { 1.,    1.,   0.,   1.,   0. },     /* C */
      { 1.,    0.,   1.,   0.,   1. },     /* A */
      { 1.,    1.,   0.,   1.,   0. } };   /* U */

/* -------------------------------------------------------------------------- */

/*    Dayhoff: 'D'    ... Dayhoff's matrix for Amino Acids      */
/*          CAUTION THIS IS A SIMILARITY MATRIX !!!             */

char  StrEdit_DayhoffA[22] = 
"_CSTPAGNDEQHRKMILVFYW";
float StrEdit_DayhoffM[21][21] =  
/*  _   C   S   T   P   A   G   N   D   E   Q   H   R   K   M   I   L   V   F   Y   W    */
 {{21.,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP},   /* _ ... < ~gap~ >     */
  {GAP,12., 0.,-2.,-3.,-2.,-3.,-4.,-5.,-5.,-5.,-3.,-4.,-5.,-5.,-2.,-6.,-2.,-4., 0.,-8.},   /* C ... Cysteine      */
  {GAP, 0., 2., 1., 1., 1., 1., 1., 0., 0.,-1.,-1., 0., 0.,-2.,-1.,-3.,-1.,-3.,-3.,-2.},   /* S ... Serine        */
  {GAP,-2., 1., 3., 0., 1., 0., 0., 0., 0.,-1.,-1.,-1., 0.,-1., 0.,-2., 0.,-3.,-3.,-5.},   /* T ... Threonine     */
  {GAP,-3., 1., 0., 6., 1.,-1.,-1.,-1.,-1., 0., 0., 0.,-1.,-2.,-2.,-3.,-1.,-5.,-5.,-6.},   /* P ... Proline       */
  {GAP,-2., 1., 1., 1., 2., 1., 0., 0., 0., 0.,-1.,-2.,-1.,-1.,-1.,-2., 0.,-4.,-3.,-6.},   /* A ... Alanine       */
  {GAP,-3., 1., 0.,-1., 1., 5., 0., 1., 0.,-1.,-2.,-3.,-2.,-3.,-3.,-4.,-1.,-5.,-5.,-7.},   /* G ... Glycine       */
  {GAP,-4., 1., 0.,-1., 0., 0., 2., 2., 2., 1., 2., 0., 1.,-2.,-2.,-3.,-2.,-4.,-2.,-4.},   /* N ... Asparagine    */
  {GAP,-5., 0., 0.,-1., 0., 1., 2., 4., 3., 2., 1.,-1., 0.,-3.,-2.,-4.,-2.,-6.,-4.,-7.},   /* D ... Aspartic A    */
  {GAP,-5., 0., 0.,-1., 0., 0., 1., 3., 4., 2., 1.,-1., 0.,-2.,-2.,-3.,-2.,-5.,-4.,-7.},   /* E ... Glutamic A    */
  {GAP,-5.,-1.,-1., 0., 0.,-1., 1., 2., 2., 4., 3., 3., 1.,-1.,-2.,-2.,-2.,-5.,-4.,-5.},   /* Q ... Glutamine     */
  {GAP,-3.,-1.,-1., 0.,-1.,-2., 2., 1., 1., 3., 6., 2., 0.,-2.,-2.,-2.,-2.,-2., 0.,-3.},   /* H ... Histidine     */
  {GAP,-4., 0.,-1., 0.,-2.,-3., 0.,-1.,-1., 1., 2., 6., 3., 0.,-2.,-3.,-2.,-4.,-4.,-2.},   /* R ... Arginine      */
  {GAP,-5., 0., 0.,-1.,-1.,-2., 1., 0., 0., 1., 0., 2., 5., 0.,-2.,-3.,-2.,-5.,-4.,-3.},   /* K ... Lysine        */
  {GAP,-5.,-2.,-1.,-2.,-1.,-3.,-2.,-3.,-2.,-1.,-2., 0., 0., 6., 2., 4., 2., 0.,-2.,-4.},   /* M ... Methionine    */
  {GAP,-2.,-1., 0.,-2.,-1.,-3.,-2.,-2.,-2.,-2.,-2.,-2.,-2., 2., 5., 2., 4., 1.,-1.,-5.},   /* I ... Isoleucine    */
  {GAP,-6.,-3.,-2.,-3.,-2.,-4.,-3.,-4.,-3.,-2.,-2.,-3.,-3., 4., 2., 6., 2., 2.,-1.,-2.},   /* L ... Leucine       */
  {GAP,-2.,-1., 0.,-1., 0.,-1.,-2.,-2.,-2.,-2.,-2.,-2.,-2., 2., 4., 2., 4.,-1.,-2.,-6.},   /* V ... Valine        */
  {GAP,-4.,-3.,-3.,-5.,-4.,-5.,-4.,-6.,-5.,-5.,-2.,-4.,-5., 0., 1., 2.,-1., 9., 7., 0.},   /* F ... Phenylalanine */
  {GAP, 0.,-3.,-3.,-5.,-3.,-5.,-2.,-4.,-4.,-4., 0.,-4.,-4.,-2.,-1.,-1.,-2., 7.,10., 0.},   /* Y ... Tyrosine      */
  {GAP,-8.,-2.,-5.,-6.,-6.,-7.,-4.,-7.,-7.,-5.,-3.,-2.,-2.,-4.,-5.,-2.,-6., 0., 0.,17.}};  /* W ... Tryptophane   */

/* -------------------------------------------------------------------------- */

/*    Hogeweg: 'H'    ... Hogeweg's matrix for String Edit of Secondary Structures */

char  StrEdit_HogewegA[6]="_.^()";
float StrEdit_HogewegM[5][5] = 
/*       _    .    ^     (     )         */
    { {  5., GAP, GAP,  GAP,  GAP },     /* _ ... <Gap>             */
      { GAP,  0.,  1.,   1.,   1. },     /* . ... unpaired          */
      { GAP,  1.,  0.,   1.,   1. },     /* ^ ... hairpin-unpaired  */
      { GAP,  1.,  1.,   0., DINF },     /* ( ... paired upstream   */
      { GAP,  1.,  1., DINF,   0. } };   /* ) ... paired downstream */

/* -------------------------------------------------------------------------- */

char  StrEdit_GLHA[22] = "_ACDEFGHIKLMNPQRSTVWY";
float StrEdit_GLHM[21][21] =  {
   {21.,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP,GAP},
   {GAP, 0.000, .156, .150, .148, .730, .045, .455, .346, .193, .335,
       1.018, .140, .058, .237, .308, .046, .010, .246, .124, .977}, /* ALA */
   {GAP, .156, 0.000, .219, .206, .779, .188, .412, .389, .150, .318,
        .870, .183, .198, .225, .256, .149, .149, .289, .073, .940}, /* CYS */
   {GAP,  .150, .219,0.000, .017, .840, .120, .337, .494, .129, .479,
       1.071, .047, .114, .122, .206, .108, .157, .395, .194, .963}, /* ASP */
   {GAP,  .148, .206, .017,0.000, .836, .122, .328, .493, .114, .473,
       1.057, .030, .117, .110, .194, .103, .154, .393, .181, .950}, /* GLU */
   {GAP,  .730, .779, .840, .836,0.000, .753,1.036, .595, .864, .586,
       1.301, .822, .763, .900, .957, .761, .729, .617, .726, .920}, /* PHE */
   {GAP,  .045, .188, .120, .122, .753,0.000, .444, .376, .193, .377,
       1.055, .124, .014, .224, .302, .046, .054, .279, .157, .989}, /* GLY */
   {GAP,  .455, .412, .337, .328,1.036, .444,0.000, .785, .279, .718,
       1.044, .321, .442, .225, .171, .411, .459, .682, .404, .824}, /* HIS */
   {GAP,  .346, .389, .494, .493, .595, .376, .785,0.000, .513, .159,
       1.006, .485, .385, .575, .631, .391, .340, .103, .385,1.143}, /* ILE */
   {GAP,  .193, .150, .129, .114, .864, .193, .279, .513,0.000, .461,
        .954, .093, .194, .077, .119, .152, .193, .410, .152, .919}, /* LYS */
   {GAP,  .335, .318, .479, .473, .586, .377, .718, .159, .461,0.000,
        .872, .456, .389, .530, .571, .372, .327, .142, .319,1.035}, /* LEU */
   {GAP, 1.018, .870,1.071,1.057,1.301,1.055,1.044,1.006, .954, .872,
       0.000,1.033,1.065,1.013, .966,1.018,1.010, .977, .921,1.282}, /* MET */
   {GAP,  .140, .183, .047, .030, .822, .124, .321, .485, .093, .456,
       1.033,0.000, .122, .102, .184, .095, .146, .383, .156, .930}, /* ASN */
   {GAP,  .058, .198, .114, .117, .763, .014, .442, .385, .194, .389,
       1.065, .122,0.000, .222, .301, .054, .067, .289, .168, .996}, /* PRO */
   {GAP,  .237, .225, .122, .110, .900, .224, .225, .575, .077, .530,
       1.013, .102, .222,0.000, .086, .192, .241, .472, .216, .910}, /* GLN */
   {GAP,  .308, .256, .206, .194, .957, .302, .171, .631, .119, .571,
        .966, .184, .301, .086,0.000, .265, .310, .528, .261, .901}, /* ARG */
   {GAP,  .046, .149, .108, .103, .761, .046, .411, .391, .152, .372,
       1.018, .095, .054, .192, .265,0.000, .051, .289, .117, .963}, /* SER */
   {GAP,  .010, .149, .157, .154, .729, .054, .459, .340, .193, .327,
       1.010, .146, .067, .241, .310, .051,0.000, .239, .119, .979}, /* THR */
   {GAP,  .246, .289, .395, .393, .617, .279, .682, .103, .410, .142,
        .977, .383, .289, .472, .528, .289, .239,0.000, .282,1.076}, /* VAL */
   {GAP,  .124, .073, .194, .181, .726, .157, .404, .385, .152, .319,
        .921, .156, .168, .216, .261, .117, .119, .282,0.000, .893}, /* TRP */
   {GAP,  .977, .940, .963, .950, .920, .989, .824,1.143, .919,1.035,
       1.282, .930, .996, .910, .901, .963, .979,1.076, .893,0.000}, /* TYR */
};
	
