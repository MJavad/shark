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
#include "ID3DFontObject.h"

namespace UI {
	class D3DFont : public virtual Utils::IDynamicObject
	{
	public:
		D3DFont(const FontDescription &fontDesc): m_fontDesc(fontDesc) {}

		void SetDevice(IDirect3DDevice9 *pDevice);

		void OnLostDevice() const {
			if (m_fontObject != nullptr)
				m_fontObject->OnLostDevice();
		}

		void OnResetDevice() const {
			if (m_fontObject != nullptr)
				m_fontObject->OnResetDevice();
		}

		void Release() {
			m_fontObject.reset();
		}

		const FontDescription& GetDescription() const { return m_fontDesc; }
		boost::shared_ptr<ID3DFontObject> GetObject() const { return m_fontObject; }
		virtual bool IsCreated() const { return m_fontObject != nullptr; }

	protected:
		FontDescription m_fontDesc;
		boost::shared_ptr<ID3DFontObject> m_fontObject;
	};
}
