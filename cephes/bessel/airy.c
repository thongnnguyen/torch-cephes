/*							airy.c
 *
 *	Airy function
 *
 *
 *
 * SYNOPSIS:
 *
 * double x, ai, aip, bi, bip;
 * int airy();
 *
 * airy( x, _&ai, _&aip, _&bi, _&bip );
 *
 *
 *
 * DESCRIPTION:
 *
 * Solution of the differential equation
 *
 *	y"(x) = xy.
 *
 * The function returns the two independent solutions Ai, Bi
 * and their first derivatives Ai'(x), Bi'(x).
 *
 * Evaluation is by power series summation for small x,
 * by rational minimax approximations for large x.
 *
 *
 *
 * ACCURACY:
 * Error criterion is absolute when function <= 1, relative
 * when function > 1, except * denotes relative error criterion.
 * For large negative x, the absolute error increases as x^1.5.
 * For large positive x, the relative error increases as x^1.5.
 *
 * Arithmetic  domain   function  # trials      peak         rms
 * IEEE        -10, 0     Ai        10000       1.6e-15     2.7e-16
 * IEEE          0, 10    Ai        10000       2.3e-14*    1.8e-15*
 * IEEE        -10, 0     Ai'       10000       4.6e-15     7.6e-16
 * IEEE          0, 10    Ai'       10000       1.8e-14*    1.5e-15*
 * IEEE        -10, 10    Bi        30000       4.2e-15     5.3e-16
 * IEEE        -10, 10    Bi'       30000       4.9e-15     7.3e-16
 * DEC         -10, 0     Ai         5000       1.7e-16     2.8e-17
 * DEC           0, 10    Ai         5000       2.1e-15*    1.7e-16*
 * DEC         -10, 0     Ai'        5000       4.7e-16     7.8e-17
 * DEC           0, 10    Ai'       12000       1.8e-15*    1.5e-16*
 * DEC         -10, 10    Bi        10000       5.5e-16     6.8e-17
 * DEC         -10, 10    Bi'        7000       5.3e-16     8.7e-17
 *
 */
/*							airy.c */

/*
Cephes Math Library Release 2.8:  June, 2000
Copyright 1984, 1987, 1989, 2000 by Stephen L. Moshier
*/

#include "mconf.h"

static double c1 = 0.35502805388781723926;
static double c2 = 0.258819403792806798405;
static double sqrt3 = 1.732050807568877293527;
static double sqpii = 5.64189583547756286948E-1;
extern double torch_cephes_PI;

extern double torch_cephes_MAXNUM, torch_cephes_MACHEP;
#ifdef UNK
#define MAXAIRY 25.77
#endif
#ifdef DEC
#define MAXAIRY 25.77
#endif
#ifdef IBMPC
#define MAXAIRY 103.892
#endif
#ifdef MIEEE
#define MAXAIRY 103.892
#endif


#ifdef UNK
static double AN[8] = {
  3.46538101525629032477E-1,
  1.20075952739645805542E1,
  7.62796053615234516538E1,
  1.68089224934630576269E2,
  1.59756391350164413639E2,
  7.05360906840444183113E1,
  1.40264691163389668864E1,
  9.99999999999999995305E-1,
};
static double AD[8] = {
  5.67594532638770212846E-1,
  1.47562562584847203173E1,
  8.45138970141474626562E1,
  1.77318088145400459522E2,
  1.64234692871529701831E2,
  7.14778400825575695274E1,
  1.40959135607834029598E1,
  1.00000000000000000470E0,
};
#endif
#ifdef DEC
static unsigned short AN[32] = {
0037661,0066561,0024675,0131301,
0041100,0017434,0034324,0101466,
0041630,0107450,0067427,0007430,
0042050,0013327,0071000,0034737,
0042037,0140642,0156417,0167366,
0041615,0011172,0075147,0051165,
0041140,0066152,0160520,0075146,
0040200,0000000,0000000,0000000,
};
static unsigned short AD[32] = {
0040021,0046740,0011422,0064606,
0041154,0014640,0024631,0062450,
0041651,0003435,0101152,0106401,
0042061,0050556,0034605,0136602,
0042044,0036024,0152377,0151414,
0041616,0172247,0072216,0115374,
0041141,0104334,0124154,0166007,
0040200,0000000,0000000,0000000,
};
#endif
#ifdef IBMPC
static unsigned short AN[32] = {
0xb658,0x2537,0x2dae,0x3fd6,
0x9067,0x871a,0x03e3,0x4028,
0xe1e3,0x0de2,0x11e5,0x4053,
0x073c,0xee40,0x02da,0x4065,
0xfddf,0x5ba1,0xf834,0x4063,
0xea4f,0x4f4c,0xa24f,0x4051,
0x0f4d,0x5c2a,0x0d8d,0x402c,
0x0000,0x0000,0x0000,0x3ff0,
};
static unsigned short AD[32] = {
0x4d31,0x0262,0x29bc,0x3fe2,
0x2ca5,0x0533,0x8334,0x402d,
0x51a0,0xb04d,0x20e3,0x4055,
0xb7b0,0xc730,0x2a2d,0x4066,
0xfa61,0x9a9f,0x8782,0x4064,
0xd35f,0xee91,0xde94,0x4051,
0x9d81,0x950d,0x311b,0x402c,
0x0000,0x0000,0x0000,0x3ff0,
};
#endif
#ifdef MIEEE
static unsigned short AN[32] = {
0x3fd6,0x2dae,0x2537,0xb658,
0x4028,0x03e3,0x871a,0x9067,
0x4053,0x11e5,0x0de2,0xe1e3,
0x4065,0x02da,0xee40,0x073c,
0x4063,0xf834,0x5ba1,0xfddf,
0x4051,0xa24f,0x4f4c,0xea4f,
0x402c,0x0d8d,0x5c2a,0x0f4d,
0x3ff0,0x0000,0x0000,0x0000,
};
static unsigned short AD[32] = {
0x3fe2,0x29bc,0x0262,0x4d31,
0x402d,0x8334,0x0533,0x2ca5,
0x4055,0x20e3,0xb04d,0x51a0,
0x4066,0x2a2d,0xc730,0xb7b0,
0x4064,0x8782,0x9a9f,0xfa61,
0x4051,0xde94,0xee91,0xd35f,
0x402c,0x311b,0x950d,0x9d81,
0x3ff0,0x0000,0x0000,0x0000,
};
#endif

