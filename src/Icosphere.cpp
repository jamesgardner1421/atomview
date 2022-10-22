
#include "Icosphere.hpp"
#include <iostream>
#include <cstdlib>

constexpr float X = .525731112119133606;
constexpr float Z = .850650808352039932;

std::vector<Triangle> Triangle::subdivide() const
{
    auto mid01 = Triangle::computeMidpointOnSphere(vertices[0], vertices[1]);
    auto mid12 = Triangle::computeMidpointOnSphere(vertices[1], vertices[2]);
    auto mid02 = Triangle::computeMidpointOnSphere(vertices[0], vertices[2]);

    return std::vector<Triangle>{
        {mid01, vertices[0], mid02},
        {vertices[1], mid01, mid12},
        {vertices[2], mid12, mid02},
        {mid12, mid01, mid02}
    };
}

const glm::vec3 ico_vertices[] {
    {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
    {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
    {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};

const glm::uvec3 ico_indices[] {
    {0,4,1},  {0,9,4},  {9,5,4},  {4,5,8},  {4,8,1},
    {8,10,1}, {8,3,10}, {5,3,8},  {5,2,3},  {2,7,3},
    {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
    {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5},  {7,2,11}
};

std::vector<Triangle> subdivide(int levels, const std::vector<Triangle>& initial_triangles)
{
    if (levels == 0)
    {
        return initial_triangles;
    }
    else
    {
        std::vector<Triangle> output;
        for (const auto& triangle : initial_triangles)
        {
            auto small_triangles = triangle.subdivide();
            for (const auto& small_triangle : small_triangles)
            {
                output.push_back(small_triangle);
            }
        }
        return subdivide(--levels, output);
    }
}

IcoSphere::IcoSphere(int levels)
{
    std::vector<Triangle> initial_triangles;
    for (const auto& face : ico_indices)
    {
        initial_triangles.push_back({
            ico_vertices[face[0]],
            ico_vertices[face[1]],
            ico_vertices[face[2]],
        });
    }

    /* for (const auto& triangle : initial_triangles) */
    /* { */
    /*     auto small_triangles = triangle.subdivide(); */
    /*     for (const auto& small_triangle : small_triangles) */
    /*     { */
    /*         triangles_.push_back(small_triangle); */
    /*     } */
    /*     /1* triangles_.push_back(triangle); *1/ */
    /* } */

    triangles_ = subdivide(levels, initial_triangles);

    std::cout << "Number of triangles: " << initial_triangles.size() << std::endl;
    std::cout << "Number of triangles: " << triangles_.size() << std::endl;
}

/* std::vector<glm::vec3> IcoSphere::getNormals() const */
/* { */
/*     std::vector<glm::vec3> output; */
/*     for (const auto& triangle : triangles_) */
/*     { */
/*         for (const auto& vertex : triangle.vertices) */
/*         { */
/*             output.push_back(glm::normalize(vertex)); */
/*         } */
/*     } */
/*     return output; */
/* } */

std::vector<Vertex> IcoSphere::getVertices() const
{
    std::vector<Vertex> output;
    for (const auto& triangle : triangles_)
    {
        for (const auto& vertex : triangle.vertices)
        {
            output.push_back({vertex, glm::normalize(vertex)});
        }
    }
    std::cout << "Number of vertices: " << output.size() << std::endl;
    return output;
}

std::vector<uint16_t> IcoSphere::getIndices() const
{
    std::vector<uint16_t> output;
    for (uint16_t i = 0; i < triangles_.size() * 3 ; i++)
    {
        output.push_back(i);
    }
    std::cout << "Number of indices: " << output.size() << std::endl;
    return output;
}

/* void IcoSphere::_subdivide(void) */
/* { */
/*   using Key = unsigned long long; */

/*   std::map<Key,int> edgeMap; */
/*   const std::vector<int>& indices = *mIndices.back(); */
/*   mIndices.push_back(new std::vector<int>); */
/*   std::vector<int>& refinedIndices = *mIndices.back(); int end = indices.size(); */
/*   for (int i=0; i<end; i+=3) */
/*   { */
/*     int ids0[3],  // indices of outer vertices */
/*         ids1[3];  // indices of edge vertices */
/*     for (int k=0; k<3; ++k) */
/*     { */
/*       int k1 = (k+1)%3; */
/*       int e0 = indices[i+k]; */
/*       int e1 = indices[i+k1]; */
/*       ids0[k] = e0; */
/*       if (e1>e0) */
/*         std::swap(e0,e1); */
/*       Key edgeKey = Key(e0) | (Key(e1)<<32); */
/*       std::map<Key,int>::iterator it = edgeMap.find(edgeKey); */
/*       if (it==edgeMap.end()) */
/*       { */
/*         ids1[k] = mVertices.size(); */
/*         edgeMap[edgeKey] = ids1[k]; */
/*         mVertices.push_back( (mVertices[e0]+mVertices[e1]).normalized() ); */
/*       } */
/*       else */
/*         ids1[k] = it->second; */
/*     } */
/*     refinedIndices.push_back(ids0[0]); refinedIndices.push_back(ids1[0]); refinedIndices.push_back(ids1[2]); */
/*     refinedIndices.push_back(ids0[1]); refinedIndices.push_back(ids1[1]); refinedIndices.push_back(ids1[0]); */
/*     refinedIndices.push_back(ids0[2]); refinedIndices.push_back(ids1[2]); refinedIndices.push_back(ids1[1]); */
/*     refinedIndices.push_back(ids1[0]); refinedIndices.push_back(ids1[1]); refinedIndices.push_back(ids1[2]); */
/*   } */
/*   mListIds.push_back(0); */
/* } */


