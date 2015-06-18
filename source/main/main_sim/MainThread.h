/*
	This source file is part of Rigs of Rods
	Copyright 2005-2012 Pierre-Michel Ricordel
	Copyright 2007-2012 Thomas Fischer
	Copyright 2013-2014 Petr Ohlidal

	For more information, see http://www.rigsofrods.com/

	Rigs of Rods is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License version 3, as
	published by the Free Software Foundation.

	Rigs of Rods is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Rigs of Rods.  If not, see <http://www.gnu.org/licenses/>.
*/

/** 
	@file   MainThread.h
	@author Petr Ohlidal
	@date   05/2014
	@brief  Main thread logic. Manages startup and shutdown etc...
*/

#pragma once

#include "Application.h"
#include "RoRPrerequisites.h"
#include "RigEditor_ForwardDeclarations.h"

#include <pthread.h>

#include <Compositor/OgreCompositorManager2.h>
#include <Compositor/OgreCompositorNodeDef.h>
#include <Compositor/OgreCompositorWorkspaceDef.h>
#include <Compositor/Pass/PassClear/OgreCompositorPassClearDef.h>
#include <Compositor/Pass/PassScene/OgreCompositorPassSceneDef.h>
#include <Compositor/OgreTextureDefinition.h>
#include <Compositor/OgreCompositorShadowNode.h>
#include <Compositor/OgreCompositorWorkspace.h>

class GameScript;

namespace RoR
{

class MainThread
{
	friend class ::GameScript; // Needs LoadTerrain()

public:

	MainThread();
	
	void Go();

	void Exit();

	void RequestShutdown();

	void RequestRestart();

	void RequestExitCurrentLoop();

	void SetNextApplicationState(Application::State next_app_state)
	{
		m_next_application_state = next_app_state;
	}

	static void ChangedCurrentVehicle(Beam *previous_vehicle, Beam *current_vehicle);
	
	void RegenCache();

	void BackToMenu();
	void ChangeMap();
	/**
	* @return True if everything was prepared OK and simulation may start.
	*/
	bool SetupGameplayLoop(bool enable_network, Ogre::String preselected_map);

	void UnloadTerrain();

	float fps;

	//TODO: move somewhere else maybe?
	Ogre::CompositorWorkspace* getMainWorkSpace()
	{
		return mWorkSpace;
	}

protected:

	void EnterMainMenuLoop();

	void MainMenuLoopUpdate(float seconds_since_last_frame);
	
	void MainMenuLoopUpdateEvents(float seconds_since_last_frame);

	void EnterGameplayLoop();

	void LoadTerrain(Ogre::String const & terrain_file);

	void ShowSurveyMap(bool hide);

	void initMatManager();

	bool               m_no_rendering;
	bool               m_exit_loop_requested;
	bool               m_shutdown_requested;
	bool               m_restart_requested;
	pthread_mutex_t    m_lock;
	unsigned long      m_start_time;
	Application::State m_next_application_state;
	Application::State m_application_state;
	RigEditor::Main*   m_rig_editor;
	bool			   m_base_resource_loaded;

	std::map<std::string, bool> isLoadedMap;

	Ogre::CompositorManager2* pCompositorManager;
	Ogre::CompositorNodeDef *nodeDef;
	Ogre::CompositorTargetDef *targetDef;
	Ogre::CompositorWorkspace *mWorkSpace;
};

} // namespace RoR
