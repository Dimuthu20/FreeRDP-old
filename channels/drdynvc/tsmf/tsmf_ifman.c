/*
   FreeRDP: A Remote Desktop Protocol client.
   Video Redirection Virtual Channel - Interface Manipulation

   Copyright 2010-2011 Vic Lee

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tsmf_constants.h"
#include "tsmf_types.h"
#include "tsmf_ifman.h"

int
tsmf_ifman_rim_exchange_capability_request(TSMF_IFMAN * ifman)
{
	uint32 CapabilityValue;

	CapabilityValue = GET_UINT32(ifman->input_buffer, 0);
	LLOGLN(0, ("tsmf_ifman_rim_exchange_capability_request: server CapabilityValue %d", CapabilityValue));

	ifman->output_buffer_size = 8;
	ifman->output_buffer = malloc(8);
	SET_UINT32(ifman->output_buffer, 0, 1); /* CapabilityValue */
	SET_UINT32(ifman->output_buffer, 4, 0); /* Result */

	return 0;
}

int
tsmf_ifman_set_channel_params(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_set_channel_params:"));
	ifman->output_pending = 1;
	return 0;
}

int
tsmf_ifman_exchange_capability_request(TSMF_IFMAN * ifman)
{
	char * p;
	uint32 numHostCapabilities;
	uint32 i;
	uint32 CapabilityType;
	uint32 cbCapabilityLength;
	uint32 v;

	ifman->output_buffer_size = ifman->input_buffer_size + 4;
	ifman->output_buffer = malloc(ifman->output_buffer_size);
	memcpy(ifman->output_buffer, ifman->input_buffer, ifman->input_buffer_size);
	SET_UINT32(ifman->output_buffer, ifman->input_buffer_size, 0); /* Result */

	numHostCapabilities = GET_UINT32(ifman->output_buffer, 0);
	p = ifman->output_buffer + 4;
	for (i = 0; i < numHostCapabilities; i++)
	{
		CapabilityType = GET_UINT32(p, 0);
		cbCapabilityLength = GET_UINT32(p, 4);
		switch (CapabilityType)
		{
			case 1: /* Protocol version request */
				v = GET_UINT32(p, 8);
				LLOGLN(0, ("tsmf_ifman_exchange_capability_request: server protocol version %d", v));
				break;
			case 2: /* Supported platform */
				v = GET_UINT32(p, 8);
				LLOGLN(0, ("tsmf_ifman_exchange_capability_request: server supported platform %d", v));
				/* Claim that we support both MF and DShow platforms. */
				SET_UINT32(p, 8, MMREDIR_CAPABILITY_PLATFORM_MF | MMREDIR_CAPABILITY_PLATFORM_DSHOW);
				break;
			default:
				LLOGLN(0, ("tsmf_ifman_exchange_capability_request: unknown capability type %d", CapabilityType));
				break;
		}
		p += 8 + cbCapabilityLength;
	}
	ifman->output_interface_id = TSMF_INTERFACE_DEFAULT | STREAM_ID_STUB;
	return 0;
}

int
tsmf_ifman_check_format_support_request(TSMF_IFMAN * ifman)
{
	uint32 PlatformCookie;
	uint32 numMediaType;

	PlatformCookie = GET_UINT32(ifman->input_buffer, 0);
	/* NoRolloverFlags (4 bytes) ignored */
	numMediaType = GET_UINT32(ifman->input_buffer, 8);

	LLOGLN(0, ("tsmf_ifman_check_format_support_request: PlatformCookie %d numMediaType %d",
		PlatformCookie, numMediaType));

	/* TODO: check the actual supported format */

	ifman->output_buffer_size = 12;
	ifman->output_buffer = malloc(12);
	SET_UINT32(ifman->output_buffer, 0, 1); /* FormatSupported */
	SET_UINT32(ifman->output_buffer, 4, PlatformCookie);
	SET_UINT32(ifman->output_buffer, 8, 0); /* Result */

	ifman->output_interface_id = TSMF_INTERFACE_DEFAULT | STREAM_ID_STUB;

	return 0;
}

int
tsmf_ifman_on_new_presentation(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_on_new_presentation:"));
	ifman->output_pending = 1;
	return 0;
}

int
tsmf_ifman_add_stream(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_add_stream:"));
	ifman->output_pending = 1;
	return 0;
}

int
tsmf_ifman_set_topology_request(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_set_topology_request:"));
	ifman->output_buffer_size = 8;
	ifman->output_buffer = malloc(8);
	SET_UINT32(ifman->output_buffer, 0, 1); /* TopologyReady */
	SET_UINT32(ifman->output_buffer, 4, 0); /* Result */
	ifman->output_interface_id = TSMF_INTERFACE_DEFAULT | STREAM_ID_STUB;
	return 0;
}

int
tsmf_ifman_remove_stream(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_remove_stream:"));
	ifman->output_pending = 1;
	return 0;
}

int
tsmf_ifman_shutdown_presentation(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_shutdown_presentation:"));
	ifman->output_buffer_size = 4;
	ifman->output_buffer = malloc(4);
	SET_UINT32(ifman->output_buffer, 0, 0); /* Result */
	ifman->output_interface_id = TSMF_INTERFACE_DEFAULT | STREAM_ID_STUB;
	return 0;
}

int
tsmf_ifman_on_stream_volume(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_on_stream_volume:"));
	ifman->output_pending = 1;
	return 0;
}

int
tsmf_ifman_on_channel_volume(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_on_channel_volume:"));
	ifman->output_pending = 1;
	return 0;
}

