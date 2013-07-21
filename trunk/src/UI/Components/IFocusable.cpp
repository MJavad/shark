#include "Misc/stdafx.h"
#include "IFocusable.h"
#include "Base/WndProc.h"

namespace UI {
namespace Components {
	boost::weak_ptr<IFocusable> IFocusable::s_activeFocus;
}
}
