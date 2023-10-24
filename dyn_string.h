#ifndef _DYN_STRING_H_
#define _DYN_STRING_H_


#include <stddef.h>

typedef struct string {
  size_t length;
  size_t capacity;
  char buffer[];
} string;


typedef char* dyn_string;


static inline size_t dyn_string_util_max(size_t lhs, size_t rhs) {
  return lhs > rhs ? lhs : rhs;
}

static inline size_t dyn_string_util_min(size_t lhs, size_t rhs) {
  return lhs < rhs ? lhs : rhs;
}

static inline string* dyn_string_cast(dyn_string dyn_str) {
  return (string*)(dyn_str - offsetof(string, buffer));
}

extern dyn_string dyn_string_construct(char* string_literal);
extern void dyn_string_destruct(dyn_string str);
extern void dyn_string_copy_construct(dyn_string* dest, dyn_string* src);
extern void dyn_string_copy(dyn_string* dest, dyn_string* src);
extern void dyn_string_push_back(dyn_string* dyn_str, char ch);
extern void dyn_string_cat(dyn_string* dyn_str_dest, dyn_string* dyn_str_src);
extern void dyn_string_ncat(dyn_string* dyn_str_dest, dyn_string* dyn_str_src, size_t bytes_to_copy);
extern size_t dyn_string_length(const dyn_string string);
extern int dyn_string_cmp(const dyn_string* first, const dyn_string* second);
extern int dyn_string_ncmp(const dyn_string* first, const dyn_string* second, size_t bytes_to_compare);

#endif
