/**
 * @file    loop_mesh_builder.h
 *
 * @author  Michal Šedý <xsedym02@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP loops
 *
 * @date    24.11.2021
 **/

#ifndef LOOP_MESH_BUILDER_H
#define LOOP_MESH_BUILDER_H

#include <vector>
#include "base_mesh_builder.h"

class LoopMeshBuilder : public BaseMeshBuilder
{
public:
    LoopMeshBuilder(unsigned gridEdgeSize);

protected:
    unsigned marchCubes(const ParametricScalarField &field);
    float evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field);
    void emitTriangle(const BaseMeshBuilder::Triangle_t &triangle);
    const Triangle_t *getTrianglesArray() const { return mTriangles.data();}
    void mergeTriangles();

    std::vector<Triangle_t> mTriangles;
    std::vector<Triangle_t> TrianglesPerThread[64];
};

#endif // LOOP_MESH_BUILDER_H
