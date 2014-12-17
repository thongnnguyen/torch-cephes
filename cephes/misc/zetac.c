 /*							zetac.c
 *
 *	Riemann zeta function
 *
 *
 *
 * SYNOPSIS:
 *
 * double x, y, zetac();
 *
 * y = zetac( x );
 *
 *
 *
 * DESCRIPTION:
 *
 *
 *
 *                inf.
 *                 -    -x
 *   zetac(x)  =   >   k   ,   x > 1,
 *                 -
 *                k=2
 *
 * is related to the Riemann zeta function by
 *
 *	Riemann zeta(x) = zetac(x) + 1.
 *
 * Extension of the function definition for x < 1 is implemented.
 * Zero is returned for x > log2(MAXNUM).
 *
 * An overflow error may occur for large negative x, due to the
 * gamma function in the reflection formula.
 *
 * ACCURACY:
 *
 * Tabulated values have full machine accuracy.
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      1,50        10000       9.8e-16	    1.3e-16
 *    DEC       1,50         2000       1.1e-16     1.9e-17
 *
 *
 */

/*
Cephes Math Library Release 2.8:  June, 2000
Copyright 1984, 1987, 1989, 2000 by Stephen L. Moshier
*/

#include "mconf.h"

extern double torch_cephes_MAXNUM, torch_cephes_PI;

/* Riemann zeta(x) - 1
 * for integer arguments between 0 and 30.
 */
#ifdef UNK
static double azetac[] = {
-1.50000000000000000000E0,
 1.70141183460469231730E38, /* infinity. */
 6.44934066848226436472E-1,
 2.02056903159594285400E-1,
 8.23232337111381915160E-2,
 3.69277551433699263314E-2,
 1.73430619844491397145E-2,
 8.34927738192282683980E-3,
 4.07735619794433937869E-3,
 2.00839282608221441785E-3,
 9.94575127818085337146E-4,
 4.94188604119464558702E-4,
 2.46086553308048298638E-4,
 1.22713347578489146752E-4,
 6.12481350587048292585E-5,
 3.05882363070204935517E-5,
 1.52822594086518717326E-5,
 7.63719763789976227360E-6,
 3.81729326499983985646E-6,
 1.90821271655393892566E-6,
 9.53962033872796113152E-7,
 4.76932986787806463117E-7,
 2.38450502727732990004E-7,
 1.19219925965311073068E-7,
 5.96081890512594796124E-8,
 2.98035035146522801861E-8,
 1.49015548283650412347E-8,
 7.45071178983542949198E-9,
 3.72533402478845705482E-9,
 1.86265972351304900640E-9,
 9.31327432419668182872E-10
};
#endif

#ifdef DEC
static unsigned short azetac[] = {
0140300,0000000,0000000,0000000,
0077777,0177777,0177777,0177777,
0040045,0015146,0022460,0076462,
0037516,0164001,0036001,0104116,
0037250,0114425,0061754,0022033,
0037027,0040616,0145174,0146670,
0036616,0011411,0100444,0104437,
0036410,0145550,0051474,0161067,
0036205,0115527,0141434,0133506,
0036003,0117475,0100553,0053403,
0035602,0056147,0045567,0027703,
0035401,0106157,0111054,0145242,
0035201,0002455,0113151,0101015,
0035000,0126235,0004273,0157260,
0034600,0071127,0112647,0005261,
0034400,0045736,0057610,0157550,
0034200,0031146,0172621,0074172,
0034000,0020603,0115503,0032007,
0033600,0013114,0124672,0023135,
0033400,0007330,0043715,0151117,
0033200,0004742,0145043,0033514,
0033000,0003225,0152624,0004411,
0032600,0002143,0033166,0035746,
0032400,0001354,0074234,0026143,
0032200,0000762,0147776,0170220,
0032000,0000514,0072452,0130631,
0031600,0000335,0114266,0063315,
0031400,0000223,0132710,0041045,
0031200,0000142,0073202,0153426,
0031000,0000101,0121400,0152065,
0030600,0000053,0140525,0072761
};
#endif

