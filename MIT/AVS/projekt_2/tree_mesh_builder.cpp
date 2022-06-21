/**
 * @file    tree_mesh_builder.cpp
 *
 * @author  Michal Šedý <xsedym02@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree early elimination
 *
 * @date    27.11.2021
 **/

#include <iostream>
#include <limits>
#include <math.h>

#include "tree_mesh_builder.h"

TreeMeshBuilder::TreeMeshBuilder(unsigned gridEdgeSize)
    : BaseMeshBuilder(gridEdgeSize, "Octree")
{

}

unsigned TreeMeshBuilder::marchCubes(const ParametricScalarField &field)
{

    #pragma omp parallel shared(field)
    #pragma omp single
    marchCubesPerPartes(Vec3_t<float>(), mGridSize, field);

    // Combine triangles emited by each thread.
    mergeTriangles();

    return mTriangles.size();
}


void TreeMeshBuilder::marchCubesPerPartes(const Vec3_t<float> &pos, const unsigned edgeLength, const ParametricScalarField &field)
{
    // Split cube untill the minimum edge length.
    // Every cut-off is unefficient.
    if (edgeLength > 1)
    {
        // Test if the cube contains points.
        if (isEmpty(pos, edgeLength, field))
            return;

        // Split the cube to 8 subcubes and evaluate each in the separate task.
        const float halfLength = edgeLength / 2.0f;
        for (unsigned x = 0; x < 2; x++)
        {
            for (unsigned y = 0; y < 2; y++)
            {
                for (unsigned z = 0; z < 2; z++)
                {
                    #pragma omp task shared(pos, halfLength, field)
                    {
                        const Vec3_t<float> newPos(pos.x + (x * halfLength),
                                                   pos.y + (y * halfLength),
                                                   pos.z + (z * halfLength));
                        marchCubesPerPartes(newPos, halfLength, field);
                    }
                }
            }
        }
    }
    else
    {
        buildCube(pos, field);
    }

    // The procedure must wait for all its tasks. Otherwise, local resources
    // will be destructed, and Segmentation fault will be occured.
    #pragma omp taskwait
}


float TreeMeshBuilder::evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field)
{
    // NOTE: This method is called from "buildCube(...)"!

    // 1. Store pointer to and number of 3D points in the field
    //    (to avoid "data()" and "size()" call in the loop).
    const Vec3_t<float> *pPoints = field.getPoints().data();
    const unsigned count = unsigned(field.getPoints().size());

    float value = std::numeric_limits<float>::max();

    // 2. Find minimum square distance from points "pos" to any point in the
    //    field.
    for(unsigned i = 0; i < count; ++i)
    {
        float distanceSquared  = (pos.x - pPoints[i].x) * (pos.x - pPoints[i].x);
        distanceSquared       += (pos.y - pPoints[i].y) * (pos.y - pPoints[i].y);
        distanceSquared       += (pos.z - pPoints[i].z) * (pos.z - pPoints[i].z);

        // Comparing squares instead of real distance to avoid unnecessary
        // "sqrt"s in the loop.
        value = std::min(value, distanceSquared);
    }

    // 3. Finally take square root of the minimal square distance to get the real distance
    return sqrt(value);
}


void TreeMeshBuilder::emitTriangle(const BaseMeshBuilder::Triangle_t &triangle)
{
    // NOTE: This method is called from "buildCube(...)"!

    // Store generated triangle into vector (array) of generated triangles.
    // The pointer to data in this array is return by "getTrianglesArray(...)" call
    // after "marchCubes(...)" call ends.

    // This was a critical section. But the separation of thread emition outputs
    // solves the problem.
    this->TrianglesPerThread[omp_get_thread_num()].push_back(triangle);
}


bool TreeMeshBuilder::isEmpty(const Vec3_t<float> &pos, const float edgeLength, const ParametricScalarField &field)
{
    const float halfLength = edgeLength / 2.0f;
    const Vec3_t<float> center((pos.x + halfLength) * mGridResolution,
		                       (pos.y + halfLength) * mGridResolution,
		                       (pos.z + halfLength) * mGridResolution);

    static const float tmp = (sqrtf(3.0f) / 2.0f)*mGridResolution;

    return evaluateFieldAt(center, field) > (mIsoLevel + tmp * edgeLength);
}

void TreeMeshBuilder::mergeTriangles()
{
    // Merge all triangles emited by each thread into mTriangles.
    for(size_t i = 0; i < omp_get_max_threads(); i++)
        this->mTriangles.insert(this->mTriangles.end(),
                                this->TrianglesPerThread[i].begin(),
                                this->TrianglesPerThread[i].end());
}