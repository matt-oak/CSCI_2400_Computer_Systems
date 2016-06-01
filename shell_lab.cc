// 
// tsh - A tiny shell program with job control
// 
// Matthew Oakley, maoa8018
//

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string>

#include "globals.h"
#include "jobs.h"
#include "helper-routines.h"

//
// Needed global variable definitions
//

static char prompt[] = "tsh> ";
int verbose = 0;

//
// You need to implement the functions eval, builtin_cmd, do_bgfg,
// waitfg, sigchld_handler, sigstp_handler, sigint_handler
//
// The code below provides the "prototypes" for those functions
// so that earlier code can refer to them. You need to fill in the
// function bodies below.
// 

void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

//
// main - The shell's main routine 
//
int main(int argc, char **argv) 
{
  int emit_prompt = 1; // emit prompt (default)

  //
  // Redirect stderr to stdout (so that driver will get all output
  // on the pipe connected to stdout)
  //
  dup2(1, 2);

  /* Parse the command line */
  char c;
  while ((c = getopt(argc, argv, "hvp")) != EOF) {
    switch (c) {
    case 'h':             // print help message
      usage();
      break;
    case 'v':             // emit additional diagnostic info
      verbose = 1;
      break;
    case 'p':             // don't print a prompt
      emit_prompt = 0;  // handy for automatic testing
      break;
    default:
      usage();
    }
  }

  //
  // Install the signal handlers
  //

  //
  // These are the ones you will need to implement
  //
  Signal(SIGINT,  sigint_handler);   // ctrl-c
  Signal(SIGTSTP, sigtstp_handler);  // ctrl-z
  Signal(SIGCHLD, sigchld_handler);  // Terminated or stopped child

  //
  // This one provides a clean way to kill the shell
  //
  Signal(SIGQUIT, sigquit_handler); 

  //
  // Initialize the job list
  //
  initjobs(jobs);

  //
  // Execute the shell's read/eval loop
  //
  for(;;) {
    //
    // Read command line
    //
    if (emit_prompt) {
      printf("%s", prompt);
      fflush(stdout);
    }

    char cmdline[MAXLINE];

    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin)) {
      app_error("fgets error");
    }
    //
    // End of file? (did user type ctrl-d?)
    //
    if (feof(stdin)) {
      fflush(stdout);
      exit(0);
    }

    //
    // Evaluate command line
    //
    eval(cmdline);
    fflush(stdout);
    fflush(stdout);
  } 

  exit(0); //control never reaches here
}
  
/////////////////////////////////////////////////////////////////////////////
//
// eval - Evaluate the command line that the user has just typed in
// 
// If the user has requested a built-in command (quit, jobs, bg or fg)
// then execute it immediately. Otherwise, fork a child process and
// run the job in the context of the child. If the job is running in
// the foreground, wait for it to terminate and then return.  Note:
// each child process must have a unique process group ID so that our
// background children don't receive SIGINT (SIGTSTP) from the kernel
// when we type ctrl-c (ctrl-z) at the keyboard.
//
void eval(char *cmdline) 
{
  /* Parse command line */
  //
  // The 'argv' vector is filled in by the parseline
  // routine below. It provides the arguments needed
  // for the execve() routine, which you'll need to
  // use below to launch a process.
  //
  char *argv[MAXARGS];

  //
  // The 'bg' variable is TRUE if the job should run
  // in background mode or FALSE if it should run in FG
  //
  int bg = parseline(cmdline, argv); 
  if (argv[0] == NULL)  
    return;   /* ignore empty lines */
  
  pid_t pid;														//Variable for the processes' PID
  sigset_t mask;													//Creating a mask to block out SIGCHLD, SIGINT, and SIGTSTP signals prior to forking to prevent a race
  sigemptyset(&mask);
  sigaddset(&mask, SIGCHLD);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGTSTP);
  
  if (!builtin_cmd(argv))											//If it's not a built-in command...
  {
	  sigprocmask(SIG_BLOCK, &mask, NULL);							//Blocking signals prior to forking
	  if ((pid = fork()) == 0)										//Fork here, child enters the if statement body
	  {
		  setpgid(0, 0);											//Set the child's PID to 0
		  sigprocmask(SIG_UNBLOCK, &mask, NULL);					//Unblock the signals
		  if (execve(argv[0], argv, environ) < 0)					//If the command can't be run, print "Command not found" and exit normally
		  {
			  printf("%s: Command not found.\n", argv[0]);
			  exit(0);
		  }
	  }
	  if (!bg)														//If the process should run in the foreground...
	  {
		  addjob(jobs, pid, FG, cmdline);							//Add it to the jobs list, noted that it is in the foreground
		  sigprocmask(SIG_UNBLOCK, &mask, NULL);					//Unblock the signals after the process has been added to the list
		  waitfg(pid);												//Wait for the child process to complete
	  }
	  else
	  {																//Otherwise, if the process should run in the background...
		  addjob(jobs, pid, BG, cmdline);							//Add it to the jobs list, noted that it is in the background
		  sigprocmask(SIG_UNBLOCK, &mask, NULL);					//Unblock the signals after the process has been added to the list
		  printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);		//Print to show the use that the process is running in the background
	  }
  }
  return;
}


