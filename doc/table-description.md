# Table Description

The following tables are used to store the tracing data. If a SQLite3 database is used, all tables are
stored in the database file. If CSV is used, one CSV file is created for each table.

## bp_hit (`*_bp_hit.csv`)

Field description:

* `id` (`INTEGER`): Unique ID for each traced breakpoint hit
* `bp_id` (`INTEGER`): ID of the hit breakpoint (corresponds to bp_hit_desc.id)
* `timestamp` (`BIGINT`): Elapsed nanoseconds since start at the time the breakpoint has been hit
* `timestamp_rt` (`BIGINT`): Elapsed real-time (ns) since program start until breakpoint hit
* `target_id` (`INTEGER`): ID that specifies the target which hit the breakpoint
(corresponds to `targets.id`)

Example table:
| id | bp_id | timestamp | timestamp_rt | target_id |
|:--:|:-----:|:---------:|:------------:|:---------:|
| 1  | 1     | 56056     | 64212        | 2         |
| 2  | 2     | 123225    | 85452        | 1         |

## bp_hit_desc (`*_bp_hit_desc.csv`)

Field description:

* `id` (`INTEGER`): Unique ID for each traceable breakpoint
* `desc` (`STRING`): Description of the breakpoint (e.g., _sched_entry_)

Example table:
| id | desc          |
|:--:|:-------------:|
| 1  | "sched_entry" |
| 2  | "my_bp"       |

## call_stack (`*_call_stack.csv`)

Field description:

* `id` (`INTEGER`): Unique ID for each call stack
* `timestamp` (`BIGINT`): Elapsed nanoseconds since start at the time the call stack is captured
* `timestamp_rt` (`BIGINT`): Elapsed real-time nanoseconds since start at the time the call stack is captured
* `target_id` ('INTEGER'): ID of the target the call stack belongs to

Example table:
| id | timestamp | timestamp_rt | target_id |
|:--:|:---------:|:------------:|:---------:|
| 1  | 1000000   | 12345        | 1         |
| 2  | 2000000   | 23456        | 2         |

## call_stack_frame (`*_call_stack_frame_.csv`)

Field description:

* `id` (`INTEGER`): Unique ID for each call stack entry
* `call_stack_id` (`INTEGER`): ID of the call stack the entry belongs to (`call_stack.id`)
* `level` (`INTEGER`): Depth of the stack frame
* `pc` (`INTEGER`): Value of the program counter
* `pc` (`INTEGER`): Value of the frame pointer
* `symbol_id` (`INTEGER`): If the name of the function is known, the field contains the ID of the corresponding symbol (`symbols.id`)

Example table:
| id | call_stack_id | level | pc               | fp               | symbol_id |
|:--:|:-------------:|:-----:|:----------------:|:----------------:|:---------:|
| 1  | 1             | 0     | 0                | 0                |           |
| 2  | 2             | 0     | FFFFFF800874D6BC | FFFFFF8008A43E20 | 1         |
| 3  | 2             | 1     | FFFFFF800874D83C | FFFFFF8008A43E30 | 2         |

## cout (`*_cout.csv`)

Field description:

* `id` (`INTEGER`): Unique ID for each printed line
* `timestamp` (`BIGINT`): Elapsed nanoseconds since start at the time the event is notified
* `timestamp_rt` (`BIGINT`): Elapsed real-time nanoseconds since start at the time the event is notified
* `text` (`STRING`): Text that is printed to the stdout

Example table:
| id | timestamp | timestamp_rt | text             |
|:--:|:---------:|:------------:|:----------------:|
| 1  | 10        | 123456       | Linux booting... |
| 2  | 20        | 223456       | Boot output line |

## event (`*_event.csv`)

Field description:

* `id` (`INTEGER`): Unique ID for each notified event
* `timestamp` (`BIGINT`): Elapsed nanoseconds since start at the time the event is notified
* `timestamp_rt` (`BIGINT`): Elapsed real-time nanoseconds since start at the time the event is notified
* `delay` (`INTEGER`): Delay between the timestamp and the time the event is notified
* `event` (`STRING`): Name of the event
* `module_id` (`INTEGER`): ID of the module the events belongs to (`modules.id`)

Example table:
| id | timestamp | timestamp_rt | delay       | event           | module_id |
|:--:|:---------:|:------------:|:-----------:|:---------------:|:---------:|
| 1  | 0         | 425708047    | 0           | CLOCK_update_ev | 1         |
| 2  | 0         | 436551892    | 0           | IN_free         | 2         |
| 3  | 293162    | 487432606    | 0           | IN_free         | 3         |

## exception (`*_exc.csv`)

Field description:

