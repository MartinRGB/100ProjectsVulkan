#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>

// If you run Vulkan in macOS,you need add this
// #include <Molten/MoltenVK/include/MoltenVK/mvk_vulkan.h>

const int WIDTH = 800;
const int HEIGHT = 600;

// Lunar SDK 提供的通用验证层
const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}

class HelloTriangleApplication {

public:
	void run() {
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
	// Debug Repot 本身的句柄
	VkDebugReportCallbackEXT callback;
	void initVulkan() {
		createInstance();
		setupDebugCallback();
		std::cout << "Application is running successful" << std::endl;
	}

	void setupDebugCallback() {
		if (!enableValidationLayers) return;

		VkDebugReportCallbackCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
		//调用 debug 回调
		createInfo.pfnCallback = debugCallback;

		//查找使用 VkDebugReportCallbackEXT
		if (CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug callback!");
		}
	}

	void createInstance() {
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

		//根据验证层开启与否，提供拓展
		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();

			std::cout << "enabled Validation Layer Counts：" << std::endl;
			std::cout << "    " << createInfo.enabledLayerCount << std::endl;
			std::cout << "enabled Validation Layer Names：" << std::endl;
			std::cout << "    " << createInfo.ppEnabledLayerNames << std::endl;

		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		//Check Instance Creation's Result
		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("************* failed to create instance! *************");
		
		}
		else {
			if (&createInfo == nullptr || &instance == nullptr) {
				std::cout << "************* VK Initialization Failed *************" << std::endl;
			}
			else {
				std::cout << "************* VK Initialization Sucessed *************" << std::endl;

				if (enableValidationLayers && !checkValidationLayerSupport()) {
					std::cout << "************* VK validation layers are not supprted *************" << std::endl;
					throw std::runtime_error("validation layers requested, but not available!");
				}
				else {
					std::cout << "************* Validation layers are supprted & enabled *************" << std::endl;
				}
			}
		}



		std::cout << "============= Extensions' supporting list at bottom: =============" << std::endl;

		//Check Extensions Support
		checkExtensionsSupport();

		std::cout << "============= Extensions' supporting list at top: =============" << std::endl;

	}

	void checkExtensionsSupport() {
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
		DestroyDebugReportCallbackEXT(instance, callback, nullptr);
		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);

		glfwTerminate();
	}

	bool checkValidationLayerSupport() {
		uint32_t layerCount;
		//用 vkEnumerateInstanceLayerProperties 函数来遍历所有可用的拓展。
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		//测 validationLayers 中所有的验证层是否存在于 availableLayers 的列表中。使用 strcmp 需要 include <cstring>
		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	std::vector<const char*> getRequiredExtensions() {
		std::vector<const char*> extensions;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		for (uint32_t i = 0; i < glfwExtensionCount; i++) {
			extensions.push_back(glfwExtensions[i]);
		}

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
		}

		return extensions;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg,
		void* userData) {

		std::cerr << "validation layer: " << msg << std::endl;

		return VK_FALSE;
	};


};


int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
