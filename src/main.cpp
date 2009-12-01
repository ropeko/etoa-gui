//////////////////////////////////////////////////
//   ____    __           ______                //
//  /\  _`\ /\ \__       /\  _  \               //
//  \ \ \L\_\ \ ,_\   ___\ \ \L\ \              //
//   \ \  _\L\ \ \/  / __`\ \  __ \             //
//    \ \ \L\ \ \ \_/\ \L\ \ \ \/\ \            //
//     \ \____/\ \__\ \____/\ \_\ \_\           //
//      \/___/  \/__/\/___/  \/_/\/_/  	        //
//                                              //
//////////////////////////////////////////////////
// The Andromeda-Project-Browsergame            //
// Ein Massive-Multiplayer-Online-Spiel         //
// Programmiert von Nicolas Perrenoud           //
// www.nicu.ch | mail@nicu.ch                   //
// als Maturaarbeit '04 am Gymnasium Oberaargau	//
//////////////////////////////////////////////////

/**
* Startup function, bootstraps the daemon and 
* initializes threads, logging and pidfile.
*
* @author Nicolas Perrenoud<mrcage@etoa.ch>
* 
* Copyright (c) 2004 by EtoA Gaming, www.etoa.net
*
* $Rev$
* $Author$
* $Date$
*/

#include "etoa.h"
#include "version.h"

using namespace std;

std::string gameRound;
std::string pidFile;

PIDFile* pf;

bool detach = false;

int ownerUID;

std::string appPath;

// Signal handler
void sighandler(int sig)
{
	// Clean up pidfile
	delete pf;
	
	if (sig == SIGTERM)
	{
		LOG(LOG_NOTICE,"Received ordinary termination signal (SIGTERM), shutting down");		
		exit(EXIT_SUCCESS);
	}
	if (sig == SIGINT)
	{
		LOG(LOG_WARNING,"Received interrupt from keyboard (SIGINT), shutting down");		
		exit(EXIT_SUCCESS);
	}

	LOG(LOG_ERR,"Caught signal "<<sig<<", shutting down due to error");		
	exit(EXIT_FAILURE);
	
	// Restart after segfault
	/*
	if (sig==SIGSEGV)
	{
		std::string cmd = appPath + " "+(detach ? "-d" : "")+" -k -r "+gameRound;
		system(cmd.c_str());
	}*/
	
	
}

// Create a daemon
void daemonize()
{
  pid_t pid, sid;
  /* Fork off the parent process */
  pid = fork();
  if (pid < 0) 
  {
  	LOG(LOG_CRIT,  "Could not fork parent process");
 		exit(EXIT_FAILURE);
  }

  /* If we got a good PID, then we can exit the parent process. */
  if (pid > 0) 
  {
    exit(EXIT_SUCCESS);
  }
  
 	/* Close out the standard file descriptors */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

  /* Change the file mode mask */
  umask(0);
          
  /* Create a new SID for the child process */
  sid = setsid();
  if (sid < 0) 
  {
  	LOG(LOG_CRIT, "Unable to get SID for child process");
    exit(EXIT_FAILURE);
  }

  // Create pidfile
  pf->write();	

  int myPid = (int)getpid();
	LOG(LOG_NOTICE,"Daemon initialized with PID " << myPid << " and owned by " << getuid());
		
}

/**
* Runs the message queue listener for receiving
* command from the frontend
*/
void msgQueueThread()
{                                   
	LOG(LOG_DEBUG,"Entering message queue thread");				
	
	IPCMessageQueue queue(Config::instance().getFrontendPath());
	if (queue.valid())
	{
		while (true)
		{
			std::string cmd = "";
			int id = 0;
			queue.rcvCommand(&cmd,&id);
			
			if (cmd == "planetupdate")
			{
				EntityUpdateQueue::instance().push(id);
			}
			else if (cmd == "configupdate")
			{
				Config::instance().reloadConfig();
			}
		}
	}
	LOG(LOG_ERR,"Entering message queue ended");				
}

