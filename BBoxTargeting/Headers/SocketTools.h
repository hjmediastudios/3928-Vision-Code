/*
 * SocketTools.h
 *
 *  Created on: Mar 11, 2012
 *      Author: frc3928
 */

#ifndef SOCKETTOOLS_H_
#define SOCKETTOOLS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "../Constants.h"

int openSocket();
int writeToSocket(char* data);

#endif /* SOCKETTOOLS_H_ */
