#ifndef TREE_GENERATION_H
#define TREE_GENERATION_H

#include <unordered_map>
#include <cstdint>
#include <iostream>
#include <cmath> 
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Chunk.h"
#include "Tools.h"

class TreeGeneration {
private:
    int treeHeight = 5;

public:
    void GenerateTree(Chunk& currentChunk, int startX, int startY, int startZ, std::unordered_map<uint64_t, Chunk>& world) {

        // 1. Генерируем ствол (он всегда внутри текущего чанка, если дерево растет не на самой границе)
        for (int dy = 0; dy < treeHeight; dy++) {
            int targetY = startY + dy;
            if (targetY >= 0 && targetY < currentChunk.CHUNK_SIZE_Y) {
                currentChunk.chunkData[startX][targetY][startZ] = 4; // Блок дерева
            }
        }

        // 2. Генерируем крону
        for (int dx = -2; dx <= 2; dx++) {
            for (int dz = -2; dz <= 2; dz++) {
                for (int dy = treeHeight - 2; dy <= treeHeight + 1; dy++) {

                    // Пропуск углов для круглой формы
                    if (std::abs(dx) == 2 && std::abs(dz) == 2) continue;
                    if (std::abs(dy) >= 6 && (std::abs(dx) > 1 || std::abs(dz) > 1)) continue;

                    // Абсолютные координаты блока относительно старта текущего чанка
                    int localX = startX + dx;
                    int targetY = startY + dy;
                    int localZ = startZ + dz;

                    // Проверка по высоте
                    if (targetY < 0 || targetY >= currentChunk.CHUNK_SIZE_Y) continue;

                    // Вычисляем смещение чанков (в какую сторону улетели за границы 0-15)
                    int chunkOffsetX = 0;
                    int chunkOffsetZ = 0;

                    // Корректируем X координату и определяем соседний чанк
                    if (localX < 0) {
                        chunkOffsetX = -1;
                        localX += 16;
                    }
                    else if (localX >= 16) {
                        chunkOffsetX = 1;
                        localX -= 16;
                    }

                    // Корректируем Z координату и определяем соседний чанк
                    if (localZ < 0) {
                        chunkOffsetZ = -1;
                        localZ += 16;
                    }
                    else if (localZ >= 16) {
                        chunkOffsetZ = 1;
                        localZ -= 16;
                    }

                    // Находим целевой чанк (текущий или один из 8 соседей вокруг)
                    Chunk* targetChunk = nullptr;
                    if (chunkOffsetX == 0 && chunkOffsetZ == 0) {
                        targetChunk = &currentChunk;
                    }
                    else {
                        targetChunk = Tools::findChunk(currentChunk.chunkX + chunkOffsetX, currentChunk.chunkZ + chunkOffsetZ, world);
                    }

                    // Если чанк найден, устанавливаем листву
                    if (targetChunk != nullptr) {
                        if (targetChunk->chunkData[localX][targetY][localZ] == 0) {
                            targetChunk->chunkData[localX][targetY][localZ] = 5; // Блок листвы
                        }
                    }
                }
            }
        }
    }

    void GenerateChunkTrees(Chunk& chunk, std::unordered_map<uint64_t, Chunk>& world) {
        // Детерминированный сид чанка
        unsigned int chunkSeed = (chunk.chunkX * 73856093) ^ (chunk.chunkZ * 19349663);
        srand(chunkSeed);

        int attempts = 1 + (rand() % 2); // 1-2 дерева на чанк

        for (int i = 0; i < attempts; i++) {
            int x = rand() % 16;
            int z = rand() % 16;

            // Находим верхнюю точку земли
            int surfaceY = -1;
            for (int y = chunk.CHUNK_SIZE_Y - 1; y >= 0; y--) {
                if (chunk.chunkData[x][y][z] != 0) {
                    surfaceY = y;
                    break;
                }
            }

            if (surfaceY != -1) {
                int blockBelow = chunk.chunkData[x][surfaceY][z];

                // Растем только на траве (1) или земле (2)
                if (blockBelow == 1 || blockBelow == 2) {
                    if (surfaceY + treeHeight + 2 < chunk.CHUNK_SIZE_Y) {
                        // Передаем координаты блока НАД землей
                        GenerateTree(chunk, x, surfaceY + 1, z, world);
                    }
                }
            }
        }
    }
};

#endif
