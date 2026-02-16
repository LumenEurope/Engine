#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

class App
{
public:
    bool Init()
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            SDL_Log("SDL_Init failed: %s", SDL_GetError());
            return false;
        }

        // Create Vulkan window
        Window = SDL_CreateWindow(
            "SDL3 Vulkan",
            800,
            600,
            SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
        );

        if (!Window)
        {
            SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
            SDL_Quit();
            return false;
        }

        if (!CreateVulkanInstance())
        {
            SDL_DestroyWindow(Window);
            SDL_Quit();
            return false;
        }

        if (!CreateSurface())
        {
            vkDestroyInstance(Instance, nullptr);
            SDL_DestroyWindow(Window);
            SDL_Quit();
            return false;
        }

        return true;
    }

    void Run()
    {
        while (Running)
        {
            ProcessEvents();
        }
    }

    void Shutdown()
    {
        if (Surface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(Instance, Surface, nullptr);
            Surface = VK_NULL_HANDLE;
        }

        if (Instance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(Instance, nullptr);
            Instance = VK_NULL_HANDLE;
        }

        if (Window)
        {
            SDL_DestroyWindow(Window);
            Window = nullptr;
        }

        SDL_Quit();
    }

private:

    bool CreateVulkanInstance()
    {
        Uint32 ExtensionCount = 0;

        const char* const* Extensions =
            SDL_Vulkan_GetInstanceExtensions(&ExtensionCount);

        if (!Extensions)
        {
            SDL_Log("SDL_Vulkan_GetInstanceExtensions failed: %s", SDL_GetError());
            return false;
        }

        if (ExtensionCount == 0)
        {
            SDL_Log("No Vulkan extensions returned by SDL");
            return false;
        }

        VkApplicationInfo AppInfo{};
        AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        AppInfo.pApplicationName = "MyGame";
        AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        AppInfo.pEngineName = "NoEngine";
        AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        AppInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo CreateInfo{};
        CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        CreateInfo.pApplicationInfo = &AppInfo;
        CreateInfo.enabledExtensionCount = ExtensionCount;
        CreateInfo.ppEnabledExtensionNames = Extensions;

        VkResult Result = vkCreateInstance(&CreateInfo, nullptr, &Instance);

        if (Result != VK_SUCCESS)
        {
            SDL_Log("vkCreateInstance failed with error code: %d", Result);
            return false;
        }

        return true;
    }

    bool CreateSurface()
    {
        if (!SDL_Vulkan_CreateSurface(Window, Instance, nullptr, &Surface))
        {
            SDL_Log("SDL_Vulkan_CreateSurface failed: %s", SDL_GetError());
            return false;
        }

        if (Surface == VK_NULL_HANDLE)
        {
            SDL_Log("Vulkan surface handle is null");
            return false;
        }

        return true;
    }

    void ProcessEvents()
    {
        SDL_Event Event{};

        while (SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_EVENT_QUIT)
            {
                Running = false;
            }
        }
    }

private:
    SDL_Window* Window = nullptr;
    VkInstance Instance = VK_NULL_HANDLE;
    VkSurfaceKHR Surface = VK_NULL_HANDLE;
    bool Running = true;
};

int main(int argc, char* argv[])
{
    // Suppress unused arguments
    (void)argc;
    (void)argv;

    App MyApp;

    if (!MyApp.Init())
        return 1;

    MyApp.Run();
    MyApp.Shutdown();

    return 0;
}
