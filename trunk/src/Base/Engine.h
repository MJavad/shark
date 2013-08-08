/*
 * This file is part of D3DShark - DirectX Component Framework
 * Copyright (C) 2012-2013 Michael Bleis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

class Engine {
	SINGLETON_CLASS(Engine) :
		m_isInitialized(false),
		m_instance(nullptr),
		m_lastPulse(0),
		m_mainThreadId(0) {}

public:
	void Initialize(HINSTANCE hInstance);
	void InitializeEnvironment();
	bool InitializeShutdown() const;

	void OnShutDown();
	void OnPulse();
	void OnProcessDetach();
	void OnWindowDestroy();

	bool IsInitialized() const {
		return m_isInitialized;
	}

	bool IsShuttingDown() const {
		return m_shutdownEvent.is_set();
	}

	HINSTANCE GetInstance() const {
		return m_instance;
	}

	uint32 GetMainThreadId() const {
		return m_mainThreadId;
	}

	// Events and Timers...
	Utils::TimerDispatcher PulseTimer;
	Utils::Event<void (uint32, uint32)> NextPulse;
	Utils::Event<void (uint32, uint32)> OnPulseEvent;

protected:
	bool m_isInitialized;
	HINSTANCE m_instance;
	Utils::Signal m_shutdownEvent;
	Utils::Signal m_shutdownComplete;
	uint32 m_lastPulse;
	uint32 m_mainThreadId;

	static BOOL WINAPI _shutdownThread(LPVOID lpParam);
};

#define sEngine ::Engine::Instance()
