#pragma once

#include <LDtkLoader/Project.hpp>
#include <Luna/Context.hpp>
#include <Luna/Logger.hpp>

#include <numeric>
#include <unordered_map>
#include <vector>
#include <string>

namespace luna {

    std::unordered_map<std::string, Texture> loadTilesets(const ldtk::Project& project, std::string rootFolder) {
        std::unordered_map<std::string, Texture> result;
        for (const auto& tileset : project.allTilesets()) {
            Texture tex = Texture::loadFromFile((rootFolder + tileset.path).c_str());
            tex.setMagFilter(luna::TextureFilter::Nearest);
            result.emplace(tileset.name, std::move(tex));
        }
        return result;
    }

    std::unordered_map<std::string, Texture> loadTilesets(const ldtk::Project& project) {
        return loadTilesets(project, project.getFilePath().directory());
    }

    class TileLayer {
    public:
        TileLayer(const ldtk::Layer& layer, const Texture* tileset, float pixelsPerUnit = 16.0f) :
            m_material(*luna::getDefaultUnlitMaterial()) {
            m_material.setMainTexture(tileset);

            std::vector<float> tileDepth;
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            tileDepth.resize(layer.getGridSize().x * layer.getGridSize().y, 0.0f);
            vertices.reserve(layer.allTiles().size() * 4);
            indices.reserve(vertices.size() * 6);

            for (const auto& tile : layer.allTiles()) {
                unsigned start = vertices.size();
                indices.push_back(start + 0);
                indices.push_back(start + 2);
                indices.push_back(start + 1);

                indices.push_back(start + 0);
                indices.push_back(start + 3);
                indices.push_back(start + 2);

                float depth = tileDepth[tile.getGridPosition().x + tile.getGridPosition().y * layer.getGridSize().x];
                for (const auto& vertex : tile.getVertices()) {
                    vertices.emplace_back(
                        glm::vec3(vertex.pos.x, -vertex.pos.y, depth * pixelsPerUnit) / pixelsPerUnit,
                        glm::vec2(vertex.tex.x, tileset->getHeight() - vertex.tex.y) / glm::vec2(tileset->getSize()),
                        glm::vec3(0.0f, 0.0f, 1.0f)
                    );
                }
                tileDepth[tile.getGridPosition().x + tile.getGridPosition().y * layer.getGridSize().x] += 0.001f;
            }

            m_mesh.setVertices(vertices.data(), vertices.size());
            m_mesh.setIndices(indices.data(), indices.size());
        }

        const Material& getMaterial() const {
            return m_material;
        }

        const Mesh& getMesh() const {
            return m_mesh;
        }

    private:
        Material m_material;
        Mesh m_mesh;
    };

    class TileLevel {
    public:
        TileLevel(const ldtk::Level& level, const std::unordered_map<std::string, Texture>& tilesets, float pixelsPerUnit = 16.0f) {
            for (const auto& layer : level.allLayers()) {
                if (!layer.hasTileset()) continue;

                auto tileset = tilesets.find(layer.getTileset().name);

                if (tileset == tilesets.end()) {
                    luna::log("Tileset referenced in tile layer did not exist in the map", luna::MessageSeverity::Error);
                    continue;
                }

                m_tileLayers.emplace_back(layer, &tileset->second, pixelsPerUnit);
            }
        }

        const std::vector<TileLayer>& tileLayers() {
            return m_tileLayers;
        }

    private:
        std::vector<TileLayer> m_tileLayers;
    };
}