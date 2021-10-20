/*
 * EEZ Modular Firmware
 * Copyright (C) 2021-present, Envox d.o.o.
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

#include <stdio.h>

#include <eez/flow/components.h>
#include <eez/flow/components/call_action.h>
#include <eez/flow/debugger.h>

using namespace eez::gui;

namespace eez {
namespace flow {

struct CallActionComponenentExecutionState : public ComponenentExecutionState {
	FlowState *flowState;

	~CallActionComponenentExecutionState() {
		freeFlowState(flowState);
	}
};

void executeCallActionComponent(FlowState *flowState, unsigned componentIndex) {
	auto assets = flowState->assets;
	auto component = (CallActionActionComponent *)flowState->flow->components.item(assets, componentIndex);

	auto flowIndex = component->flowIndex;
	if (flowIndex < 0 || flowIndex >= (int)flowState->flowDefinition->flows.count) {
		throwError(flowState, componentIndex, "Invalid action flow index in CallAction component\n");
		return;
	}

	auto callActionComponenentExecutionState = (CallActionComponenentExecutionState *)flowState->componenentExecutionStates[componentIndex];
	if (callActionComponenentExecutionState) {
		throwError(flowState, componentIndex, "CallAction component is already running\n");
		return;
	}

	FlowState *actionFlowState = initActionFlowState(assets, flowIndex, flowState, componentIndex);

	if (actionFlowState->numActiveComponents == 0) {
		freeFlowState(actionFlowState);
		propagateValueThroughSeqout(flowState, componentIndex);
	} else {
		flowState->numActiveComponents++;

		callActionComponenentExecutionState = ObjectAllocator<CallActionComponenentExecutionState>::allocate(0x4c669d4b);
		callActionComponenentExecutionState->flowState = actionFlowState;
		flowState->componenentExecutionStates[componentIndex] = callActionComponenentExecutionState;
	}
}

} // namespace flow
} // namespace eez