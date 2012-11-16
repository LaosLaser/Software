/*
 * LogEvent.h
 *
 *  Created on: Dec 26, 2011
 *      Author: jaap
 */

#ifndef LOGEVENT_H_
#define LOGEVENT_H_

#include <stdio.h>
#include <string.h>

#define CPERROR		1
#define CPSTATUS	2
#define CPDEBUG		4
#define LOGLEVEL	7

class LogEvent {
public:
	LogEvent();
	virtual ~LogEvent();
	void SetLogLevel(int loglevel);
	void Log(int type, char *message);
	int isDebug();

	int loglevel;
};

#endif /* LOGEVENT_H_ */
