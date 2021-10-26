#include "string.h"

struct string_view string_make_view(struct string str) {
  return (struct string_view){
      .data = str.data,
      .length = str.length,
  };
}
