/*
 * @file: V2Windows.h
 * @author: Ondrej Hirjak, 2006 - 2007
 * @description: Windows includes.
 */

#ifndef V2Windows_h
#define V2Windows_h


/*
 * =============================================
 * Windows-specificke includy
 * =============================================
*/

#define _CRT_SECURE_NO_DEPRECATE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <stdlib.h>

#include <mmsystem.h>

#include <gl\gl.h>
#include <gl\glu.h>

// TODO: STL throws too many warnings in debug mode
// TODO: replace STL with home-breed classes

#include <queue>
#include <list>
#include <map>

#include <math.h>

#define INFINITY 0xFFFFFFFF


#endif // V2Windows_h
