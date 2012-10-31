/*
 * Config.h
 *
 *  Created on: Dec 25, 2011
 *      Author: jaap
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "LogEvent.h"

extern LogEvent* logf;

class Config {

public:
	Config();
	virtual ~Config();
	void readURI(char *envp[]);
	int readArgs(int argc, char *argv[], char *envp[]);

	char jobname[32];
	char user[32];
	char title[32];
	char pstoedit[256];
	char tmpdir[256];
	char options[4096];
	char filename[256];
	char ipAddress[15];
	unsigned int port;
	char myURI[256];
	char LaserCuttingPower[40];
	char LaserCuttingSpeed[40];
	char LaserMarkingPower[40];
	char LaserMarkingSpeed[40];

private:

};

#endif /* CONFIG_H_ */
