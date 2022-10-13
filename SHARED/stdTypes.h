/*Standard Types Header File*/

#ifndef _STDTYPES_H
#define _STDTYPES_H

/************************************************
 * Unsigned integer data types			u-types	*
 ************************************************/
typedef unsigned int u8  __attribute__((__mode__(__QI__)));					/* u8  */
typedef unsigned int u16 __attribute__((__mode__(__HI__)));					/* u16 */
typedef unsigned int u32 __attribute__((__mode__(__SI__)));					/* u32 */
typedef unsigned int u64 __attribute__((__mode__(__DI__)));					/* u64 */

/************************************************
 *	Signed integer data types			s-types	*
 ************************************************/
typedef signed int s8  __attribute__((__mode__(__QI__)));					/* s8  */
typedef signed int s16 __attribute__((__mode__(__HI__)));					/* s16 */
typedef signed int s32 __attribute__((__mode__(__SI__)));					/* s32 */
typedef signed int s64 __attribute__((__mode__(__DI__)));					/* s64 */

/************************************************
 *	Floating point data types			f-types	*
 ************************************************/
typedef float f32 ;															/* f32 */
typedef double f64 ;														/* f64 */
typedef long double f80 ;													/* f80 */

/*************************************************
 *	Boolean data types				boolean-types*
 *************************************************/
typedef enum {	TRUE = 1,													/* TRUE  */
				FALSE= 0,													/* FALSE */
			 }bool;

/************************************************
 *		Miscellaneous helpful definitions		*
 ************************************************/
#define NUL		'\0'														/*	NUL */
#define NULL (void*)0x0														/* NULL */

#endif
