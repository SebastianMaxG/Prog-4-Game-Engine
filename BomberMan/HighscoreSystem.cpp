#include "HighScoreSystem.h"
#include <fstream>
#include <algorithm>

#include "Command.h"
#include "SelectComponent.h"
#include "ScoreSystem.h"
#include "GameObject.h"
#include "GlobalEngineSignals.h"
#include "GlobalSignals.h"
#include "InputHandler.h"
#include "SceneManager.h"
#include "Transform.h"
#include "TransformComponent.h"

namespace lsmf {
    HighScoreSystem::HighScoreSystem(GameObject* gameObject, ScoreSystem* scoreSystem, const std::string& filePath, size_t nrElements, std::shared_ptr<Font> font, Scene* scene)
        : BaseComponent(gameObject)
        , m_FilePath(filePath)
        , m_NrElements(nrElements)
        , m_ScoreSystem(scoreSystem)
		, m_Font(font)
		, m_Scene(scene)
    {
        LoadScores(filePath);

        // create a SelectComponent and add it to the GameObject
        auto selectComp = std::make_unique<SelectComponent>(gameObject,font);
        m_SelectComponent = selectComp.get();
        gameObject->AddComponent(std::move(selectComp));

        auto registerCmd = std::make_unique<Command>();
        registerCmd->BindKey(SDLK_RETURN);
        registerCmd->BindKey(SDL_CONTROLLER_BUTTON_A);
        registerCmd->BindFunction(this, &HighScoreSystem::Register);
        InputHandler::GetInstance().BindCommand("RegisterScore", std::move(registerCmd));

        auto returnCmd = std::make_unique<Command>();
        returnCmd->BindKey(SDLK_ESCAPE);
        returnCmd->BindKey(SDL_CONTROLLER_BUTTON_START);
        returnCmd->BindFunction(this, &HighScoreSystem::ReturnToMenu);
        InputHandler::GetInstance().BindCommand("ReturnToMenu", std::move(returnCmd));
    }

    HighScoreSystem::~HighScoreSystem()
    {
        InputHandler::GetInstance().RemoveCommand("RegisterScore");
        InputHandler::GetInstance().RemoveCommand("ReturnToMenu");
    }

    void HighScoreSystem::AddScore(const std::string& name, uint32_t score) {
        highScores.emplace_back(name, score);
        std::ranges::sort(highScores, [](const auto& a, const auto& b) {
            return a.second > b.second;
            });
        if (highScores.size() > m_NrElements) {
            highScores.pop_back();
        }
        SaveScore();
    }

    void HighScoreSystem::Register(SDL_Event e)
    {
        if (m_Saved)
		{
			return;
		}
        if (e.type != SDL_KEYUP && e.type != SDL_CONTROLLERBUTTONUP)
		{
			return;
		}
        if (m_SelectComponent)
        {
            AddScore(m_SelectComponent->GetString(), m_ScoreSystem->GetScore());

            //GetGameObject()->RemoveComponent(static_cast<std::unique_ptr<BaseComponent>>(m_SelectComponent));
            Transform transform = GetGameObject()->GetTransform()->GetWorldTransform();
            for (size_t i = 0; i < m_NrElements && i < highScores.size(); ++i)
            {
                transform.Translate({ 0,25,0 });
                const auto& [name, score] = highScores[i];
                auto gameObject = std::make_unique<GameObject>();
                gameObject->GetTransform()->SetPosition(transform.GetPosition());
                auto textComponent = std::make_unique<TextComponent>(gameObject.get(), name + ": " + std::to_string(score), m_Font);
                gameObject->AddComponent(std::move(textComponent));
                m_Scene->Add(std::move(gameObject));

            }

            m_Saved = true;

            m_SelectComponent->Deactivate();
        }
    }

    void HighScoreSystem::ReturnToMenu(SDL_Event e)
    {
		if (e.type != SDL_KEYUP && e.type != SDL_CONTROLLERBUTTONUP)
		{
			return;
		}
        globalSignals::ReturnToMenu.Emit();
        globalSignals::ReturnToMenu.Update();

    }

    void HighScoreSystem::SaveScore() const
    {
        if (std::ofstream file{ m_FilePath, std::ios::binary }; file.is_open())
        {
            // write the highscores to the file
            for (const auto& [name, score] : highScores)
			{
				file.write(name.c_str(), 3);
				file.write(reinterpret_cast<const char*>(&score), sizeof(score));
			}
        }


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
