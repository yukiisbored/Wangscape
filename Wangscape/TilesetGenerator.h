#pragma once
#include <SFML/Graphics.hpp>
#include "Options.h"
#include "TileGenerator.h"
#include "TerrainImages.h"
#include "MetaOutput.h"
class TilesetGenerator
{
public:
    TilesetGenerator(const Options& options);
    ~TilesetGenerator();
    void generate(std::function<void(const sf::Texture&, std::string)> callback);
    void generateClique(const Options::Clique& clique, sf::RenderTexture& image, std::string filename,
                        TileGenerator::TileGenerateFunction callback);
    const Options& options;
    MetaOutput mo;
    TerrainImages images;
};
