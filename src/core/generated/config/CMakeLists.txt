set(FORTE_TicksPerSecond
    "1000"
    CACHE STRING "FORTE ticks per second"
)
mark_as_advanced(FORTE_TicksPerSecond)

set(FORTE_EventChainEventListSize
    "256"
    CACHE STRING "FORTE eventchain event list size"
)
mark_as_advanced(FORTE_EventChainEventListSize)

set(FORTE_EventChainExternalEventListSize
    "16"
    CACHE STRING "FORTE eventchain external event list size"
)
mark_as_advanced(FORTE_EventChainExternalEventListSize)

set(FORTE_CommunicationInterruptQueueSize
    "10"
    CACHE STRING "FORTE Communication interrupt queue size"
)
mark_as_advanced(FORTE_CommunicationInterruptQueueSize)

if (FORTE_DYNAMIC_TYPE_LOAD)
    set(FORTE_IPLayerRecvBufferSize
        "20000"
        CACHE STRING "FORTE IP layer recv buffer size"
    )
else ()
    set(FORTE_IPLayerRecvBufferSize
        "1500"
        CACHE STRING "FORTE IP layer recv buffer size"
    )
endif ()
mark_as_advanced(FORTE_IPLayerRecvBufferSize)
configure_file(forte_config.h.in
               "${FORTE_GENERATED_FILES_DIR}/forte_config.h"
)

set(FORTE_DEVICE_FIXED_IMPL
        "util::factory::DynamicImpl"
        CACHE STRING "FORTE fixed device implementation"
)
mark_as_advanced(FORTE_DEVICE_FIXED_IMPL)

set(FORTE_DEVICE_DEFAULT_IMPL
        "RMT_DEV"
        CACHE STRING "FORTE default device implementation"
)
mark_as_advanced(FORTE_DEVICE_DEFAULT_IMPL)

set(FORTE_DEVICE_FIXED_INCLUDE
        "core/util/factory.h"
        CACHE STRING "FORTE fixed device include directive"
)
mark_as_advanced(FORTE_DEVICE_FIXED_INCLUDE)

configure_file(devicefactory_config.h.in
        "${FORTE_GENERATED_FILES_DIR}/devicefactory_config.h"
)

set(FORTE_TIMER_HANDLER_FIXED_IMPL
        "util::factory::DynamicImpl"
        CACHE STRING "FORTE fixed timer handler implementation"
)
mark_as_advanced(FORTE_TIMER_HANDLER_FIXED_IMPL)

set(FORTE_TIMER_HANDLER_DEFAULT_IMPL
        "default"
        CACHE STRING "FORTE default timer handler implementation"
)
mark_as_advanced(FORTE_TIMER_HANDLER_DEFAULT_IMPL)

set(FORTE_TIMER_HANDLER_FIXED_INCLUDE
        "core/util/factory.h"
        CACHE STRING "FORTE fixed timer handler include directive"
)
mark_as_advanced(FORTE_TIMER_HANDLER_FIXED_INCLUDE)

configure_file(timerhandlerfactory_config.h.in
        "${FORTE_GENERATED_FILES_DIR}/timerhandlerfactory_config.h"
)

set(FORTE_ECET_FIXED_IMPL
        "CEventChainExecutionThread"
        CACHE STRING "FORTE fixed ecet implementation"
)
mark_as_advanced(FORTE_ECET_FIXED_IMPL)

set(FORTE_ECET_DEFAULT_IMPL
        "default"
        CACHE STRING "FORTE default ecet implementation"
)
mark_as_advanced(FORTE_ECET_FIXED_IMPL)

set(FORTE_ECET_FIXED_INCLUDE
        "core/ecet.h"
        CACHE STRING "FORTE fixed ecet include directive"
)
mark_as_advanced(FORTE_ECET_FIXED_INCLUDE)

configure_file(ecetfactory_config.h.in
        "${FORTE_GENERATED_FILES_DIR}/ecetfactory_config.h"
)

set(FORTE_LOGGER_BUFFER_SIZE "300" CACHE STRING "Buffer length of the logger")
mark_as_advanced(FORTE_LOGGER_BUFFER_SIZE)

configure_file(devlog_config.h.in
        "${FORTE_GENERATED_FILES_DIR}/devlog_config.h"
)
