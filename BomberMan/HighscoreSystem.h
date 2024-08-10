#pragma once
#include <vector>
#include <string>
#include <utility>

#include "BaseComponent.h"

namespace lsmf
{
	class ScoreSystem;
}

namespace lsmf {
    class HighScoreSystem : public BaseComponent
	{

    public:
        HighScoreSystem(GameObject* gameObject, ScoreSystem* scoreSystem, const std::string& filePath ,size_t nrElements);

        void AddScore(const std::string& name, uint32_t score);
        std::vector<std::pair<std::string, uint32_t>> GetHighScores() const { return highScores; }

    private:
        void SaveScore(const std::string& name, uint32_t score) const;
        void LoadScores(const std::string& filename);
        std::vector<std::pair<std::string, uint32_t>> highScores{};
        std::string m_FilePath;
        size_t m_NrElements;
        ScoreSystem* m_ScoreSystem;
        
    };
}

