// Scenario that tests the usage of shared variables
#include <inc/lib.h>

void
_main(void)
{
	/*[1] CREATE SHARED VARIABLE & INITIALIZE IT*/
	int *X = smalloc("X", sizeof(int) , 1) ;
	*X = 5 ;

	/*[2] SPECIFY WHETHER TO USE SEMAPHORE OR NOT*/
	char select;
	sys_lock_cons();
	{
		cprintf("Do you want to use semaphore (y/n)? ") ;
		select = getchar() ;
		cputchar(select);
		cputchar('\n');
	}
	sys_unlock_cons();

	/*[3] SHARE THIS SELECTION WITH OTHER PROCESSES*/
	int *useSem = smalloc("useSem", sizeof(int) , 0) ;
	*useSem = 0 ;
	if (select == 'Y' || select == 'y')
		*useSem = 1 ;

	struct semaphore T, finished, finishedCountMutex;
	int *numOfFinished ;
	if (*useSem == 1)
	{
		T = create_semaphore("T", 0);
		finished = create_semaphore("finished", 0);
		finishedCountMutex = create_semaphore("finishedCountMutex", 1);
	}
	//Create the check-finishing counter
	numOfFinished = smalloc("finishedCount", sizeof(int), 1) ;
	*numOfFinished = 0 ;

	/*[4] CREATE AND RUN ProcessA & ProcessB*/

	//Create the 2 processes
	int32 envIdProcessA = sys_create_env("midterm_a", (myEnv->page_WS_max_size),(myEnv->SecondListSize), (myEnv->percentage_of_WS_pages_to_be_removed));
	int32 envIdProcessB = sys_create_env("midterm_b", (myEnv->page_WS_max_size), (myEnv->SecondListSize),(myEnv->percentage_of_WS_pages_to_be_removed));

	//Run the 2 processes
	sys_run_env(envIdProcessA);
	sys_run_env(envIdProcessB);

	/*[5] WAIT TILL FINISHING BOTH PROCESSES*/
	if (*useSem == 1)
	{
		wait_semaphore(finished);
		wait_semaphore(finished);
	}
	else
	{
		while (*numOfFinished != 2) ;
	}

	/*[6] PRINT X*/
	atomic_cprintf("Final value of X = %d\n", *X);

	return;
}
