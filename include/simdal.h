/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_H_
#define SIMDAL_H_

#include "simdal/csv/csv_file.h"
#include "simdal/db/db.h"
#include "simdal/db/db_csv.h"
#include "simdal/db/db_sqlite.h"
#include "simdal/db_entry/entry.h"
#include "simdal/db_entry/entry_bp_hit.h"
#include "simdal/db_entry/entry_bp_hit_desc.h"
#include "simdal/db_entry/entry_call_stack.h"
#include "simdal/db_entry/entry_cout.h"
#include "simdal/db_entry/entry_event.h"
#include "simdal/db_entry/entry_exception.h"
#include "simdal/db_entry/entry_interrupt.h"
#include "simdal/db_entry/entry_quantum.h"
#include "simdal/db_entry/entry_sc_proc.h"
#include "simdal/db_entry/entry_sc_sched.h"
#include "simdal/db_entry/entry_sniffer.h"
#include "simdal/db_entry/entry_target.h"
#include "simdal/db_entry/entry_task_switch.h"
#include "simdal/db_entry/entry_transaction.h"
#include "simdal/db_entry/entry_vcml_payload_irq.h"
#include "simdal/db_entry/entry_vcml_payload_pci.h"
#include "simdal/db_entry/entry_vcml_payload_sd_cmd.h"
#include "simdal/db_entry/entry_vcml_payload_sd_data.h"
#include "simdal/db_entry/entry_vcml_payload_spi.h"
#include "simdal/db_entry/entry_vcml_payload_tlm.h"
#include "simdal/db_entry/entry_vcml_payload_virtio.h"
#include "simdal/db_entry/entry_wait_event.h"
#include "simdal/mem_pool.h"
#include "simdal/utils.h"

#endif  // SIMDAL_H_
