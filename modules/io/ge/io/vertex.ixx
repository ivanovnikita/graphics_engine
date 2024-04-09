export module vertex;

export import world_3d_coords;
export import texture_coords;

namespace ge
{
    export struct Vertex final
    {
        World3dCoords world_coords;
        TextureCoords texture_coords;
    };
}
