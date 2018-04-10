#include <vulkan/vulkan.hpp>

#include <experimental/filesystem>

namespace ge::impl::storage
{

    namespace fs = std::experimental::filesystem;

    class Shaders final
    {
    public:
        using ShaderModuleRef = std::reference_wrapper<const vk::ShaderModule>;
        using Shader = std::pair<vk::ShaderStageFlagBits, ShaderModuleRef>;

        explicit Shaders(const vk::Device& device, const fs::path& shaders_dir);

        std::vector<Shader> shaders() const;

    private:
        std::vector<std::pair<vk::ShaderStageFlagBits, vk::UniqueShaderModule>> shaders_;
    };

}