#ifdef UNK
static double APN[8] = {
  6.13759184814035759225E-1,
  1.47454670787755323881E1,
  8.20584123476060982430E1,
  1.71184781360976385540E2,
  1.59317847137141783523E2,
  6.99778599330103016170E1,
  1.39470856980481566958E1,
  1.00000000000000000550E0,
};
static double APD[8] = {
  3.34203677749736953049E-1,
  1.11810297306158156705E1,
  7.11727352147859965283E1,
  1.58778084372838313640E2,
  1.53206427475809220834E2,
  6.86752304592780337944E1,
  1.38498634758259442477E1,
  9.99999999999999994502E-1,
};
#endif
#ifdef DEC
static unsigned short APN[32] = {
0040035,0017522,0065145,0054755,
0041153,0166556,0161471,0057174,
0041644,0016750,0034445,0046462,
0042053,0027515,0152316,0046717,
0042037,0050536,0067023,0023264,
0041613,0172252,0007240,0131055,
0041137,0023503,0052472,0002305,
0040200,0000000,0000000,0000000,
};
static unsigned short APD[32] = {
0037653,0016276,0112106,0126625,
0041062,0162577,0067111,0111761,
0041616,0054160,0140004,0137455,
0042036,0143460,0104626,0157206,
0042031,0032330,0067131,0114260,
0041611,0054667,0147207,0134564,
0041135,0114412,0070653,0146015,
0040200,0000000,0000000,0000000,
};
#endif
#ifdef IBMPC
static unsigned short APN[32] = {
0xab3e,0x4d4c,0xa3ea,0x3fe3,
0x2bcf,0xdc67,0x7dad,0x402d,
0xa9a6,0x0724,0x83bd,0x4054,
0xc9ba,0xba99,0x65e9,0x4065,
0x64d7,0xcdc2,0xea2b,0x4063,
0x1646,0x41d4,0x7e95,0x4051,
0x4099,0x6aa7,0xe4e8,0x402b,
0x0000,0x0000,0x0000,0x3ff0,
};
static unsigned short APD[32] = {
0xd5b3,0xd288,0x6397,0x3fd5,
0x327e,0xedc9,0x5caf,0x4026,
0x97e6,0x1800,0xcb0e,0x4051,
0xdbd1,0x1132,0xd8e6,0x4063,
0x3316,0x0dcb,0x269b,0x4063,
0xf72f,0xf9d0,0x2b36,0x4051,
0x7982,0x4e35,0xb321,0x402b,
0x0000,0x0000,0x0000,0x3ff0,
};
#endif
#ifdef MIEEE
static unsigned short APN[32] = {
0x3fe3,0xa3ea,0x4d4c,0xab3e,
0x402d,0x7dad,0xdc67,0x2bcf,
0x4054,0x83bd,0x0724,0xa9a6,
0x4065,0x65e9,0xba99,0xc9ba,
0x4063,0xea2b,0xcdc2,0x64d7,
0x4051,0x7e95,0x41d4,0x1646,
0x402b,0xe4e8,0x6aa7,0x4099,
0x3ff0,0x0000,0x0000,0x0000,
};
static unsigned short APD[32] = {
0x3fd5,0x6397,0xd288,0xd5b3,
0x4026,0x5caf,0xedc9,0x327e,
0x4051,0xcb0e,0x1800,0x97e6,
0x4063,0xd8e6,0x1132,0xdbd1,
0x4063,0x269b,0x0dcb,0x3316,
0x4051,0x2b36,0xf9d0,0xf72f,
0x402b,0xb321,0x4e35,0x7982,
0x3ff0,0x0000,0x0000,0x0000,
};
#endif

