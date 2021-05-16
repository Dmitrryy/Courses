//
// Created by artem on 20.04.2021.
//
#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sysinfo.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>


struct sockaddr_in
get_hello_message (int *error);

int connect_to_server (struct sockaddr_in serv_addr, int *error);

int do_computation (int socket);
