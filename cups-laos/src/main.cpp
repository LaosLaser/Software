/* cups-laos.cpp -- CUPS Backend (version 0.0.2, 2011-12-25)

   25.12.2011, Jaap Vermaas
   http://www.fablabtruck.nl

   inspired by:
   cups-pdf driver: Volker C. Behr, http://www.cups-pdf.de
        <behr@physik.uni-wuerzburg.de>
   cups-epilog driver: AS220 Labs http://www.as220.org <brandon@as220.org>

   This code may be freely distributed as long as this header
   is preserved.

   This code is distributed under the GPL.
   (http://www.gnu.org/copyleft/gpl.html)

   ---------------------------------------------------------------------------

   Copyright (C) 2010, 2011  Jaap Vermaas

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

   ---------------------------------------------------------------------------

   If you want to redistribute modified sources/binaries this header
   has to be preserved and all modifications should be clearly
   indicated.
   In case you want to include this code into your own programs
   I would appreciate your feedback via email.

   HISTORY: see ChangeLog in the parent directory of the source archive
*/

#include "Config.h"
#include "LogEvent.h"

#define CPVERSION       "0.0.2"

char psfile[256], gcodefile[256];
LogEvent* logf;
Config* cfg;
char logstring[256];

void readfile(int argc, char *argv[]) {
	// actually read the postscript file
	FILE *file_ps, *file_cups;
	char buff[512];
	file_ps = fopen(psfile, "w");
	if (!file_ps) {
		sprintf(logstring, "Cannot create ps-file (%s)", psfile);
		logf->Log(CPERROR, logstring);
		exit (EXIT_FAILURE);
	}
	sprintf(logstring, "Created ps-file (%s)", psfile);
	logf->Log(CPDEBUG, logstring);
     if (argc > 6)
    	 file_cups = fopen(argv[6], "r");
	 else
		 file_cups = stdin;
	 if (!file_cups) {
		 sprintf(logstring, "Cannot open input file");
		 logf->Log(CPERROR, logstring);
		 exit (EXIT_FAILURE);
	 }
	 while (fgets((char *)buff, sizeof (buff), file_cups))
		 fprintf(file_ps, "%s", (char *)buff);
	 fclose(file_cups);
	 fclose(file_ps);
}

void gen_gcode() {
	// generate gcode with pstoedit
	char pscall[4096];
	sprintf(psfile, "%s.ps", cfg->filename);
	// sprintf(pscall, "%s -f \"laos:-configname cutting\" %s %s", cfg->pstoedit, psfile, cutfile);
	sprintf(pscall, "%s %s %s", cfg->pstoedit, psfile, gcodefile);
	sprintf(logstring, "pstoedit command built (%s)", pscall);
	logf->Log(CPDEBUG, logstring);
	system(pscall);
}

void upload_gcode() {
	char cmd[256];
	sprintf(cmd, "/usr/bin/tftp %s %d -m binary -c put %s %s.lgc", cfg->ipAddress, cfg->port, gcodefile, cfg->title);
	sprintf(logstring, "tftp cmnd: %s", cmd);
	logf->Log(CPDEBUG, logstring);
	system(cmd);
}

void delete_tmpfiles() {
	// delete tmpfiles
	if (logf->isDebug()) {
		unlink(psfile);
		unlink(gcodefile);
	}
}

void namefiles() {
	sprintf(psfile, "%s.ps", cfg->filename);
	sprintf(gcodefile, "%s.lgc", cfg->filename);
}



int main(int argc, char *argv[], char *envp[]) {
  logf = new LogEvent();
  cfg = new Config();
  cfg->readURI(envp);
  if (! cfg->readArgs(argc, argv, envp)) {
	namefiles();
    readfile(argc, argv);
    gen_gcode();
    upload_gcode();
    //delete_tmpfiles();
  }
  return 0;
}