#ifdef UNK
static double BN16[5] = {
-2.53240795869364152689E-1,
 5.75285167332467384228E-1,
-3.29907036873225371650E-1,
 6.44404068948199951727E-2,
-3.82519546641336734394E-3,
};
static double BD16[5] = {
/* 1.00000000000000000000E0,*/
-7.15685095054035237902E0,
 1.06039580715664694291E1,
-5.23246636471251500874E0,
 9.57395864378383833152E-1,
-5.50828147163549611107E-2,
};
#endif
#ifdef DEC
static unsigned short BN16[20] = {
0137601,0124307,0010213,0035210,
0040023,0042743,0101621,0016031,
0137650,0164623,0036056,0074511,
0037203,0174525,0000473,0142474,
0136172,0130041,0066726,0064324,
};
static unsigned short BD16[20] = {
/*0040200,0000000,0000000,0000000,*/
0140745,0002354,0044335,0055276,
0041051,0124717,0170130,0104013,
0140647,0070135,0046473,0103501,
0040165,0013745,0033324,0127766,
0137141,0117204,0076164,0033107,
};
#endif
#ifdef IBMPC
static unsigned short BN16[20] = {
0x6751,0xe211,0x3518,0xbfd0,
0x2383,0x7072,0x68bc,0x3fe2,
0xcf29,0x6785,0x1d32,0xbfd5,
0x78a8,0xa027,0x7f2a,0x3fb0,
0xcd1b,0x2dba,0x5604,0xbf6f,
};
static unsigned short BD16[20] = {
/*0x0000,0x0000,0x0000,0x3ff0,*/
0xab58,0x891b,0xa09d,0xc01c,
0x1101,0xfe0b,0x3539,0x4025,
0x70e8,0xa9a7,0xee0b,0xc014,
0x95ff,0xa6da,0xa2fc,0x3fee,
0x86c9,0x8f8e,0x33d0,0xbfac,
};
#endif
#ifdef MIEEE
static unsigned short BN16[20] = {
0xbfd0,0x3518,0xe211,0x6751,
0x3fe2,0x68bc,0x7072,0x2383,
0xbfd5,0x1d32,0x6785,0xcf29,
0x3fb0,0x7f2a,0xa027,0x78a8,
0xbf6f,0x5604,0x2dba,0xcd1b,
};
static unsigned short BD16[20] = {
/*0x3ff0,0x0000,0x0000,0x0000,*/
0xc01c,0xa09d,0x891b,0xab58,
0x4025,0x3539,0xfe0b,0x1101,
0xc014,0xee0b,0xa9a7,0x70e8,
0x3fee,0xa2fc,0xa6da,0x95ff,
0xbfac,0x33d0,0x8f8e,0x86c9,
};
#endif

#ifdef UNK
static double BPPN[5] = {
 4.65461162774651610328E-1,
-1.08992173800493920734E0,
 6.38800117371827987759E-1,
-1.26844349553102907034E-1,
 7.62487844342109852105E-3,
};
static double BPPD[5] = {
/* 1.00000000000000000000E0,*/
-8.70622787633159124240E0,
 1.38993162704553213172E1,
-7.14116144616431159572E0,
 1.34008595960680518666E0,
-7.84273211323341930448E-2,
};
#endif
#ifdef DEC
static unsigned short BPPN[20] = {
0037756,0050354,0167531,0135731,
0140213,0101216,0032767,0020375,
0040043,0104147,0106312,0177632,
0137401,0161574,0032015,0043714,
0036371,0155035,0143165,0142262,
};
static unsigned short BPPD[20] = {
/*0040200,0000000,0000000,0000000,*/
0141013,0046265,0115005,0161053,
0041136,0061631,0072445,0156131,
0140744,0102145,0001127,0065304,
0040253,0103757,0146453,0102513,
0137240,0117200,0155402,0113500,
};
#endif
#ifdef IBMPC
static unsigned short BPPN[20] = {
0x377b,0x9deb,0xca1d,0x3fdd,
0xe420,0xc6be,0x7051,0xbff1,
0x5ff3,0xf199,0x710c,0x3fe4,
0xa8fa,0x8681,0x3c6f,0xbfc0,
0xb896,0xb8ce,0x3b43,0x3f7f,
};
static unsigned short BPPD[20] = {
/*0x0000,0x0000,0x0000,0x3ff0,*/
0xbc45,0xb340,0x6996,0xc021,
0xbb8b,0x2ea4,0xcc73,0x402b,
0xed59,0xa04a,0x908c,0xc01c,
0x70a9,0xf9a5,0x70fd,0x3ff5,
0x52e8,0x1b60,0x13d0,0xbfb4,
};
#endif
#ifdef MIEEE
static unsigned short BPPN[20] = {
0x3fdd,0xca1d,0x9deb,0x377b,
0xbff1,0x7051,0xc6be,0xe420,
0x3fe4,0x710c,0xf199,0x5ff3,
0xbfc0,0x3c6f,0x8681,0xa8fa,
0x3f7f,0x3b43,0xb8ce,0xb896,
};
static unsigned short BPPD[20] = {
/*0x3ff0,0x0000,0x0000,0x0000,*/
0xc021,0x6996,0xb340,0xbc45,
0x402b,0xcc73,0x2ea4,0xbb8b,
0xc01c,0x908c,0xa04a,0xed59,
0x3ff5,0x70fd,0xf9a5,0x70a9,
0xbfb4,0x13d0,0x1b60,0x52e8,
};
#endif

