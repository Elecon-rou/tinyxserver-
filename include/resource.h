/***********************************************************

Copyright 1987, 1989, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from The Open Group.


Copyright 1987, 1989 by Digital Equipment Corporation, Maynard, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of Digital not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#ifndef RESOURCE_H
#define RESOURCE_H 1
#include "misc.h"

/*****************************************************************
 * STUFF FOR RESOURCES
 *****************************************************************/

/* classes for Resource routines */

typedef unsigned long RESTYPE;

#define RC_VANILLA	((RESTYPE)0)
#define RC_CACHED	((RESTYPE)1<<31)
#define RC_DRAWABLE	((RESTYPE)1<<30)
/*  Use class RC_NEVERRETAIN for resources that should not be retained
 *  regardless of the close down mode when the client dies.  (A client's
 *  event selections on objects that it doesn't own are good candidates.)
 *  Extensions can use this too!
 */
#define RC_NEVERRETAIN	((RESTYPE)1<<29)
#define RC_LASTPREDEF	RC_NEVERRETAIN
#define RC_ANY		(~(RESTYPE)0)

/* types for Resource routines */

#define RT_WINDOW	((RESTYPE)1|RC_CACHED|RC_DRAWABLE)
#define RT_PIXMAP	((RESTYPE)2|RC_CACHED|RC_DRAWABLE)
#define RT_GC		((RESTYPE)3|RC_CACHED)
#undef RT_FONT
#undef RT_CURSOR
#define RT_FONT		((RESTYPE)4)
#define RT_CURSOR	((RESTYPE)5)
#define RT_COLORMAP	((RESTYPE)6)
#define RT_CMAPENTRY	((RESTYPE)7)
#define RT_OTHERCLIENT	((RESTYPE)8|RC_NEVERRETAIN)
#define RT_PASSIVEGRAB	((RESTYPE)9|RC_NEVERRETAIN)
#define RT_LASTPREDEF	((RESTYPE)9)
#define RT_NONE		((RESTYPE)0)

/* bits and fields within a resource id */
#define RESOURCE_AND_CLIENT_COUNT   29			/* 29 bits for XIDs */
#if MAXCLIENTS == 64
#define RESOURCE_CLIENT_BITS	6
#endif
#if MAXCLIENTS == 128
#define RESOURCE_CLIENT_BITS	7
#endif
#if MAXCLIENTS == 256
#define RESOURCE_CLIENT_BITS	8
#endif
#if MAXCLIENTS == 512
#define RESOURCE_CLIENT_BITS	9
#endif
/* client field offset */
#define CLIENTOFFSET	    (RESOURCE_AND_CLIENT_COUNT - RESOURCE_CLIENT_BITS)
/* resource field */
#define RESOURCE_ID_MASK	((1 << CLIENTOFFSET) - 1)
/* client field */
#define RESOURCE_CLIENT_MASK	(((1 << RESOURCE_CLIENT_BITS) - 1) << CLIENTOFFSET)
/* extract the client mask from an XID */
#define CLIENT_BITS(id) ((id) & RESOURCE_CLIENT_MASK)
/* extract the client id from an XID */
#define CLIENT_ID(id) ((int)(CLIENT_BITS(id) >> CLIENTOFFSET))
#define SERVER_BIT		(Mask)0x40000000	/* use illegal bit */

#ifdef INVALID
#undef INVALID	/* needed on HP/UX */
#endif

/* Invalid resource id */
#define INVALID	(0)

#define BAD_RESOURCE 0xe0000000

typedef int (*DeleteType)(
    pointer /*value*/,
    XID /*id*/);

typedef void (*FindResType)(
    pointer /*value*/,
    XID /*id*/,
    pointer /*cdata*/);

typedef void (*FindAllRes)(
    pointer /*value*/,
    XID /*id*/,
    RESTYPE /*type*/,
    pointer /*cdata*/);

typedef Bool (*FindComplexResType)(
    pointer /*value*/,
    XID /*id*/,
    pointer /*cdata*/);

RESTYPE CreateNewResourceType(
    DeleteType /*deleteFunc*/);

Bool InitClientResources(
    ClientPtr /*client*/);

XID FakeClientID(
    int /*client*/);

/* Quartz support on Mac OS X uses the CarbonCore
   framework whose AddResource function conflicts here. */
Bool AddResource(
    XID /*id*/,
    RESTYPE /*type*/,
    pointer /*value*/);

void FreeResource(
    XID /*id*/,
    RESTYPE /*skipDeleteFuncType*/);

void FreeResourceByType(
    XID /*id*/,
    RESTYPE /*type*/,
    Bool /*skipFree*/);

Bool ChangeResourceValue(
    XID /*id*/,
    RESTYPE /*rtype*/,
    pointer /*value*/);

void FindClientResourcesByType(
    ClientPtr /*client*/,
    RESTYPE /*type*/,
    FindResType /*func*/,
    pointer /*cdata*/);

void FindAllClientResources(
    ClientPtr /*client*/,
    FindAllRes /*func*/,
    pointer /*cdata*/);

void FreeClientNeverRetainResources(
    ClientPtr /*client*/);

void FreeClientResources(
    ClientPtr /*client*/);

void FreeAllResources(void);

Bool LegalNewID(
    XID /*id*/,
    ClientPtr /*client*/);

pointer LookupIDByType(
    XID /*id*/,
    RESTYPE /*rtype*/);

pointer LookupIDByClass(
    XID /*id*/,
    RESTYPE /*classes*/);

pointer LookupClientResourceComplex(
    ClientPtr client,
    RESTYPE type,
    FindComplexResType func,
    pointer cdata);

/* These are the access modes that can be passed in the last parameter
 * to SecurityLookupIDByType/Class.  The Security extension doesn't
 * currently make much use of these; they're mainly provided as an
 * example of what you might need for discretionary access control.
 * You can or these values together to indicate multiple modes
 * simultaneously.
 */

#define SecurityUnknownAccess	0	/* don't know intentions */
#define SecurityReadAccess	(1<<0)	/* inspecting the object */
#define SecurityWriteAccess	(1<<1)	/* changing the object */
#define SecurityDestroyAccess	(1<<2)	/* destroying the object */

pointer SecurityLookupIDByType(
    ClientPtr /*client*/,
    XID /*id*/,
    RESTYPE /*rtype*/,
    Mask /*access_mode*/);

pointer SecurityLookupIDByClass(
    ClientPtr /*client*/,
    XID /*id*/,
    RESTYPE /*classes*/,
    Mask /*access_mode*/);


void GetXIDRange(
    int /*client*/,
    Bool /*server*/,
    XID * /*minp*/,
    XID * /*maxp*/);

unsigned int GetXIDList(
    ClientPtr /*client*/,
    unsigned int /*count*/,
    XID * /*pids*/);

extern RESTYPE lastResourceType;
extern RESTYPE TypeMask;

#ifdef XResExtension
extern Atom *ResourceNames;
void RegisterResourceName(RESTYPE type, char* name);
#endif

#endif /* RESOURCE_H */