* `id` (`INTEGER`): Unique ID for each traced exception
* `exc_class` (`INTEGER`): Exception class
* `timestamp_entry` (`BIGINT`): Elapsed nanoseconds since start at the time the exception handler
has been called
* `timestamp_entry_rt` (`BIGINT`): Elapsed real-time (ns) since program start until Exception Handler call
* `timestamp_exit` (`BIGINT`): Elapsed nanoseconds since start at the time the exception handler has
returned
* `timestamp_exit_rt` (`BIGINT`): Elapsed real-time (ns) since program start until Exception Handler call
end
* `target_id` (`INTEGER`): ID that specifies the target which executed the exception handler
(corresponds to `targets.id`)

Example table:
| id | exc_class | timestamp_entry | timestamp_entry_rt | timestamp_exit | timestamp_exit_rt | target_id |
|:--:|:---------:|:---------------:|:------------------:|:--------------:|:-----------------:|:---------:|
| 1  | 0         | 256             | 1638533854         | 300            | 1638533860        | 1         |
| 2  | 0         | 560             | 1638533865         | 602            | 1638533870        | 1         |

## interrupt (`*_int.csv`)

Field description:

* `id` (`INTEGER`): Unique ID for each traced ISR hit
* `number` (`INTEGER`): Interrupt number
* `timestamp_entry` (`BIGINT`): Elapsed nanoseconds since start at the time the ISR has been called
* `timestamp_entry_rt` (`BIGINT`): Elapsed real-time (ns) since program start until ISR call
* `timestamp_exit` (`BIGINT`): Elapsed nanoseconds since start at the time the ISR has been finished
* `timestamp_exit_rt` (`BIGINT`): Elapsed real-time (ns) since program start until ISR end
* `target_id` (`INTEGER`): ID that specifies the target which executed the ISR (corresponds
to `targets.id`)

Example table:
| id | number | timestamp_entry | timestamp_entry_rt | timestamp_exit | timestamp_exit_rt | target_id |
|:--:|:------:|:---------------:|:------------------:|:--------------:|:-----------------:|:---------:|
| 1  | 27     | 102324          | 1638533854         | 102512         | 1638533860        | 1         |
| 2  | 27     | 102836          | 1638533865         | 112166         | 1638533870        | 1         |

## module (`*_module.csv`)

Field description:

* `id` (`INTEGER`): Unique ID for each module
* `name` (`STRING`): Name of the module

Example table:
| id | name         |
|:--:|:------------:|
| 1  | system.clock |
| 2  | system.ram   |
| 3  | system.uart0 |

## quantum (`*_quantum.csv`)

Field description:

* `id` (`INTEGER`): Unique ID for each ended quantum
* `length` (`INTEGER`): length of the quantum (ns)
* `timestamp` (`BIGINT`): Elapsed nanoseconds since start at the time the quantum has been ended
* `timestamp_rt` (`BIGINT`): Elapsed real-time (ns) since program start until quantum end
* `target_id` (`INTEGER`): ID that specifies the target which ended the quantum (corresponds to `targets.id`)

Example table:
| id | length | timestamp | timestamp_rt | target_id |
|:--:|:------:|:---------:|:------------:|:---------:|
| 1  | 1000   | 0         | 64212        | 1         |
| 2  | 10002  | 10000     | 85452        | 2         |

## sc_proc (`*_sc_proc.csv`)

* `id` (`INTEGER`): Unique ID for each SystemC process
* `name` (`STRING`): Name of the SystemC process
* `type` (`INTEGER`): Type of the process (0: `NONE`, 1: `SC_THREAD`, 2: `SC_CTHREAD`, 3: `SC_METHOD`)

Example table:
| id | name                                 | type |
|:--:|:------------------------------------:| :--: |
| 1  | system.arm0.arm_timer_ns_trigger_0   | 3    |
| 2  | system.arm0.arm_timer_virt_trigger_0 | 3    |

## sc_sched (`*_sc_sched.csv`)

Field description:

* `id` (`INTEGER`): Unique ID for each schedule of a process
* `timestamp_begin` (`BIGINT`): Elapsed nanoseconds since start at the time the process is scheduled
* `timestamp_begin_rt` (`BIGINT`): Elapsed real-time (ns) since program start at the time the process is scheduled
* `timestamp_end` (`BIGINT`): Elapsed nanoseconds since start at the time the process is done / calls `wait`
* `timestamp_end_rt` (`BIGINT`): Elapsed real-time (ns) since program start at the time the process is done / calls `wait`
* `sc_proc_id` (`INTEGER`): ID that specifies the process that is scheduled (corresponds to `sc_proc.id`)

Example table:
| id | timestamp_begin | timestamp_begin_rt | timestamp_end | timestamp_end_rt | sc_proc_id |
|:--:|:---------------:|:------------------:|:-------------:|:----------------:|:----------:|
| 1  | 0              | 696072266           | 0             | 696072476        | 9          |
| 2  | 0              | 696083517           | 100000        | 723195164        | 9          |

## sniffer (`*_snif.csv`)

Field description:

* `id` (`INTEGER`): Unique ID for each sniffer
* `name` (`STRING`): Module name of the sniffer