#ifdef UNK
static double AFN[9] = {
-1.31696323418331795333E-1,
-6.26456544431912369773E-1,
-6.93158036036933542233E-1,
-2.79779981545119124951E-1,
-4.91900132609500318020E-2,
-4.06265923594885404393E-3,
-1.59276496239262096340E-4,
-2.77649108155232920844E-6,
-1.67787698489114633780E-8,
};
static double AFD[9] = {
/* 1.00000000000000000000E0,*/
 1.33560420706553243746E1,
 3.26825032795224613948E1,
 2.67367040941499554804E1,
 9.18707402907259625840E0,
 1.47529146771666414581E0,
 1.15687173795188044134E-1,
 4.40291641615211203805E-3,
 7.54720348287414296618E-5,
 4.51850092970580378464E-7,
};
#endif
#ifdef DEC
static unsigned short AFN[36] = {
0137406,0155546,0124127,0033732,
0140040,0057564,0141263,0041222,
0140061,0071316,0013674,0175754,
0137617,0037522,0056637,0120130,
0137111,0075567,0121755,0166122,
0136205,0020016,0043317,0002201,
0135047,0001565,0075130,0002334,
0133472,0051700,0165021,0131551,
0131620,0020347,0132165,0013215,
};
static unsigned short AFD[36] = {
/*0040200,0000000,0000000,0000000,*/
0041125,0131131,0025627,0067623,
0041402,0135342,0021703,0154315,
0041325,0162305,0016671,0120175,
0041022,0177101,0053114,0141632,
0040274,0153131,0147364,0114306,
0037354,0166545,0120042,0150530,
0036220,0043127,0000727,0130273,
0034636,0043275,0075667,0034733,
0032762,0112715,0146250,0142474,
};
#endif
#ifdef IBMPC
static unsigned short AFN[36] = {
0xe6fb,0xd50a,0xdb6c,0xbfc0,
0x6852,0x9856,0x0bee,0xbfe4,
0x9f7d,0xc2f7,0x2e59,0xbfe6,
0xf40b,0x4bb3,0xe7ea,0xbfd1,
0xbd8a,0xf47d,0x2f6e,0xbfa9,
0xe090,0xc8d9,0xa401,0xbf70,
0x009c,0xaf4b,0xe06e,0xbf24,
0x366d,0x1d42,0x4a78,0xbec7,
0xa2d2,0xf68e,0x041c,0xbe52,
};
static unsigned short AFD[36] = {
/*0x0000,0x0000,0x0000,0x3ff0,*/
0xedf2,0x2572,0xb64b,0x402a,
0x7b1a,0x4478,0x575c,0x4040,
0x3410,0xa3b7,0xbc98,0x403a,
0x9873,0x2ac9,0x5fc8,0x4022,
0x9319,0x39de,0x9acb,0x3ff7,
0x5a2b,0xb404,0x9dac,0x3fbd,
0xf617,0xe03a,0x08ca,0x3f72,
0xe73b,0xaf76,0xc8d7,0x3f13,
0x18a7,0xb995,0x52b9,0x3e9e,
};
#endif
#ifdef MIEEE
static unsigned short AFN[36] = {
0xbfc0,0xdb6c,0xd50a,0xe6fb,
0xbfe4,0x0bee,0x9856,0x6852,
0xbfe6,0x2e59,0xc2f7,0x9f7d,
0xbfd1,0xe7ea,0x4bb3,0xf40b,
0xbfa9,0x2f6e,0xf47d,0xbd8a,
0xbf70,0xa401,0xc8d9,0xe090,
0xbf24,0xe06e,0xaf4b,0x009c,
0xbec7,0x4a78,0x1d42,0x366d,
0xbe52,0x041c,0xf68e,0xa2d2,
};
static unsigned short AFD[36] = {
/*0x3ff0,0x0000,0x0000,0x0000,*/
0x402a,0xb64b,0x2572,0xedf2,
0x4040,0x575c,0x4478,0x7b1a,
0x403a,0xbc98,0xa3b7,0x3410,
0x4022,0x5fc8,0x2ac9,0x9873,
0x3ff7,0x9acb,0x39de,0x9319,
0x3fbd,0x9dac,0xb404,0x5a2b,
0x3f72,0x08ca,0xe03a,0xf617,
0x3f13,0xc8d7,0xaf76,0xe73b,
0x3e9e,0x52b9,0xb995,0x18a7,
};
#endif

