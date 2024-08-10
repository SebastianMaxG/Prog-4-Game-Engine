#include "HighScoreSystem.h"
#include <fstream>
#include <algorithm>

#include "SelectComponent.h"

namespace lsmf {
    HighScoreSystem::HighScoreSystem(GameObject* gameObject, ScoreSystem* scoreSystem, const std::string& filePath, size_t nrElements)
        : BaseComponent(gameObject)
        , m_FilePath(filePath)
        , m_NrElements(nrElements)
        , m_ScoreSystem(scoreSystem)
    {
        LoadScores(filePath);

        // create a SelectComponent and add it to the GameObject
    }
    void HighScoreSystem::AddScore(const std::string& name, uint32_t score) {
        highScores.emplace_back(name, score);
        std::ranges::sort(highScores, [](const auto& a, const auto& b) {
            return a.second > b.second;
            });
        if (highScores.size() > m_NrElements) {
            highScores.pop_back();
        }
        SaveScore(name, score);
    }

    void HighScoreSystem::SaveScore(const std::string& name, uint32_t score) const
    {
        std::ofstream file(m_FilePath, std::ios::binary);
        file.write(name.c_str(), 3);
        file.write(reinterpret_cast<const char*>(&score), sizeof(score));

    }

    void HighScoreSystem::LoadScores(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (file.fail())
        {
            return;
		}
        char name[4] = { 0 };
        uint32_t score = 0;
        while (file.read(name, 3) && file.read(reinterpret_cast<char*>(&score), sizeof(score))) {
            highScores.emplace_back(name, score);
        }
    }
}
