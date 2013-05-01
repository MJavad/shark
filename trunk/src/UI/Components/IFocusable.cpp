#include "Misc/stdafx.h"
#include "IFocusable.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	std::weak_ptr<IFocusable> IFocusable::s_activeFocus;
}
}
