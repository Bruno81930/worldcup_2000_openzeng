#include <stdio.h>
#include "mymath.h"
#include "utility.h"

char* next_token(char* msg)
{
  int i = 0;

  while (true) {
    if (*msg == '(')
      i++;
    else if (*msg == ')') {
      if (i == 0) {
	*msg = '\0';
	return NULL;
      }
      i--;
    }
    else if (*msg == ' ' && i == 0)
      break;
    else if (*msg == '\0')
      return NULL;
    msg++;
  }
  *msg = '\0';
  msg++;

  return msg;
}

double normalize_angle(double dir)
{
  while(1) {
    if (dir < Rad2Deg(-PI))
      dir += 2 * Rad2Deg(PI);
    else if (dir > Rad2Deg(PI))
      dir -= 2 * Rad2Deg(PI);
    else
      break;
  }
  return dir;
}
