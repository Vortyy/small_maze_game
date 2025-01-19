/*********************************************************************
 * Dynamic array v1.0
 *
 * implementation of dynamyc array such vector(C++), ArrayList(Java),... 
 * using small buffer optimization which help to keep small buffer inside 
 * the stack memory until, it needs to grow
 *
 * REF : 
 *  - https://en.wikipedia.org/wiki/Dynamic_array
 *  - https://nullprogram.com/blog/2016/10/07/
 *********************************************************************/
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdlib.h>

typedef struct _vector {
  void *values;
  size_t size;
  size_t cnt;
} Vector;

void vec_init(Vector *v, void *values, size_t size);
int vec_memory_supply(Vector *v, size_t current_size, size_t size_value);
void vec_free(Vector *v);

// int *pt = (int *) v.values + 1; // since the casting is done in first place add 7 will add 7 * 4 bytes to the address of values 
// int *pt2 = (int *) (v.values + 1 * sizeof(int)); // in this case sizeof is required
/* vec_push_generic : push any type inside values */
#define vec_push_generic(v_ptr, value, type){                     \
  size_t current_size = (v_ptr)->size & ~(size_t)1;               \
  if((v_ptr)->cnt == current_size)                                \
    if(!vec_memory_supply((v_ptr), current_size, sizeof(type)))   \
      exit(1);                                                    \
  * ((type *) (v_ptr)->values + (v_ptr)->cnt) = value;            \
  (v_ptr)->cnt++;}

/* vec_get : get i element of a Vector (v_ptr) */
#define vec_get(v_ptr, i, type) (* ((type *) (v_ptr)->values + i))

#if __STDC_VERSION__ >= 201112L /* check C11 or higher */

//Macro to generate vec_push easily for C natural type 
#define vec_push_fct(type) void vec_push_##type(Vector *v, type value) { \
  size_t current_size = v->size & ~(size_t)1;                            \
  if(v->cnt == current_size)                                             \
    if(!vec_memory_supply(v, current_size, sizeof(type)))                \
      exit(1);                                                           \
  * ((type *) v->values + v->cnt) = value;                               \
  v->cnt++;}

/* Declaring vector natural type using _Generic (only avaible since C11) */
static inline vec_push_fct(char);
static inline vec_push_fct(short);
static inline vec_push_fct(int);
static inline vec_push_fct(long);
static inline vec_push_fct(float);
static inline vec_push_fct(double);

#define vec_push(v_ptr, i)              \
  _Generic((i),                         \
    char : vec_push_char(v_ptr, i),     \
    int : vec_push_int(v_ptr, i),       \
    short : vec_push_short(v_ptr, i),   \
    long : vec_push_long(v_ptr, i),     \
    float : vec_push_float(v_ptr, i),   \
    double : vec_push_double(v_ptr, i)) 

#endif // >=(C11) 

#endif // !DYNAMIC_ARRAY.H
