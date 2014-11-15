/*
  delay.c

  Provide a delay.

  ------------------------------------------------------------------------------

  This file is part of coilgun.

  coilgun is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  coilgun is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with coilgun.  If not, see <http://www.gnu.org/licenses/>.

  Copyright 2014 John Jacques <eruditelite@gmail.com>
*/

#include <stdio.h>
#include <time.h>

void
msp_mdelay(unsigned long milliseconds)
{
	struct timespec start;
	struct timespec end;
	int rc;

	start.tv_sec = (milliseconds / 1000);
	milliseconds -= (start.tv_sec * 1000);
	start.tv_nsec = 1000000 * milliseconds;

	rc = nanosleep(&start, &end);

	if (0 > rc)
		fprintf(stderr,
			"nanosleep() returned %lu.%lu!\n",
			end.tv_sec, end.tv_nsec);

	return;
}

void
msp_udelay(unsigned long microseconds)
{
	struct timespec start;
	struct timespec end;
	int rc;

	start.tv_sec = (microseconds / 1000000);
	microseconds -= (start.tv_sec * 1000000);
	start.tv_nsec = 1000 * microseconds;

	rc = nanosleep(&start, &end);

	if (0 > rc)
		fprintf(stderr,
			"nanosleep() returned %lu.%lu!\n",
			end.tv_sec, end.tv_nsec);

	return;
}
