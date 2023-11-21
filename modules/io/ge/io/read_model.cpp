#include "read_model.h"

#include "ge/common/exception.h"
#include "ge/common/safe_cast.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace ge
{
    Model read_model_obj(const std::string& filepath)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;

        if (not tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
        {
            GE_THROW_EXPECTED_ERROR
            (
                "Obj model loading failed",
                warn + err
            );
        }

        std::vector<Vertex> vertices;
        std::vector<TriangleIndexed> triangles;

        for (const tinyobj::shape_t& shape : shapes)
        {
            if ((shape.mesh.indices.size() % 3) != 0)
            {
                GE_THROW_EXPECTED_ERROR("Wrong number of triangle vertices");
            }

            for (const tinyobj::index_t& index : shape.mesh.indices)
            {
                TriangleIndexed triangle;

                for (size_t i = 0; i < 3; ++i)
                {
                    triangle.inds[i] = vertices.size();

                    Vertex vertex{};

                    vertex.world_coords.coords =
                    {
                        attrib.vertices[3 * safe_cast<size_t>(index.vertex_index) + 0],
                        attrib.vertices[3 * safe_cast<size_t>(index.vertex_index) + 1],
                        attrib.vertices[3 * safe_cast<size_t>(index.vertex_index) + 2]
                    };

                    vertex.texture_coords.coords =
                    {
                        attrib.texcoords[2 * safe_cast<size_t>(index.texcoord_index) + 0],
                        attrib.texcoords[2 * safe_cast<size_t>(index.texcoord_index) + 1]
                    };

                    vertices.push_back(std::move(vertex));
                }

                triangles.emplace_back(std::move(triangle));
            }
        }

        return Model
        {
            std::move(vertices),
            std::move(triangles)
        };
    }
}
