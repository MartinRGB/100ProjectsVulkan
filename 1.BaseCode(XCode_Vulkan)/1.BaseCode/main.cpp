////
////  main.cpp
////  Test
////
////  Created by KingMartin on 2017/11/28.
////  Copyright © 2017年 KingMartin. All rights reserved.
////
//
//#include "main.hpp"
//
//#define GLFW_INCLUDE_VULKAN
//#include <GLFW/glfw3.h>
//
//#include <iostream>
//#include <stdexcept>
//#include <unistd.h>
//
//#include <Molten/MoltenVK/include/MoltenVK/mvk_vulkan.h>
//
//const int WIDTH = 800;
//const int HEIGHT = 600;
//
//class HelloTriangleApplication {
//public:
//    void run() {
//        initWindow();
//        initVulkan();
//        mainLoop();
//        cleanup();
//    }
//
//private:
//    GLFWwindow* window;
//
//    VkInstance instance;
//
//    void initWindow() {
//        glfwInit();
//
//        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
//
//        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
//    }
//
//    void initVulkan() {
//        createInstance();
//    }
//
//    void mainLoop() {
//        while (!glfwWindowShouldClose(window)) {
//            glfwPollEvents();
//        }
//    }
//
//    void cleanup() {
//        vkDestroyInstance(instance, nullptr);
//
//        glfwDestroyWindow(window);
//
//        glfwTerminate();
//    }
//
//    void createInstance() {
//        VkApplicationInfo appInfo = {};
//        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//        appInfo.pApplicationName = "Hello Triangle";
//        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
//        appInfo.pEngineName = "No Engine";
//        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
//        appInfo.apiVersion = VK_API_VERSION_1_0;
//
//        VkInstanceCreateInfo createInfo = {};
//        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//        createInfo.pApplicationInfo = &appInfo;
//
//        unsigned int glfwExtensionCount = 0;
//        const char** glfwExtensions;
//        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//
//        createInfo.enabledExtensionCount = glfwExtensionCount;
//        createInfo.ppEnabledExtensionNames = glfwExtensions;
//
//        createInfo.enabledLayerCount = 0;
//
//        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
//            throw std::runtime_error("failed to create instance!");
//        }
//    }
//};
//
//int main() {
//    HelloTriangleApplication app;
//
//    try {
//        app.run();
//    } catch (const std::runtime_error& e) {
//        std::cerr << e.what() << std::endl;
//        return EXIT_FAILURE;
//    }
//
//    return EXIT_SUCCESS;
//}


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <Molten/MoltenVK/include/MoltenVK/mvk_vulkan.h>

int main() {
    glfwInit();
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);
    
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    
    std::cout << extensionCount << " extensions supported" << std::endl;
    
    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;
    
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    
    glfwTerminate();
    
    return 0;
}

