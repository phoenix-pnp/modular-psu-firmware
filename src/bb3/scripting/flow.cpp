/*
* EEZ Generic Firmware
* Copyright (C) 2018-present, Envox d.o.o.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <bb3/psu/gui/psu.h>

#include <bb3/scripting/scripting.h>
#include <bb3/scripting/flow.h>
#include <bb3/scripting/internal.h>
#include <bb3/scripting/scpi_context.h>
#include <bb3/scripting/thread.h>

#include <bb3/psu/gui/keypad.h>

#include <eez/flow/flow.h>

namespace eez {
namespace scripting {

bool loadFlowScript(int *err) {
	int localErr = SCPI_RES_OK;
	if (!err) {
		err = &localErr;
	}

	if (!eez::loadExternalAssets(g_scriptPath, err)) {
		char scriptName[64];
		getFileName(g_scriptPath, scriptName, sizeof(scriptName));
		ErrorTrace("App error: %s\n", scriptName);

		afterScriptCleanup(true);
		setStateIdle();

		if (err == &localErr) {
			generateError(*err);
		}

		return false;
	}

	startFlowScript();

	return true;
}

void startFlowScript() {
	if (!isGuiThread()) {
		sendMessageToGuiThread(GUI_QUEUE_MESSAGE_FLOW_START);
		return;
	}

	char scriptName[64];
	getFileName(g_scriptPath, scriptName, sizeof(scriptName));
	InfoTrace("App started: %s\n", scriptName);

    afterScriptCleanup(false);

	flow::start(g_externalAssets);

	psu::gui::g_psuAppContext.dialogOpen(nullptr);
}

void flowTick() {
	if (isFlowRunning() && g_state != STATE_STOPPING) {
		flow::tick();
	}
}

void stopFlowScript() {
	if (!isGuiThread()) {
		sendMessageToGuiThread(GUI_QUEUE_MESSAGE_FLOW_STOP);
		return;
	}

	flow::stop();
}

////////////////////////////////////////////////////////////////////////////////

void executeFlowAction(const WidgetCursor &widgetCursor, int16_t actionId) {
	if (isFlowRunning()) {
		flow::executeFlowAction(widgetCursor, actionId, nullptr);
	}
}

////////////////////////////////////////////////////////////////////////////////

bool isFlowRunning() {
	return !flow::isFlowStopped();
}

void dataOperation(int16_t dataId, DataOperationEnum operation, const gui::WidgetCursor &widgetCursor, Value &value) {
	if (isFlowRunning()) {
		flow::dataOperation(dataId, operation, widgetCursor, value);
	}
}

} // scripting
} // eez

namespace eez {
namespace flow {

bool isFlowRunning() {
	return scripting::isFlowRunning();
}

void replacePage(int16_t pageId, uint32_t animType, uint32_t speed, uint32_t delay) {
	eez::psu::gui::replacePage(pageId);
}

void showKeyboard(Value label, Value initialText, Value minChars, Value maxChars, bool isPassword, void(*onOk)(char *), void(*onCancel)()) {
	eez::psu::gui::startTextKeypad(label.getString(), initialText.getString(), minChars.toInt32(), maxChars.toInt32(), isPassword, onOk, onCancel);
}

void showKeypad(Value label, Value initialValue, Value min, Value max, Unit unit, void(*onOk)(float), void(*onCancel)()) {
	NumericKeypadOptions options;
	options.pageId = PAGE_ID_NUMERIC_KEYPAD;
	options.min = min.toFloat();
	options.max = max.toFloat();
	options.editValueUnit = unit;

	eez::psu::gui::startNumericKeypad(&eez::psu::gui::g_psuAppContext, label.getString(), initialValue, options, onOk, nullptr, onCancel);
}

void stopScript() {
	scripting::stopScript();
}

} // flow
} // eez