int main(int argc, char* argv[])
{
	// Register signal handlers
  signal(SIGABRT, &sighandler);
	signal(SIGTERM, &sighandler);
	signal(SIGINT, &sighandler);
	signal(SIGHUP, &sighandler);
	signal(SIGSEGV, &sighandler);
	signal(SIGQUIT, &sighandler);
	signal(SIGFPE, &sighandler);

	appPath = std::string(argv[0]);

	// Parse command line
	AnyOption *opt = new AnyOption();
  opt->addUsage( "Usage: " );
  opt->addUsage( "" );
  opt->addUsage( " -r  --round roundname   Select round to be used (necessary)");
  opt->addUsage( " -u  --uid userid        Select user id under which it runs (necessary if you are root)");
  opt->addUsage( " -p  --pidfile path      Select path to pidfile");
  opt->addUsage( " -k  --killexisting      Kills an already running instance of this backend before starting this instance");
  opt->addUsage( " -s  --stop              Stops a running instance of this backend");
  opt->addUsage( " -d  --daemon            Detach from console and run as daemon in background");
  opt->addUsage( " -l  --log level       	 Specify log level (0=emerg, ... , 7=everything");
  opt->addUsage( " --debug       					 Enable debug mode");
  opt->addUsage( " -h  --help              Prints this help");
  opt->addUsage( " --version           Prints version information");
  opt->setFlag("help",'h');
  opt->setFlag("version");
  opt->setFlag("killexisting",'k');
  opt->setFlag("stop",'s');
  opt->setFlag("daemon",'d');
  opt->setFlag("debug");
  opt->setOption("log",'l');
  opt->setOption("userid",'u');
  opt->setOption("round",'r');
  opt->setOption("pidfile",'p');  
  opt->processCommandArgs( argc, argv );
	if( ! opt->hasOptions()) 
	{ 
    opt->printUsage();
	 	return EXIT_FAILURE;
	}  
  if( opt->getFlag( "help" ) || opt->getFlag( 'h' )) 
  {	
  	opt->printUsage();
 		return EXIT_SUCCESS;
	}
  if( opt->getFlag( "version" )) 
  {	
  	std::cout << getVersion()<<endl;
 		return EXIT_SUCCESS;
	}
	bool killExistingInstance = false;
  if( opt->getFlag( "killexisting" ) || opt->getFlag( 'k' )) 
  {	
		killExistingInstance = true;
	}
	bool stop = false;
  if( opt->getFlag( "stop" ) || opt->getFlag( 's' )) 
  {	
		stop = true;
	}
  if( opt->getFlag( "daemon" ) || opt->getFlag( 'd' )) 
  {	
		detach = true;
	}
	else
	{
		std::cout << "Escape to Andromeda Event-Handler" << std::endl;
		std::cout << "(C) 2007 EtoA Gaming Switzerland, www.etoa.ch" << std::endl;
		std::cout << "Version " <<versionNumber() << std::endl<< std::endl;
	}
	

  if( opt->getFlag( "debug" )) 
  {	
  	debugEnable(1);
	}

	logPrio(LOG_NOTICE);
  if( opt->getValue('l') != NULL) 
  {	
  	int lvl = atoi(opt->getValue('l'));
  	if (LOG_DEBUG >= lvl && lvl >= LOG_EMERG)
  	{
  		std::cout << "Setting log verbosity to " << lvl << std::endl;
			logPrio(lvl);
		}
	}
  else if( opt->getValue("log") != NULL) 
  {	
  	int lvl = atoi(opt->getValue("log"));
  	if (LOG_DEBUG >= lvl && lvl >= LOG_EMERG)
  	{
  		std::cout << "Setting log verbosity to " << lvl << std::endl;
			logPrio(lvl);
		}
	}			

	
	if( opt->getValue( 'r' ) != NULL)
		gameRound = opt->getValue( 'r' );
	else if (opt->getValue("round") != NULL )
		gameRound = opt->getValue("round");
	else
	{
		std::cerr << "Error: No gameround name given!"<<endl;	
	 	return EXIT_FAILURE;
	}
	logProgam(gameRound);
	
	if( opt->getValue('p') != NULL)
		pidFile = opt->getValue('p');
	else if (opt->getValue("pidfile") != NULL )
		pidFile = opt->getValue("pidfile");
	else
		pidFile = "/var/run/etoa/"+gameRound+".pid";
		
	if( opt->getValue('u') != NULL)
		ownerUID = atoi(opt->getValue('u'));
	else if (opt->getValue("uid") != NULL )
		ownerUID = atoi(opt->getValue("uid"));
	else
		ownerUID = (int)getuid();

  // Set correct uid
  if (setuid(ownerUID)!=0)
  {
  	std::cerr << "Unable to change user id" << endl;
    exit(EXIT_FAILURE);  	
  }
  // Check uid
  if (getuid()==0)
  {
  	std::cerr << "This software cannot be run as root!" <<endl;
    exit(EXIT_FAILURE);  	
  }  

  pf = new PIDFile(pidFile);

	// Check for existing instance
	if (pf->fileExists())
	{
		int existingPid = pf->readPid();
   	
   	if (stop)
   	{
   		kill(existingPid,SIGTERM);   
   		std::cout << "Killing process "<<existingPid<<endl;
   		exit(EXIT_SUCCESS);		
   	}
		if (killExistingInstance)
		{
			std::cout << "EtoA Daemon " << gameRound << " seems to run already with PID "<<existingPid<<"! Killing this instance..." << std::endl;
			int kres = kill(existingPid,SIGTERM);
			if (kres<0)
			{
				if (errno==EPERM)
				{
					std::cerr << "I am not allowed to kill the instance. Exiting..." << std::endl;
					exit(EXIT_FAILURE);
				}
				else
				{
					std::cerr << "The process doesn't exist, perhaps the PID file was outdated. Continuing..." << std::endl;
				}
			}
			sleep(1);
		}
		else
		{
			std::cerr << "EtoA Daemon " << gameRound << " is already running with PID "<<existingPid<<"!"<<std::endl<<"Use the -k flag to force killing it and continue with this instance. Exiting..." << std::endl;
			exit(EXIT_FAILURE);
		}
	}
 	else if (stop)
 	{
 		std::cerr << "No running process found, exiting..."<<std::endl;
 		return EXIT_FAILURE;		
 	}	

	LOG(LOG_NOTICE,"Starting EtoA event-handler "<<versionNumber()<<" for universe " << gameRound);

	if (detach)
		daemonize();

	Config &config = Config::instance();
	config.setRoundName(gameRound);

	boost::thread mThread(&etoamain);
	boost::thread qThread(&msgQueueThread);

	mThread.join();	
	qThread.join();
	
	// This point should never be reached
	cerr << "Unexpectedly reached end of main()";
	return EXIT_FAILURE;
}