/////////////////////////////////////////////////////////////////////////////
//
// builtin_cmd - If the user has typed a built-in command then execute
// it immediately. The command name would be in argv[0] and
// is a C string. We've cast this to a C++ string type to simplify
// string comparisons; however, the do_bgfg routine will need 
// to use the argv array as well to look for a job number.
//
int builtin_cmd(char **argv) 
{
  string cmd(argv[0]);
  if (cmd == "quit")												//If the user inputs "quit", exit normally
  {
	  exit(0);
  }
  else if (cmd == "bg" || cmd == "fg")								//If the user inputs "bg" or "fg", run the do_bgfg function and return 1
  {
	  do_bgfg(argv);
	  return 1;
  }
  else if (cmd == "jobs")											//If the user inputs "jobs", print out the list of jobs and return 1
  {
	  listjobs(jobs);
	  return 1;
  }
  else
  {	
	  return 0;														//Otherwise, the user didn't input a builtin command and we return 0
  }
}

/////////////////////////////////////////////////////////////////////////////
//
// do_bgfg - Execute the builtin bg and fg commands
//
void do_bgfg(char **argv) 
{
  struct job_t *jobp=NULL;
    
  /* Ignore command if no argument */
  if (argv[1] == NULL) {
    printf("%s command requires PID or %%jobid argument\n", argv[0]);
    return;
  }
    
  /* Parse the required PID or %JID arg */
  if (isdigit(argv[1][0])) {
    pid_t pid = atoi(argv[1]);
    if (!(jobp = getjobpid(jobs, pid))) {
      printf("(%d): No such process\n", pid);
      return;
    }
  }
  else if (argv[1][0] == '%') {
    int jid = atoi(&argv[1][1]);
    if (!(jobp = getjobjid(jobs, jid))) {
      printf("%s: No such job\n", argv[1]);
      return;
    }
  }	    
  else {
    printf("%s: argument must be a PID or %%jobid\n", argv[0]);
    return;
  }

  //
  // You need to complete rest. At this point,
  // the variable 'jobp' is the job pointer
  // for the job ID specified as an argument.
  //
  // Your actions will depend on the specified command
  // so we've converted argv[0] to a string (cmd) for
  // your benefit.
  //
  string cmd(argv[0]);
  if (cmd == "fg")													//If the process should run in the foreground...
  {
	  if (jobp -> state == ST)										//If it's been stopped, continue it
	  {
		  kill(-jobp -> pid, SIGCONT);
	  }
	  jobp -> state = FG;											//Then set its state to the foreground
	  waitfg(jobp -> pid);											//And wait for it to complete
  }
  else if (cmd == "bg")												//If the process should run in the background...
  {
	  kill(-jobp -> pid, SIGCONT);									//Continue it
	  jobp -> state = BG;											//Set its state to the background
	  printf("[%d] (%d) %s", jobp -> jid, jobp -> pid, jobp -> cmdline);//Print to show the user that the process is running in the background
  }
  return;
}

/////////////////////////////////////////////////////////////////////////////
//
// waitfg - Block until process pid is no longer the foreground process
//
void waitfg(pid_t pid)
{
  while (pid == fgpid(jobs))										//Wait until the process is out of the foreground by sleeping until a signal is received
  {
	  sleep(5);
  }
}

/////////////////////////////////////////////////////////////////////////////
//
// Signal handlers
//


/////////////////////////////////////////////////////////////////////////////
//
// sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
//     a child job terminates (becomes a zombie), or stops because it
//     received a SIGSTOP or SIGTSTP signal. The handler reaps all
//     available zombie children, but doesn't wait for any other
//     currently running children to terminate.  
//
void sigchld_handler(int sig) 
{
  pid_t pid;														//Create local variables for the pid and status
  int status;
  
  while ((pid = waitpid(-1, &status, WNOHANG|WUNTRACED)) > 0)		//While there are still children to be reaped...
  {
	  if (WIFEXITED(status))										//If a SIGCHLD is received, delete the job
	  {
		  deletejob(jobs, pid);
	  }
	  else if (WIFSIGNALED(status))									//If a SIGINT is received, print out that the job has been terminated and delete the job
	  {
		  printf("Job [%d] (%d) terminated by signal %d \n", pid2jid(pid), pid, WTERMSIG(status));
		  deletejob(jobs, pid);
	  }
	  else if (WIFSTOPPED(status))									//If a SIGTSTP is received, print out that the job has been stopped and set its state to ST to indicate that it's been stopped
	  {
		  printf("Job [%d] (%d) stopped by signal %d \n", pid2jid(pid), pid, WSTOPSIG(status));
		  job_t* temp = getjobpid(jobs, pid);
		  temp -> state = ST;
	  }
  }
}

/////////////////////////////////////////////////////////////////////////////
//
// sigint_handler - The kernel sends a SIGINT to the shell whenver the
//    user types ctrl-c at the keyboard.  Catch it and send it along
//    to the foreground job.  
//
void sigint_handler(int sig) 
{
  pid_t pid = fgpid(jobs);										
  if (pid != 0)														//Send the process group a SIGINT signal and reap accordingly
  {
	  kill(-pid, SIGINT);
  }
  return;
}

/////////////////////////////////////////////////////////////////////////////
//
// sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
//     the user types ctrl-z at the keyboard. Catch it and suspend the
//     foreground job by sending it a SIGTSTP.  
//
void sigtstp_handler(int sig) 
{
  pid_t pid = fgpid(jobs);
  if (pid != 0)														//Send the process group a SIGTSTP signal set the state to ST to indicate that it's been stopped
  {
	  kill(-pid, SIGTSTP);
	  job_t* temp = getjobpid(jobs, pid);
	  temp -> state = ST;
  }
  return;
}

/*********************
 * End signal handlers
 *********************/




