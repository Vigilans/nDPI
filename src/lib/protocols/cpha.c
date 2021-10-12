/*
 * qq.c
 *
 * Copyright (C) 2011-21 - ntop.org
 *
 * This file is part of nDPI, an open source deep packet inspection
 * library based on the OpenDPI and PACE technology by ipoque GmbH
 *
 * nDPI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * nDPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with nDPI.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "ndpi_protocol_ids.h"

/* CPHA - CheckPoint High Availability Protocol */

#define NDPI_CURRENT_PROTO NDPI_PROTOCOL_CPHA

#include "ndpi_api.h"


void ndpi_search_cpha(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow) {
  struct ndpi_packet_struct *packet = ndpi_get_packet_struct(ndpi_struct);
  const u_int16_t cpha_port = htons(8116);
  
  NDPI_LOG_DBG(ndpi_struct, "search CPHA\n");

  if((packet->payload_packet_len > 20)
     && (packet->payload[0] == 0x1a)
     && (packet->payload[1] == 0x90)
     && packet->udp
     && packet->iph
     && (packet->udp->source == cpha_port)
     && (packet->udp->dest   == cpha_port)
     && packet->iph->saddr   == 0 /* 0.0.0.0 */
     ) {
    ndpi_set_detected_protocol(ndpi_struct, flow, NDPI_PROTOCOL_CPHA, NDPI_PROTOCOL_UNKNOWN);
  } else
    NDPI_EXCLUDE_PROTO(ndpi_struct, flow);      
}


void init_cpha_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id,
			 NDPI_PROTOCOL_BITMASK *detection_bitmask) {
  ndpi_set_bitmask_protocol_detection("CPHA", ndpi_struct, detection_bitmask, *id,
				      NDPI_PROTOCOL_CPHA,
				      ndpi_search_cpha,
				      NDPI_SELECTION_BITMASK_PROTOCOL_UDP_WITH_PAYLOAD,
				      SAVE_DETECTION_BITMASK_AS_UNKNOWN,
				      ADD_TO_DETECTION_BITMASK);

  *id += 1;
}
