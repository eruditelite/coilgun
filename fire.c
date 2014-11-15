/*
  fire.c

  Fire a coil gun.

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
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <wiringPi.h>

#include "msp.h"

enum command {
	COMMAND_ON, COMMAND_OFF, COMMAND_DELAY,
	COMMAND_LOOP, COMMAND_LOG, COMMAND_RUN,
	COMMAND_END
};

struct action {
	enum command command;
	int argument;
};

#define MAX_ACTIONS 100

static struct action actions[MAX_ACTIONS];

static void
sigint_handler(int unused)
{
	fprintf(stderr, "Caught Ctrl-C!  Closing up...\n");
	digitalWrite(0, 0);

	exit(EXIT_FAILURE);
}

static int
log_commands(struct action *actions)
{
	while (COMMAND_END != actions->command) {
		switch (actions->command) {
		case COMMAND_ON:
			printf("Turn On Coil %d\n", actions->argument);
			break;
		case COMMAND_OFF:
			printf("Turn Off Coil %d\n", actions->argument);
			break;
		case COMMAND_DELAY:
			printf("Delay for %d microseconds\n",
			       actions->argument);
			break;
		case COMMAND_LOOP:
			printf("Loop\n");
			break;
		default:
			fprintf(stderr, "Invalid Command!\n");

			return EXIT_FAILURE;
			break;
		}

		++actions;
	}

	return EXIT_SUCCESS;
}

static int
run_commands(struct action *actions)
{
	struct action *temp;

	temp = actions;

	while (COMMAND_END != temp->command) {
		switch (temp->command) {
		case COMMAND_ON:
			digitalWrite(temp->argument, ON);
			break;
		case COMMAND_OFF:
			digitalWrite(temp->argument, OFF);
			break;
		case COMMAND_DELAY:
			msp_udelay(temp->argument);
			break;
		case COMMAND_LOOP:
			temp = actions;
			continue;
			break;
		default:
			fprintf(stderr,
				"Unknown Command: %d!\n", temp->command);
			break;
		}

		++temp;
	}

	return EXIT_SUCCESS;
}

int
main(int argc, char *argv[])
{
	int i;
	int j;
	int log = 0;
	int run = 0;
	int return_code = EXIT_SUCCESS;

	memset(&actions[0], 0, sizeof(struct action) * MAX_ACTIONS);

	i = 1;
	j = 0;

	while (i < argc) {
		char *input;
		char *command;
		char *argument;

		input = malloc(strlen(argv[i]) + 1);
		strncpy(input, argv[i], strlen(argv[i]) + 1);
		++i;
		command = strtok(input, ":");
		argument = strtok(NULL, ":");

		if (0 == strncmp(command, "on", 2)) {
			actions[j].command = COMMAND_ON;
		} else if (0 == strncmp(command, "off", 3)) {
			actions[j].command = COMMAND_OFF;
		} else if (0 == strncmp(command, "delay", 5)) {
			actions[j].command = COMMAND_DELAY;
		} else if (0 == strncmp(command, "loop", 4)) {
			actions[j].command = COMMAND_LOOP;
		} else if (0 == strncmp(command, "log", 4)) {
			log = 1;
			continue;
		} else if (0 == strncmp(command, "run", 4)) {
			run = 1;
			continue;
		} else {
			fprintf(stderr, "Unknown Command: %s\n", command);

			return EXIT_FAILURE;
		}

		if (COMMAND_ON == actions[j].command ||
		    COMMAND_OFF == actions[j].command ||
		    COMMAND_DELAY == actions[j].command)
			actions[j].argument = strtol(argument, NULL, 10);

		free(input);
		++j;
	}

	actions[j].command = COMMAND_END;

	if (0 != log) {
		return_code = log_commands(&actions[0]);

		if (EXIT_SUCCESS != return_code) {
			fprintf(stderr, "Logging Failed!\n");

			return EXIT_FAILURE;
		}
	}

	if (0 != run) {
		struct action *temp;

		/* Catch Control-C */
		signal(SIGINT, sigint_handler);

		/* Initialize the Pi Wire Library */
		wiringPiSetup();

		temp = &actions[0];

		while (COMMAND_END != temp->command) {
			if (COMMAND_ON == temp->command ||
			    COMMAND_OFF == temp->command)
				pinMode(temp->argument, OUTPUT);

			++temp;
		}

		return_code = run_commands(&actions[0]);
		
		if (EXIT_SUCCESS != return_code) {
			fprintf(stderr, "Running Failed!\n");

			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
