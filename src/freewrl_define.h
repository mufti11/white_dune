// The following uses code from FreeWRL
/****************************************************************************
    ... is part of the FreeWRL/FreeX3D Distribution.

    Copyright 2009 CRC Canada. (http://www.crc.gc.ca)

    FreeWRL/FreeX3D is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FreeWRL/FreeX3D is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FreeWRL/FreeX3D.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

#ifdef _WIN32
#define min(A,B) A < B ? A : B;
#define max(A,B) A < B ? B : A;
#else
#define min(A,B) ({ __typeof__(A) __a = (A); __typeof__(B) __b = (B); __a < __b ? __a : __b; })
#define max(A,B) ({ __typeof__(A) __a = (A); __typeof__(B) __b = (B); __a < __b ? __b : __a; })
#endif

#define UNUSED(v) ((void) v)
#define MALLOC(t,sz) ((t)malloc(sz))
#define MALLOCV(_sz) (malloc(_sz))
#define STRDUP strdup
#define FREE free
#define REALLOC realloc
#define ASSERT(_whatever)

#define FPRINTF fprintf
#define ERROR_MSG(...) FPRINTF(stderr, __VA_ARGS__)
#define DEBUG_MSG(...) FPRINTF(stderr, __VA_ARGS__)
#define DEBUG_MEM(...)
#define DEBUG_TEX(...)

#define FREE_IF_NZ(_ptr) if (_ptr) { \
                             FREE(_ptr); \
                             _ptr = 0; } \
                         else { \
                             DEBUG_MEM("free, pointer is already null at %s:%d\n", __FILE__, __LINE__); \
                         }


/* ************************************************************************** */
/* ******************************** Vector ********************************** */
/* ************************************************************************** */

/* This is the vector structure. */
struct Vector
{
 int    n;
 int    allocn;
 void*    data;
};

typedef struct Vector Stack;

/* Constructor/destructor */
struct Vector* newVector_(int elSize, int initSize,const char *,int);
#define newVector(type, initSize) \
 newVector_((int)sizeof(type), initSize,__FILE__,__LINE__)

#if defined(WRAP_MALLOC) || defined(DEBUG_MALLOC)
    void deleteVectorDebug_(char *file, int line, int elSize, struct Vector**);
    #define deleteVector(type, me) deleteVectorDebug_(__FILE__,__LINE__,(int)sizeof(type), &(me))
#else
    void deleteVector_(int elSize, struct Vector**);
    #define deleteVector(type, me) deleteVector_((int)sizeof(type), ((struct Vector**)(&me)))
#endif

/* Ensures there's at least one space free. */
void vector_ensureSpace_(int, struct Vector*, const char *fi, int line);

/* Element retrieval. */
#define vector_get(type, me, ind) \
 ((type*)((struct Vector*)me)->data)[ind]

/* pointer to element retrieval */
#define vector_get_ptr(type, me, ind) \
 &((type*)((struct Vector*)me)->data)[ind]


/* Element set. */
#define vector_set(type,me,element,value) \
        ((type*)((struct Vector *)  (me))->data)[element]=value

void vector_removeElement(int elSize,struct Vector* myp, int element);
#define vector_remove_elem(type,me,element) \
 vector_removeElement((int)sizeof(type),me,element)

/* Size of vector */
#define vectorSize(me) \
 (((struct Vector*)me)->n)

/* Back of a vector */
#define vector_back(type, me) \
 vector_get(type, me, vectorSize(me)-1)

/* Is the vector empty? */
#define vector_empty(me) \
 (!vectorSize(me))

/* Shrink the vector to minimum required space. */
void vector_shrink_(int, struct Vector*);
#define vector_shrink(type, me) \
 vector_shrink_((int)sizeof(type), me)

 /* clear out allocated data, n=0,nalloc=0 so ready for pushBack*/
 void vector_clear(struct Vector* me);

/* Push back operation. */
#define vector_pushBack(type, me, el) \
 { \
  vector_ensureSpace_((int)sizeof(type), me,__FILE__,__LINE__); \
  ASSERT(((struct Vector*)me)->n<((struct Vector*)me)->allocn); \
  vector_get(type, me, ((struct Vector*)me)->n)=el; \
  ++((struct Vector*)me)->n; \
 }

/* Pop back operation */
void vector_popBack_(struct Vector*, size_t count);
#define vector_popBack(type, me) \
 { \
  ASSERT(!vector_empty(me)); \
  --((struct Vector*)me)->n; \
 }
#define vector_popBackN(type, me, popn) \
 { \
  ASSERT(popn<=vectorSize(me)); \
  ((struct Vector*)me)->n-=popn; \
 }

/* Release and get vector data. */
void* vector_releaseData_(int, struct Vector*);
#define vector_releaseData(type, me) \
 vector_releaseData_((int)sizeof(type), me)


/* Constructor and destructor */
#define newStack(type) \
 newVector(type, 4)
#define deleteStack(type, me) \
 deleteVector(type, me)

/* Push and pop */
#define stack_push(type, me, el) \
 vector_pushBack(type, me, el)
#define stack_pop(type, me) \
 vector_popBack(type, me)

/* Top of stack */
#define stack_top(type, me) \
 vector_get(type, me, vectorSize(me)-1)

/* Is the stack empty? */
#define stack_empty(me) \
 vector_empty(me)


