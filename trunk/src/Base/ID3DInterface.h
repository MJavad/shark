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

class ID3DInterface abstract : public virtual Utils::IDynamicObject
{
public:
	virtual ~ID3DInterface() {};
	virtual void OnRender(uint32 timePassed) = 0;
	virtual void OnMessageReceived(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	void RegisterAsScriptElement();
	static void BindToLua(const boost::shared_ptr<lua_State> &luaState);

protected:
	ID3DInterface() {}
};
