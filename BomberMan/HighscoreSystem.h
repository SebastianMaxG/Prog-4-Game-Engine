#pragma once
#include <vector>
#include <string>
#include <utility>

#include "BaseComponent.h"

namespace lsmf
{
	class Scene;
}

namespace lsmf {
	class Font;

	class ScoreSystem;
	class SelectComponent;


    class HighScoreSystem : public BaseComponent
	{

    public:
        HighScoreSystem(GameObject* gameObject, ScoreSystem* scoreSystem, const std::string& filePath ,size_t nrElements, std::shared_ptr<Font> font, Scene* scene);
        ~HighScoreSystem() override;

        void AddScore(const std::string& name, uint32_t score);
        std::vector<std::pair<std::string, uint32_t>> GetHighScores() const { return highScores; }

        void Register(SDL_Event e);
        void ReturnToMenu(SDL_Event e);

    private:
        void SaveScore() const;
        void LoadScores(const std::string& filename);
        std::vector<std::pair<std::string, uint32_t>> highScores{};
        std::string m_FilePath;
        size_t m_NrElements;
        ScoreSystem* m_ScoreSystem;
        SelectComponent* m_SelectComponent;
        Scene* m_Scene;
        std::shared_ptr<Font> m_Font;
        bool m_Saved = false;


        
    };
}