int
tsmf_ifman_set_video_window(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_set_video_window:"));
	ifman->output_pending = 1;
	return 0;
}

int
tsmf_ifman_update_geometry_info(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_update_geometry_info:"));
	ifman->output_pending = 1;
	return 0;
}

int
tsmf_ifman_set_allocator(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_set_allocator:"));
	ifman->output_pending = 1;
	return 0;
}

int
tsmf_ifman_notify_preroll(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_notify_preroll:"));
	ifman->output_pending = 1;
	return 0;
}

int
tsmf_ifman_on_sample(TSMF_IFMAN * ifman)
{
	uint32 StreamId;
	uint64 ThrottleDuration;
	uint32 cbData;

	StreamId = GET_UINT32(ifman->input_buffer, 16);
	ThrottleDuration = GET_UINT64(ifman->input_buffer, 40);
	cbData = GET_UINT32(ifman->input_buffer, 56);
	
	LLOGLN(0, ("tsmf_ifman_on_sample: StreamId %d ThrottleDuration %llu cbData %d",
		StreamId, ThrottleDuration, cbData));

	ifman->output_buffer_size = 24;
	ifman->output_buffer = malloc(24);
	SET_UINT32(ifman->output_buffer, 0, PLAYBACK_ACK); /* FunctionId */
	SET_UINT32(ifman->output_buffer, 4, StreamId); /* StreamId */
	SET_UINT64(ifman->output_buffer, 8, ThrottleDuration); /* DataDuration */
	SET_UINT64(ifman->output_buffer, 16, cbData); /* cbData */
	ifman->output_interface_id = TSMF_INTERFACE_CLIENT_NOTIFICATIONS | STREAM_ID_PROXY;

	return 0;
}

int
tsmf_ifman_on_flush(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_on_flush:"));
	ifman->output_pending = 1;
	return 0;
}

int
tsmf_ifman_on_end_of_stream(TSMF_IFMAN * ifman)
{
	uint32 StreamId;

	StreamId = GET_UINT32(ifman->input_buffer, 16);

	LLOGLN(0, ("tsmf_ifman_on_end_of_stream: StreamId %d", StreamId));

	ifman->output_buffer_size = 16;
	ifman->output_buffer = malloc(16);
	SET_UINT32(ifman->output_buffer, 0, CLIENT_EVENT_NOTIFICATION); /* FunctionId */
	SET_UINT32(ifman->output_buffer, 4, StreamId); /* StreamId */
	SET_UINT32(ifman->output_buffer, 8, TSMM_CLIENT_EVENT_ENDOFSTREAM); /* EventId */
	SET_UINT32(ifman->output_buffer, 12, 0); /* cbData */
	ifman->output_interface_id = TSMF_INTERFACE_CLIENT_NOTIFICATIONS | STREAM_ID_PROXY;

	return 0;
}

int
tsmf_ifman_on_playback_started(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_on_playback_started:"));

	ifman->output_buffer_size = 16;
	ifman->output_buffer = malloc(16);
	SET_UINT32(ifman->output_buffer, 0, CLIENT_EVENT_NOTIFICATION); /* FunctionId */
	SET_UINT32(ifman->output_buffer, 4, 0); /* StreamId */
	SET_UINT32(ifman->output_buffer, 8, TSMM_CLIENT_EVENT_START_COMPLETED); /* EventId */
	SET_UINT32(ifman->output_buffer, 12, 0); /* cbData */
	ifman->output_interface_id = TSMF_INTERFACE_CLIENT_NOTIFICATIONS | STREAM_ID_PROXY;

	return 0;
}

int
tsmf_ifman_on_playback_paused(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_on_playback_paused:"));
	ifman->output_pending = 1;
	return 0;
}

int
tsmf_ifman_on_playback_restarted(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_on_playback_restarted:"));
	ifman->output_pending = 1;
	return 0;
}

int
tsmf_ifman_on_playback_stopped(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_on_playback_stopped:"));

	ifman->output_buffer_size = 16;
	ifman->output_buffer = malloc(16);
	SET_UINT32(ifman->output_buffer, 0, CLIENT_EVENT_NOTIFICATION); /* FunctionId */
	SET_UINT32(ifman->output_buffer, 4, 0); /* StreamId */
	SET_UINT32(ifman->output_buffer, 8, TSMM_CLIENT_EVENT_STOP_COMPLETED); /* EventId */
	SET_UINT32(ifman->output_buffer, 12, 0); /* cbData */
	ifman->output_interface_id = TSMF_INTERFACE_CLIENT_NOTIFICATIONS | STREAM_ID_PROXY;

	return 0;
}

int
tsmf_ifman_on_playback_rate_changed(TSMF_IFMAN * ifman)
{
	LLOGLN(0, ("tsmf_ifman_on_playback_rate_changed:"));

	ifman->output_buffer_size = 16;
	ifman->output_buffer = malloc(16);
	SET_UINT32(ifman->output_buffer, 0, CLIENT_EVENT_NOTIFICATION); /* FunctionId */
	SET_UINT32(ifman->output_buffer, 4, 0); /* StreamId */
	SET_UINT32(ifman->output_buffer, 8, TSMM_CLIENT_EVENT_MONITORCHANGED); /* EventId */
	SET_UINT32(ifman->output_buffer, 12, 0); /* cbData */
	ifman->output_interface_id = TSMF_INTERFACE_CLIENT_NOTIFICATIONS | STREAM_ID_PROXY;

	return 0;
}