#ifdef UNK
static double AGN[11] = {
  1.97339932091685679179E-2,
  3.91103029615688277255E-1,
  1.06579897599595591108E0,
  9.39169229816650230044E-1,
  3.51465656105547619242E-1,
  6.33888919628925490927E-2,
  5.85804113048388458567E-3,
  2.82851600836737019778E-4,
  6.98793669997260967291E-6,
  8.11789239554389293311E-8,
  3.41551784765923618484E-10,
};
static double AGD[10] = {
/*  1.00000000000000000000E0,*/
  9.30892908077441974853E0,
  1.98352928718312140417E1,
  1.55646628932864612953E1,
  5.47686069422975497931E0,
  9.54293611618961883998E-1,
  8.64580826352392193095E-2,
  4.12656523824222607191E-3,
  1.01259085116509135510E-4,
  1.17166733214413521882E-6,
  4.91834570062930015649E-9,
};
#endif
#ifdef DEC
static unsigned short AGN[44] = {
0036641,0124456,0167175,0157354,
0037710,0037250,0001441,0136671,
0040210,0066031,0150401,0123532,
0040160,0066545,0003570,0153133,
0037663,0171516,0072507,0170345,
0037201,0151011,0007510,0045702,
0036277,0172317,0104572,0101030,
0035224,0045663,0000160,0136422,
0033752,0074753,0047702,0135160,
0032256,0052225,0156550,0107103,
0030273,0142443,0166277,0071720,
};
static unsigned short AGD[40] = {
/*0040200,0000000,0000000,0000000,*/
0041024,0170537,0117253,0055003,
0041236,0127256,0003570,0143240,
0041171,0004333,0172476,0160645,
0040657,0041161,0055716,0157161,
0040164,0046226,0006257,0063431,
0037261,0010357,0065445,0047563,
0036207,0034043,0057434,0116732,
0034724,0055416,0130035,0026377,
0033235,0041056,0154071,0023502,
0031250,0177071,0167254,0047242,
};
#endif
#ifdef IBMPC
static unsigned short AGN[44] = {
0xbbde,0xddcf,0x3525,0x3f94,
0x37b7,0x0064,0x07d5,0x3fd9,
0x34eb,0x3a20,0x0d83,0x3ff1,
0x1acb,0xa0ef,0x0dac,0x3fee,
0xfe1d,0xcea8,0x7e69,0x3fd6,
0x0978,0x21e9,0x3a41,0x3fb0,
0x5043,0xf12f,0xfe99,0x3f77,
0x17a2,0x600e,0x8976,0x3f32,
0x574e,0x69f8,0x4f3d,0x3edd,
0x11c8,0xbbad,0xca92,0x3e75,
0xee7a,0x7d97,0x78a4,0x3df7,
};
static unsigned short AGD[40] = {
/*0x0000,0x0000,0x0000,0x3ff0,*/
0x6b40,0xf3d5,0x9e2b,0x4022,
0x18d4,0xc0ef,0xd5d5,0x4033,
0xdc35,0x7ea7,0x211b,0x402f,
0xdbce,0x2b79,0xe84e,0x4015,
0xece3,0xc195,0x8992,0x3fee,
0xa9ee,0xed64,0x221d,0x3fb6,
0x93bb,0x6be3,0xe704,0x3f70,
0xa5a0,0xd603,0x8b61,0x3f1a,
0x24e8,0xdb07,0xa845,0x3eb3,
0x89d4,0x3dd5,0x1fc7,0x3e35,
};
#endif
#ifdef MIEEE
static unsigned short AGN[44] = {
0x3f94,0x3525,0xddcf,0xbbde,
0x3fd9,0x07d5,0x0064,0x37b7,
0x3ff1,0x0d83,0x3a20,0x34eb,
0x3fee,0x0dac,0xa0ef,0x1acb,
0x3fd6,0x7e69,0xcea8,0xfe1d,
0x3fb0,0x3a41,0x21e9,0x0978,
0x3f77,0xfe99,0xf12f,0x5043,
0x3f32,0x8976,0x600e,0x17a2,
0x3edd,0x4f3d,0x69f8,0x574e,
0x3e75,0xca92,0xbbad,0x11c8,
0x3df7,0x78a4,0x7d97,0xee7a,
};
static unsigned short AGD[40] = {
/*0x3ff0,0x0000,0x0000,0x0000,*/
0x4022,0x9e2b,0xf3d5,0x6b40,
0x4033,0xd5d5,0xc0ef,0x18d4,
0x402f,0x211b,0x7ea7,0xdc35,
0x4015,0xe84e,0x2b79,0xdbce,
0x3fee,0x8992,0xc195,0xece3,
0x3fb6,0x221d,0xed64,0xa9ee,
0x3f70,0xe704,0x6be3,0x93bb,
0x3f1a,0x8b61,0xd603,0xa5a0,
0x3eb3,0xa845,0xdb07,0x24e8,
0x3e35,0x1fc7,0x3dd5,0x89d4,
};
#endif

