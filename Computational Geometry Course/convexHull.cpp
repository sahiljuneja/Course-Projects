/*
 *  convexHull.cpp
 *
 *      Course: NCSU Spring 2013 - ECE 592-032
 *  Assignment: Convex Hull
 *      Author: Farrokh F. Mohammazadeh, Sahil Juneja
 *  Created on: Aug 31, 2013
 *     Purpose: Determine the convex hull of a randomly generated set of points.
 *    Requires: -
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

class Point2D {
public:
	
	double x, y;
	
	Point2D(double px, double py) {
		x = px;
		y = py;
	}
	
};

bool ascendingPredecate(Point2D p1, Point2D p2) {
	return p1.x < p2.x;
}

double isRightTurn(Point2D p1, Point2D p2, Point2D p3) {
	Point2D v1 = Point2D(p2.x - p1.x, p2.y - p1.y);
	Point2D v2 = Point2D(p3.x - p2.x, p3.y - p2.y);
	// if cross product is positive it was a right turn.
	return v1.x * v2.y - v1.y * v2.x < 0;
}

int main( int argc, char **argv )
{
	
	vector<Point2D> points;  // holds all the points (will be sorted)
	vector<int> upperHull;
	vector<int> lowerHull;
	vector<int> convexHull;  // combined upper and lower hull point-indexes
	int n = 10;
	
	// create random points (x and y between 0 and 100)
	points.clear();  // clear from previous trial
	srand((unsigned)time(NULL));
	for (int i = 0; i < n; i++) points.push_back(Point2D(100.0 * (double)rand() / (double)RAND_MAX, 100.0 * (double)rand() / (double)RAND_MAX));
	
	// sort the points by their x values
	std::sort(points.begin(), points.end(), ascendingPredecate);
	
	// find the upper hull points
	upperHull.clear();
	// algorithm: if last 3 points make left turn (ie: not convex) (or are colinear) remove middle of the 3
	int ind = 0;
	while (ind < (int)points.size()) {  // while there still points to test
		// add next point to hull
		upperHull.push_back(ind++);
		while (upperHull.size() >= 3) {
			// remove a point if it doesn't turn right (ie: turns left or is colinear)
			if (isRightTurn(points[upperHull[upperHull.size() - 3]], points[upperHull[upperHull.size() - 2]], points[upperHull[upperHull.size() - 1]])) break;
			upperHull.erase(upperHull.end() - 2);
		}
	}

	// find the lower hull points
	lowerHull.clear();
	ind = points.size() - 1;
	while (ind >= 0) {
		lowerHull.push_back(ind--);
		while (lowerHull.size() >= 3) {
			if (isRightTurn(points[lowerHull[lowerHull.size() - 3]], points[lowerHull[lowerHull.size() - 2]], points[lowerHull[lowerHull.size() - 1]])) break;
			lowerHull.erase(lowerHull.end() - 2);
		}
	}
	
	// combine upper and lower hull
	convexHull.clear();
	upperHull.pop_back();  // remove last element from upper (and lower) hull
	lowerHull.pop_back();
	convexHull.reserve(upperHull.size() + lowerHull.size());
	convexHull.insert(convexHull.end(), upperHull.begin(), upperHull.end());
	convexHull.insert(convexHull.end(), lowerHull.begin(), lowerHull.end());

	
	// output points to table
	ofstream convexHullFile;
	convexHullFile.open("convexHull.txt");
	for (int i = 0; i < n; ++i) convexHullFile << points[i].x << ", " << points[i].y << endl;
	convexHullFile << endl;
	for (int i = 0; i < convexHull.size(); ++i) convexHullFile << points[convexHull[i]].x << ", " << points[convexHull[i]].y << endl;
	convexHullFile.close();
	
	return 0;

}