Example table:
| id | name                     |
|:--:|:------------------------:|
| 1  | system.sniffer_data_cpu0 |
| 2  | system.sniffer_insn_cpu0 |

## symbol (`*_symbol.csv`)

Field description:

* `id` (`INTEGER`): Unique ID for each symbol
* `name` (`STRING`): Name of the symbol (function)
* `address` (`INTEGER`): Virtual address of the symbol
* `size` (`INTEGER`): Size of the symbol (function) in bytes

Example table:
| id | name                   | address          | size |
|:--:|:----------------------:|:----------------:|:----:|
| 1  | fdt_offset_ptr         | FFFFFF800874D670 | 116  |
| 2  | fdt_check_node_offset_ | FFFFFF800874D810 | 64   |

## target (`*_target.csv`)

Field description:

* `id` (`INTEGER`): Unique ID for each target
* `name` (`STRING`): Name of the target

Example table:
| id | name |
|:--:|:----:|
| 1  |"cpu0"|
| 2  |"cpu1"|

## task_switch (`*_ts.csv`)

Field description:

* `id` (`INTEGER`): Unique ID for each traced task switch
* `pid_prev` (`INTEGER`): Process ID of the previous task; assigned by the os
* `name_prev` (`STRING`): Name of the previous process; assigned by the os
* `pid_next` (`INTEGER`): Process ID of the next task; assigned by the os
* `name_next` (`STRING`): Name of the next process; assigned by the os
* `timestamp` (`BIGINT`): Elapsed nanoseconds since start
* `timestamp_rt` (`BIGINT`): Elapsed real-time (ns) since program start until task switch
* `target_id` (`INTEGER`): ID that specifies the target which executed the task switch (corresponds to `targets.id`)

Example table:
| id | pid_prev | name_prev | pid_next | name_next| timestamp | timestamp_rt | target_id |
|:--:|:--------:|:---------:|:--------:|:--------:|:---------:|:------------:|:---------:|
| 1  | 0        | "task0"   | 1        | "task1"  | 12        | 1638533854   | 1         |
| 2  | 1        | "task1"   | 2        | "task2"  | 18        | 1638533954   | 1         |
| 3  | 2        | "task2"   | 0        | "task0"  | 30        | 1638533854   | 1         |

## trans (`*_trans.csv`)

Field description:

* `id` (`INTEGER`): Unique ID for each transaction
* `timestamp_fw` (`BIGINT`): Elapsed nanoseconds since start at the time the transaction has been sent (Forward path)
* `timestamp_fw_rt` (`BIGINT`): Elapsed real-time (ns) since program start until forward path of the
transaction has been called
* `timestamp_bw` (`BIGINT`): Elapsed nanoseconds since start at the time the transaction has been sent (Backward path)
* `timestamp_bw_rt` (`BIGINT`): Elapsed real-time (ns) since program start until backward path of the
transaction has been called
* `address` (`BIGINT`): Address that should be read / written
* `length` (`INTEGER`): Number of bytes that are read / written
* `write` (`BOOLEAN`): Determines whether the transaction is a write or read transaction
* `dmi_req` (`BOOLEAN`): If true, the transaction is a DMI pointer request
* `sniffer_id` (`INTEGER`): ID of the sniffer that sniffed the transaction

Example table:

| id | timestamp_fw | timestamp_fw_rt | timestamp_bw | timestamp_bw_rt | address   | length | write | dmi_req | sniffer_id |
|:--:|:------------:|:---------------:|:------------:|:---------------:|:---------:|:------:|:-----:|:-------:|:----------:|
| 1  | 33           | 1638533854      | 33           | 1638533860      | 10772480  | 8      | 1     | 0       | 1          |
| 2  | 293171       | 1638533865      | 293171       | 1638533870      | 268472344 | 4      | 0     | 0       | 1          |

## wait_event (`*_wait_event.csv`)

Field description:

* `id` (`INTEGER`): Unique ID for each wait-on-event call
* `timestamp_begin` (`BIGINT`): Elapsed nanoseconds since start at the time the `wait` function is called
* `timestamp_begin_rt` (`BIGINT`): Elapsed real-time (ns) since program start at the time the `wait` function is called
* `timestamp_end` (`BIGINT`): Elapsed nanoseconds since start at the time the `wait` function returns
* `timestamp_end_rt` (`BIGINT`): Elapsed real-time (ns) since program start at the time the `wait` function returns
* `target_id` (`INTEGER`): ID that specifies the target which ended the quantum (corresponds to `targets.id`)

Example table:
| id | timestamp_begin | timestamp_begin_rt | timestamp_end | timestamp_end_rt | target_id |
|:--:|:---------------:|:------------------:|:-------------:|:----------------:|:---------:|
| 1  | 1000            | 12312              | 2000          | 12402            | 2         |
| 2  | 1500            | 12323              | 2000          | 12402            | 1         |
