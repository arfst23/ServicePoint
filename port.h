//******************************************************************************

#pragma once

void port_create(int port);
void port_close();

void port_write(const char *str, int len);
int port_read_char();

//******************************************************************************
