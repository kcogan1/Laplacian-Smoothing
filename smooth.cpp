#include "io.h"
#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Dense>
using namespace std;

//global vars
std::vector<Eigen::Vector3d> pts;
std::vector<Tri> triangles;
//L is a vector of eigen Vector3ds
std::vector<Eigen::Vector3d> L;

void smooth(int, int, std::vector<Tri>& triangles, std::vector<Eigen::Vector3d>& pts, int, int);

void smooth(int numVertices, int numFaces, std::vector<Tri> &triangles, std::vector<Eigen::Vector3d>& pts, int stepsize, int iterations)
{
    std::vector<double> m; //number of neighbors

    //set size of vectors
    m.resize(numVertices);
    L.resize(numVertices);

    //zero everything out to start
    for (int i = 0; i < numVertices; i++)
    {
        m[i] = 0.0;
        L[i] = { 0,0,0 };
    }

    for (int j = 0; j < iterations; j++)
    {
        //add up the differences between the current vertex and the others in the triangle
        for (int i = 0; i < numFaces; i++)
        {
            //x is position of verts- pts

            //vert 1
            L[triangles[i][0]] += pts[triangles[i][1]] - pts[triangles[i][0]] + pts[triangles[i][2]] - pts[triangles[i][0]];
            //update neighbors
            m[triangles[i][0]] += 2.0;


            //vert 2
            L[triangles[i][1]] += pts[triangles[i][0]] - pts[triangles[i][1]] + pts[triangles[i][2]] - pts[triangles[i][1]];
            m[triangles[i][1]] += 2.0;


            //vert 3
            L[triangles[i][2]] += pts[triangles[i][1]] - pts[triangles[i][2]] + pts[triangles[i][0]] - pts[triangles[i][2]];
            m[triangles[i][2]] += 2.0;
        }


        //apply the Laplacian operator
        //stepsize given by user
        for (int i = 0; i < numVertices; i++)
        {
            pts[i] += stepsize * (L[i] / m[i]);
        }
    }  
}

int main(int argc, char *argv[])
{
    int stepsize;
    int iterations;

    //command line arguments
    char *fname = argv[1];
    char* output = argv[2];
    stepsize = atoi(argv[3]);
    iterations = atoi(argv[4]);

    //read, smooth, and write
    readObjFile(fname, pts, triangles); //fix later
    smooth(pts.size(), triangles.size(), triangles, pts, stepsize, iterations);
    writeObjFile(output, pts, triangles);

    return 0;
}