#ifdef IBMPC
static unsigned short azetac[] = {
0x0000,0x0000,0x0000,0xbff8,
0xffff,0xffff,0xffff,0x7fef,
0x0fa6,0xc4a6,0xa34c,0x3fe4,
0x310a,0x2780,0xdd00,0x3fc9,
0x8483,0xac7d,0x1322,0x3fb5,
0x99b7,0xd94f,0xe831,0x3fa2,
0x9124,0x3024,0xc261,0x3f91,
0x9c47,0x0a67,0x196d,0x3f81,
0x96e9,0xf863,0xb36a,0x3f70,
0x6ae0,0xb02d,0x73e7,0x3f60,
0xe5f8,0xe96e,0x4b8c,0x3f50,
0x9954,0xf245,0x318d,0x3f40,
0x3042,0xb2cd,0x20a5,0x3f30,
0x7bd6,0xa117,0x1593,0x3f20,
0xe156,0xf2b4,0x0e4a,0x3f10,
0x1bed,0xcbf1,0x097b,0x3f00,
0x2f0f,0xdeb2,0x064c,0x3ef0,
0x6681,0x7368,0x0430,0x3ee0,
0x44cc,0x9537,0x02c9,0x3ed0,
0xba4a,0x08f9,0x01db,0x3ec0,
0x66ea,0x5944,0x013c,0x3eb0,
0x8121,0xbab2,0x00d2,0x3ea0,
0xc77d,0x66ce,0x008c,0x3e90,
0x858c,0x8f13,0x005d,0x3e80,
0xde12,0x59ff,0x003e,0x3e70,
0x5633,0x8ea5,0x0029,0x3e60,
0xccda,0xb316,0x001b,0x3e50,
0x0845,0x76b9,0x0012,0x3e40,
0x5ae3,0x4ed0,0x000c,0x3e30,
0x1a87,0x3460,0x0008,0x3e20,
0xaebe,0x782a,0x0005,0x3e10
};
#endif

#ifdef MIEEE
static unsigned short azetac[] = {
0xbff8,0x0000,0x0000,0x0000,
0x7fef,0xffff,0xffff,0xffff,
0x3fe4,0xa34c,0xc4a6,0x0fa6,
0x3fc9,0xdd00,0x2780,0x310a,
0x3fb5,0x1322,0xac7d,0x8483,
0x3fa2,0xe831,0xd94f,0x99b7,
0x3f91,0xc261,0x3024,0x9124,
0x3f81,0x196d,0x0a67,0x9c47,
0x3f70,0xb36a,0xf863,0x96e9,
0x3f60,0x73e7,0xb02d,0x6ae0,
0x3f50,0x4b8c,0xe96e,0xe5f8,
0x3f40,0x318d,0xf245,0x9954,
0x3f30,0x20a5,0xb2cd,0x3042,
0x3f20,0x1593,0xa117,0x7bd6,
0x3f10,0x0e4a,0xf2b4,0xe156,
0x3f00,0x097b,0xcbf1,0x1bed,
0x3ef0,0x064c,0xdeb2,0x2f0f,
0x3ee0,0x0430,0x7368,0x6681,
0x3ed0,0x02c9,0x9537,0x44cc,
0x3ec0,0x01db,0x08f9,0xba4a,
0x3eb0,0x013c,0x5944,0x66ea,
0x3ea0,0x00d2,0xbab2,0x8121,
0x3e90,0x008c,0x66ce,0xc77d,
0x3e80,0x005d,0x8f13,0x858c,
0x3e70,0x003e,0x59ff,0xde12,
0x3e60,0x0029,0x8ea5,0x5633,
0x3e50,0x001b,0xb316,0xccda,
0x3e40,0x0012,0x76b9,0x0845,
0x3e30,0x000c,0x4ed0,0x5ae3,
0x3e20,0x0008,0x3460,0x1a87,
0x3e10,0x0005,0x782a,0xaebe
};
#endif


