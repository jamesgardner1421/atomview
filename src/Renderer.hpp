#pragma once

#define GLFW_INCLUDE_VULKAN
#define VK_ENABLE_BETA_EXTENSIONS
#include <GLFW/glfw3.h>

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
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
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

struct ModelData {
    alignas(16) glm::mat4 model;
    alignas(16) glm::vec3 color;
};

class Renderer
{
public:
    Renderer(Window& window, const Camera& camera);
    ~Renderer();

    void drawFrame(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& colors);

    Window& window;
    const Camera& camera;
    VkDevice device;

private:

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    std::vector<VkFramebuffer> imguiSwapChainFramebuffers;

    VkRenderPass renderPass;
    VkRenderPass imguiRenderPass;
    VkDescriptorSetLayout uniformDescriptorSetLayout;
    VkDescriptorSetLayout storageDescriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkCommandPool commandPool;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
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

    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

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

    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

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

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, size_t instances);
    void recordImGuiCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void createSyncObjects();

    void updateUniformBuffer(uint32_t currentImage, const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& colors);

    VkShaderModule createShaderModule(const std::vector<char>& code);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    bool isDeviceSuitable(VkPhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    std::vector<const char*> getRequiredExtensions();

    bool checkValidationLayerSupport();

    static std::vector<char> readFile(std::string filename);

    static bool hasStencilComponent(VkFormat format);

    static bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    VkFormat findDepthFormat();
};


