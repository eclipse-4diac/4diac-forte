IMPORTANT! NOLOG must be set. LOGGING brings errors to the library

In order to run forte on rcX, some things must be adjusted at linking time.

In SECTION part of the linker file, another section must be added as follow:

.init_array :
{
  . = ALIGN (4); 
  _init_array_start_forte = ABSOLUTE (.); 
  KEEP (*(SORT (.init_array.*))) 
  KEEP (*(SORT (.init_array))) 
  _init_array_end_forte = ABSOLUTE (.);
} >SDRAM

.fini_array :
{
  . = ALIGN (4); 
  _fini_array_start_forte = ABSOLUTE (.); 
  KEEP (*(SORT (.fini_array.*))) 
  KEEP (*(SORT (.fini_array))) 
  _fini_array_end_forte = ABSOLUTE (.);
} >SDRAM 

this creates a section called init_array and fini_array that will then be used to call the constructors/destructors of all classes.

forte_global_initialize(); must be called only once, before the first forte instance is created and 
forte_global_deinitialize(); must be called only once after the last forte instance is stopped

The normal sequence to start a Forte instance is to call the follow functions:

forte_global_initialize();
TForteInstance forteInstance = 0;
int resultForte = forte_start_instance(0, &forteInstance);

and to finish

forte_stop_instance(resultForte, forteInstance);
forte_global_deinitialize();
