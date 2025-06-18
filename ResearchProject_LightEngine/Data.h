#pragma once

// window params
#define WIDTH 1200
#define HEIGHT 800

// viewport
constexpr float g_viewPortScaleW = 1.0f;
constexpr float g_viewPortScaleH = 1.0f;

// vsync
constexpr bool g_vSync = false;

// camera
constexpr float g_defaultNearZ = 0.1f;
constexpr float g_defaultFarZ = 100.0f;
constexpr float g_defaultFOV = 90.0f;
constexpr float g_defaultCameraPositionZ = -2.0f;

constexpr float g_cameraMoveSpeed = 0.01f;
constexpr float g_cameraMouseSensitivity = 0.001f;

// sample texture
constexpr const char* g_sampleTexturePath = "Textures\\tile.jpg";