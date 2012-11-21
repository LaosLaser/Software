/*
 * Config.cpp
 *
 *  Created on: Dec 25, 2011
 *      Author: jaap
 */

#include "Config.h"

extern char logstring[256];

Config::Config() {
	sprintf(jobname, "laos");
	sprintf(user, "nobody");
	sprintf(title, "unnamed");
	sprintf(pstoedit, "TMPDIR=/tmp /usr/local/bin/pstoedit");
	sprintf(tmpdir, "/var/spool/cups/tmp");
	sprintf(ipAddress, "127.0.0.1");
	port = 69;
	sprintf(myURI, "cups-laos://127.0.0.1:69/laos");
	sprintf(filename, "%s/%s-%d", tmpdir, jobname, getpid());
	sprintf(logstring, "Created filename (%s)", filename);
	//logf->Log(CPDEBUG, logstring);
}

Config::~Config() {
	// Nothing to be done for destroyer
}

void Config::readURI(char *envp[]) {
	// Read ipAddress and port from URI
	char *uri;
	char *host, *iport;
	if (getenv("DEVICE_URI")) {
		uri = getenv("DEVICE_URI");
		const char delimiters[] = ":/";
		strtok (uri, delimiters);
		host = strtok (NULL, delimiters);
		iport = strtok (NULL, delimiters);
		sprintf(ipAddress, "%s", host);
		port = atoi(iport);
		sprintf(myURI, "cups-laos://%s:%d",
			ipAddress, port);
		sprintf(logstring, "Read URI (%s)", myURI);
		logf->Log(CPDEBUG, logstring); 
	} else {
		sprintf(logstring, "No URI");
		logf->Log(CPDEBUG, logstring);
	}
}

int Config::readArgs(int argc, char *argv[], char *envp[]) {
	// Read cmdline arguments
	char *mytmpdir = getenv("TMDIR");
	if (mytmpdir)
			sprintf(tmpdir, "%s", mytmpdir);
	sprintf(logstring, "Start process commandline");
	logf->Log(CPDEBUG, logstring);
	if (argc==1) { 		// status call from CUPS, or no input on cmdline
	    printf("file %s \"Generic Laser Driver\" \"CUPS-LAOS\" \"MFG:Generic;MDL:CUPS-LAOS Printer;DES:Generic CUPS-LAOS Printer;CLS:PRINTER;CMD:POSTSCRIPT;\"\n", myURI);
	    sprintf(logstring, "identification string sent");
	    logf->Log(CPSTATUS, logstring);
	    return 1;
	}
	if (argc<6 || argc>7) {
	    (void) fputs("Usage: cups-laos job-id user title copies options [file]\n", stderr);
	    sprintf(logstring, "call contained illegal number of arguments");
	    logf->Log(CPERROR, logstring);
	    return 1;
	}

	sprintf(jobname, "%s", argv[1]);
	sprintf(logstring, "jobname set (%s)", jobname);
	logf->Log(CPDEBUG, logstring);
	sprintf(user, "%s", argv[2]);
	sprintf(logstring, "username (%s)", user);
	logf->Log(CPDEBUG, logstring);
	char lt[256];
	sprintf(lt, "%s", &argv[3][6]);

	char nochar[] = "\"'?*,;=+#>|[]/\\";
	int c=0, x=0;
	int max = strlen(lt);
	if (max > 16) max=16;
	while (x<max) {
		int legal = 1;
		legal *= ((lt[x] > 32) && (lt[x] < 126));
		for (int y=0; y<strlen(nochar); y++)
			legal *= lt[x] != nochar[y];
		if (legal) title[c++] =  lt[x];
		x++;
	}
	title[c] = 0;
	sprintf(logstring, "title (%s)", title);
	logf->Log(CPDEBUG, logstring);
	
	// argv[4] is # copies, we do not send multiple copies to the lasercutter
	sprintf(options, "%s", argv[5]);
	sprintf(logstring, "options (%s)", options);
	logf->Log(CPDEBUG, logstring);

	//process options
	char *option, *xp;
	const char delim[] = " \t\n";
	x = 0;
	char option_name[256];
	  option = strtok(options, delim);
	  while (option != NULL) {
	    strcpy(option_name,"");
	    xp = strchr(option, '=');
	    if (xp != NULL) {
	      x = xp - option;
	    } else {
	      x = strlen(option);
	    }
	    strncat (option_name, option, x);
	    xp++;
	    //_assign_value(option_name, option);
        /*
	    if (!strcmp("LaserCuttingPower", option_name)) {
	      strcpy(LaserCuttingPower, option);
	      sprintf(logstring, "option read (%s)", option_name);
	      logf->Log(CPDEBUG, logstring);
	    }
	    if (!strcmp("LaserCuttingSpeed", option_name)) {
	      strcpy(LaserCuttingSpeed, option);
	      sprintf(logstring, "option read (%s)", option_name);
	      logf->Log(CPDEBUG, logstring);
	    }
        */
        sprintf(logstring, "option %s read (%s)", option_name, option);
        logf->Log(CPDEBUG, logstring);
	    option = strtok(NULL, delim);
	  }
	sprintf(logstring, "readArgs done");
	logf->Log(CPDEBUG, logstring);
    return 0;
}
