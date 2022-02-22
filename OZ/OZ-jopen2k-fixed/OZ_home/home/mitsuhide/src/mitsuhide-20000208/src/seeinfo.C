#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mymath.h"
#include "vector.h"
#include "utility.h"
#include "seeinfo.h"

const FlagInfo Flags[] = FLAGINFOTAB;

double calc_static_dist(double see_dist) 
{
  if (see_dist == DIST_ERR)
    return DIST_ERR;
  if (see_dist > 159.2) {
    printf("illigal dist : %f\n", see_dist);
    return DIST_ERR;
  }

  return calc_static_dist2(see_dist, 0, 377);
}
const StaticObjDist static_obj_dist_table[] = STATICOBJDISTTAB;
double calc_static_dist2(double dist, int left, int right) {
  if (left == right) {
    return static_obj_dist_table[left].near_dist;
  } else if (right - left == 1) {
    if (dist == static_obj_dist_table[left].see_dist)
      return static_obj_dist_table[left].near_dist;
    else
      return static_obj_dist_table[right].near_dist;
  } else {
    int center = (left + right) / 2;
    if (dist == static_obj_dist_table[center].see_dist)
      return static_obj_dist_table[center].near_dist;
    else if (dist < static_obj_dist_table[center].see_dist)
      return calc_static_dist2(dist, left, center - 1);
    else
      return calc_static_dist2(dist, center + 1, right);
  }
}

double calc_static_dist_error(double see_dist) 
{
  if (see_dist == DIST_ERR)
    return DIST_ERR;
  if (see_dist > 159.2) {
    printf("illigal dist : %f\n", see_dist);
    return DIST_ERR;
  }

  return calc_static_dist_error2(see_dist, 0, 377);
}
const StaticObjDist static_obj_dist_error_table[] = STATICOBJDISTERRORTAB;
double calc_static_dist_error2(double dist, int left, int right) {
  if (left == right) {
    return static_obj_dist_error_table[left].near_dist;
  } else if (right - left == 1) {
    if (dist == static_obj_dist_error_table[left].see_dist)
      return static_obj_dist_error_table[left].near_dist;
    else
      return static_obj_dist_error_table[right].near_dist;
  } else {
    int center = (left + right) / 2;
    if (dist == static_obj_dist_error_table[center].see_dist)
      return static_obj_dist_error_table[center].near_dist;
    else if (dist < static_obj_dist_error_table[center].see_dist)
      return calc_static_dist_error2(dist, left, center - 1);
    else
      return calc_static_dist_error2(dist, center + 1, right);
  }
}

double calc_dynamic_dist(double see_dist) 
{
  if (see_dist == DIST_ERR)
    return DIST_ERR;
  if (see_dist > 148.4) {
    printf("illigal dist : %f\n", see_dist);
    return DIST_ERR;
  }

  return calc_dynamic_dist2(see_dist, 0, 60);
}
const StaticObjDist dynamic_obj_dist_table[] = DYNAMICOBJDISTTAB;
double calc_dynamic_dist2(double dist, int left, int right) {
  if (left == right) {
    return dynamic_obj_dist_table[left].near_dist;
  } else if (right - left == 1) {
    if (dist == dynamic_obj_dist_table[left].see_dist)
      return dynamic_obj_dist_table[left].near_dist;
    else
      return dynamic_obj_dist_table[right].near_dist;
  } else {
    int center = (left + right) / 2;
    if (dist == dynamic_obj_dist_table[center].see_dist)
      return dynamic_obj_dist_table[center].near_dist;
    else if (dist < dynamic_obj_dist_table[center].see_dist)
      return calc_dynamic_dist2(dist, left, center - 1);
    else
      return calc_dynamic_dist2(dist, center + 1, right);
  }
}

