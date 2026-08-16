#include"InputSystem.h"

void InputSystem::Placement(GLFWwindow* window, std::unordered_map<uint64_t, Chunk>& world, Camera camera) {
	bool isRightClicked = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

	if (isRightClicked && !rightMousePressedLastFrame)
	{
		// Запускаем трассировку луча длиной 5.5 блоков
		Raycast::HitResult hitResult = Raycast::Trace(camera, world, 10.0f);

		// Если во что-то попали и целевой чанк существует
		if (hitResult.hit && hitResult.placeChunk != nullptr)
		{
			// Переводим мировую позицию установки в локальные координаты чанка
			int cx = hitResult.placeChunk->GetX();
			int cz = hitResult.placeChunk->GetZ();

			int localX = hitResult.placePos.x - (cx * Chunk::CHUNK_SIZE_X);
			int localY = hitResult.placePos.y;
			int localZ = hitResult.placePos.z - (cz * Chunk::CHUNK_SIZE_Z);

			// Ставим блок Травы (ID = 1)
			hitResult.placeChunk->SetBlock(localX, localY, localZ, 3);

			// Перегенерация меша и обновление данных на GPU
			hitResult.placeChunk->BuildChunkMesh();
			hitResult.placeChunk->UploadChunkToGPU(); // Убедись, что метод в твоем main называется именно так
		}
	}
	rightMousePressedLastFrame = isRightClicked;
}

void InputSystem::Destruction(GLFWwindow* window, std::unordered_map<uint64_t, Chunk>& world, Camera camera) {
	bool isLeftClicked = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

	if (isLeftClicked && !leftMousePressedLastFrame)
	{
		Raycast::HitResult hitResult = Raycast::Trace(camera, world, 10.0f);

		// Если во что-то попали и целевой чанк существует
		if (hitResult.hit && hitResult.targetChunk != nullptr)
		{
			// Переводим мировую позицию установки в локальные координаты чанка
			int cx = hitResult.targetChunk->GetX();
			int cz = hitResult.targetChunk->GetZ();

			int localX = hitResult.blockPos.x - (cx * Chunk::CHUNK_SIZE_X);
			int localY = hitResult.blockPos.y;
			int localZ = hitResult.blockPos.z - (cz * Chunk::CHUNK_SIZE_Z);

			// Удаляем блок
			hitResult.targetChunk->SetBlock(localX, localY, localZ, 0);

			// Перегенерация меша и обновление данных на GPU
			hitResult.targetChunk->BuildChunkMesh();
			hitResult.targetChunk->UploadChunkToGPU(); // Убедись, что метод в твоем main называется именно так
		}
	}
	leftMousePressedLastFrame = isLeftClicked;
}