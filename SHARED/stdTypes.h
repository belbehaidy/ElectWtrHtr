/*Standard Types Header File*/

#ifndef _STDTYPES_H
#define _STDTYPES_H

/************************************************
 * Unsigned integer data types			u-types	*
 ************************************************/
typedef unsigned char 			u8  ;										/* u8  */
typedef unsigned short			u16 ;										/* u16 */
typedef unsigned long int		u32 ;										/* u32 */
typedef unsigned long long int	u64 ;										/* u64 */

/************************************************
 *	Signed integer data types			s-types	*
 ************************************************/
typedef signed char				s8 ;										/* s8  */
typedef signed short			s16 ;										/* s16 */
typedef signed long int			s32 ;										/* s32 */
typedef signed long long int	s64 ;										/* s64 */

/************************************************
 *	Floating point data types			f-types	*
 ************************************************/
typedef float					f32 ;										/* f32 */
typedef double					f64 ;										/* f64 */
typedef long double				f80 ;										/* f80 */

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
#ifndef NULL
#define NULL (void*)0x0														/* NULL */
#endif
#endif