double calc_dynamic_dist_error(double see_dist) 
{
  if (see_dist == DIST_ERR)
    return DIST_ERR;
  if (see_dist > 148.4) {
    printf("illigal dist : %f\n", see_dist);
    return DIST_ERR;
  }

  return calc_dynamic_dist_error2(see_dist, 0, 60);
}
const StaticObjDist dynamic_obj_dist_error_table[] = DYNAMICOBJDISTERRORTAB;
double calc_dynamic_dist_error2(double dist, int left, int right) {
  if (left == right) {
    return dynamic_obj_dist_error_table[left].near_dist;
  } else if (right - left == 1) {
    if (dist == dynamic_obj_dist_error_table[left].see_dist)
      return dynamic_obj_dist_error_table[left].near_dist;
    else
      return dynamic_obj_dist_error_table[right].near_dist;
  } else {
    int center = (left + right) / 2;
    if (dist == dynamic_obj_dist_error_table[center].see_dist)
      return dynamic_obj_dist_error_table[center].near_dist;
    else if (dist < dynamic_obj_dist_error_table[center].see_dist)
      return calc_dynamic_dist_error2(dist, left, center - 1);
    else
      return calc_dynamic_dist_error2(dist, center + 1, right);
  }
}


double calc_dir(double see_dir) 
{
  const double threshold = 0.1;

  if (see_dir == DIR_ERR) {
    return DIR_ERR;
  } else if (fabs(see_dir - 0.0) <= threshold) {
    return 0;
  } else if (see_dir > 0) {
    return normalize_angle(see_dir + 0.5);
  } else {
    return normalize_angle(see_dir - 0.5);
  }
}

double calc_dir_error(double see_dir) 
{
  const double threshold = 0.1;

  if (fabs(see_dir - 0.0) <= threshold) {
    return 1.0;
  } else {
    return 0.5;
  }
}

double calc_dist_chng(double see_dist, double see_dist_chng, double dist)
{
  const double threshold = 0.0001;

  if (fabs(see_dist - 0.000000) <= threshold && 
      fabs(see_dist_chng - 0.000000) <= threshold) {
    return CHNG_ERR;
  } else {
    return see_dist_chng * dist / see_dist;
  }
}

double calc_dist_chng_error(double see_dist, double see_dist_chng, double dist, double dist_error)
{
  if (see_dist == 0) {
    return 0;
  } else {
    return 0.02 / 2.0 * (dist + dist_error) + fabs(see_dist_chng) * dist_error / see_dist;
  }
}

double calc_dir_chng(double see_dist, double see_dir_chng) 
{
  const double threshold = 0.01;

  if (fabs(see_dist - 0.000000) <= threshold && 
      fabs(see_dir_chng - 0.000000) <= threshold) {
    return CHNG_ERR;
  } else {
    return see_dir_chng;
  }
}

double calc_dir_chng_error(void) 
{
  return 0.1 / 2.0;
}

double max_dist_by_sin(double dist, double dist_error, 
                      double dir, double dir_error) 
{
  if (dir - dir_error <= 90.0 && 90.0 <= dir + dir_error)
    return dist + dist_error;
  else if (dir - dir_error <= -270.0 && -270.0 <= dir + dir_error)
    return dist + dist_error;
  else if (Sin(dir - dir_error) < Sin(dir + dir_error))
    if (Sin(dir + dir_error) > 0) {
      return (dist + dist_error) * Sin(dir + dir_error);
    } else {
      return (dist - dist_error) * Sin(dir + dir_error);
    }
  else
    if (Sin(dir - dir_error) > 0) {
      return (dist + dist_error) * Sin(dir - dir_error);
    } else {
      return (dist - dist_error) * Sin(dir - dir_error);
    }
}

double min_dist_by_sin(double dist, double dist_error, 
                      double dir, double dir_error) 
{
  if (dir - dir_error <= -90.0 && -90.0 <= dir + dir_error)
    return -(dist + dist_error);
  else if (dir - dir_error <= 270.0 && 270.0 <= dir + dir_error)
    return -(dist + dist_error);
  else if (Sin(dir - dir_error) < Sin(dir + dir_error))
    if (Sin(dir - dir_error) > 0) {
      return (dist - dist_error) * Sin(dir - dir_error);
    } else {
      return (dist + dist_error) * Sin(dir - dir_error);
    }
  else
    if (Sin(dir + dir_error) > 0) {
      return (dist - dist_error) * Sin(dir + dir_error);
    } else {
      return (dist + dist_error) * Sin(dir + dir_error);
    }
}

