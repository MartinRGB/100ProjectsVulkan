#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <stdexcept>
#include <vector>

// If you run Vulkan in macOS,you need add this
#include <Molten/MoltenVK/include/MoltenVK/mvk_vulkan.h>

const int WIDTH = 800;
const int HEIGHT = 600;

class HelloTriangleApplication{
    
    public:
        void run(){
            initWindow();
            initVulkan();
            mainLoop();
            cleanup();
        }
    
    private:
        GLFWwindow* window;
        void initWindow() {
            glfwInit();
            
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            
            window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
        }
    
        void mainLoop() {
            while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();
            }
        }
    
    
        VkInstance instance;
    
        void initVulkan(){
            createInstance();
            std::cout << "Application is running successful" << std::endl;
        }

        void createInstance(){
            //应用信息
            VkApplicationInfo appInfo = {};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "Hello Triangle";
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.pEngineName = "No Engine";
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.apiVersion = VK_API_VERSION_1_0;
            
            //创建信息
            VkInstanceCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;
            
            //GLFW 界面拓展 -> vulkan create Info
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
            
            createInfo.enabledExtensionCount = glfwExtensionCount;
            createInfo.ppEnabledExtensionNames = glfwExtensions;
            
            createInfo.enabledLayerCount = 0;
            
            //Check Instance Creation's Result
            if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
                throw std::runtime_error("failed to create instance!");
            }
            
            std::cout << "============= Extensions' supporting list at bottom: =============" << std::endl;
            
            //Check Extensions Support
            checkExtensionsSupport();
            
            std::cout << "============= Extensions' supporting list at top: =============" << std::endl;
        }
    
        void checkExtensionsSupport(){
            //查询拓展个数
            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            //将拓展信息分配到数组中
            std::vector<VkExtensionProperties> extensions(extensionCount);
            //获取拓展信息细节
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
            //打印出来
            std::cout << "available extensions:" << std::endl;
            
            for (const auto& extension : extensions) {
                std::cout << "    " << extension.extensionName << std::endl;
            }
        };
    
        void cleanup() {
            vkDestroyInstance(instance, nullptr);
            
            glfwDestroyWindow(window);
            
            glfwTerminate();
        }

    

};


int main() {
    HelloTriangleApplication app;
    
    try {
        app.run();
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

