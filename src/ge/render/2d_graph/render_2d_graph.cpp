#include "render_2d_graph.h"

namespace ge
{
    Render2dGraph::Render2dGraph(const SurfaceParams& /*surface_params*/, const Logger& logger)
        : instance_data_{InstanceData::create_default(factory::options::Instance::create_default(), logger)}
    {
    }

    Render2dGraph::~Render2dGraph()
    {
    }
}