#ifdef UNK
static double APFN[9] = {
  1.85365624022535566142E-1,
  8.86712188052584095637E-1,
  9.87391981747398547272E-1,
  4.01241082318003734092E-1,
  7.10304926289631174579E-2,
  5.90618657995661810071E-3,
  2.33051409401776799569E-4,
  4.08718778289035454598E-6,
  2.48379932900442457853E-8,
};
static double APFD[9] = {
/*  1.00000000000000000000E0,*/
  1.47345854687502542552E1,
  3.75423933435489594466E1,
  3.14657751203046424330E1,
  1.09969125207298778536E1,
  1.78885054766999417817E0,
  1.41733275753662636873E-1,
  5.44066067017226003627E-3,
  9.39421290654511171663E-5,
  5.65978713036027009243E-7,
};
#endif
#ifdef DEC
static unsigned short APFN[36] = {
0037475,0150174,0071752,0166651,
0040142,0177621,0164246,0101757,
0040174,0142670,0106760,0006573,
0037715,0067570,0116274,0022404,
0037221,0074157,0053341,0117207,
0036301,0104257,0015075,0004777,
0035164,0057502,0164034,0001313,
0033611,0022254,0176000,0112565,
0031725,0055523,0025153,0166057,
};
static unsigned short APFD[36] = {
/*0040200,0000000,0000000,0000000,*/
0041153,0140334,0130506,0061402,
0041426,0025551,0024440,0070611,
0041373,0134750,0047147,0176702,
0041057,0171532,0105430,0017674,
0040344,0174416,0001726,0047754,
0037421,0021207,0020167,0136264,
0036262,0043621,0151321,0124324,
0034705,0001313,0163733,0016407,
0033027,0166702,0150440,0170561,
};
#endif
#ifdef IBMPC
static unsigned short APFN[36] = {
0x5db5,0x8e7d,0xba0f,0x3fc7,
0xd07e,0x3d14,0x5ff2,0x3fec,
0x01af,0x11be,0x98b7,0x3fef,
0x84a1,0x1397,0xadef,0x3fd9,
0x33d1,0xeadc,0x2f0d,0x3fb2,
0xa140,0xe347,0x3115,0x3f78,
0x8059,0x5d03,0x8be8,0x3f2e,
0x12af,0x9f80,0x2495,0x3ed1,
0x7d86,0x654d,0xab6a,0x3e5a,
};
static unsigned short APFD[36] = {
/*0x0000,0x0000,0x0000,0x3ff0,*/
0xcc60,0x9628,0x781b,0x402d,
0x0e31,0x2524,0xc56d,0x4042,
0xffb8,0x09cc,0x773d,0x403f,
0x03f7,0x5163,0xfe6b,0x4025,
0xc9fd,0xc07a,0x9f21,0x3ffc,
0xf796,0xe40e,0x2450,0x3fc2,
0x351a,0x3a5a,0x48f2,0x3f76,
0x63a1,0x7cfb,0xa059,0x3f18,
0x1e2e,0x5a24,0xfdb8,0x3ea2,
};
#endif
#ifdef MIEEE
static unsigned short APFN[36] = {
0x3fc7,0xba0f,0x8e7d,0x5db5,
0x3fec,0x5ff2,0x3d14,0xd07e,
0x3fef,0x98b7,0x11be,0x01af,
0x3fd9,0xadef,0x1397,0x84a1,
0x3fb2,0x2f0d,0xeadc,0x33d1,
0x3f78,0x3115,0xe347,0xa140,
0x3f2e,0x8be8,0x5d03,0x8059,
0x3ed1,0x2495,0x9f80,0x12af,
0x3e5a,0xab6a,0x654d,0x7d86,
};
static unsigned short APFD[36] = {
/*0x3ff0,0x0000,0x0000,0x0000,*/
0x402d,0x781b,0x9628,0xcc60,
0x4042,0xc56d,0x2524,0x0e31,
0x403f,0x773d,0x09cc,0xffb8,
0x4025,0xfe6b,0x5163,0x03f7,
0x3ffc,0x9f21,0xc07a,0xc9fd,
0x3fc2,0x2450,0xe40e,0xf796,
0x3f76,0x48f2,0x3a5a,0x351a,
0x3f18,0xa059,0x7cfb,0x63a1,
0x3ea2,0xfdb8,0x5a24,0x1e2e,
};
#endif

