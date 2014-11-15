#    This file is part of coilgun.
#
#    coilgun is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    coilgun is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with coilgun.  If not, see <http://www.gnu.org/licenses/>.

all: clean fire

clean:
	rm -f fire *.o

delay.o: delay.c
	gcc -g -Wall -c -o $@ $<

fire: fire.c delay.o
	gcc -g -Wall -o $@ $^ -lwiringPi
