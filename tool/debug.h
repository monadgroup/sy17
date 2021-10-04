#pragma once

//#define LOG_OUT_GROUPS

void log(const char *str, bool addGlEntry = true);
void dbgEnter(const char *name);
void dbgExit();
