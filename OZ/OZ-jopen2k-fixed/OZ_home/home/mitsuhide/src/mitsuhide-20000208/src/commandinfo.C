#include <stdio.h>
#include "mymath.h"
#include "commandinfo.h"

double min_sin(double dir, double dir_error) 
{
  double min, max;

  if (dir < -180 || dir > 180) {
    printf("min_sin() : input error\n");
    exit(0);
  }
  if (dir_error < 0 || 180 < dir_error) {
    printf("min_sin() : input error\n");
    exit(0);
  }
  min = dir - dir_error;
  max = dir + dir_error;
  if (min <= -90 && -90 <= max) 
    return -1;
  else if (min <= 270 && 270 <= max)
    return -1;
  else {
    if (Sin(min) < Sin(max))
      return Sin(min);
    else 
      return Sin(max);
  }
}

double max_sin(double dir, double dir_error) 
{
  double min, max;

  if (dir < -180 || dir > 180) {
    printf("max_sin() : input error\n");
    exit(0);
  }
  if (dir_error < 0 || 180 < dir_error) {
    printf("max_sin() : input error\n");
    exit(0);
  }
  min = dir - dir_error;
  max = dir + dir_error;
  if (min <= -270 && -270 <= max) 
    return 1;
  else if (min <= 90 && 90 <= max)
    return 1;
  else {
    if (Sin(min) < Sin(max))
      return Sin(max);
    else 
      return Sin(min);
  }
}

double min_cos(double dir, double dir_error) 
{
  double min, max;

  if (dir < -180 || dir > 180) {
    printf("min_cos() : input error\n");
    exit(0);
  }
  if (dir_error < 0 || 180 < dir_error) {
    printf("min_cos() : input error\n");
    exit(0);
  }
  min = dir - dir_error;
  max = dir + dir_error;
  if (min <= -180 && -180 <= max) 
    return -1;
  else if (min <= 180 && 180 <= max)
    return -1;
  else {
    if (Cos(min) < Cos(max))
      return Cos(min);
    else 
      return Cos(max);
  }
}

double max_cos(double dir, double dir_error) 
{
  double min, max;

  if (dir < -180 || dir > 180) {
    printf("max_cos() : input error\n");
    exit(0);
  }
  if (dir_error < 0 || 180 < dir_error) {
    printf("max_cos() : input error\n");
    exit(0);
  }
  min = dir - dir_error;
  max = dir + dir_error;
  if (min <= -360 && -360 <= max)
    return 1;
  else if (min <= 0 && 0 <= max) 
    return 1;
  else if (min <= 360 && 360 <= max)
    return 1;
  else {
    if (Cos(min) < Cos(max))
      return Cos(max);
    else 
      return Cos(min);
  }
}
