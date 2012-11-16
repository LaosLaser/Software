/*
 * LogEvent.cpp
 *
 *  Created on: Dec 26, 2011
 *      Author: jaap
 */

#include "LogEvent.h"

LogEvent::LogEvent() {
	loglevel = LOGLEVEL;
}

LogEvent::~LogEvent() {
	// No destructor yet
}

void LogEvent::SetLogLevel (int level) {
	loglevel = level;
}

void LogEvent::Log(int type, char *message) {
	if (type & loglevel) {
		fprintf(stderr,"CRIT: %s\n", message);
	}
}

int LogEvent::isDebug() {
	return (loglevel & CPDEBUG);
}
