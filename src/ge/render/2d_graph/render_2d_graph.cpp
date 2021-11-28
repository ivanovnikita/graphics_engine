#include "render_2d_graph.h"

namespace ge
{
    Render2dGraph::Render2dGraph(const SurfaceParams& /*surface_params*/)
        : instance_data_{InstanceData::create_default(factory::options::Instance::create_default())}
    {
    }

    Render2dGraph::~Render2dGraph()
    {
    }
}
