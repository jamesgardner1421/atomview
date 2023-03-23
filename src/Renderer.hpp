#pragma once

#define GLFW_INCLUDE_VULKAN
#define VK_ENABLE_BETA_EXTENSIONS
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <vector>
#include <optional>
#include <set>
#include <chemfiles.hpp>

#include "Icosphere.hpp"
#include "Camera.hpp"
#include "Window.hpp"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;

    SwapChainSupportDetails(vk::PhysicalDevice device, VkSurfaceKHR surface);
};

const uint32_t WIDTH = 1000;
const uint32_t HEIGHT = 800;

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME,
    VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME,
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const size_t MAX_PARTICLES{10000};

struct UniformBufferObject {
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

struct alignas(16) ModelData {
    glm::mat4 model;
    glm::vec3 color;
    VkBool32 selected;
};

class Renderer
{
public:
    Renderer(Window& window, const Camera& camera);
    ~Renderer();

    void drawFrame(const std::vector<glm::vec3>& positions,
                   const std::vector<glm::vec3>& colors,
                   const std::vector<int>& selectedAtoms);

    Window& window;
    const Camera& camera;
    vk::Device device;

private:

    vk::Instance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    vk::SurfaceKHR surface;

    vk::PhysicalDevice physicalDevice = VK_NULL_HANDLE;

    vk::Queue graphicsQueue;
    vk::Queue presentQueue;

    vk::SwapchainKHR swapChain;
    std::vector<vk::Image> swapChainImages;
    vk::Format swapChainImageFormat;
    vk::Extent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    std::vector<VkFramebuffer> imguiSwapChainFramebuffers;

    vk::RenderPass renderPass;
    vk::RenderPass imguiRenderPass;
    vk::DescriptorSetLayout uniformDescriptorSetLayout;
    vk::DescriptorSetLayout storageDescriptorSetLayout;
    vk::PipelineLayout pipelineLayout;
    vk::Pipeline graphicsPipeline;

    vk::CommandPool commandPool;

    vk::Image depthImage;
    vk::DeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    IcoSphere icosphere_;
    std::vector<Vertex> vertices_;
    std::vector<uint16_t> indices_;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    std::vector<VkBuffer> storageBuffers;
    std::vector<VkDeviceMemory> storageBuffersMemory;

	VkDescriptorPool imguiPool;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> uniformDescriptorSets;
    std::vector<VkDescriptorSet> storageDescriptorSets;

    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkCommandBuffer> imguiCommandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

    bool framebufferResized = false;

    void initVulkan();
    void initImGui();

    void cleanupSwapChain();
    void cleanupImGui();
    void recreateSwapChain();
    void createInstance();

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    
    void setupDebugMessenger();

    void createSurface();

    void pickPhysicalDevice();

    void createLogicalDevice();

    void createSwapChain();

    void createImageViews();

    vk::ImageView createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);

    void createRenderPass();
    void createImGuiRenderPass();

    void createUniformDescriptorSetLayout();

    void createStorageDescriptorSetLayout();

    void createGraphicsPipeline();

    void createFramebuffers();
    void createImGuiFramebuffers();

    void createCommandBuffers();
    void createImGuiCommandBuffers();

    void createCommandPool();

    void createDepthResources();

    void createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image& image, vk::DeviceMemory& imageMemory);

    void createVertexBuffer();

    void createIndexBuffer();

    void createUniformBuffers();

    void createStorageBuffers();

    void createDescriptorPool();

    void createUniformDescriptorSets();

    void createStorageDescriptorSets();

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, size_t instances);
    void recordImGuiCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void createSyncObjects();

    void updateUniformBuffer(uint32_t currentImage, const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& colors, const std::vector<int>& selectedAtoms);

    vk::ShaderModule createShaderModule(const std::vector<char>& code);

    vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);

    vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);

    vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

    SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device);

    bool isDeviceSuitable(vk::PhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device);

    std::vector<const char*> getRequiredExtensions();

    bool checkValidationLayerSupport();

    static std::vector<char> readFile(std::string filename);

    static bool hasStencilComponent(VkFormat format);

    static bool checkDeviceExtensionSupport(vk::PhysicalDevice device);

    vk::Format findDepthFormat();
};


