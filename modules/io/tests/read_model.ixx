module;

#include <gtest/gtest.h>

export module tests.io.read_model;

import io.read_model;
import exception;

module : private;

TEST(read_model, success)
{
    using namespace ge;

    const std::string filepath{"../res/viking_room.obj"};

    const Model model = read_model_obj(filepath);

    EXPECT_FALSE(model.vertices().empty());
    EXPECT_FALSE(model.triangles().empty());
}

TEST(read_model, fail)
{
    using namespace ge;

    EXPECT_THROW
    (
        read_model_obj("unknow_model.obj"),
        expected_error
    );
}
