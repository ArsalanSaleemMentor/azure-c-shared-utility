// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

/* This is a template file used for porting */

/* Please go through all the TODO sections below and implement the needed code */

#include <stdint.h>
#include <stdlib.h>
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/xlogging.h"
#include "kernel/nu_kernel.h"

DEFINE_ENUM_STRINGS(THREADAPI_RESULT, THREADAPI_RESULT_VALUES);

typedef struct THREAD_INSTANCE_TAG
{
	/* TODO: add here your variable that is a handle to the thread for the threading library that you are using.
    ... thread_handle;*/
	NU_TASK task_cb;
    THREAD_START_FUNC ThreadStartFunc;
    void* Arg;
	char stack_ptr[2048];
} THREAD_INSTANCE;

/* TODO: Modify this to be to the format of the thread start function that is used by your threading library.
Each library has its own signature for the thread entry point
static void* ThreadWrapper(void* threadInstanceArg)
{
    THREAD_INSTANCE* threadInstance = (THREAD_INSTANCE*)threadInstanceArg;
    int result = threadInstance->ThreadStartFunc(threadInstance->Arg);
    return (void*)(intptr_t)result;
}*/

VOID NU_Task_Wrapper (UNSIGNED argc, VOID *argv)
{
	THREAD_INSTANCE* threadInstance = (THREAD_INSTANCE *) (argv);
	threadInstance->ThreadStartFunc (threadInstance->Arg);
}

THREADAPI_RESULT ThreadAPI_Create(THREAD_HANDLE* threadHandle, THREAD_START_FUNC func, void* arg)
{
    THREADAPI_RESULT result;
    STATUS status;

    if ((threadHandle == NULL) ||
        (func == NULL))
    {
        result = THREADAPI_INVALID_ARG;
        LogError("(result = %s)", ENUM_TO_STRING(THREADAPI_RESULT, result));
    }
    else
    {
        THREAD_INSTANCE* threadInstance = malloc(sizeof(THREAD_INSTANCE));
        if (threadInstance == NULL)
        {
            result = THREADAPI_NO_MEMORY;
            LogError("(result = %s)", ENUM_TO_STRING(THREADAPI_RESULT, result));
        }
        else
        {
            threadInstance->ThreadStartFunc = func;
            threadInstance->Arg = arg;
			
			/* TODO: Invoke here the threading library that you use in order to create a new thread.
			Make sure that you pass as a context the threadInstance.
            ... createResult = my_function_to_create_a_thread(&threadInstance->thread_handle, ThreadWrapper, threadInstance);*/

            status = NU_Create_Task(&threadInstance->task_cb, "AzureT", NU_Task_Wrapper, 1, (VOID*) threadInstance, threadInstance->stack_ptr, 2048, 50, 0,
            		NU_NO_PREEMPT, NU_START);

            /*if (...error...)*/

            if(status != NU_SUCCESS)
			{
                free(threadInstance);

                result = THREADAPI_ERROR;
                LogError("(result = %s)", ENUM_TO_STRING(THREADAPI_RESULT, result));
                //break;
			} else
			{
                *threadHandle = threadInstance;
                result = THREADAPI_OK;
                //break;
			}
        }
    }

    return result;
}

THREADAPI_RESULT ThreadAPI_Join(THREAD_HANDLE threadHandle, int* res)
{

	printf("ThreadAPI_Join called with threadHandle");
    while(1);

    return THREADAPI_ERROR;
}

void ThreadAPI_Exit(int res)
{
	/* TODO: call here the function that terminates the thread.
	This is called from within the thread code itself. */

	printf("ThreadAPI_Exit called with res = %d", res);
}

void ThreadAPI_Sleep(unsigned int milliseconds)
{
	/* TODO: call a function that sleeps the indicated amount in milliseconds.
	For some libraries conversions are needed here */

	int nucleus_ticks = (milliseconds /1000) * NU_PLUS_TICKS_PER_SEC;

	if (nucleus_ticks >= 1)
	{
		(void) NU_Sleep(nucleus_ticks);
	}
	else
	{
		(void) NU_Sleep(1);
	}
}
