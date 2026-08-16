#ifndef TOOLS_H
#define TOOLS_H

#include <unordered_map>
#include <cstdint>
#include <cmath>
#include "Chunk.h"

class Tools {
public:
	static int NormalizeIntForChunk(float x) {
		float k = x / 16;
		float a = std::floor(k);
		return a;
	}

	static uint64_t GetKey(int x, int z) {
		return ((uint64_t)x << 32) | ((uint32_t)z & 0xFFFFFFFF);
	}

	// Мгновенный поиск чанка по его сетковым координатам X и Z за O(1)
	static Chunk* findChunk(int chunkX, int chunkZ, std::unordered_map<uint64_t, Chunk>& world) {
		auto it = world.find(GetKey(chunkX, chunkZ));
		if (it != world.end()) {
			return &(it->second); // Возвращаем указатель на реальный чанк из карты
		}
		return nullptr; // Если соседний чанк еще не сгенерирован
	}
};

#endif