/* 2**x (1 - 1/x) (zeta(x) - 1) = P(1/x)/Q(1/x), 1 <= x <= 10 */
#ifdef UNK
static double P[9] = {
  5.85746514569725319540E11,
  2.57534127756102572888E11,
  4.87781159567948256438E10,
  5.15399538023885770696E9,
  3.41646073514754094281E8,
  1.60837006880656492731E7,
  5.92785467342109522998E5,
  1.51129169964938823117E4,
  2.01822444485997955865E2,
};
static double Q[8] = {
/*  1.00000000000000000000E0,*/
  3.90497676373371157516E11,
  5.22858235368272161797E10,
  5.64451517271280543351E9,
  3.39006746015350418834E8,
  1.79410371500126453702E7,
  5.66666825131384797029E5,
  1.60382976810944131506E4,
  1.96436237223387314144E2,
};
#endif
#ifdef DEC
static unsigned short P[36] = {
0052010,0060466,0101211,0134657,
0051557,0154353,0135060,0064411,
0051065,0133157,0133514,0133633,
0050231,0114735,0035036,0111344,
0047242,0164327,0146036,0033545,
0046165,0065364,0130045,0011005,
0045020,0134427,0075073,0134107,
0043554,0021653,0000440,0177426,
0042111,0151213,0134312,0021402,
};
static unsigned short Q[32] = {
/*0040200,0000000,0000000,0000000,*/
0051665,0153363,0054252,0137010,
0051102,0143645,0121415,0036107,
0050250,0034073,0131133,0036465,
0047241,0123250,0150037,0070012,
0046210,0160426,0111463,0116507,
0045012,0054255,0031674,0173612,
0043572,0114460,0151520,0012221,
0042104,0067655,0037037,0137421,
};
#endif
#ifdef IBMPC
static unsigned short P[36] = {
0x3736,0xd051,0x0c26,0x4261,
0x0d21,0x7746,0xfb1d,0x424d,
0x96f3,0xf6e9,0xb6cd,0x4226,
0xd25c,0xa743,0x333b,0x41f3,
0xc6ed,0xf983,0x5d1a,0x41b4,
0xa241,0x9604,0xad5e,0x416e,
0x7709,0xef47,0x1722,0x4122,
0x1fe3,0x6024,0x8475,0x40cd,
0x4460,0x7719,0x3a51,0x4069,
};
static unsigned short Q[32] = {
/*0x0000,0x0000,0x0000,0x3ff0,*/
0x57c1,0x6b15,0xbade,0x4256,
0xa789,0xb461,0x58f4,0x4228,
0x67a7,0x764b,0x0707,0x41f5,
0xee01,0x1a03,0x34d5,0x41b4,
0x73a9,0xd266,0x1c22,0x4171,
0x9ef1,0xa677,0x4b15,0x4121,
0x0292,0x1a6a,0x5326,0x40cf,
0xf7e2,0xa7c3,0x8df5,0x4068,
};
#endif
#ifdef MIEEE
static unsigned short P[36] = {
0x4261,0x0c26,0xd051,0x3736,
0x424d,0xfb1d,0x7746,0x0d21,
0x4226,0xb6cd,0xf6e9,0x96f3,
0x41f3,0x333b,0xa743,0xd25c,
0x41b4,0x5d1a,0xf983,0xc6ed,
0x416e,0xad5e,0x9604,0xa241,
0x4122,0x1722,0xef47,0x7709,
0x40cd,0x8475,0x6024,0x1fe3,
0x4069,0x3a51,0x7719,0x4460,
};
static unsigned short Q[32] = {
/*0x3ff0,0x0000,0x0000,0x0000,*/
0x4256,0xbade,0x6b15,0x57c1,
0x4228,0x58f4,0xb461,0xa789,
0x41f5,0x0707,0x764b,0x67a7,
0x41b4,0x34d5,0x1a03,0xee01,
0x4171,0x1c22,0xd266,0x73a9,
0x4121,0x4b15,0xa677,0x9ef1,
0x40cf,0x5326,0x1a6a,0x0292,
0x4068,0x8df5,0xa7c3,0xf7e2,
};
#endif

