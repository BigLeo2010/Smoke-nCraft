#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include<glad//glad.h>
#include<GLFW/glfw3.h>
#include <unordered_map>
#include <cstdint>
#include"Chunk.h"
#include"Raycast.h"
#include"World.h";

class InputSystem {
private:
	bool leftMousePressedLastFrame = false;
	bool rightMousePressedLastFrame = false;

public:
	void Placement(GLFWwindow* window, std::unordered_map<uint64_t, Chunk>& world, Camera camera);
	void Destruction(GLFWwindow* window, std::unordered_map<uint64_t, Chunk>& world, Camera camera);
};

#endif 
