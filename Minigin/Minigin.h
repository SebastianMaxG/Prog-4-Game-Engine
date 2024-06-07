#pragma once
#include <string>
#include <functional>

namespace lsmf
{
	class Minigin
	{
	public:
		explicit Minigin(const std::string& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

		SDL_Window* GetSDLWindow();
	private:
		SDL_Window* m_windowPtr{};

		double fixed_time_step;
		int ms_per_frame;

		int max_framerate{120}; // max framerate if v-sync is off
		bool v_sync{true};
	};
}