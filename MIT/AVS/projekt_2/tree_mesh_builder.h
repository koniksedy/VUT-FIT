/**
 * @file    tree_mesh_builder.h
 *
 * @author  Michal Šedý <xsedym02@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree early elimination
 *
 * @date    27.11.2021
 **/

#ifndef TREE_MESH_BUILDER_H
#define TREE_MESH_BUILDER_H

#include "base_mesh_builder.h"
#include <omp.h>

class TreeMeshBuilder : public BaseMeshBuilder
{
public:
    TreeMeshBuilder(unsigned gridEdgeSize);

protected:
    unsigned marchCubes(const ParametricScalarField &field);
    float evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field);
    void emitTriangle(const Triangle_t &triangle);
    const Triangle_t *getTrianglesArray() const { return mTriangles.data();}
    bool isEmpty(const Vec3_t<float> &pos, const float edgeLength, const ParametricScalarField &field);
    void marchCubesPerPartes(const Vec3_t<float> &pos, const unsigned edgeLength, const ParametricScalarField &field);
    void mergeTriangles();

    std::vector<Triangle_t> mTriangles;
    std::vector<Triangle_t> TrianglesPerThread[64];
};


#endif // TREE_MESH_BUILDER_H
