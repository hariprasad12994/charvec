#include <stddef.h>
#include <stdlib.h>
#include "dyn_string.h"


char* allocate_raw_string(size_t string_length) {
  char* new_string = malloc(string_length + 1);
  *new_string = '\0';
  return new_string;
}


void deallocate_raw_string(char* str) { free(str); }


size_t raw_string_length(const char* string) {
  size_t length = 0;
  while(*string++) length++;
  return length;
}


void raw_string_copy(char* dest, char* src) {
  while(*src) *dest++ = *src++;
  *dest = '\0';
}


void raw_string_ncopy(char* dest, const char* src, size_t bytes_to_copy) {
  size_t count = 0;
  while(*src && count < bytes_to_copy) *dest++ = *src++, count++;
}


int raw_string_compare(char* first, char* second) {
  while(*first && *second && *first == *second) first++, second++;
  return *first - *second;
}


int raw_string_ncompare(char* first, char* second, size_t bytes_to_compare) {
  size_t count = 0;
  while(*first == *second && count < bytes_to_compare) first++, second++, count++;

  if(count == bytes_to_compare) return 0;
  return *first - *second;
}


dyn_string dyn_string_construct(char* string_literal) {
  size_t string_len = raw_string_length(string_literal);

  string* new_string = malloc(sizeof(string) + string_len + 1);
  new_string->length = string_len;
  new_string->capacity = string_len;
  raw_string_copy(new_string->buffer, string_literal);

  return (char*)new_string->buffer;
}


void dyn_string_destruct(dyn_string str) { free(str - offsetof(string, buffer)); }


void dyn_string_destruct_prv(string* str) { free(str); }


void dyn_string_copy_construct(dyn_string* dest, dyn_string* src) {
  string* src_str = (string*)(*src - offsetof(string, buffer));
  string* dest_str = malloc(sizeof(string) + src_str->length + 1);
  raw_string_copy(dest_str->buffer, src_str->buffer);
  dest_str->length = src_str->length;
  dest_str->capacity = src_str->capacity;
  *dest = dest_str->buffer;
}


void dyn_string_resize(string** str, size_t new_capacity) {
  string* new_string = malloc(sizeof(string) + new_capacity + 1);

  raw_string_copy(new_string->buffer, (*str)->buffer);
  new_string->length = (*str)->length;
  new_string->capacity = new_capacity;

  dyn_string_destruct_prv(*str);
  *str = new_string;
}


void dyn_string_push_back(dyn_string* dyn_str, char ch) {
  string* str = (string*)(*dyn_str - offsetof(string, buffer));

  if(str->length == str->capacity)
    dyn_string_resize(&str, str->length * 2);
  str->buffer[str->length] = ch;
  str->buffer[str->length + 1] = '\0';
  str->length++;

  *dyn_str = str->buffer;
}


void dyn_string_copy(dyn_string* dest, dyn_string* src) {
  string* src_str = (string*)(*src - offsetof(string, buffer));
  string* dest_str = (string*)(*dest - offsetof(string, buffer));

  if(src_str->length > dest_str->capacity)
    dyn_string_resize(&dest_str, src_str->length);

  raw_string_ncopy(dest_str->buffer, src_str->buffer, src_str->length);

  if(src_str->length >= dest_str->length) {
    dest_str->length = src_str->length;
    dest_str->buffer[dest_str->length] = '\0';
  }

  // todo: possible refactoring: dest_str->length = dyn_string_util_max(dest_str->length, src_str->length);

  *dest = dest_str->buffer;
}

void dyn_string_cat(dyn_string* dyn_str_dest, dyn_string* dyn_str_src) {
  string* dest = (string*)(*dyn_str_dest - offsetof(string, buffer));
  string* src = (string*)(*dyn_str_src - offsetof(string, buffer));

  // todo: resize factor for concatenation or in general to be formulated
  if(src->length + dest->length > dest->capacity)
    dyn_string_resize(&dest, dest->length + src->length);

  raw_string_copy(dest->buffer + dest->length, src->buffer);

  dest->length = dest->length + src->length;

  // dest could have been freed and relocated to different location, change
  // argument pointer ti point to latest location
  *dyn_str_dest = dest->buffer;
}


void dyn_string_ncat(dyn_string* dyn_str_dest, dyn_string* dyn_str_src, size_t bytes_to_copy) {
  string* dest = (string*)(*dyn_str_dest - offsetof(string, buffer));
  string* src = (string*)(*dyn_str_src - offsetof(string, buffer));

  size_t actual_bytes_to_copy = dyn_string_util_min(bytes_to_copy, src->length);

  if(dest->capacity + actual_bytes_to_copy > dest->length)
    dyn_string_resize(&dest, dest->length + actual_bytes_to_copy);

  raw_string_ncopy(dest->buffer + dest->length, src->buffer, actual_bytes_to_copy);

  dest->length = dest->length + actual_bytes_to_copy;
  dest->buffer[dest->length] = '\0';

  // dest could have been freed and relocated to different location, change
  // argument pointer to point to latest location
  *dyn_str_dest = dest->buffer;
}


size_t dyn_string_length(const dyn_string str) {
  size_t offsetof_buffer = offsetof(string, buffer);
  size_t offsetof_length = offsetof(string, length);

  return (size_t)*(str - offsetof_buffer + offsetof_length);
}


int dyn_string_cmp(const dyn_string* first, const dyn_string* second) {
  string* first_str = (string*)(*first - offsetof(string, buffer));
  string* second_str = (string*)(*second - offsetof(string, buffer));
  return raw_string_compare(first_str->buffer, second_str->buffer);
}


int dyn_string_ncmp(const dyn_string* first, const dyn_string* second, size_t bytes_to_compare) {
  string* first_str = (string*)(*first - offsetof(string, buffer));
  string* second_str = (string*)(*second - offsetof(string, buffer));

  size_t max_bytes_to_compare = dyn_string_util_min(dyn_string_util_min(first_str->length, second_str->length), bytes_to_compare);

  return raw_string_ncompare(first_str->buffer, second_str->buffer, max_bytes_to_compare);
}