#ifdef UNK
static double APGN[11] = {
-3.55615429033082288335E-2,
-6.37311518129435504426E-1,
-1.70856738884312371053E0,
-1.50221872117316635393E0,
-5.63606665822102676611E-1,
-1.02101031120216891789E-1,
-9.48396695961445269093E-3,
-4.60325307486780994357E-4,
-1.14300836484517375919E-5,
-1.33415518685547420648E-7,
-5.63803833958893494476E-10,
};
static double APGD[11] = {
/*  1.00000000000000000000E0,*/
  9.85865801696130355144E0,
  2.16401867356585941885E1,
  1.73130776389749389525E1,
  6.17872175280828766327E0,
  1.08848694396321495475E0,
  9.95005543440888479402E-2,
  4.78468199683886610842E-3,
  1.18159633322838625562E-4,
  1.37480673554219441465E-6,
  5.79912514929147598821E-9,
};
#endif
#ifdef DEC
static unsigned short APGN[44] = {
0137021,0124372,0176075,0075331,
0140043,0023330,0177672,0161655,
0140332,0131126,0010413,0171112,
0140300,0044263,0175560,0054070,
0140020,0044206,0142603,0073324,
0137321,0015130,0066144,0144033,
0136433,0061243,0175542,0103373,
0135361,0053721,0020441,0053203,
0134077,0141725,0160277,0130612,
0132417,0040372,0100363,0060200,
0130432,0175052,0171064,0034147,
};
static unsigned short APGD[40] = {
/*0040200,0000000,0000000,0000000,*/
0041035,0136420,0030124,0140220,
0041255,0017432,0034447,0162256,
0041212,0100456,0154544,0006321,
0040705,0134026,0127154,0123414,
0040213,0051612,0044470,0172607,
0037313,0143362,0053273,0157051,
0036234,0144322,0054536,0007264,
0034767,0146170,0054265,0170342,
0033270,0102777,0167362,0073631,
0031307,0040644,0167103,0021763,
};
#endif
#ifdef IBMPC
static unsigned short APGN[44] = {
0xaf5b,0x5f87,0x351f,0xbfa2,
0x5c76,0x1ff7,0x64db,0xbfe4,
0x7e49,0xc221,0x564a,0xbffb,
0x0b07,0x7f6e,0x0916,0xbff8,
0x6edb,0xd8b0,0x0910,0xbfe2,
0x9903,0x0d8c,0x234b,0xbfba,
0x50df,0x7f6c,0x6c54,0xbf83,
0x2ad0,0x2424,0x2afa,0xbf3e,
0xf631,0xbc17,0xf87a,0xbee7,
0x6c10,0x501e,0xe81f,0xbe81,
0x870d,0x5e46,0x5f45,0xbe03,
};
static unsigned short APGD[40] = {
/*0x0000,0x0000,0x0000,0x3ff0,*/
0x9812,0x060a,0xb7a2,0x4023,
0xfc96,0x4724,0xa3e3,0x4035,
0x819a,0xdb2c,0x5025,0x4031,
0x94e2,0xd5cd,0xb702,0x4018,
0x1eb1,0x4927,0x6a71,0x3ff1,
0x7bc5,0x4ad7,0x78de,0x3fb9,
0xc1d7,0x4b2b,0x991a,0x3f73,
0xbe1c,0x0b16,0xf98f,0x3f1e,
0x4ef3,0xfdde,0x10bf,0x3eb7,
0x647e,0x9dc8,0xe834,0x3e38,
};
#endif
#ifdef MIEEE
static unsigned short APGN[44] = {
0xbfa2,0x351f,0x5f87,0xaf5b,
0xbfe4,0x64db,0x1ff7,0x5c76,
0xbffb,0x564a,0xc221,0x7e49,
0xbff8,0x0916,0x7f6e,0x0b07,
0xbfe2,0x0910,0xd8b0,0x6edb,
0xbfba,0x234b,0x0d8c,0x9903,
0xbf83,0x6c54,0x7f6c,0x50df,
0xbf3e,0x2afa,0x2424,0x2ad0,
0xbee7,0xf87a,0xbc17,0xf631,
0xbe81,0xe81f,0x501e,0x6c10,
0xbe03,0x5f45,0x5e46,0x870d,
};
static unsigned short APGD[40] = {
/*0x3ff0,0x0000,0x0000,0x0000,*/
0x4023,0xb7a2,0x060a,0x9812,
0x4035,0xa3e3,0x4724,0xfc96,
0x4031,0x5025,0xdb2c,0x819a,
0x4018,0xb702,0xd5cd,0x94e2,
0x3ff1,0x6a71,0x4927,0x1eb1,
0x3fb9,0x78de,0x4ad7,0x7bc5,
0x3f73,0x991a,0x4b2b,0xc1d7,
0x3f1e,0xf98f,0x0b16,0xbe1c,
0x3eb7,0x10bf,0xfdde,0x4ef3,
0x3e38,0xe834,0x9dc8,0x647e,
};
#endif

