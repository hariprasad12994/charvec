Yet another string library written in plain [modern] C. Currently the library supports dynamically allocated length prefixed 'Pascal' strings.

There is no automatic memory management features baked into C. For instance, like Rust's Borrow Checking and C++'s RAII.
C as always requires the developer to ensure that  memory is allocated, deallocated and accessed accordingly.

The library aims to share this burden by providing abstracted methods, emulating C++ or Rust, which when properly used reduce chances of undefined behavior.

In otherwords, the library will protect you from shards of glass and to a little extend from shooting yourself in foot, which C allows you to do so. We do 
this by getting closer to C++ and Rust but not compromising the simplicity and idiomatic ways of C.


## Features
* Dynamically shrinkable and growable strings
* Abstracted memory allocation and deallocation
* Data type compatibility with C standard and existing libraries
  * Especially printf!
  ```c
  dyn_string str = dyn_string_construct("hello");
  printf("%s\n", str); // Prints 'hello'. Yayy!
  ```
* Methods, on buffer overflow detection, become no-op and return error code
  ```c
  dyn_string str = dyn_string_construct("hello ");
  dyn_string suffix = dyn_string_construct("world");

  // Protected against overflow, becomes no-op
  int cat_status = dyn_string_cat(&str, &suffix, 10);
  // Check the status and handle error
  if(cat_status == -1) {
    // Do something
  }
  ```
* Constant time length and capacity query


## Things to remember
* Do remember that 
  * Using the data type with existing or standard 'unsafe' functions might introduce lifetime, overflow and mutability issues
  * Out of bounds access using [] operator results still results in undefined behavior
   ```c
  dyn_string str = dyn_string_construct("hello ");
  dyn_string suffix = dyn_string_construct("world");

  strncpy(str, suffix, 2);  // Ok!
  strncpy(str, suffix, 10); // Yikes!
  suffix[0] = 'a';          // Ok!
  suffix[12] = 'c';         // Yikes!
  ```


### Tips
* Run valgrind on your code whenever possible
