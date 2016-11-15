#ifndef METHODS_TORBEN_H
#define METHODS_TORBEN_H
#include "../header/LinMat.h"

void insertion_sort(double arr[], int length);
double fRand(double fMin, double fMax);
void createInitPos(LinMat &pos, double xmin, double xmax, double ymin, double ymax);
void createInitVel(LinMat &vel, double biasX, double biasY, double absMaxRandVel);
void step();
void insertion_sort(const LinMat &mat, int* sorted, int col);
void resolveCollisions(const LinMat &pos, LinMat &vel,bool* collided, int* sortedX, double radius);
#endif