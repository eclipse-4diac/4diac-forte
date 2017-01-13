In order to run forte on rcX, some things must be adjusted at linking time.

In SECTION part of the linker file, another section must be added as follow:

.init_array :
{
  . = ALIGN (4); 
  _init_array_start = ABSOLUTE (.); 
  KEEP (*(SORT (.init_array.*))) 
  KEEP (*(SORT (.init_array))) 
  _init_array_end = ABSOLUTE (.);
} >SDRAM

this creates a section called init_array that will then be used to call the constructors of all classes.

Next you need to create a file called crti.c and put the following code there:


typedef void (*func_ptr)(void);

extern func_ptr _init_array_start[0], _init_array_end[0];

void _init1(void)
{
  func_ptr* func;
  for ( func = _init_array_start; func != _init_array_end; func++ )
    (*func)();
}


this defines a function called _init1 that will call all the constructors.

In you rcX application, before calling the startForte() function that will start a forte server, you must call the _init1() and initForte() functions, in that order.

So, your application code will look like:

 _init1();
 initForte();
 startForte();