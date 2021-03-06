#include "CEGUIScriptModule.h"
#include "CEGUISystem.h"
#include "CEGUILogger.h"
#include "CEGUIString.h"


namespace CEGUI
{
	// holds the default resource group ID for loading script files.
	String ScriptModule::d_defaultResourceGroup;

	ScriptModule::ScriptModule(void) :
	d_identifierString("Unknown scripting module (vendor did not set the ID string!)")
	{}

	const String& ScriptModule::getIdentifierString() const
	{
		return d_identifierString;
	}

	bool ScriptFunctor::operator()(const EventArgs& e) const
	{
		ScriptModule* scriptModule = System::getSingleton().getScriptingModule();

		if (scriptModule)
		{
			return scriptModule->executeScriptedEventHandler(scriptFunctionName, e);
		}
		else
		{
			Logger::getSingleton().logEvent("Scripted event handler '" + scriptFunctionName + "' could not be called as no ScriptModule is available.", Errors);

			return false;
		}
	}

}