#ifdef ANSIPROT
extern double torch_cephes_fabs ( double );
extern double torch_cephes_exp ( double );
extern double torch_cephes_sqrt ( double );
extern double torch_cephes_polevl ( double, void *, int );
extern double torch_cephes_p1evl ( double, void *, int );
extern double torch_cephes_sin ( double );
extern double torch_cephes_cos ( double );
#else
double torch_cephes_fabs(), torch_cephes_exp(), torch_cephes_sqrt();
double torch_cephes_polevl(), torch_cephes_p1evl(),
    torch_cephes_sin(), torch_cephes_cos();
#endif

int torch_cephes_airy( x, ai, aip, bi, bip )
double x, *ai, *aip, *bi, *bip;
{
double z, zz, t, f, g, uf, ug, k, zeta, theta;
int domflg;

domflg = 0;
if( x > MAXAIRY )
	{
	*ai = 0;
	*aip = 0;
	*bi = torch_cephes_MAXNUM;
	*bip = torch_cephes_MAXNUM;
	return(-1);
	}

if( x < -2.09 )
	{
	domflg = 15;
	t = torch_cephes_sqrt(-x);
	zeta = -2.0 * x * t / 3.0;
	t = torch_cephes_sqrt(t);
	k = sqpii / t;
	z = 1.0/zeta;
	zz = z * z;
	uf = 1.0 + zz * torch_cephes_polevl( zz, AFN, 8 ) /
            torch_cephes_p1evl( zz, AFD, 9 );
	ug = z * torch_cephes_polevl( zz, AGN, 10 ) /
            torch_cephes_p1evl( zz, AGD, 10 );
	theta = zeta + 0.25 * torch_cephes_PI;
	f = torch_cephes_sin( theta );
	g = torch_cephes_cos( theta );
	*ai = k * (f * uf - g * ug);
	*bi = k * (g * uf + f * ug);
	uf = 1.0 + zz * torch_cephes_polevl( zz, APFN, 8 ) /
            torch_cephes_p1evl( zz, APFD, 9 );
	ug = z * torch_cephes_polevl( zz, APGN, 10 ) /
            torch_cephes_p1evl( zz, APGD, 10 );
	k = sqpii * t;
	*aip = -k * (g * uf + f * ug);
	*bip = k * (f * uf - g * ug);
	return(0);
	}

if( x >= 2.09 )	/* cbrt(9) */
	{
	domflg = 5;
	t = torch_cephes_sqrt(x);
	zeta = 2.0 * x * t / 3.0;
	g = torch_cephes_exp( zeta );
	t = torch_cephes_sqrt(t);
	k = 2.0 * t * g;
	z = 1.0/zeta;
	f = torch_cephes_polevl( z, AN, 7 ) / torch_cephes_polevl( z, AD, 7 );
	*ai = sqpii * f / k;
	k = -0.5 * sqpii * t / g;
	f = torch_cephes_polevl( z, APN, 7 ) / torch_cephes_polevl( z, APD, 7 );
	*aip = f * k;

	if( x > 8.3203353 )	/* zeta > 16 */
		{
		f = z * torch_cephes_polevl( z, BN16, 4 ) /
                    torch_cephes_p1evl( z, BD16, 5 );
		k = sqpii * g;
		*bi = k * (1.0 + f) / t;
		f = z * torch_cephes_polevl( z, BPPN, 4 ) /
                    torch_cephes_p1evl( z, BPPD, 5 );
		*bip = k * t * (1.0 + f);
		return(0);
		}
	}

f = 1.0;
g = x;
t = 1.0;
uf = 1.0;
ug = x;
k = 1.0;
z = x * x * x;
while( t > torch_cephes_MACHEP )
	{
	uf *= z;
	k += 1.0;
	uf /=k;
	ug *= z;
	k += 1.0;
	ug /=k;
	uf /=k;
	f += uf;
	k += 1.0;
	ug /=k;
	g += ug;
	t = torch_cephes_fabs(uf/f);
	}
uf = c1 * f;
ug = c2 * g;
if( (domflg & 1) == 0 )
	*ai = uf - ug;
if( (domflg & 2) == 0 )
	*bi = sqrt3 * (uf + ug);

/* the deriviative of ai */
k = 4.0;
uf = x * x/2.0;
ug = z/3.0;
f = uf;
g = 1.0 + ug;
uf /= 3.0;
t = 1.0;

while( t > torch_cephes_MACHEP )
	{
	uf *= z;
	ug /=k;
	k += 1.0;
	ug *= z;
	uf /=k;
	f += uf;
	k += 1.0;
	ug /=k;
	uf /=k;
	g += ug;
	k += 1.0;
	t = torch_cephes_fabs(ug/g);
	}

uf = c1 * f;
ug = c2 * g;
if( (domflg & 4) == 0 )
	*aip = uf - ug;
if( (domflg & 8) == 0 )
	*bip = sqrt3 * (uf + ug);
return(0);
}