double max_dist_by_cos(double dist, double dist_error, 
                      double dir, double dir_error) 
{
  if (dir - dir_error <= -360.0 && -360.0 <= dir + dir_error)
    return dist + dist_error;
  else if (dir - dir_error <= 0.0 && 0.0 <= dir + dir_error)
    return dist + dist_error;
  else if (dir - dir_error <= 360.0 && 360.0 <= dir + dir_error)
    return dist + dist_error;
  else if (Cos(dir - dir_error) < Cos(dir + dir_error))
    if (Cos(dir + dir_error) > 0) {
      return (dist + dist_error) * Cos(dir + dir_error);
    } else {
      return (dist - dist_error) * Cos(dir + dir_error);
    }
  else
    if (Cos(dir - dir_error) > 0) {
      return (dist + dist_error) * Cos(dir - dir_error);
    } else {
      return (dist - dist_error) * Cos(dir - dir_error);
    }
}

double min_dist_by_cos(double dist, double dist_error, 
                      double dir, double dir_error) 
{
  if (dir - dir_error <= -180.0 && -180.0 <= dir + dir_error)
    return -(dist + dist_error);
  else if (dir - dir_error <= 180.0 && 180.0 <= dir + dir_error)
    return -(dist + dist_error);
  else if (Cos(dir - dir_error) < Cos(dir + dir_error))
    if (Cos(dir - dir_error) > 0) {
      return (dist - dist_error) * Cos(dir - dir_error);
    } else {
      return (dist + dist_error) * Cos(dir - dir_error);
    }
  else
    if (Cos(dir + dir_error) > 0) {
      return (dist - dist_error) * Cos(dir + dir_error);
    } else {
      return (dist + dist_error) * Cos(dir + dir_error);
    }
}

double ave_dist_by_sin(double dist, double dist_error, 
                      double dir, double dir_error) 
{
  if (dist == DIST_ERR || dist_error < 0 ||
      dir == DIR_ERR || dir_error < 0) {
    printf("SeeInfoSensor::ave_dist_by_sin() : input error\n");
    return 0;
  }

  if (dir - dir_error < -360.0 || 360.0 < dir + dir_error) {
    printf("SeeInfoSensor::ave_dist_by_sin() : input error\n");
    return 0;
  }

  return (max_dist_by_sin(dist, dist_error, dir, dir_error) + 
          min_dist_by_sin(dist, dist_error, dir, dir_error)) / 2.0;
}

double error_dist_by_sin(double dist, double dist_error, 
                      double dir, double dir_error) 
{
  if (dist == DIST_ERR || dist_error < 0 ||
      dir == DIR_ERR || dir_error < 0) {
    printf("SeeInfoSensor::error_dist_by_sin() : input error\n");
    return 0;
  }

  if (dir - dir_error < -360.0 || 360.0 < dir + dir_error) {
    printf("SeeInfoSensor::error_dist_by_sin() : input error\n");
    return 0;
  }

  return (max_dist_by_sin(dist, dist_error, dir, dir_error) - 
          min_dist_by_sin(dist, dist_error, dir, dir_error)) / 2.0;
}

double ave_dist_by_cos(double dist, double dist_error, 
                      double dir, double dir_error) 
{
  if (dist == DIST_ERR || dist_error < 0 ||
      dir == DIR_ERR || dir_error < 0) {
    printf("SeeInfoSensor::ave_dist_by_cos() : input error\n");
    return 0;
  }

  if (dir - dir_error < -360.0 || 360.0 < dir + dir_error) {
    printf("SeeInfoSensor::ave_dist_by_cos() : input error\n");
    return 0;
  }

  return (max_dist_by_cos(dist, dist_error, dir, dir_error) + 
          min_dist_by_cos(dist, dist_error, dir, dir_error)) / 2.0;
}

double error_dist_by_cos(double dist, double dist_error, 
                      double dir, double dir_error) 
{
  if (dist == DIST_ERR || dist_error < 0 ||
      dir == DIR_ERR || dir_error < 0) {
    printf("SeeInfoSensor::error_dist_by_cos() : input error\n");
    return 0;
  }

  if (dir - dir_error < -360.0 || 360.0 < dir + dir_error) {
    printf("SeeInfoSensor::error_dist_by_cos() : input error\n");
    return 0;
  }
  return (max_dist_by_cos(dist, dist_error, dir, dir_error) -
          min_dist_by_cos(dist, dist_error, dir, dir_error)) / 2.0;
}

double multiplication_error(double a, double ae, double b, double be) {
  return fabs(a) * be + fabs(b) * ae + ae * be;
}

double division_error(double a, double ae, double b, double be) {
  return (fabs(a) * be + fabs(b) * ae) / (square(b) - fabs(b) * ae);
}
