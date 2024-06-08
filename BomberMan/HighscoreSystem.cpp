#include "HighScoreSystem.h"
#include <fstream>
#include <algorithm>

namespace lsmf {
    void HighScoreSystem::AddScore(const std::string& name, uint32_t score) {
        highScores.emplace_back(name, score);
        std::sort(highScores.begin(), highScores.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
            });
        if (highScores.size() > 5) {
            highScores.pop_back();
        }
    }

    void HighScoreSystem::SaveScores(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        for (const auto& [name, score] : highScores) {
            file.write(name.c_str(), 3);
            file.write(reinterpret_cast<const char*>(&score), sizeof(score));
        }
    }

    void HighScoreSystem::LoadScores(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        char name[4] = { 0 };
        uint32_t score = 0;
        while (file.read(name, 3) && file.read(reinterpret_cast<char*>(&score), sizeof(score))) {
            highScores.emplace_back(name, score);
        }
    }
}