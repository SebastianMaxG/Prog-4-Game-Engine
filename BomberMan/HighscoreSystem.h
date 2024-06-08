#pragma once
#include <vector>
#include <string>
#include <utility>

namespace lsmf {
    class HighScoreSystem {
    public:
        void AddScore(const std::string& name, uint32_t score);
        void SaveScores(const std::string& filename);
        void LoadScores(const std::string& filename);
        std::vector<std::pair<std::string, uint32_t>> GetHighScores() const { return highScores; }

    private:
        std::vector<std::pair<std::string, uint32_t>> highScores;
    };
}

