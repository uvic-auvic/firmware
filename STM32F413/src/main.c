#include "main.h"

int main(int argc, char const *argv[])
{
	// MainLoop is the entry point for the hardware.
	MainLoop();

   return 0;
}

/* vApplicationMallocFailedHook() will only be called if
   configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
   function that will get called if a call to pvPortMalloc() fails.
   pvPortMalloc() is called internally by the kernel whenever a task, queue,
   timer or semaphore is created.  It is also called by various parts of the
   demo application.  If heap_1.c or heap_2.c are used, then the size of the
   heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   to query the size of free heap space that remains (although it does not
   provide information on how the remaining heap might be fragmented). */
void vApplicationMallocFailedHook(void) {
  taskDISABLE_INTERRUPTS();
  for(;;);
}


void vApplicationStackOverflowHook(TaskHandle_t xTask, char * pcTaskName){
	  (void) pcTaskName;
	  (void) xTask;
	  /* Run time stack overflow checking is performed if
	     configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	     function is called if a stack overflow is detected. */
	  taskDISABLE_INTERRUPTS();
	  for(;;);
}