/* log(zeta(x) - 1 - 2**-x), 10 <= x <= 50 */
#ifdef UNK
static double A[11] = {
 8.70728567484590192539E6,
 1.76506865670346462757E8,
 2.60889506707483264896E10,
 5.29806374009894791647E11,
 2.26888156119238241487E13,
 3.31884402932705083599E14,
 5.13778997975868230192E15,
-1.98123688133907171455E15,
-9.92763810039983572356E16,
 7.82905376180870586444E16,
 9.26786275768927717187E16,
};
static double B[10] = {
/* 1.00000000000000000000E0,*/
-7.92625410563741062861E6,
-1.60529969932920229676E8,
-2.37669260975543221788E10,
-4.80319584350455169857E11,
-2.07820961754173320170E13,
-2.96075404507272223680E14,
-4.86299103694609136686E15,
 5.34589509675789930199E15,
 5.71464111092297631292E16,
-1.79915597658676556828E16,
};
#endif
#ifdef DEC
static unsigned short A[44] = {
0046004,0156325,0126302,0131567,
0047050,0052177,0015271,0136466,
0050702,0060271,0070727,0171112,
0051766,0132727,0064363,0145042,
0053245,0012466,0056000,0117230,
0054226,0166155,0174275,0170213,
0055222,0003127,0112544,0101322,
0154741,0036625,0010346,0053767,
0156260,0054653,0154052,0031113,
0056213,0011152,0021000,0007111,
0056244,0120534,0040576,0163262,
};
static unsigned short B[40] = {
/*0040200,0000000,0000000,0000000,*/
0145761,0161734,0033026,0015520,
0147031,0013743,0017355,0036703,
0150661,0011720,0061061,0136402,
0151737,0125216,0070274,0164414,
0153227,0032653,0127211,0145250,
0154206,0121666,0123774,0042035,
0155212,0033352,0125154,0132533,
0055227,0170201,0110775,0072132,
0056113,0003133,0127132,0122303,
0155577,0126351,0141462,0171037,
};
#endif
#ifdef IBMPC
static unsigned short A[44] = {
0x566f,0xb598,0x9b9a,0x4160,
0x37a7,0xe357,0x0a8f,0x41a5,
0xfe49,0x2e3a,0x4c17,0x4218,
0x7944,0xed1e,0xd6ba,0x425e,
0x13d3,0xcb80,0xa2a6,0x42b4,
0xbe11,0xbf17,0xdd8d,0x42f2,
0x905a,0xf2ac,0x40ca,0x4332,
0xcaff,0xa21c,0x27b2,0xc31c,
0x4649,0x7b05,0x0b35,0xc376,
0x01c9,0x4440,0x624d,0x4371,
0xdcd6,0x882f,0x942b,0x4374,
};
static unsigned short B[40] = {
/*0x0000,0x0000,0x0000,0x3ff0,*/
0xc36a,0x86c2,0x3c7b,0xc15e,
0xa7b8,0x63dd,0x22fc,0xc1a3,
0x37a0,0x0c46,0x227a,0xc216,
0x9d22,0xce17,0xf551,0xc25b,
0x3955,0x75d1,0xe6b5,0xc2b2,
0x8884,0xd4ff,0xd476,0xc2f0,
0x96ab,0x554d,0x46dd,0xc331,
0xae8b,0x323f,0xfe10,0x4332,
0x5498,0x75cb,0x60cb,0x4369,
0x5e44,0x3866,0xf59d,0xc34f,
};
#endif
#ifdef MIEEE
static unsigned short A[44] = {
0x4160,0x9b9a,0xb598,0x566f,
0x41a5,0x0a8f,0xe357,0x37a7,
0x4218,0x4c17,0x2e3a,0xfe49,
0x425e,0xd6ba,0xed1e,0x7944,
0x42b4,0xa2a6,0xcb80,0x13d3,
0x42f2,0xdd8d,0xbf17,0xbe11,
0x4332,0x40ca,0xf2ac,0x905a,
0xc31c,0x27b2,0xa21c,0xcaff,
0xc376,0x0b35,0x7b05,0x4649,
0x4371,0x624d,0x4440,0x01c9,
0x4374,0x942b,0x882f,0xdcd6,
};
static unsigned short B[40] = {
/*0x3ff0,0x0000,0x0000,0x0000,*/
0xc15e,0x3c7b,0x86c2,0xc36a,
0xc1a3,0x22fc,0x63dd,0xa7b8,
0xc216,0x227a,0x0c46,0x37a0,
0xc25b,0xf551,0xce17,0x9d22,
0xc2b2,0xe6b5,0x75d1,0x3955,
0xc2f0,0xd476,0xd4ff,0x8884,
0xc331,0x46dd,0x554d,0x96ab,
0x4332,0xfe10,0x323f,0xae8b,
0x4369,0x60cb,0x75cb,0x5498,
0xc34f,0xf59d,0x3866,0x5e44,
};
#endif

/* (1-x) (zeta(x) - 1), 0 <= x <= 1 */

