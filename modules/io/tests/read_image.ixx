module;

#include <gtest/gtest.h>

export module tests.io.read_image;

import io.read_image;
import exception;

module : private;

TEST(read_image, success)
{
    using namespace ge;

    const std::string filepath{"../res/dwarf_king.jpg"};

    const Image image = read_image(filepath);

    EXPECT_EQ(image.x(), 1920);
    EXPECT_EQ(image.y(), 2013);
    EXPECT_EQ(image.channels_count(), 4);

    EXPECT_EQ(image.pixels_count(), image.x() * image.y());
    EXPECT_EQ(image.bytes_count(), image.pixels_count() * image.channels_count());

    EXPECT_NE(image.data(), nullptr);
}

TEST(read_image, fail)
{
    using namespace ge;

    EXPECT_THROW
    (
        read_image("unknow_image.jpg"),
        expected_error
    );
}
