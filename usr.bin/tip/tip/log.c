/*
 * Copyright (c) 1983, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef lint
#if 0
static char sccsid[] = "@(#)log.c	8.1 (Berkeley) 6/6/93";
#endif
static const char rcsid[] =
	"$Id: log.c,v 1.2 1997/08/18 07:16:03 charnier Exp $";
#endif /* not lint */

#include "tipconf.h"
#include "tip.h"
#include <err.h>

#if ACULOG
static	FILE *flog = NULL;

/*
 * Log file maintenance routines
 */

void
logent(group, num, acu, message)
	char *group, *num, *acu, *message;
{
	char *user, *timestamp;
	struct passwd *pwd;
	time_t t;

	if (flog == NULL)
		return;
	if (flock(fileno(flog), LOCK_EX) < 0) {
		warn("flock");
		return;
	}
	if ((user = getlogin()) == NOSTR)
		if ((pwd = getpwuid(getuid())) == NOPWD)
			user = "???";
		else
			user = pwd->pw_name;
	t = time(0);
	timestamp = ctime(&t);
	timestamp[24] = '\0';
	fprintf(flog, "%s (%s) <%s, %s, %s> %s\n",
		user, timestamp, group,
#if PRISTINE
		"",
#else
		num,
#endif
		acu, message);
	(void) fflush(flog);
	(void) flock(fileno(flog), LOCK_UN);
}

void
loginit()
{
	flog = fopen(value(LOG), "a");
	if (flog == NULL)
		fprintf(stderr, "can't open log file %s.\r\n", value(LOG));
}
#endif