#ifdef UNK
static double R[6] = {
-3.28717474506562731748E-1,
 1.55162528742623950834E1,
-2.48762831680821954401E2,
 1.01050368053237678329E3,
 1.26726061410235149405E4,
-1.11578094770515181334E5,
};
static double S[5] = {
/* 1.00000000000000000000E0,*/
 1.95107674914060531512E1,
 3.17710311750646984099E2,
 3.03835500874445748734E3,
 2.03665876435770579345E4,
 7.43853965136767874343E4,
};
#endif
#ifdef DEC
static unsigned short R[24] = {
0137650,0046650,0022502,0040316,
0041170,0041222,0057666,0142216,
0142170,0141510,0167741,0075646,
0042574,0120074,0046505,0106053,
0043506,0001154,0130073,0101413,
0144331,0166414,0020560,0131652,
};
static unsigned short S[20] = {
/*0040200,0000000,0000000,0000000,*/
0041234,0013015,0042073,0113570,
0042236,0155353,0077325,0077445,
0043075,0162656,0016646,0031723,
0043637,0016454,0157636,0071126,
0044221,0044262,0140365,0146434,
};
#endif
#ifdef IBMPC
static unsigned short R[24] = {
0x481a,0x04a8,0x09b5,0xbfd5,
0xd892,0x4bf6,0x0852,0x402f,
0x2f75,0x1dfc,0x1869,0xc06f,
0xb185,0x89a8,0x9407,0x408f,
0x7061,0x9607,0xc04d,0x40c8,
0x1675,0x842e,0x3da1,0xc0fb,
};
static unsigned short S[20] = {
/*0x0000,0x0000,0x0000,0x3ff0,*/
0x72ef,0xa887,0x82c1,0x4033,
0xafe5,0x6fda,0xdb5d,0x4073,
0xc67a,0xc3b4,0xbcb5,0x40a7,
0xce4b,0x9bf3,0xe3a5,0x40d3,
0xb9a3,0x581e,0x2916,0x40f2,
};
#endif
#ifdef MIEEE
static unsigned short R[24] = {
0xbfd5,0x09b5,0x04a8,0x481a,
0x402f,0x0852,0x4bf6,0xd892,
0xc06f,0x1869,0x1dfc,0x2f75,
0x408f,0x9407,0x89a8,0xb185,
0x40c8,0xc04d,0x9607,0x7061,
0xc0fb,0x3da1,0x842e,0x1675,
};
static unsigned short S[20] = {
/*0x3ff0,0x0000,0x0000,0x0000,*/
0x4033,0x82c1,0xa887,0x72ef,
0x4073,0xdb5d,0x6fda,0xafe5,
0x40a7,0xbcb5,0xc3b4,0xc67a,
0x40d3,0xe3a5,0x9bf3,0xce4b,
0x40f2,0x2916,0x581e,0xb9a3,
};
#endif

#define MAXL2 127

/*
 * Riemann zeta function, minus one
 */
#ifdef ANSIPROT
extern double torch_cephes_sin ( double );
extern double torch_cephes_floor ( double );
extern double torch_cephes_gamma ( double );
extern double torch_cephes_pow ( double, double );
extern double torch_cephes_exp ( double );
extern double torch_cephes_polevl ( double, void *, int );
extern double torch_cephes_p1evl ( double, void *, int );
double torch_cephes_zetac ( double );
#else
double torch_cephes_sin(), torch_cephes_floor(), torch_cephes_gamma(),
    torch_cephes_pow(), torch_cephes_exp();
double torch_cephes_polevl(), torch_cephes_p1evl(), torch_cephes_zetac();
#endif
extern double torch_cephes_MACHEP;

double torch_cephes_zetac(x)
double x;
{
int i;
double a, b, s, w;

if( x < 0.0 )
	{
#ifdef DEC
	if( x < -30.8148 )
#else
	if( x < -170.6243 )
#endif
		{
		torch_cephes_mtherr( "zetac", OVERFLOW );
		return(0.0);
		}
	s = 1.0 - x;
	w = torch_cephes_zetac( s );
	b = torch_cephes_sin(0.5*torch_cephes_PI*x) *
                torch_cephes_pow(2.0*torch_cephes_PI, x)
                * torch_cephes_gamma(s) *
                (1.0 + w) / torch_cephes_PI;
	return(b - 1.0);
	}

if( x >= MAXL2 )
	return(0.0);	/* because first term is 2**-x */

/* Tabulated values for integer argument */
w = floor(x);
if( w == x )
	{
	i = x;
	if( i < 31 )
		{
#ifdef UNK
		return( azetac[i] );
#else
		return( *(double *)&azetac[4*i]  );
#endif
		}
	}


if( x < 1.0 )
	{
	w = 1.0 - x;
	a = torch_cephes_polevl( x, R, 5 ) /
            ( w * torch_cephes_p1evl( x, S, 5 ));
	return( a );
	}

if( x == 1.0 )
	{
	torch_cephes_mtherr( "zetac", SING );
	return( torch_cephes_MAXNUM );
	}

if( x <= 10.0 )
	{
	b = torch_cephes_pow( 2.0, x ) * (x - 1.0);
	w = 1.0/x;
	s = (x * torch_cephes_polevl( w, P, 8 )) /
            (b * torch_cephes_p1evl( w, Q, 8 ));
	return( s );
	}

if( x <= 50.0 )
	{
	b = torch_cephes_pow( 2.0, -x );
	w = torch_cephes_polevl( x, A, 10 ) / torch_cephes_p1evl( x, B, 10 );
	w = torch_cephes_exp(w) + b;
	return(w);
	}


/* Basic sum of inverse powers */


s = 0.0;
a = 1.0;
do
	{
	a += 2.0;
	b = torch_cephes_pow( a, -x );
	s += b;
	}
while( b/s > torch_cephes_MACHEP );

b = torch_cephes_pow( 2.0, -x );
s = (s + b)/(1.0-b);
return(s);
}
