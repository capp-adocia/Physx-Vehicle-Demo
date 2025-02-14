#pragma once

//注意：glad头文件必须在glfw引用之前引用
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/string_cast.hpp>

#include <gtx/matrix_decompose.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/quaternion.hpp>

#include <vector>
#include <map>
#include <iostream>
#include <memory>
#include <assert.h>
#include <thread>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#define M_PI 3.14159265358979323846