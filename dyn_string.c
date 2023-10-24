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
  new_string->length;
}
