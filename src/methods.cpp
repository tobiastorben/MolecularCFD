#include "../header/LinMat.h"
#include <random>
#include <SFML/Graphics.hpp>
#include <math.h>

void insertion_sort(double arr[], int length){
	int j;
	double temp;

	for (int i = 0; i < length; i++){
		j = i;

		while (j > 0 && arr[j] < arr[j - 1]){
			temp = arr[j];
			arr[j] = arr[j - 1];
			arr[j - 1] = temp;
			j--;
		}
	}
}

void insertion_sort(const LinMat &mat, int* sorted, int col) {
	int j;
	int temp;
	int length = mat.getRows();

	for (int i = 0; i < length; i++){
		j = i;
		while (j > 0 && mat.get(mat.index(sorted[j], col)) < mat.get(mat.index(sorted[j - 1], col))){
			temp = sorted[j];
			sorted[j] = sorted[j - 1];
			sorted[j - 1] = temp;
			j--;
		}
	}

}

double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

void createInitPos(LinMat& pos, double xmin, double xmax, double ymin, double ymax) {
	srand(39);
	int n = pos.getRows();
	for (int i = 0; i < n; i++) {
		pos.set(pos.index(i, 0), fRand(xmin, xmax));
		pos.set(pos.index(i, 1), fRand(ymin, ymax));
	}
}

void createInitVel(LinMat &vel, double biasX, double biasY, double absMaxRandVel) {
	srand(64);
	int n = vel.getRows();

	for (int i = 0; i < n; i++) {
		vel.set(vel.index(i, 0), biasX + fRand(-absMaxRandVel, absMaxRandVel));
		vel.set(vel.index(i, 1), biasY + fRand(-absMaxRandVel, absMaxRandVel));
	}
}

void resolveCollisions(const LinMat &pos, LinMat &vel, bool* collided, int* sortedX, double radius) {

	int n = pos.getRows();
	int j,ind;
	double currentPosX, currentPosY, otherPosY, otherPosX, relVel, currentVelX, currentVelY, otherVelX, otherVelY;
	double D = 2 * radius;
	double Dsq = D*D;
	double unitVec[2];
	double distSq, dist;
	for (int i = 0; i < n; i++) {
		if (!collided[sortedX[i]]){
			currentPosX = pos.get(pos.index(sortedX[i], 0));
			currentPosY = pos.get(pos.index(sortedX[i], 1));
			currentVelX = vel.get(vel.index(sortedX[i], 0));
			currentVelY = vel.get(vel.index(sortedX[i], 1));
			j = i + 1;
			ind = pos.index(sortedX[j], 0);

			while (j < n && (currentPosX - pos.get(ind)) < D) {
				ind = pos.index(sortedX[j], 0);
				otherPosY = pos.get(ind+1);
				otherPosX = pos.get(ind);
				distSq = (currentPosX - otherPosX)*(currentPosX - otherPosX) + (currentPosY - otherPosY)*(currentPosY - otherPosY);
				if (distSq < Dsq) {
					dist = sqrt(distSq);
					unitVec[0] = (currentPosX - otherPosX) / dist;
					unitVec[1] = (currentPosY - otherPosY) / dist;
					otherVelX = vel.get(ind);
					otherVelY = vel.get(ind+1);
					relVel = (currentVelX - otherVelX)*unitVec[0] + (currentVelY - otherVelY)*unitVec[1];
					if (relVel < 0 && !collided[sortedX[j]]) {//TODO USIKKER
						vel.set(vel.index(sortedX[i], 0), currentVelX - 0.98*relVel*unitVec[0]);
						vel.set(vel.index(sortedX[i], 1), currentVelY - 0.98*relVel*unitVec[1]);
						vel.set(ind, otherVelX + 0.98*relVel*unitVec[0]);
						vel.set(ind+1, otherVelY + 0.98*relVel*unitVec[1]);
						collided[sortedX[i]] = true;
						collided[sortedX[j]] = true;
					}
				}
				j++;
			}

			j = i - 1;
			ind = pos.index(sortedX[j], 0);
			while (j >= 0 && (currentPosX - pos.get(ind)) < D) {
				ind = pos.index(sortedX[j], 0);
				otherPosY = pos.get(ind+1);
				otherPosX = pos.get(ind);
				distSq = (currentPosX - otherPosX)*(currentPosX - otherPosX) + (currentPosY - otherPosY)*(currentPosY - otherPosY);
				if (distSq < Dsq) {
					dist = sqrt(distSq);
					unitVec[0] = (currentPosX - otherPosX) / dist;
					unitVec[1] = (currentPosY - otherPosY) / dist;
					otherVelX = vel.get(ind);
					otherVelY = vel.get(ind+1);
					relVel = (currentVelX - otherVelX)*unitVec[0] + (currentVelY - otherVelY)*unitVec[1];
					if (relVel < 0 && !collided[sortedX[j]]) {//TODO USIKKER
						vel.set(vel.index(sortedX[i], 0), currentVelX - 0.98*relVel*unitVec[0]);
						vel.set(vel.index(sortedX[i], 1), currentVelY - 0.98*relVel*unitVec[1]);
						vel.set(ind, otherVelX + 0.98*relVel*unitVec[0]);
						vel.set(ind+1, otherVelY + 0.98*relVel*unitVec[1]);
						collided[sortedX[i]] = true;
						collided[sortedX[j]] = true;
					}
				}
				j--;
			}
		}
	}
}