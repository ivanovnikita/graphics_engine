module;

#include "ge/common/exception_macro.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

module io.read_model;

import safe_cast;
import exception;

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

            TriangleIndexed triangle;
            size_t i = 0;

            for (const tinyobj::index_t& index : shape.mesh.indices)
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
                    1.0f - attrib.texcoords[2 * safe_cast<size_t>(index.texcoord_index) + 1]
                };

                vertices.push_back(std::move(vertex));

                ++i;
                if (i == 3)
                {
                    triangles.emplace_back(std::move(triangle));
                    i = 0;
                }
            }
        }

        return Model
        {
            std::move(vertices),
            std::move(triangles)
        };
    }
}
