#pragma once

#include <Windows.h>
#include <stdio.h>

void err_quit(const char* msg);
void err_display(const char* msg);
void err_quitW(const wchar_t* msg);
void err_displayW(const wchar_t* msg);