#ifndef BEZIERATTACK
#define BEZIERATTACK

// g++ main.cpp -o out -I raylib/ -L raylib/ -lraylib -lopengl32 -lgdi32 -lwinmm

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <vector>
#include <string>
#include <bits/stdc++.h> 

Vector2 VectorLerp(Vector2 v0, Vector2 v1, float t)
{
    Vector2 newVector = Vector2Subtract(v1, v0);
    newVector = Vector2Scale(newVector, t);
    newVector = Vector2Add(v0, newVector);

    return newVector;
}

int factorial(int x)
{
    if(x == 0)
    {
        return 1;
    }
    else
    {
        return x * factorial(x - 1);
    }
}

int GetPascalTriangleCoefficient(int order, int index)
{
    int numerator = factorial(order);
    int denominator = factorial(order - index);
    denominator = denominator * factorial(index);

    return numerator/denominator;
}

std::vector<int> GeneratePascalTriangleRow(int order)
{
    std::vector<int> pt;

    for(int i = 0; i < order + 1; i++)
    {
        pt.push_back(GetPascalTriangleCoefficient(order, i));
    }

    return pt;
}

Vector2 GetPoint(Vector2 points[], float t, std::vector<int> ptrow, int localOrder)
{
    Vector2 output = {0, 0};

    for(int i = 0; i <= localOrder; i++)
    {
        output.x += (float) ptrow.at(i) * (float) points[i].x * (float) pow(1 - t, localOrder - i) * (float) pow(t, i);
    }

    for(int i = 0; i <= localOrder; i++)
    {
        output.y += (float) ptrow.at(i) * (float) points[i].y * (float) pow(1 - t, localOrder - i) * (float) pow(t, i);
    }

    return output;
}


void MakeBezierCurveVerticesParametric(int startIndex, std::vector<Vector2> points, int localOrder, int numberOfSteps, std::vector<Vector2> *curve, std::vector<int> pt)
{
    curve -> clear();

    // start point
    curve -> push_back(points.at(startIndex));

    int numberOfControlPoints = localOrder + 1;

    Vector2 curveControlPoints[numberOfControlPoints];

    for(int i = 0; i < numberOfControlPoints; i++)
    {
        curveControlPoints[i] = points.at(startIndex + i);
    }

    for(int i = 0; i <= numberOfSteps; i++)
    {
        float t = ((float) i)/((float) numberOfSteps);

        Vector2 newPoint = GetPoint(curveControlPoints, t, pt, localOrder);

        curve -> push_back(newPoint);
    }

    // end point
    curve -> push_back(points.at(startIndex + localOrder));
}

std::vector<Vector2> ConstructAttackPath(Vector2 origin, Vector2 target)
{
    int order = 3;

    int totalSteps = 50;

    int totalControlPoints = 4;

    int numberOfCurves = totalControlPoints/order;

    std::vector<Vector2> controlPoints;
    std::vector<Vector2> bezierPoints;

    std::vector<int> ptCoefficients{1, 3, 3, 1};

    // GET CONTROL POINTS
    Vector2 targetDirection = Vector2Normalize(Vector2Subtract(target, origin));

    Vector2 targetDirectionNormal = {-targetDirection.y, targetDirection.x};

    Vector2 point_a = VectorLerp(origin, target, 0.33f);
    point_a = Vector2Add(point_a, Vector2Scale(targetDirectionNormal, 100));

    Vector2 point_b = VectorLerp(origin, target, 0.66f);
    point_b = Vector2Add(point_b, Vector2Scale(targetDirectionNormal, 100));

    controlPoints.push_back(origin);
    controlPoints.push_back(point_a);
    controlPoints.push_back(point_b);
    controlPoints.push_back(target);

    // MAKE CURVE
    MakeBezierCurveVerticesParametric(0, controlPoints, order, totalSteps, &bezierPoints, ptCoefficients);

    return bezierPoints;
}

Vector2 ReturnPointOverTime(float t, float maxTime, std::vector<Vector2> points)
{
    int maxIndex = points.size() - 1;

    int currentPoint = maxIndex * t/maxTime;

    return points[currentPoint];

}

#endif