//
// Created by fonkam on 6/3/19.
//

#ifndef FINALYEARPROJECT_DEPENDENCIES_H
#define FINALYEARPROJECT_DEPENDENCIES_H


#define DEFAULTFLOOD  100000
#define DEFAULTSIZE 1000
/*
 * Returns int conresponding to using option from option list
 */
short actionList(void);

/*
 * Print usage documentation of the program, similar to it man page
 */
void usage(void);

/*
 * Return int from 0 to arg1 and resets if arg2 is false
 */
long floodCount( bool reset);

/**
 * Returns number of seconds between b and a.
 */
double calculate(const struct rusage *b, const struct rusage *a);

#endif //FINALYEARPROJECT_DEPENDENCIES_H
